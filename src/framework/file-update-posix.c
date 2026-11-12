/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Name     : file-update-posix.c
 * File Authors  : @Mikachu2333
 * Contributors  : @ccmywish
 *               |
 * Created On    : <2026-11-12>
 * Last Modified : <2026-11-12>
 *
 * POSIX 后端：使用 mkstemp/rename 暂存并原子替换目标文件。
 * ------------------------------------------------------------*/

#ifndef XY_Build_On_Windows

#include <sys/stat.h>

static bool
file_update_create_parent_dirs (const char *path)
{
  char *dir = xy_parent_dir (path);
  for (char *p = dir + 1; *p; p++)
    {
      if ('/' != *p) continue;
      *p = '\0';
      int retcode = mkdir (dir, 0777);
      *p = '/';
      if (0 != retcode && EEXIST != errno) { free (dir); return false; }
    }
  bool ok = 0 == mkdir (dir, 0777) || EEXIST == errno;
  free (dir);
  return ok;
}

/* 在目标文件同目录创建临时文件并写入指定字节；保留原文件的属主与权限。
 * 失败时删除临时文件并返回 NULL，errno 保持失败原因。 */
static char *
file_update_write_temp_file (const char *target, const char *bytes, size_t len,
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
      /* 与普通配置文件的创建方式保持一致：/etc/environment 必须能被
       * 登录会话读取。本 CLI 为单线程，读取并恢复 umask 不会产生竞争。 */
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
  if (ok) ok = 0 == fsync (fd);
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

/* 解析已有符号链接，替换其目标以保留链接本身；再把新内容与回滚内容
 * 写入同目录临时文件。旧快照已被外部修改时以 EAGAIN 拒绝整批更新。 */
static bool
file_update_stage (const FileUpdate_t *op, StagedFileUpdate_t *stage)
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
      if (!xy_file_read_bytes (stage->target, &current, &len)) return false;
      bool same = len == op->old_len && (0 == len || 0 == memcmp (current, op->old_content, len));
      free (current);
      if (!same) { errno = EAGAIN; return false; }
    }
  else
    {
      /* 不替换悬空符号链接，也不替换计划生成后才出现的路径。 */
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
  stage->replacement = file_update_write_temp_file (stage->target, op->new_content, op->new_len,
                                                    op->existed ? &st : NULL);
  if (!stage->replacement) return false;
  if (op->existed)
    {
      stage->rollback = file_update_write_temp_file (stage->target, op->old_content, op->old_len, &st);
      if (!stage->rollback) return false;
    }
  return true;
}

static bool
file_update_commit (StagedFileUpdate_t *stage, const FileUpdate_t *op)
{
  (void) op;
  return 0 == rename (stage->replacement, stage->target);
}

static bool
file_update_rollback (StagedFileUpdate_t *stage, const FileUpdate_t *op)
{
  return 0 == (op->existed ? rename (stage->rollback, stage->target)
                           : unlink (stage->target));
}

static void
file_update_discard (StagedFileUpdate_t *stage)
{
  if (stage->replacement) unlink (stage->replacement);
  if (stage->rollback && !stage->keep_rollback) unlink (stage->rollback);
  free (stage->target);
  free (stage->replacement);
  free (stage->rollback);
}

static bool
file_update_same_target (const StagedFileUpdate_t *a, const StagedFileUpdate_t *b)
{
  return a->target && b->target && xy_streql (a->target, b->target);
}

#endif /* !XY_Build_On_Windows */
