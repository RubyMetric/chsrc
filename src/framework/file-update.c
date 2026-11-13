/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Name     : file-update.c
 * File Authors  : @Mikachu2333
 * Contributors  : @ccmywish
 *               |
 * Created On    : <2026-10-28>
 * Last Modified : <2026-11-13>
 *
 * 文件更新统一入口；按平台包含 POSIX 或 Win32 后端。
 * 本层只处理字节，不解释配置语法、镜像地址或管理区块。
 * ------------------------------------------------------------*/

#include <errno.h>

#ifndef XY_Build_On_Windows
#include <sys/types.h>
#endif

typedef enum FileUpdateResult_t
{
  FileUpdate_OK,
  FileUpdate_Unchanged,
  FileUpdate_Failed
}
FileUpdateResult_t;

/* 两个暂存项的关系：路径相同可直接合并，inode 相同只能用于冲突检测。 */
typedef enum FileUpdateIdentity_t
{
  FileUpdate_Different,
  FileUpdate_SamePath,
  FileUpdate_SameFile
}
FileUpdateIdentity_t;

/* 路径与缓冲区均由调用方持有，提交完成前不得修改或释放。
 * norm_path 是平台相关路径；长度不包含用于方便文本处理的尾部 NUL。
 * 已有文件的新内容为空时截断文件；不存在且新内容为空时不创建文件。
 * 不提供删除文件操作，也不负责判断某条配置是否由 chsrc 管理。 */
typedef struct FileUpdate_t
{
  const char *display_path;
  char       *norm_path;
  bool        existed;
  char       *old_content;
  size_t      old_len;
  char       *new_content;
  size_t      new_len;
}
FileUpdate_t;

#ifdef CHSRC_TESTING
/* 仅测试构建可用：把批次中下标等于该值的提交强制判为失败，用于验证回滚；
 * -1 表示不注入。测试在调用前设为目标下标，验证完成后立即恢复为 -1。 */
static int FileUpdate_Test_Fail_Commit = -1;
#endif

typedef struct StagedFileUpdate_t
{
  char *target;
  char *replacement;
  char *rollback;
  bool  committed;
  bool  keep_rollback;
  bool  duplicate;
  /* has_id 与平台 id 用于识别硬链接；新建文件没有 id。 */
  bool  has_id;
#ifdef XY_Build_On_Windows
  DWORD    volume;
  DWORD    index_high;
  DWORD    index_low;
  wchar_t *target_w;
  wchar_t *replacement_w;
  wchar_t *rollback_w;
#else
  dev_t    dev;
  ino_t    ino;
#endif
}
StagedFileUpdate_t;

static bool
file_update_changed (const FileUpdate_t *op)
{
  return op->old_len != op->new_len
    || (op->old_len > 0 && 0 != memcmp (op->old_content, op->new_content, op->old_len));
}

#ifdef XY_Build_On_Windows
#include "file-update-win32.c"
#else
#include "file-update-posix.c"
#endif

/* 全部内容由调用方预先生成。本层比较字节并复核旧快照，再暂存、备份、替换。
 * 无变化时不写入、不备份；Dry Run 只显示将要修改的文件。
 * 每个文件原子替换，后续失败回滚已替换文件；不保证崩溃时的多文件原子性。
 * 平台后端负责保留符号链接及权限/ACL；复核快照不能消除外部写入竞态。 */
