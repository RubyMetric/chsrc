/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * 通用文件更新测试：刻意不引入 env.c 或任何配置格式解析器。
 * ------------------------------------------------------------*/

#define Chsrc_Version "chsrc-file-update-tester"
#define CHSRC_TESTING

#include "../src/framework/base.c"
#include "../src/framework/OS.c"
#include "../src/framework/file-update.c"

#ifndef XY_Build_On_Windows
static char Test_Root[] = "/tmp/chsrc_file_update_XXXXXX";

static char *
new_path (const char *name)
{
  return xy_path_join (Test_Root, name);
}

static void
overwrite_file_with_bytes (const char *path, const char *bytes, size_t len)
{
  FILE *fp = fopen (path, "wb");
  assert (NULL != fp);
  assert (len == fwrite (bytes, 1, len, fp));
  assert (0 == fclose (fp));
}

static void
assert_file_with_bytes (const char *path, const char *expected, size_t expected_len)
{
  char *actual = NULL;
  size_t len = 0;
  assert (chsrc_read_file_bytes (path, &actual, &len));
  assert (expected_len == len);
  assert (0 == len || 0 == memcmp (actual, expected, len));
  assert ('\0' == actual[len]);
  free (actual);
}

static FileUpdate_t
plan_file_update (char *path, char *content, size_t len)
{
  FileUpdate_t op = {
    .display_path = path, .norm_path = path, .existed = xy_file_exist (path),
    .new_content = content, .new_len = len
  };
  if (op.existed)
    assert (chsrc_read_file_bytes (path, &op.old_content, &op.old_len));
  return op;
}

static size_t
get_TestRootDir_entries_count (void)
{
  DIR *dir = opendir (Test_Root);
  assert (NULL != dir);
  size_t count = 0;
  struct dirent *entry;
  while (NULL != (entry = readdir (dir)))
    {
      if (xy_streql (entry->d_name, ".") || xy_streql (entry->d_name, "..")) continue;
      /* 暂存及回滚临时文件不能在成功或已处理的失败后残留。 */
      assert (NULL == strstr (entry->d_name, ".chsrc-"));
      count++;
    }
  assert (0 == closedir (dir));
  return count;
}

static void
test_binary_update_and_noop (void)
{
  char old[9001], desired[9001];
  for (size_t i = 0; i < sizeof (old); i++) old[i] = desired[i] = (char) (i % 251);
  desired[8000] = '!';
  char *path = new_path ("binary config");
  overwrite_file_with_bytes (path, old, sizeof (old));
  FileUpdate_t op = plan_file_update (path, desired, sizeof (desired));
  char *err = NULL;
  assert (FileUpdate_OK == chsrc_update_files (&op, 1, &err));
  assert (NULL == err);
  assert_file_with_bytes (path, desired, sizeof (desired));
  free (op.old_content);

  op = plan_file_update (path, desired, sizeof (desired));
  struct stat before, after;
  assert (0 == stat (path, &before));
  size_t entries = get_TestRootDir_entries_count ();
  assert (FileUpdate_Unchanged == chsrc_update_files (&op, 1, &err));
  assert (NULL == err);
  assert (entries == get_TestRootDir_entries_count ());
  assert (0 == stat (path, &after));
  assert (before.st_ino == after.st_ino && before.st_mtime == after.st_mtime);
  assert (FileUpdate_Unchanged == chsrc_update_files (NULL, 0, &err));
  free (op.old_content);
  free (path);
}

static void
test_dry_run_has_no_side_effects (void)
{
  char *path = new_path ("dry.json");
  char *missing = new_path ("missing-parent/config.json");
  char *parent = new_path ("missing-parent");
  char original[] = "{\"registry\":\"old\"}\r\n";
  char desired[] = "{\"registry\":\"new\"}\r\n";
  overwrite_file_with_bytes (path, original, strlen (original));
  FileUpdate_t ops[] = { plan_file_update (path, desired, strlen (desired)),
                         plan_file_update (missing, desired, strlen (desired)) };
  size_t entries = get_TestRootDir_entries_count ();
  ProgMode.DryRunMode = true;
  char *err = NULL;
  assert (FileUpdate_OK == chsrc_update_files (ops, 2, &err));
  /* 立即改回非 Dry Run 模式，避免影响后续测试。 */
  ProgMode.DryRunMode = false;
  assert (NULL == err);
  assert_file_with_bytes (path, original, strlen (original));
  assert (!xy_path_exist (parent));
  assert (entries == get_TestRootDir_entries_count ());
  free (ops[0].old_content);
  free (path);
  free (missing);
  free (parent);
}

