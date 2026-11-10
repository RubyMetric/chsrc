/** ------------------------------------------------------------
 * SPDX-License-Identifier: MIT
 * -------------------------------------------------------------
 * Test normalized paths, target types and query errors using real filesystem objects.
 * ------------------------------------------------------------*/

#ifndef _WIN32
  #define _POSIX_C_SOURCE 200809L
#endif

#include "xy.h"

#ifdef XY_Build_On_Windows
  #include <direct.h>
#else
  #include <sys/socket.h>
  #include <sys/un.h>
  #include <sys/wait.h>
#endif


static void
check_path (const char *path, bool exists, bool is_dir, bool is_file, bool has_error)
{
  bool actual_exists = xy_path_is_exist (path);
  bool actual_dir = xy_path_is_dir (path);
  bool actual_file = xy_path_is_file (path);
  bool actual_error = xy_path_has_error (path);
  if (actual_exists != exists || actual_dir != is_dir || actual_file != is_file || actual_error != has_error)
    fprintf (stderr, "path=%s: exists=%d dir=%d file=%d error=%d\n",
             path ? path : "(NULL)", actual_exists, actual_dir, actual_file, actual_error);
  assert (actual_exists == exists);
  assert (actual_dir == is_dir);
  assert (actual_file == is_file);
  assert (actual_error == has_error);
  assert (xy_file_exist (path) == is_file);
  assert (xy_dir_exist (path) == is_dir);
}


static void
create_file (const char *path)
{
  FILE *file = xy_fopen (path, "wb");
  assert (file);
  assert (fputs ("path test\n", file) >= 0);
  assert (0 == fclose (file));
}


#ifdef XY_Build_On_Windows
static bool
create_link (const wchar_t *path, const wchar_t *target, DWORD flags)
{
  if (CreateSymbolicLinkW (path, target, flags | SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE))
    return true;
  if (ERROR_INVALID_PARAMETER == GetLastError ())
    return CreateSymbolicLinkW (path, target, flags);
  return false;
}
#endif


static void
test_links (void)
{
#ifdef XY_Build_On_Windows
  if (!create_link (L"file-link", L"file", 0))
    {
      DWORD error = GetLastError ();
      assert (ERROR_PRIVILEGE_NOT_HELD == error || ERROR_NOT_SUPPORTED == error);
      fprintf (stderr, "SKIP symbolic links: Windows error %lu\n", (unsigned long) error);
      return;
    }
  assert (create_link (L"file-link-chain", L"file-link", 0));
  assert (create_link (L"dir-link", L"dir", SYMBOLIC_LINK_FLAG_DIRECTORY));
  assert (create_link (L"missing-link", L"missing", 0));
  assert (create_link (L"missing-dir-link", L"missing-dir", SYMBOLIC_LINK_FLAG_DIRECTORY));
  assert (create_link (L"loop", L"loop", 0));
#else
  assert (0 == symlink ("file", "file-link"));
  assert (0 == symlink ("file-link", "file-link-chain"));
  assert (0 == symlink ("dir", "dir-link"));
  assert (0 == symlink ("missing", "missing-link"));
  assert (0 == symlink ("missing-dir", "missing-dir-link"));
  assert (0 == symlink ("loop", "loop"));
#endif

  check_path ("file-link", true, false, true, false);
  check_path ("file-link-chain", true, false, true, false);
  check_path ("dir-link", true, true, false, false);
  check_path ("missing-link", false, false, false, false);
  check_path ("missing-dir-link", false, false, false, false);
  check_path ("loop", false, false, false, true);

  /* 删除目标后，已有链接应变为不存在，而不是继续报告链接自身的类型。 */
  create_file ("temporary-target");
#ifdef XY_Build_On_Windows
  assert (create_link (L"temporary-link", L"temporary-target", 0));
#else
  assert (0 == symlink ("temporary-target", "temporary-link"));
#endif
  check_path ("temporary-link", true, false, true, false);
  assert (0 == xy_file_remove ("temporary-target"));
  check_path ("temporary-link", false, false, false, false);

  assert (0 == xy_file_remove ("file-link"));
  assert (0 == xy_file_remove ("file-link-chain"));
  assert (0 == xy_file_remove ("missing-link"));
  assert (0 == xy_file_remove ("loop"));
  assert (0 == xy_file_remove ("temporary-link"));
#ifdef XY_Build_On_Windows
  assert (RemoveDirectoryW (L"dir-link"));
  assert (RemoveDirectoryW (L"missing-dir-link"));
#else
  assert (0 == unlink ("dir-link"));
  assert (0 == unlink ("missing-dir-link"));
#endif
}


#ifndef XY_Build_On_Windows
static void
check_permissions (void)
{
  /* 无读写权限不影响对象本身的类型判断；父目录不可搜索则无法查询其子项。 */
  check_path ("file", true, false, true, false);
  check_path ("dir", true, true, false, false);
  check_path ("dir/child", false, false, false, true);
  check_path ("dir/missing", false, false, false, true);
}


