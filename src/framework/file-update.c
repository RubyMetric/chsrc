/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * 通用文件内容读取与 POSIX 批量更新；在 OS.c 之后引入。
 * 本层只处理字节，不解释配置语法、镜像地址或管理区块。
 * ------------------------------------------------------------*/

#include <errno.h>
#include <sys/stat.h>

typedef enum FileUpdateResult_t
{
  FileUpdate_OK,
  FileUpdate_Unchanged,
  FileUpdate_Failed
}
FileUpdateResult_t;

/* 路径与缓冲区均由调用方持有，提交完成前不得修改或释放。
 * norm_path 是规范化后的路径；长度不包含用于方便文本处理的尾部 NUL。
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

/* 二进制读取，额外补一个 NUL；成功时 *out_buf 由调用方释放。 */
static bool
chsrc_read_file_bytes (const char *path, char **out_buf, size_t *out_len)
{
  *out_buf = NULL;
  *out_len = 0;
  FILE *fp = xy_fopen (path, "rb");
  if (!fp) return false;

  size_t cap = 4096;
  size_t len = 0;
  char *buf = xy_malloc0 (cap);
  bool ok = true;
  int err = 0;
  for (;;)
    {
      if (len == cap - 1)
        {
          if (cap > SIZE_MAX / 2) { ok = false; err = EOVERFLOW; break; }
          size_t next_cap = cap * 2;
          char *grown = realloc (buf, next_cap);
          if (!grown) { ok = false; err = ENOMEM; break; }
          buf = grown;
          cap = next_cap;
        }
      size_t n = fread (buf + len, 1, cap - len - 1, fp);
      len += n;
      if (ferror (fp)) { ok = false; err = errno; break; }
      if (feof (fp)) break;
    }
  if (0 != fclose (fp)) { ok = false; err = errno; }
  if (!ok)
    {
      free (buf);
      errno = err;
      return false;
    }
  buf[len] = '\0';
  *out_buf = buf;
  *out_len = len;
  return true;
}

#ifndef XY_Build_On_Windows

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
  bool committed;
  bool keep_rollback;
  bool duplicate;
}
StagedFileUpdate_t;

static bool
file_update_changed (const FileUpdate_t *op)
{
  return op->old_len != op->new_len
    || (op->old_len > 0 && 0 != memcmp (op->old_content, op->new_content, op->old_len));
}

static bool
file_update_create_parent_dirs (const char *path)
{
  char *dir = xy_parent_dir (path);
  for (char *p = dir + 1; *p; p++)
    {
      if ('/' != *p) continue;
      *p = '\0';
      int rc = mkdir (dir, 0777);
      *p = '/';
      if (0 != rc && EEXIST != errno) { free (dir); return false; }
    }
  bool ok = 0 == mkdir (dir, 0777) || EEXIST == errno;
  free (dir);
  return ok;
}

static char *
file_update_stage_bytes (const char *target, const char *bytes, size_t len,
                  const struct stat *original)
{
  char *path = xy_2strcat (target, ".chsrc-XXXXXX");
  int fd = mkstemp (path);
  if (fd < 0) { free (path); return NULL; }
  bool ok = true;
  if (original)
    {
      struct stat created;
      ok = 0 == fstat (fd, &created);
      if (ok && (created.st_uid != original->st_uid || created.st_gid != original->st_gid))
        ok = 0 == fchown (fd, original->st_uid, original->st_gid);
      if (ok) ok = 0 == fchmod (fd, original->st_mode & 07777);
    }
  else
    {
      /* Match normal configuration-file creation; /etc/environment must
       * remain readable by login sessions. This CLI is single-threaded. */
      mode_t mask = umask (0);
      umask (mask);
      ok = 0 == fchmod (fd, 0666 & ~mask);
    }
  FILE *fp = ok ? fdopen (fd, "wb") : NULL;
  if (!fp)
    {
      int err = errno;
      close (fd);
      unlink (path);
      free (path);
      errno = err;
      return NULL;
    }
  ok = 0 == len || len == fwrite (bytes, 1, len, fp);
  if (ok) ok = 0 == fflush (fp);
  if (ok) ok = 0 == fsync (fileno (fp));
  int err = errno;
  if (0 != fclose (fp)) { ok = false; err = errno; }
  if (!ok)
    {
      unlink (path);
      free (path);
      errno = err;
      return NULL;
    }
  return path;
}