static void
test_commit_rollback (void)
{
  char *first = new_path ("commit-first");
  char *second = new_path ("commit-second");
  overwrite_file_with_bytes (first, "first original\n", 15);
  overwrite_file_with_bytes (second, "second original\n", 16);
  assert (0 == chmod (first, 0640));
  FileUpdate_t ops[2] = { plan_file_update (first, "new\n", 4), plan_file_update (second, "new\n", 4) };
  char *err = NULL;
  FileUpdate_Test_Fail_Commit = 1;
  assert (FileUpdate_Failed == chsrc_update_files (ops, 2, &err));
  assert (NULL != err);
  free (err);
  assert_file_with_bytes (first, "first original\n", 15);
  assert_file_with_bytes (second, "second original\n", 16);
  struct stat st;
  assert (0 == stat (first, &st));
  assert (0640 == (st.st_mode & 0777));
  get_TestRootDir_entries_count ();

  /* 新建文件在后续失败时回滚为不存在。 */
  assert (0 == unlink (first));
  free (ops[0].old_content);
  ops[0] = plan_file_update (first, "new\n", 4);
  assert (FileUpdate_Failed == chsrc_update_files (ops, 2, &err));
  free (err);
  assert (!xy_file_exist (first));
  assert_file_with_bytes (second, "second original\n", 16);
  get_TestRootDir_entries_count ();
  FileUpdate_Test_Fail_Commit = -1;
  mode_t old_mask = umask (0022);
  assert (FileUpdate_OK == chsrc_update_files (ops, 2, &err));
  umask (old_mask);
  assert (NULL == err);
  assert (0 == stat (first, &st));
  assert (0644 == (st.st_mode & 0777));
  assert_file_with_bytes (first, "new\n", 4);
  assert_file_with_bytes (second, "new\n", 4);
  free (ops[1].old_content);
  free (first);
  free (second);
}

static void
test_stale_snapshot_rejects_batch (void)
{
  char *first = new_path ("stale-first");
  char *second = new_path ("stale-second");
  overwrite_file_with_bytes (first, "old", 3);
  overwrite_file_with_bytes (second, "old", 3);
  FileUpdate_t ops[] = { plan_file_update (first, "new", 3), plan_file_update (second, "new", 3) };
  overwrite_file_with_bytes (second, "edited by user", 14);
  size_t entries = get_TestRootDir_entries_count ();
  char *err = NULL;
  assert (FileUpdate_Failed == chsrc_update_files (ops, 2, &err));
  assert (NULL != err);
  free (err);
  assert_file_with_bytes (first, "old", 3);
  assert_file_with_bytes (second, "edited by user", 14);
  assert (entries == get_TestRootDir_entries_count ());
  free (ops[0].old_content);
  free (ops[1].old_content);
  free (first);
  free (second);
}

static void
test_symlink_and_duplicate_targets (void)
{
  char *target = new_path ("link-target");
  char *link = new_path ("config-link");
  overwrite_file_with_bytes (target, "old", 3);
  assert (0 == chmod (target, 0640));
  assert (0 == symlink (target, link));
  FileUpdate_t op = plan_file_update (link, "new", 3);
  char *err = NULL;
  assert (FileUpdate_OK == chsrc_update_files (&op, 1, &err));
  struct stat st;
  assert (0 == lstat (link, &st) && S_ISLNK (st.st_mode));
  assert (0 == stat (target, &st) && 0640 == (st.st_mode & 0777));
  assert_file_with_bytes (target, "new", 3);
  free (op.old_content);

  FileUpdate_t ops[] = { plan_file_update (link, "first", 5), plan_file_update (target, "second", 6) };
  size_t entries = get_TestRootDir_entries_count ();
  assert (FileUpdate_Failed == chsrc_update_files (ops, 2, &err));
  free (err);
  assert_file_with_bytes (target, "new", 3);
  assert (entries == get_TestRootDir_entries_count ());
  ops[1].new_content = ops[0].new_content;
  ops[1].new_len = ops[0].new_len;
  assert (FileUpdate_OK == chsrc_update_files (ops, 2, &err));
  assert (NULL == err);
  assert_file_with_bytes (target, "first", 5);
  assert (0 == lstat (link, &st) && S_ISLNK (st.st_mode));
  free (ops[0].old_content);
  free (ops[1].old_content);
  free (target);
  free (link);
}

