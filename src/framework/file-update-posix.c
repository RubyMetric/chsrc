/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Name     : file-update-posix.c
 * File Authors  : @Mikachu2333
 * Contributors  : @ccmywish
 *               |
 * Created On    : <2026-11-12>
 * Last Modified : <2026-11-13>
 *
 * POSIX 后端：使用 mkstemp/rename 暂存并原子替换目标文件。
 * 保留目标属主与权限，Linux 上尽力复制 ACL 等扩展属性。
 * rename 后刷新父目录，尽可能保证替换在崩溃后仍然可见。
 * ------------------------------------------------------------*/

#ifndef XY_Build_On_Windows

#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef __linux__
#include <sys/xattr.h>
#endif


/* 返回 target 的父目录；根目录下的路径返回 "/"。
 * 调用方负责 free；失败返回 NULL 并保留 errno。 */
static char *
file_update_parent_dir (const char *target)
{
  char *dir = xy_parent_dir (target);
  if (!dir) return NULL;

  if ('\0' == *dir)
    {
      free (dir);
      dir = xy_strdup ("/");
    }
  return dir;
}


/* 逐级创建 path 的父目录。
 * 已存在的目录视为成功，但最终组件必须是目录，否则返回 ENOTDIR。
 * 根目录下的文件不需要创建父目录。
 * @param path 目标文件路径，可为相对路径
 * @return 成功 true；失败 false 并设置 errno */
static bool
file_update_create_parent_dirs (const char *path)
{
  char *dir = file_update_parent_dir (path);
  if (!dir) { errno = ENOMEM; return false; }

  for (char *p = dir + 1; *p; p++)
    {
      if ('/' != *p) continue;
      *p = '\0';
      int retcode = mkdir (dir, 0777);
      *p = '/';
      if (0 != retcode && EEXIST != errno) { free (dir); return false; }
    }

  bool ok = 0 == mkdir (dir, 0777) || EEXIST == errno;
  if (ok)
    {
      struct stat st;
      /* 同名路径可能是普通文件，mkdir 的 EEXIST 不能直接当作成功。 */
      if (0 != stat (dir, &st) || !S_ISDIR (st.st_mode))
        {
          ok = false;
          errno = ENOTDIR;
        }
    }

  free (dir);
  return ok;
}


/* 刷新目录项，确保 rename/unlink 在崩溃后仍然可见。
 * 目录 fsync 失败不改变已完成的替换，因此不返回错误。 */
static void
file_update_sync_parent_dir (const char *target)
{
  char *dir = file_update_parent_dir (target);
  if (!dir) return;

  int flags = O_RDONLY;
#ifdef O_DIRECTORY
  flags |= O_DIRECTORY;
#endif
  int fd = open (dir, flags);
  if (fd >= 0)
    {
      (void) fsync (fd);
      (void) close (fd);
    }

  free (dir);
}


/* 尽力把 from 的扩展属性复制到 fd；ACL 在 Linux 上以 xattr 形式保存。
 * 复制失败（权限不足、文件系统不支持等）时忽略，不影响更新结果。 */
static void
file_update_copy_xattrs (const char *from, int fd)
{
#ifdef __linux__
  ssize_t size = listxattr (from, NULL, 0);
  if (size <= 0) return;

  char *names = malloc ((size_t) size);
  if (!names) return;

  ssize_t got = listxattr (from, names, size);
  if (got < 0) { free (names); return; }

  for (char *name = names; name < names + got; name += strlen (name) + 1)
    {
      /* 默认 ACL 只对目录有意义，普通文件的 ACL 位于 system.posix_acl_access。 */
      if (0 == strcmp (name, "system.posix_acl_default")) continue;

      ssize_t value_size = getxattr (from, name, NULL, 0);
      if (value_size <= 0) continue;

      void *value = malloc ((size_t) value_size);
      if (!value) continue;

      if (getxattr (from, name, value, (size_t) value_size) == value_size)
        (void) fsetxattr (fd, name, value, (size_t) value_size, 0);
      free (value);
    }

  free (names);
#else
  (void) from;
  (void) fd;
#endif
}


/* 在目标文件同目录创建临时文件并写入指定字节。
 * original 非空时复制其属主、权限与（Linux）扩展属性；否则按 umask 设置权限。
 * @param target 目标文件路径
 * @param original_path 读取扩展属性的源路径，可为 NULL
 * @param bytes 待写入内容
 * @param len 内容长度
 * @param original 源文件状态，可为 NULL
 * @return 临时文件路径（调用方负责 free）；失败返回 NULL 并保留 errno */