static void
test_permissions (void)
{
  create_file ("dir/child");
  assert (0 == chmod ("file", 0));
  assert (0 == chmod ("dir", 0));
  if (0 == geteuid ())
    {
      /* root 会绕过权限检查，仅在子进程中降权，确保实际触发 EACCES。 */
      pid_t child = fork ();
      assert (child >= 0);
      if (0 == child)
        {
          assert (0 == setgid (65534));
          assert (0 == setuid (65534));
          check_permissions ();
          _exit (0);
        }
      int status;
      assert (child == waitpid (child, &status, 0));
      assert (WIFEXITED (status) && 0 == WEXITSTATUS (status));
    }
  else
    {
      check_permissions ();
    }
  assert (0 == chmod ("file", 0600));
  assert (0 == chmod ("dir", 0700));
  assert (0 == xy_file_remove ("dir/child"));
}


static void
test_special_files (void)
{
  assert (0 == mkfifo ("fifo", 0600));
  check_path ("fifo", true, false, false, false);
  check_path ("/dev/null", true, false, false, false);

  int fd = socket (AF_UNIX, SOCK_STREAM, 0);
  assert (fd >= 0);
  struct sockaddr_un address = {0};
  address.sun_family = AF_UNIX;
  strcpy (address.sun_path, "socket");
  assert (0 == bind (fd, (struct sockaddr *) &address, sizeof (address)));
  check_path ("socket", true, false, false, false);
  assert (0 == close (fd));
  assert (0 == xy_file_remove ("socket"));
  assert (0 == xy_file_remove ("fifo"));
}
#endif


int
main (void)
{
  xy_init ();

#ifdef XY_Build_On_Windows
  wchar_t previous_dir[MAX_PATH];
  wchar_t temp_dir[MAX_PATH];
  DWORD cwd_length = GetCurrentDirectoryW (MAX_PATH, previous_dir);
  assert (cwd_length > 0 && cwd_length < MAX_PATH);
  int length = swprintf (temp_dir, MAX_PATH, L"%ls\\xy-path-%lu.tmp", previous_dir, GetCurrentProcessId ());
  assert (length > 0 && length < MAX_PATH);
  assert (CreateDirectoryW (temp_dir, NULL));
  assert (SetCurrentDirectoryW (temp_dir));
  assert (CreateDirectoryW (L"dir", NULL));
  /* 在首次使用 xy_os_home 缓存前设置测试进程自己的 HOME。 */
  assert (0 == _wputenv_s (L"USERPROFILE", temp_dir));
  assert (0 == _wputenv_s (L"XY_PATH_TEST_ROOT", temp_dir));
#else
  char previous_dir[4096];
  char temp_dir[] = "/tmp/chsrc-xy-path-XXXXXX";
  assert (getcwd (previous_dir, sizeof (previous_dir)));
  assert (mkdtemp (temp_dir));
  assert (0 == chmod (temp_dir, 0755));
  assert (0 == chdir (temp_dir));
  assert (0 == mkdir ("dir", 0700));
  assert (0 == setenv ("HOME", temp_dir, 1));
#endif

  create_file ("file");
  create_file ("空 格.txt");
  create_file ("100%.txt");
  check_path ("file", true, false, true, false);
  check_path (" \t ./空 格.txt \r\n", true, false, true, false);
  check_path ("100%.txt", true, false, true, false);
  check_path ("dir", true, true, false, false);
  check_path (" \t dir/ \n", true, true, false, false);
  check_path ("missing", false, false, false, false);
  check_path ("missing/child", false, false, false, false);
  check_path ("file/child", false, false, false, false);
  check_path (NULL, false, false, false, true);
  check_path ("", false, false, false, true);
  check_path (" \t\r\n", false, false, false, true);
  check_path (" ~ ", true, true, false, false);
  check_path (" ~/file ", true, false, true, false);
  check_path ("~/missing", false, false, false, false);

  test_links ();

#ifdef XY_Build_On_Windows
  check_path ("%XY_PATH_TEST_ROOT%", true, true, false, false);
  check_path (" %XY_PATH_TEST_ROOT%/file ", true, false, true, false);
  check_path ("%XY_PATH_TEST_ROOT%/missing", false, false, false, false);
  check_path ("%XY_PATH_TEST_ROOT%/bad?name", false, false, false, true);
  assert (0 == _wputenv_s (L"XY_PATH_TEST_FILE", L"dir/../空 格.txt"));
  char *expanded = xy_normalize_path (" %XY_PATH_TEST_FILE% ");
  assert_str (expanded, "dir\\..\\空 格.txt");
  free (expanded);
  check_path (" %XY_PATH_TEST_FILE% ", true, false, true, false);
  check_path ("NUL", true, false, false, false);
#else
  test_special_files ();
  test_permissions ();
#endif

  assert (0 == xy_file_remove ("file"));
  assert (0 == xy_file_remove ("空 格.txt"));
  assert (0 == xy_file_remove ("100%.txt"));
#ifdef XY_Build_On_Windows
  assert (RemoveDirectoryW (L"dir"));
  assert (SetCurrentDirectoryW (previous_dir));
  assert (RemoveDirectoryW (temp_dir));
#else
  assert (0 == rmdir ("dir"));
  assert (0 == chdir (previous_dir));
  assert (0 == rmdir (temp_dir));
#endif

  puts ("xy path tests passed");
  return 0;
}