/* 两个不同路径指向同一个新建文件时，内容冲突必须整批拒绝。 */
static void
test_duplicate_new_paths (void)
{
  char *missing = new_path ("duplicate-new");
  char *alias = new_path ("./duplicate-new");
  FileUpdate_t ops[] = { plan_file_update (missing, "first", 5),
                         plan_file_update (alias, "second", 6) };
  size_t entries = get_TestRootDir_entries_count ();
  char *err = NULL;
  assert (FileUpdate_Failed == chsrc_update_files (ops, 2, &err));
  free (err);
  assert (!xy_path_exist (missing));
  assert (entries == get_TestRootDir_entries_count ());
  free (missing);
  free (alias);
}

static void
test_empty_content (void)
{
  char *path = new_path ("empty-config");
  FileUpdate_t op = plan_file_update (path, NULL, 0);
  char *err = NULL;
  assert (FileUpdate_Unchanged == chsrc_update_files (&op, 1, &err));
  assert (!xy_path_exist (path));
  overwrite_file_with_bytes (path, "old", 3);
  op = plan_file_update (path, NULL, 0);
  assert (FileUpdate_OK == chsrc_update_files (&op, 1, &err));
  assert_file_with_bytes (path, "", 0);
  free (op.old_content);
  free (path);
}

/* 悬空符号链接不能被跟随，也不能顺带创建其目标。 */
static void
test_dangling_symlink_target (void)
{
  char *link = new_path ("dangling-link");
  char *missing = new_path ("missing-target");
  assert (0 == symlink (missing, link));
  FileUpdate_t op = plan_file_update (link, "new", 3);
  size_t entries = get_TestRootDir_entries_count ();
  char *err = NULL;
  assert (FileUpdate_Failed == chsrc_update_files (&op, 1, &err));
  free (err);
  struct stat st;
  assert (0 == lstat (link, &st) && S_ISLNK (st.st_mode));
  assert (!xy_path_exist (missing));
  assert (entries == get_TestRootDir_entries_count ());
  free (link);
  free (missing);
}

/* 缺失计划或把目录当作文件时，必须在写入前拒绝。 */
static void
test_invalid_update_plans (void)
{
  size_t entries = get_TestRootDir_entries_count ();
  char *err = NULL;
  assert (FileUpdate_Failed == chsrc_update_files (NULL, 1, &err));
  assert (NULL != err);
  free (err);
  err = NULL;
  FileUpdate_t op = { .display_path = Test_Root, .norm_path = Test_Root,
                      .existed = true, .new_content = "new", .new_len = 3 };
  assert (FileUpdate_Failed == chsrc_update_files (&op, 1, &err));
  assert (NULL != err);
  free (err);
  assert (entries == get_TestRootDir_entries_count ());
}
#endif

int
main (void)
{
#ifndef XY_Build_On_Windows
  assert (NULL != mkdtemp (Test_Root));
  assert (0 == setenv ("HOME", Test_Root, 1));
  xy_init ();
  setvbuf (stdout, NULL, _IOLBF, 0);
  test_binary_update_and_noop ();
  test_dry_run_has_no_side_effects ();
  test_commit_rollback ();
  test_stale_snapshot_rejects_batch ();
  test_symlink_and_duplicate_targets ();
  test_duplicate_new_paths ();
  test_empty_content ();
  test_dangling_symlink_target ();
  test_invalid_update_plans ();
  get_TestRootDir_entries_count ();
  /* Test_Root 只能来自上面的固定 mkdtemp 模板；失败时保留现场。 */
  char *cleanup = xy_strcat (3, "rm -rf -- \"", Test_Root, "\"");
  assert (0 == system (cleanup));
  free (cleanup);
  puts ("file-update: all 9 test groups passed");
#else
  puts ("file-update: POSIX update tests skipped on Windows");
#endif
  return 0;
}
