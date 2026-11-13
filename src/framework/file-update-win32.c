/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Name     : file-update-win32.c
 * File Authors  : @Mikachu2333
 * Contributors  : @ccmywish
 *               |
 * Created On    : <2026-11-12>
 * Last Modified : <2026-11-13>
 *
 * Win32 后端：使用 ReplaceFileW/MoveFileExW 暂存并原子替换目标文件。
 * 已有文件通过 ReplaceFileW 保留目标 ACL；新建文件继承父目录 ACL。
 * 文件系统操作保留 \\?\ 前缀，使超过 MAX_PATH 的路径仍可更新。
 * ------------------------------------------------------------*/

#ifdef XY_Build_On_Windows

static void
file_update_win32_seterrno (DWORD error)
{
  switch (error)
    {
    case ERROR_FILE_NOT_FOUND:
    case ERROR_PATH_NOT_FOUND:
    case ERROR_INVALID_DRIVE:        errno = ENOENT; break;
    case ERROR_ACCESS_DENIED:
    case ERROR_PRIVILEGE_NOT_HELD:
    case ERROR_CANT_ACCESS_FILE:     errno = EACCES; break;
    case ERROR_ALREADY_EXISTS:
    case ERROR_FILE_EXISTS:          errno = EEXIST; break;
    case ERROR_SHARING_VIOLATION:
    case ERROR_LOCK_VIOLATION:       errno = EBUSY;  break;
    case ERROR_DISK_FULL:
    case ERROR_HANDLE_DISK_FULL:     errno = ENOSPC; break;
    case ERROR_WRITE_PROTECT:        errno = EROFS;  break;
    case ERROR_NOT_SAME_DEVICE:      errno = EXDEV;  break;
    case ERROR_INVALID_NAME:
    case ERROR_INVALID_PARAMETER:    errno = EINVAL; break;
    case ERROR_DIRECTORY:            errno = ENOTDIR; break;
    case ERROR_OUTOFMEMORY:
    case ERROR_NOT_ENOUGH_MEMORY:    errno = ENOMEM; break;
    case ERROR_TOO_MANY_OPEN_FILES:  errno = EMFILE; break;
    case ERROR_FILENAME_EXCED_RANGE:
    case ERROR_BUFFER_OVERFLOW:      errno = ENAMETOOLONG; break;
    default:                         errno = EIO;    break;
    }
}


/* 返回带 \\?\ 前缀的最终路径，并输出文件信息。
 * 保留前缀是有意的：去掉后超过 MAX_PATH 的路径会重新受长度限制。
 * @param path 目标路径
 * @param out_info 可为 NULL；非空时写入文件信息
 * @return 调用方负责 free 的宽字符路径；失败返回 NULL 并设置 errno */