/* Resolve existing symlinks so replacing the target preserves the link. */
static bool
file_update_prepare (const FileUpdate_t *op, StagedFileUpdate_t *stage)
{
  struct stat st;
  if (op->existed)
    {
      stage->target = realpath (op->norm_path, NULL);
      if (!stage->target || 0 != stat (stage->target, &st)) return false;
      if (!S_ISREG (st.st_mode)) { errno = EINVAL; return false; }
      if (0 != access (stage->target, W_OK)) return false;
      char *current = NULL;
      size_t len = 0;
      if (!chsrc_read_file_bytes (stage->target, &current, &len)) return false;
      bool same = len == op->old_len && (0 == len || 0 == memcmp (current, op->old_content, len));
      free (current);
      if (!same) { errno = EAGAIN; return false; }
    }
  else
    {
      /* Do not replace a dangling symlink or a target created after planning. */
      if (0 == lstat (op->norm_path, &st)) { errno = EEXIST; return false; }
      if (ENOENT != errno || !file_update_create_parent_dirs (op->norm_path)) return false;
      char *dir = xy_parent_dir (op->norm_path);
      char *resolved = realpath (dir, NULL);
      free (dir);
      if (!resolved) return false;
      const char *name = strrchr (op->norm_path, '/');
      stage->target = xy_path_join (resolved, name ? name + 1 : op->norm_path);
      free (resolved);
    }
  stage->replacement = file_update_stage_bytes (stage->target, op->new_content, op->new_len,
                                        op->existed ? &st : NULL);
  if (!stage->replacement) return false;
  if (op->existed)
    {
      stage->rollback = file_update_stage_bytes (stage->target, op->old_content, op->old_len, &st);
      if (!stage->rollback) return false;
    }
  return true;
}

/* 全部内容由调用方预先生成。本层比较字节并复核旧快照，再暂存、备份、替换。
 * 无变化时不写入、不备份；Dry Run 只显示将要修改的文件。
 * 每个文件原子替换，后续失败回滚已替换文件；不保证崩溃时的多文件原子性。
 * 保留符号链接和目标的属主、属组、权限位；尚不复制 ACL 与扩展属性。
 * 复核快照不能消除外部写入竞态，调用方仍应避免并发修改同一配置。
 * out_err 可为 NULL，否则失败文案由调用方释放。 */
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
  int err = 0;
  for (size_t i = 0; i < count; i++)
    {
      if (!file_update_changed (&ops[i])) continue;
      if (!file_update_prepare (&ops[i], &stages[i]))
        { failed = i; err = errno; ok = false; break; }
      /* Shell 启动文件可能互为符号链接：相同更新合并，冲突更新拒绝。 */
      for (size_t j = 0; j < i; j++)
        if (stages[j].target && xy_streql (stages[j].target, stages[i].target))
          {
            if (ops[j].new_len != ops[i].new_len
                || (ops[i].new_len > 0 && 0 != memcmp (ops[j].new_content, ops[i].new_content, ops[i].new_len)))
              { failed = i; err = EINVAL; ok = false; }
            else
              stages[i].duplicate = true;
            break;
          }
      if (!ok) break;
    }

  /* Finish all preparation and backups before touching the first target. */
  if (ok)
    for (size_t i = 0; i < count; i++)
      if (file_update_changed (&ops[i]) && !stages[i].duplicate && ops[i].existed)
        chsrc_backup (ops[i].display_path);

  for (size_t i = 0; ok && i < count; i++)
    {
      if (!file_update_changed (&ops[i]) || stages[i].duplicate) continue;
      int rc;
#ifdef CHSRC_TESTING
      if ((int) i == FileUpdate_Test_Fail_Commit) { errno = EIO; rc = -1; }
      else
#endif
        rc = rename (stages[i].replacement, stages[i].target);
      if (0 != rc) { failed = i; err = errno; ok = false; break; }
      stages[i].committed = true;
    }

  if (!ok)
    {
      for (size_t n = count; n > 0; n--)
        {
          size_t i = n - 1;
          if (!stages[i].committed) continue;
          int rc = ops[i].existed ? rename (stages[i].rollback, stages[i].target)
                                 : unlink (stages[i].target);
          if (0 != rc)
            {
              rollback_ok = false;
              stages[i].keep_rollback = true;
              chsrc_error2 (xy_2strcat (
                ENGLISH ? "Rollback failed; recover using backup: " : "回滚失败，请使用备份恢复: ",
                stages[i].rollback ? stages[i].rollback : stages[i].target));
            }
        }
      if (out_err)
        *out_err = xy_strcat (6,
          ENGLISH ? "Unable to update file: " : "无法更新文件: ", ops[failed].norm_path,
          " (", strerror (err), ")",
          rollback_ok ? (ENGLISH ? "; configuration changes rolled back" : "；配置修改已回滚")
                      : (ENGLISH ? "; rollback incomplete, see recovery paths above" : "；回滚未完成，请使用上述恢复路径"));
    }
  else
    {
      for (size_t i = 0; i < count; i++)
        if (file_update_changed (&ops[i]) && !stages[i].duplicate)
          log_for_file_write (ops[i].display_path, true);
    }

  for (size_t i = 0; i < count; i++)
    {
      if (stages[i].replacement) unlink (stages[i].replacement);
      if (stages[i].rollback && !stages[i].keep_rollback) unlink (stages[i].rollback);
      free (stages[i].target);
      free (stages[i].replacement);
      free (stages[i].rollback);
    }
  free (stages);
  return ok ? FileUpdate_OK : FileUpdate_Failed;
}

#endif /* !XY_Build_On_Windows */