static char *
file_update_write_temp_file (const char *target, const char *original_path,
                             const char *bytes, size_t len, const struct stat *original)
{
  char *dir = file_update_parent_dir (target);
  if (!dir) { errno = ENOMEM; return NULL; }

  /* 临时文件名只取固定短名，避免受目标 basename 的 NAME_MAX 限制。 */
  char *path = xy_path_join (dir, ".chsrc-XXXXXX");
  free (dir);
  if (!path) { errno = ENOMEM; return NULL; }

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
      if (ok && original_path) file_update_copy_xattrs (original_path, fd);
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

  errno = 0;
  ok = 0 == len || len == fwrite (bytes, 1, len, fp);
  if (!ok && 0 == errno) errno = EIO;   /* 短写时 fwrite 可能不设置 errno。 */
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
 * 写入同目录临时文件。旧快照已被外部修改时以 EAGAIN 拒绝整批更新。
 * 已有文件记录 (st_dev, st_ino)，供重复检测识别硬链接与其他别名。 */
static bool
file_update_stage (const FileUpdate_t *op, StagedFileUpdate_t *stage)
{
  char *norm = xy_normalize_path (op->norm_path);
  if (!norm) { errno = ENOMEM; return false; }

  bool ok = false;
  struct stat st;
  const struct stat *original = NULL;

  if (op->existed)
    {
      stage->target = realpath (norm, NULL);
      if (!stage->target) goto done;
      if (0 != stat (stage->target, &st)) goto done;
      if (!S_ISREG (st.st_mode)) { errno = EINVAL; goto done; }
      /* access() 按真实 UID 判断，root 或 ACL 场景可能偏乐观；这里只作为
       * 尽早失败的前置检查，最终权限由临时文件写入与替换决定。 */
      if (0 != access (stage->target, W_OK)) goto done;

      char *current = NULL;
      size_t len = 0;
      if (!xy_file_read_bytes (stage->target, &current, &len)) goto done;
      bool same = len == op->old_len
               && (0 == len || 0 == memcmp (current, op->old_content, len));
      free (current);
      if (!same) { errno = EAGAIN; goto done; }

      original = &st;
      stage->has_id = true;
      stage->dev = st.st_dev;
      stage->ino = st.st_ino;
    }
  else
    {
      /* 不替换悬空符号链接，也不替换计划生成后才出现的路径。 */
      if (0 == lstat (norm, &st)) { errno = EEXIST; goto done; }
      if (ENOENT != errno) goto done;
      if (!file_update_create_parent_dirs (norm)) goto done;

      char *dir = file_update_parent_dir (norm);
      if (!dir) { errno = ENOMEM; goto done; }
      char *resolved = realpath (dir, NULL);
      free (dir);
      if (!resolved) goto done;

      const char *name = strrchr (norm, '/');
      const char *base = name ? name + 1 : norm;
      if ('\0' == *base)
        {
          free (resolved);
          errno = EINVAL;
          goto done;
        }
      stage->target = xy_path_join (resolved, base);
      free (resolved);
      if (!stage->target) { errno = ENOMEM; goto done; }
    }

  stage->replacement = file_update_write_temp_file (
    stage->target, op->existed ? stage->target : NULL,
    op->new_content, op->new_len, original);
  if (!stage->replacement) goto done;

  if (op->existed)
    {
      stage->rollback = file_update_write_temp_file (
        stage->target, stage->target, op->old_content, op->old_len, original);
      if (!stage->rollback) goto done;
    }
  ok = true;

done:
  free (norm);
  return ok;
}


static bool
file_update_commit (StagedFileUpdate_t *stage, const FileUpdate_t *op)
{
  if (op->existed)
    {
      if (0 != rename (stage->replacement, stage->target)) return false;
    }
  else
    {
      /* 目标可能在计划生成后被其他程序创建。Win32 后端的 MoveFileExW 不带
       * REPLACE_EXISTING，会拒绝覆盖；这里用 link 取得同样的不覆盖语义。 */
      if (0 != link (stage->replacement, stage->target))
        {
          int err = errno;
          /* 不支持硬链接的文件系统退回 rename，行为与旧实现一致。 */
          if (ENOTSUP != err && EOPNOTSUPP != err && EPERM != err && EXDEV != err)
            {
              errno = err;
              return false;
            }
          if (0 != rename (stage->replacement, stage->target)) return false;
        }
      else if (0 != unlink (stage->replacement))
        {
          /* 目标已建立、提交已经生效；临时名留给 discard 再清理一次。 */
          file_update_sync_parent_dir (stage->target);
          return true;
        }
    }

  /* 提交成功后清空指针，避免 discard 误删已不存在的临时路径。 */
  free (stage->replacement);
  stage->replacement = NULL;
  file_update_sync_parent_dir (stage->target);
  return true;
}


static bool
file_update_rollback (StagedFileUpdate_t *stage, const FileUpdate_t *op)
{
  if (op->existed)
    {
      if (0 != rename (stage->rollback, stage->target)) return false;
      free (stage->rollback);
      stage->rollback = NULL;
    }
  else if (0 != unlink (stage->target)) return false;

  file_update_sync_parent_dir (stage->target);
  return true;
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


/* 判断两个暂存项是否指向同一文件。
 * 路径相同表示同一条目录项；路径不同但 inode 相同表示硬链接。
 * 硬链接必须分别提交，因此调用方只在冲突检测和同路径合并时使用本函数。 */
static FileUpdateIdentity_t
file_update_identity (const StagedFileUpdate_t *a, const StagedFileUpdate_t *b)
{
  if (a->target && b->target && xy_streql (a->target, b->target))
    return FileUpdate_SamePath;

  if (a->has_id && b->has_id && a->dev == b->dev && a->ino == b->ino)
    return FileUpdate_SameFile;

  return FileUpdate_Different;
}

#endif /* !XY_Build_On_Windows */