static wchar_t *
file_update_win32_final_path (const wchar_t *path, BY_HANDLE_FILE_INFORMATION *out_info)
{
  HANDLE handle = CreateFileW (path, 0,
                               FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                               NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
  if (INVALID_HANDLE_VALUE == handle)
    {
      file_update_win32_seterrno (GetLastError ());
      return NULL;
    }

  BY_HANDLE_FILE_INFORMATION info;
  if (!GetFileInformationByHandle (handle, &info))
    {
      DWORD error = GetLastError ();
      CloseHandle (handle);
      file_update_win32_seterrno (error);
      return NULL;
    }
  if (out_info) *out_info = info;

  DWORD cap = 512;
  wchar_t *buf = NULL;
  for (;;)
    {
      buf = xy_malloc0 ((size_t) cap * sizeof (wchar_t));
      DWORD len = GetFinalPathNameByHandleW (handle, buf, cap, VOLUME_NAME_DOS);
      if (0 == len)
        {
          DWORD error = GetLastError ();
          free (buf);
          CloseHandle (handle);
          file_update_win32_seterrno (error);
          return NULL;
        }
      if (len < cap) break;
      free (buf);
      buf = NULL;
      cap = len + 1;
    }
  CloseHandle (handle);
  return buf;
}


/* 复用 xy.h 的宽字符绝对路径实现，这里只补 errno 映射。 */
static wchar_t *
file_update_win32_full_path (const wchar_t *path)
{
  wchar_t *ret = _xy_win_full_path (path);
  if (!ret)
    file_update_win32_seterrno (GetLastError ());
  return ret;
}


/* 逐级创建 path 的父目录；已存在的目录视为成功。
 * 失败返回 false 并设置 errno。 */
static bool
file_update_win32_create_parent_dirs (const wchar_t *path)
{
  wchar_t *dir = _wcsdup (path);
  if (!dir) { errno = ENOMEM; return false; }

  wchar_t *slash = wcsrchr (dir, L'\\');
  if (!slash) { free (dir); return true; }
  *slash = L'\0';

  size_t len = wcslen (dir);
  if (0 == len) { free (dir); return true; }
  if (2 == len && L':' == dir[1]) { free (dir); return true; }

  size_t start = 0;
  if (len >= 2 && L':' == dir[1])
    {
      start = (len >= 3) ? 3 : len;
    }
  else if (len >= 2 && L'\\' == dir[0] && L'\\' == dir[1])
    {
      wchar_t *share = wcschr (dir + 2, L'\\');
      if (share)
        {
          wchar_t *next = wcschr (share + 1, L'\\');
          start = next ? (size_t) (next - dir + 1) : len;
        }
      else
        {
          start = len;
        }
    }

  for (wchar_t *p = dir + start; ; p++)
    {
      if (L'\\' != *p && L'\0' != *p) continue;
      wchar_t saved = *p;
      *p = L'\0';
      if (0 == CreateDirectoryW (dir, NULL))
        {
          DWORD error = GetLastError ();
          if (ERROR_ALREADY_EXISTS != error)
            {
              *p = saved;
              free (dir);
              file_update_win32_seterrno (error);
              return false;
            }
        }
      *p = saved;
      if (L'\0' == saved) break;
    }
  free (dir);
  return true;
}


/* 在目标文件同目录生成 chsrc-XXXXXX 临时文件；CREATE_NEW 保证不会覆盖
 * 其他进程刚创建的文件，命名冲突时重新生成。 */
static bool
file_update_win32_make_temp (const wchar_t *target, HANDLE *out_handle, wchar_t **out_path)
{
  size_t len = wcslen (target);
  wchar_t *pattern = xy_malloc0 ((len + 16) * sizeof (wchar_t));
  wcscpy (pattern, target);
  wchar_t *slash = wcsrchr (pattern, L'\\');
  if (!slash) { free (pattern); errno = EINVAL; return false; }
  slash[1] = L'\0';
  wcscat (pattern, L"chsrc-XXXXXX");

  for (int i = 0; i < 16; i++)
    {
      wchar_t *candidate = _wcsdup (pattern);
      if (!candidate) { free (pattern); errno = ENOMEM; return false; }
      if (0 != _wmktemp_s (candidate, wcslen (candidate) + 1))
        {
          free (candidate);
          free (pattern);
          errno = EEXIST;
          return false;
        }
      HANDLE handle = CreateFileW (candidate, GENERIC_WRITE, 0, NULL, CREATE_NEW,
                                   FILE_ATTRIBUTE_NORMAL, NULL);
      if (INVALID_HANDLE_VALUE != handle)
        {
          *out_handle = handle;
          *out_path = candidate;
          free (pattern);
          return true;
        }
      DWORD error = GetLastError ();
      free (candidate);
      if (ERROR_FILE_EXISTS != error && ERROR_ALREADY_EXISTS != error)
        {
          free (pattern);
          file_update_win32_seterrno (error);
          return false;
        }
    }
  free (pattern);
  errno = EEXIST;
  return false;
}


/* 写入并 FlushFileBuffers；已有文件复制目标属性，ACL 由 ReplaceFileW 合并。 */
static bool
file_update_win32_write_temp_file (const wchar_t *target, const char *bytes, size_t len,
                                   bool preserve_attrs, DWORD attrs, wchar_t **out_path)
{
  *out_path = NULL;

  HANDLE handle;
  wchar_t *path;
  if (!file_update_win32_make_temp (target, &handle, &path)) return false;

  bool ok = true;
  DWORD error = 0;
  size_t written = 0;
  while (written < len)
    {
      DWORD chunk = (DWORD) ((len - written) > 65536 ? 65536 : (len - written));
      DWORD n = 0;
      if (!WriteFile (handle, bytes + written, chunk, &n, NULL))
        {
          ok = false;
          error = GetLastError ();
          break;
        }
      if (n != chunk)
        {
          ok = false;
          error = ERROR_WRITE_FAULT;
          break;
        }
      written += n;
    }
  if (ok && !FlushFileBuffers (handle)) { ok = false; error = GetLastError (); }
  if (!CloseHandle (handle) && ok) { ok = false; error = GetLastError (); }
  if (!ok)
    {
      DeleteFileW (path);
      free (path);
      file_update_win32_seterrno (error ? error : ERROR_WRITE_FAULT);
      return false;
    }

  if (preserve_attrs)
    {
      DWORD keep = attrs & (FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_HIDDEN
                            | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_NOT_CONTENT_INDEXED);
      if (0 != keep && !SetFileAttributesW (path, keep))
        {
          error = GetLastError ();
          DeleteFileW (path);
          free (path);
          file_update_win32_seterrno (error);
          return false;
        }
    }

  *out_path = path;
  return true;
}


/* 解析已有符号链接/重解析点，替换最终目标以保留链接本身；再把新内容与
 * 回滚内容写入同目录临时文件。旧快照已被外部修改时以 EAGAIN 拒绝整批更新。
 * 记录卷序列号与文件索引，供重复检测识别硬链接。 */
static bool
file_update_stage (const FileUpdate_t *op, StagedFileUpdate_t *stage)
{
  char *norm = xy_normalize_path (op->norm_path);
  if (!norm) { errno = ENOMEM; return false; }
  wchar_t *wnorm = xy_utf8_to_wstr (norm);
  free (norm);
  if (!wnorm) { errno = ENOMEM; return false; }

  bool ok = false;
  if (op->existed)
    {
      BY_HANDLE_FILE_INFORMATION info;
      wchar_t *target = file_update_win32_final_path (wnorm, &info);
      if (!target) goto done;
      if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
          errno = EINVAL;
          free (target);
          goto done;
        }

      HANDLE access = CreateFileW (target, GENERIC_WRITE,
                                   FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                                   NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
      if (INVALID_HANDLE_VALUE == access)
        {
          file_update_win32_seterrno (GetLastError ());
          free (target);
          goto done;
        }
      CloseHandle (access);

      stage->target_w = target;
      stage->target = xy_wstr_to_utf8 (target);
      if (!stage->target) { errno = ENOMEM; goto done; }

      stage->has_id = true;
      stage->volume = info.dwVolumeSerialNumber;
      stage->index_high = info.nFileIndexHigh;
      stage->index_low = info.nFileIndexLow;

      char *current = NULL;
      size_t current_len = 0;
      if (!xy_file_read_bytes (stage->target, &current, &current_len)) goto done;
      bool same = current_len == op->old_len
               && (0 == current_len || 0 == memcmp (current, op->old_content, current_len));
      free (current);
      if (!same) { errno = EAGAIN; goto done; }

      if (!file_update_win32_write_temp_file (target, op->new_content, op->new_len,
                                              true, info.dwFileAttributes,
                                              &stage->replacement_w)) goto done;
      stage->replacement = xy_wstr_to_utf8 (stage->replacement_w);
      if (!stage->replacement) { errno = ENOMEM; goto done; }

      if (!file_update_win32_write_temp_file (target, op->old_content, op->old_len,
                                              true, info.dwFileAttributes,
                                              &stage->rollback_w)) goto done;
      stage->rollback = xy_wstr_to_utf8 (stage->rollback_w);
      if (!stage->rollback) { errno = ENOMEM; goto done; }
    }
  else
    {
      DWORD attrs = GetFileAttributesW (wnorm);
      if (INVALID_FILE_ATTRIBUTES != attrs) { errno = EEXIST; goto done; }
      DWORD error = GetLastError ();
      if (ERROR_FILE_NOT_FOUND != error && ERROR_PATH_NOT_FOUND != error)
        {
          file_update_win32_seterrno (error);
          goto done;
        }

      /* GetFileAttributesW 对悬空重解析点也返回失败，这里用
       * OPEN_REPARSE_POINT 再确认路径没有被占用。 */
      HANDLE occupied = CreateFileW (
        wnorm, 0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL,
        OPEN_EXISTING, FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_BACKUP_SEMANTICS, NULL);
      if (INVALID_HANDLE_VALUE != occupied)
        {
          CloseHandle (occupied);
          errno = EEXIST;
          goto done;
        }
      error = GetLastError ();
      if (ERROR_FILE_NOT_FOUND != error && ERROR_PATH_NOT_FOUND != error)
        {
          file_update_win32_seterrno (error);
          goto done;
        }

      wchar_t *full = file_update_win32_full_path (wnorm);
      if (!full) goto done;

      wchar_t *parent = _wcsdup (full);
      if (!parent) { free (full); errno = ENOMEM; goto done; }
      wchar_t *slash = wcsrchr (parent, L'\\');
      size_t base_off = slash ? (size_t) (slash - parent) + 1 : 0;
      const wchar_t *base = full + base_off;
      if (slash) *slash = L'\0';
      if (L'\0' == *base || 0 == wcscmp (base, L".") || 0 == wcscmp (base, L".."))
        {
          free (parent);
          free (full);
          errno = EINVAL;
          goto done;
        }

      /* 先校验最终组件，再创建父目录，避免非法计划留下空目录。 */
      if (!file_update_win32_create_parent_dirs (full))
        {
          free (parent);
          free (full);
          goto done;
        }

      wchar_t *parent_final = file_update_win32_final_path (parent, NULL);
      free (parent);
      if (!parent_final) { free (full); goto done; }

      size_t parent_len = wcslen (parent_final);
      size_t base_len = wcslen (base);
      bool need_sep = parent_len > 0 && L'\\' != parent_final[parent_len - 1];
      wchar_t *target = xy_malloc0 ((parent_len + (need_sep ? 1 : 0) + base_len + 1)
                                    * sizeof (wchar_t));
      memcpy (target, parent_final, parent_len * sizeof (wchar_t));
      if (need_sep) target[parent_len++] = L'\\';
      memcpy (target + parent_len, base, (base_len + 1) * sizeof (wchar_t));
      free (parent_final);
      free (full);

      stage->target_w = target;
      stage->target = xy_wstr_to_utf8 (target);
      if (!stage->target) { errno = ENOMEM; goto done; }

      if (!file_update_win32_write_temp_file (target, op->new_content, op->new_len,
                                              false, 0, &stage->replacement_w)) goto done;
      stage->replacement = xy_wstr_to_utf8 (stage->replacement_w);
      if (!stage->replacement) { errno = ENOMEM; goto done; }
    }
  ok = true;

done:
  free (wnorm);
  return ok;
}


/* ReplaceFileW 合并目标 ACL 失败时返回 ERROR_ACCESS_DENIED。受限环境可能
 * 注入无法合并的拒绝 ACE；此时退回忽略 ACL 合并，由新文件继承目录 ACL。
 * 正常 ACL 下仍走第一条路径，保留目标 ACL。 */
static BOOL
file_update_win32_replace (const wchar_t *target, const wchar_t *replacement)
{
  if (ReplaceFileW (target, replacement, NULL, REPLACEFILE_WRITE_THROUGH, NULL, NULL))
    return TRUE;

  DWORD error = GetLastError ();
  if (ERROR_ACCESS_DENIED != error)
    {
      SetLastError (error);
      return FALSE;
    }
  return ReplaceFileW (target, replacement, NULL,
                       REPLACEFILE_WRITE_THROUGH | REPLACEFILE_IGNORE_ACL_ERRORS, NULL, NULL);
}


static bool
file_update_commit (StagedFileUpdate_t *stage, const FileUpdate_t *op)
{
  BOOL ok = op->existed
    ? file_update_win32_replace (stage->target_w, stage->replacement_w)
    : MoveFileExW (stage->replacement_w, stage->target_w, MOVEFILE_WRITE_THROUGH);
  if (!ok)
    {
      file_update_win32_seterrno (GetLastError ());
      return false;
    }
  free (stage->replacement);
  stage->replacement = NULL;
  free (stage->replacement_w);
  stage->replacement_w = NULL;
  return true;
}


static bool
file_update_rollback (StagedFileUpdate_t *stage, const FileUpdate_t *op)
{
  BOOL ok = op->existed
    ? file_update_win32_replace (stage->target_w, stage->rollback_w)
    : DeleteFileW (stage->target_w);
  if (!ok)
    {
      file_update_win32_seterrno (GetLastError ());
      return false;
    }
  if (op->existed)
    {
      free (stage->rollback);
      stage->rollback = NULL;
      free (stage->rollback_w);
      stage->rollback_w = NULL;
    }
  return true;
}


static void
file_update_discard (StagedFileUpdate_t *stage)
{
  if (stage->replacement_w) DeleteFileW (stage->replacement_w);
  if (stage->rollback_w && !stage->keep_rollback) DeleteFileW (stage->rollback_w);
  free (stage->target);
  free (stage->replacement);
  free (stage->rollback);
  free (stage->target_w);
  free (stage->replacement_w);
  free (stage->rollback_w);
}


/* 判断两个暂存项是否指向同一文件。
 * 路径相同（大小写不敏感）表示同一条目录项；路径不同但卷序列号与文件
 * 索引相同表示硬链接。硬链接必须分别提交，因此仅在冲突检测时使用。 */
static FileUpdateIdentity_t
file_update_identity (const StagedFileUpdate_t *a, const StagedFileUpdate_t *b)
{
  if (a->target_w && b->target_w)
    {
      int result = CompareStringOrdinal (a->target_w, -1, b->target_w, -1, TRUE);
      bool same_path = 0 == result ? 0 == _wcsicmp (a->target_w, b->target_w)
                                   : CSTR_EQUAL == result;
      if (same_path) return FileUpdate_SamePath;
    }

  if (a->has_id && b->has_id
      && a->volume == b->volume
      && a->index_high == b->index_high
      && a->index_low == b->index_low)
    return FileUpdate_SameFile;

  return FileUpdate_Different;
}

#endif /* XY_Build_On_Windows */