static FileUpdateResult_t
chsrc_update_files (const FileUpdate_t *ops, size_t count, char **out_err)
{
  if (out_err) *out_err = NULL;
  if (count > 0 && !ops)
    {
      if (out_err) *out_err = xy_strdup (ENGLISH ? "Missing file update plan" : "缺少文件更新计划");
      return FileUpdate_Failed;
    }

  bool any_changed = false;
  for (size_t i = 0; i < count; i++)
    {
      if (!ops[i].display_path || !ops[i].norm_path || !*ops[i].norm_path
          || (ops[i].old_len > 0 && !ops[i].old_content)
          || (ops[i].new_len > 0 && !ops[i].new_content)
          || (!ops[i].existed && ops[i].old_len > 0))
        {
          if (out_err) *out_err = xy_strdup (ENGLISH ? "Invalid file update plan" : "无效的文件更新计划");
          return FileUpdate_Failed;
        }
      if (file_update_changed (&ops[i])) any_changed = true;
    }
  if (!any_changed) return FileUpdate_Unchanged;

  if (in_dry_run_mode ())
    {
      for (size_t i = 0; i < count; i++)
        if (file_update_changed (&ops[i])) log_for_file_write (ops[i].display_path, true);
      return FileUpdate_OK;
    }

  StagedFileUpdate_t *stages = xy_malloc0 (count * sizeof (StagedFileUpdate_t));
  size_t failed = 0;
  bool ok = true;
  bool rollback_ok = true;
  bool duplicate_conflict = false;
  int err = 0;
  for (size_t i = 0; i < count; i++)
    {
      if (!file_update_changed (&ops[i])) continue;
      if (!file_update_stage (&ops[i], &stages[i]))
        { failed = i; err = errno; ok = false; break; }
      /* Shell 启动文件可能互为符号链接：相同更新合并，冲突更新拒绝。
       * 硬链接解析出的路径不同但 inode 相同，必须分别提交，否则会漏写。 */
      for (size_t j = 0; j < i; j++)
        {
          FileUpdateIdentity_t identity = file_update_identity (&stages[j], &stages[i]);
          if (FileUpdate_Different == identity) continue;

          if (ops[j].new_len != ops[i].new_len
              || (ops[i].new_len > 0 && 0 != memcmp (ops[j].new_content, ops[i].new_content, ops[i].new_len)))
            {
              failed = i;
              err = EINVAL;
              duplicate_conflict = true;
              ok = false;
            }
          else if (FileUpdate_SamePath == identity)
            stages[i].duplicate = true;
          break;
        }
      if (!ok) break;
    }

  /* 先完成全部暂存与备份，再开始写入第一个目标文件。 */
  if (ok)
    for (size_t i = 0; i < count; i++)
      if (file_update_changed (&ops[i]) && !stages[i].duplicate && ops[i].existed)
        chsrc_backup (ops[i].display_path);

  for (size_t i = 0; ok && i < count; i++)
    {
      if (!file_update_changed (&ops[i]) || stages[i].duplicate) continue;
      int retcode;
#ifdef CHSRC_TESTING
      if ((int) i == FileUpdate_Test_Fail_Commit) { errno = EIO; retcode = -1; }
      else
#endif
        retcode = file_update_commit (&stages[i], &ops[i]) ? 0 : -1;
      if (0 != retcode) { failed = i; err = errno; ok = false; break; }
      stages[i].committed = true;
    }

  if (!ok)
    {
      for (size_t n = count; n > 0; n--)
        {
          size_t i = n - 1;
          if (!stages[i].committed) continue;
          if (!file_update_rollback (&stages[i], &ops[i]))
            {
              rollback_ok = false;
              stages[i].keep_rollback = true;
              chsrc_error2 (xy_2strcat (
                ENGLISH ? "Rollback failed; recover using backup: " : "回滚失败，请使用备份恢复: ",
                stages[i].rollback ? stages[i].rollback : stages[i].target));
            }
        }
      if (out_err)
        {
          char reason_buf[128];
          const char *reason;
          if (duplicate_conflict)
            reason = ENGLISH ? "another path in the plan updates the same file with different content"
                             : "计划中另有路径更新同一文件且内容不同";
          else if (EAGAIN == err)
            reason = ENGLISH ? "file changed on disk before it could be updated"
                             : "文件在写入前已被其他程序修改";
          else
            {
              snprintf (reason_buf, sizeof reason_buf, "%s", strerror (err));
              reason = reason_buf;
            }

          *out_err = xy_strcat (6,
            ENGLISH ? "Unable to update file: " : "无法更新文件: ", ops[failed].norm_path,
            " (", reason, ")",
            rollback_ok ? (ENGLISH ? "; configuration changes rolled back" : "；配置修改已回滚")
                        : (ENGLISH ? "; rollback incomplete, see recovery paths above" : "；回滚未完成，请使用上述恢复路径"));
        }
    }
  else
    {
      for (size_t i = 0; i < count; i++)
        if (file_update_changed (&ops[i]) && !stages[i].duplicate)
          log_for_file_write (ops[i].display_path, true);
    }

  for (size_t i = 0; i < count; i++)
    file_update_discard (&stages[i]);
  free (stages);
  return ok ? FileUpdate_OK : FileUpdate_Failed;
}
