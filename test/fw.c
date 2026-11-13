/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : @ccmywish
 * Contributors  : @Mikachu2333
 * Created On    : <2024-12-14>
 * Last Modified : <2026-10-28>
 *
 * 该文件最好启用 DEBUG mode 编译
 * ------------------------------------------------------------*/

#define Chsrc_Version "chsrc-framework-tester"
#define CHSRC_TESTING

#include "../src/framework/base.c"
#include "../src/framework/OS.c"
#include "../src/framework/file-update.c"
#include "../src/framework/env.c"

#ifndef XY_Build_On_Windows
static char Test_Home[] = "/tmp/chsrc_fw_env_XXXXXX";

/* 必须在任何路径查询（包括 xy_os_home 的首次缓存）之前调用。 */
static void
test_isolate_home (void)
{
  assert (NULL != mkdtemp (Test_Home));
  assert (0 == setenv ("HOME", Test_Home, 1));
  assert (0 == unsetenv ("ZDOTDIR"));
  assert (0 == unsetenv ("XDG_CONFIG_HOME"));
}
#endif

static void
test_env_utf16_and_reg_serializer (void)
{
  /* 1. UTF-8 <-> UTF-16LE 往返测试（覆盖 ASCII、中文与代理对 Emoji） */
  const char *sample = "C:\\Users\\张三\\Mirror🦀Test";
  uint16_t   *u16    = NULL;
  size_t      u16_n  = 0;
  env_utf8_to_utf16le (sample, &u16, &u16_n);
  assert (NULL != u16);
  assert (u16_n > 0);

  char *roundtrip = env_utf16le_to_utf8 (u16, u16_n);
  assert_str (roundtrip, sample);
  free (roundtrip);
  free (u16);

  /* 2. .reg UTF-16LE + BOM 序列化校验（覆盖 REG_SZ 转义、删除标记与 hex(2)） */
  unsigned char expand_raw[] = { '%', 0x00, 'T', 0x00, '%', 0x00, 0x00, 0x00 };
  EnvRegSnapshot_t snaps[3] = {
    { "RUSTUP_DIST_SERVER", true,  1, "https://a.org/path\\\"q", NULL,       0                   },
    { "NEW_ADDED_VAR",      false, 0, NULL,                      NULL,       0                   },
    { "EXPAND_VAR",         true,  2, NULL,                      expand_raw, sizeof (expand_raw) }
  };

  size_t         reg_len   = 0;
  unsigned char *reg_bytes = env_build_reg_backup_bytes (
    "HKEY_CURRENT_USER\\Environment", snaps, 3, &reg_len);

  assert (reg_len >= 4);
  assert (0xFF == reg_bytes[0] && 0xFE == reg_bytes[1]);

  size_t    units = (reg_len - 2) / 2;
  uint16_t *dec   = xy_malloc0 ((units + 1) * sizeof (uint16_t));
  for (size_t i = 0; i < units; i++)
    {
      dec[i] = (uint16_t) reg_bytes[2 + i * 2] | ((uint16_t) reg_bytes[2 + i * 2 + 1] << 8);
    }
  char *reg_txt = env_utf16le_to_utf8 (dec, units);
  free (dec);
  free (reg_bytes);

  assert (NULL != strstr (reg_txt, "Windows Registry Editor Version 5.00\r\n\r\n[HKEY_CURRENT_USER\\Environment]\r\n"));
  assert (NULL != strstr (reg_txt, "\"RUSTUP_DIST_SERVER\"=\"https://a.org/path\\\\\\\"q\"\r\n"));
  assert (NULL != strstr (reg_txt, "\"NEW_ADDED_VAR\"=-\r\n"));
  assert (NULL != strstr (reg_txt, "\"EXPAND_VAR\"=hex(2):25,00,54,00,25,00,00,00\r\n"));
  free (reg_txt);
}

#ifndef XY_Build_On_Windows
static void
test_env_zsh_startup_paths (void)
{
  ProgMode.Scope = UserScope;
  Env_Test_Force_Shell = PosixShell_Zsh;
  EnvKV_t kv = { "ZSH_TEST_MIRROR", "https://example.org" };
  char *err = NULL;
  assert (!xy_file_exist ("~/.zshrc"));
  assert (!xy_file_exist ("~/.profile"));
  assert (Env_OK == env_apply_kvs (&kv, 1, NULL, &err));
  assert (NULL == err);
  assert (xy_file_exist ("~/.zshrc"));
  assert (xy_file_exist ("~/.zprofile"));
  assert (!xy_file_exist ("~/.profile"));

  char *custom = xy_2strcat (Test_Home, "/custom zsh");
  assert (0 == setenv ("ZDOTDIR", custom, 1));
  assert (Env_OK == env_apply_kvs (&kv, 1, NULL, &err));
  char *rc = xy_path_join (custom, ".zshrc");
  assert (xy_file_exist (rc));
  bool found = false;
  char *value = NULL;
  assert (Env_OK == env_query_persisted (kv.name, &found, &value, &err));
  assert (found);
  assert_str (value, kv.value);
  free (value);
  free (rc);
  free (custom);
  assert (0 == unsetenv ("ZDOTDIR"));
  Env_Test_Force_Shell = -1;
}

static void
test_env_posix_backend (void)
{
  const char *tmp_home = Test_Home;
  assert_str (xy_os_home, tmp_home);
  char *expected_rc = xy_2strcat (tmp_home, "/.bashrc");
  char *actual_rc = xy_normalize_path ("~/.bashrc");
  assert_str (actual_rc, expected_rc);
  free (actual_rc);
  free (expected_rc);
  Scope_t saved_scope   = ProgMode.Scope;
  bool saved_dry        = ProgMode.DryRunMode;

  ProgMode.Scope = UserScope;

  /* 1. 变量名与危险字符校验 */
  Env_Test_Force_Shell = PosixShell_Bash;
  char *err = NULL;

  EnvKV_t bad_name1 = { "1BAD", "ok" };
  assert (Env_BadName == env_apply_kvs (&bad_name1, 1, NULL, &err));
  free (err); err = NULL;

  EnvKV_t bad_name2 = { "BAD-NAME", "ok" };
  assert (Env_BadName == env_apply_kvs (&bad_name2, 1, NULL, &err));
  free (err); err = NULL;

  const char *bad_vals[] = {
    "https://a.org/$(id)",
    "https://a.org/`id`",
    "https://a.org/\"inj",
    "https://a.org/\\esc",
    "https://a.org/\nexport X=1",
    "https://a.org/\r"
  };
  for (size_t i = 0; i < sizeof (bad_vals) / sizeof (bad_vals[0]); i++)
    {
      EnvKV_t bad_v = { "TEST_URL", bad_vals[i] };
      assert (Env_BadValue == env_apply_kvs (&bad_v, 1, NULL, &err));
      free (err); err = NULL;
    }

  /* 2. ProjectScope 拒绝与非 root SystemScope 权限拒绝 */
  ProgMode.Scope = ProjectScope;
  EnvKV_t ok_kv = { "TEST_URL", "https://example.org" };
  assert (Env_ScopeUnsupported == env_apply_kvs (&ok_kv, 1, NULL, &err));
  free (err); err = NULL;

  if (!chsrc_is_root ())
    {
      ProgMode.Scope = SystemScope;
      assert (Env_NoPermission == env_apply_kvs (&ok_kv, 1, NULL, &err));
      free (err); err = NULL;
    }
  ProgMode.Scope = UserScope;

  /* 3. 未知 Shell 在 set 与 query 时均精确拒绝，不污染目录 */
  Env_Test_Force_Shell = PosixShell_Unknown;
  assert (Env_AmbiguousTarget == env_apply_kvs (&ok_kv, 1, NULL, &err));
  assert (NULL != err);
  free (err); err = NULL;

  bool  q_found = false;
  char *q_val   = NULL;
  assert (Env_AmbiguousTarget == env_query_persisted ("TEST_URL", &q_found, &q_val, &err));
  free (err); err = NULL;

  /* 4. Bash UserScope：保留注释、前缀边界隔离、重复行折叠、无尾换行补齐、幂等与删除 */
  Env_Test_Force_Shell = PosixShell_Bash;
  char *bashrc_path  = xy_normalize_path ("~/.bashrc");
  char *profile_path = xy_normalize_path ("~/.profile");

  chsrc_overwrite_file (
    "# user comment\n"
    "# ------ chsrc BLOCK BEGIN legacy ------\n"
    "export RUSTUP_DIST_SERVER_EXTRA=\"keep_me\"\n"
    "export RUSTUP_DIST_SERVER=\"https://old.one\"\n"
    "export NVM_NODEJS_ORG_MIRROR=https://old.nvm\n"
    "export RUSTUP_DIST_SERVER=\"https://old.duplicate\"\n"
    "# ------ chsrc BLOCK ENDIN legacy ------",
    bashrc_path);

  EnvKV_t rust_kvs[2] = {
    { "RUSTUP_DIST_SERVER", "https://mirrors.tuna.tsinghua.edu.cn/rustup" },
    { "RUSTUP_UPDATE_ROOT", "https://mirrors.tuna.tsinghua.edu.cn/rustup/rustup" }
  };
  assert (Env_OK == env_apply_kvs (rust_kvs, 2, NULL, &err));
  assert (NULL == err);
  assert (xy_file_exist (profile_path));
  assert (!xy_file_exist ("~/.bash_profile"));

  /* 第二次相同写入必须返回 Env_Unchanged */
  assert (Env_Unchanged == env_apply_kvs (rust_kvs, 2, NULL, &err));

  assert (Env_OK == env_query_persisted ("RUSTUP_DIST_SERVER", &q_found, &q_val, &err));
  assert (q_found);
  assert_str (q_val, "https://mirrors.tuna.tsinghua.edu.cn/rustup");
  free (q_val); q_val = NULL;

  assert (Env_OK == env_query_persisted ("RUSTUP_DIST_SERVER_EXTRA", &q_found, &q_val, &err));
  assert (q_found);
  assert_str (q_val, "keep_me");
  free (q_val); q_val = NULL;

  char *bashrc_after = xy_file_read (bashrc_path);
  assert (NULL != strstr (bashrc_after, "# user comment\n"));
  assert (NULL != strstr (bashrc_after, "# ------ chsrc BLOCK BEGIN legacy ------\n"));
  assert (NULL == strstr (bashrc_after, "old.duplicate"));
  free (bashrc_after);

  /* Dry-run 不落盘 */
  ProgMode.DryRunMode = true;
  EnvKV_t dry_kv = { "DRY_VAR", "https://dry.org" };
  assert (Env_OK == env_apply_kvs (&dry_kv, 1, NULL, &err));
  ProgMode.DryRunMode = false;
  assert (Env_OK == env_query_persisted ("DRY_VAR", &q_found, &q_val, &err));
  assert (!q_found);

  /* 删除变量幂等验证 */
  EnvKV_t del_kvs[2] = {
    { "RUSTUP_DIST_SERVER", NULL },
    { "RUSTUP_UPDATE_ROOT", NULL }
  };
  assert (Env_OK == env_apply_kvs (del_kvs, 2, NULL, &err));
  assert (Env_Unchanged == env_apply_kvs (del_kvs, 2, NULL, &err));
  assert_str (xy_file_read (profile_path), "");

  /* 5. 多文件不同值冲突检测 */
  chsrc_overwrite_file ("export CONFLICT_VAR=\"https://one\"\n", bashrc_path);
  chsrc_overwrite_file ("export CONFLICT_VAR=\"https://two\"\n", profile_path);
  assert (Env_AmbiguousTarget == env_query_persisted ("CONFLICT_VAR", &q_found, &q_val, &err));
  assert (NULL != err && NULL != strstr (err, ".bashrc") && NULL != strstr (err, ".profile"));
  free (err); err = NULL;

  /* 6. 复杂动态语法行保护（带行号且零修改） */
  const char *complex_rc = "# line 1\nexport SAFE=\"1\"\nexport DYN_VAR=\"$HOME/mirror\"\n";
  chsrc_overwrite_file (complex_rc, bashrc_path);
  chsrc_overwrite_file ("", profile_path);
  EnvKV_t dyn_kv = { "DYN_VAR", "https://override.org" };
  assert (Env_AmbiguousTarget == env_apply_kvs (&dyn_kv, 1, NULL, &err));
  assert (NULL != err && NULL != strstr (err, ":3"));
  free (err); err = NULL;
  assert_str (xy_file_read (bashrc_path), complex_rc);

  /* 7. Fish conf.d/chsrc.fish 写入与 config.fish 一次性精确清理 */
  Env_Test_Force_Shell = PosixShell_Fish;
  char *fish_cfg  = xy_normalize_path ("~/.config/fish/config.fish");
  char *fish_conf = xy_normalize_path ("~/.config/fish/conf.d/chsrc.fish");

  chsrc_overwrite_file (
    "# fish comment\n"
    "# ------ chsrc BLOCK BEGIN for Homebrew ------\n"
    "set -x RUSTUP_DIST_SERVER https://legacy.fish\n"
    "set -gx PUB_HOSTED_URL \"https://legacy.pub\"\n"
    "set -x MY_FISH_VAR \"stay_here\"\n",
    fish_cfg);

  EnvKV_t fish_kvs[2] = {
    { "RUSTUP_DIST_SERVER", "https://rsproxy.cn" },
    { "PUB_HOSTED_URL",     "https://pub.flutter-io.cn" }
  };
  assert (Env_OK == env_apply_kvs (fish_kvs, 2, NULL, &err));
  assert (Env_Unchanged == env_apply_kvs (fish_kvs, 2, NULL, &err));

  char *fish_conf_txt = xy_file_read (fish_conf);
  assert_str (fish_conf_txt,
    "set -x RUSTUP_DIST_SERVER \"https://rsproxy.cn\"\n"
    "set -x PUB_HOSTED_URL \"https://pub.flutter-io.cn\"\n");
  free (fish_conf_txt);

  char *fish_cfg_txt = xy_file_read (fish_cfg);
  assert (NULL == strstr (fish_cfg_txt, "RUSTUP_DIST_SERVER"));
  assert (NULL == strstr (fish_cfg_txt, "PUB_HOSTED_URL"));
  assert (NULL != strstr (fish_cfg_txt, "MY_FISH_VAR"));
  assert (NULL != strstr (fish_cfg_txt, "# ------ chsrc BLOCK BEGIN for Homebrew ------"));
  free (fish_cfg_txt);

  /* Fish universal 变量 (-Ux) 视为复杂行拒绝覆写 */
  chsrc_overwrite_file ("set -Ux UNI_VAR \"abc\"\n", fish_conf);
  EnvKV_t uni_kv = { "UNI_VAR", "def" };
  assert (Env_AmbiguousTarget == env_apply_kvs (&uni_kv, 1, NULL, &err));
  free (err); err = NULL;

  /* 8. /etc/environment 方言验证（通过 Env_Test_Etc_Env_Path 指向临时文件） */
  char *etc_mock = xy_2strcat (tmp_home, "/mock_etc_environment");
  Env_Test_Etc_Env_Path = etc_mock;
  ProgMode.Scope = SystemScope;

  EnvKV_t etc_space_bad = { "PUB_HOSTED_URL", "https://has space.org" };
  assert (Env_BadValue == env_apply_kvs (&etc_space_bad, 1, NULL, &err));
  free (err); err = NULL;

  EnvKV_t etc_ok = { "PUB_HOSTED_URL", "https://pub.flutter-io.cn" };
  assert (Env_OK == env_apply_kvs (&etc_ok, 1, NULL, &err));
  assert (Env_Unchanged == env_apply_kvs (&etc_ok, 1, NULL, &err));
  assert_str (xy_file_read (etc_mock), "PUB_HOSTED_URL=https://pub.flutter-io.cn\n");

  assert (Env_OK == env_query_persisted ("PUB_HOSTED_URL", &q_found, &q_val, &err));
  assert (q_found);
  assert_str (q_val, "https://pub.flutter-io.cn");
  free (q_val); q_val = NULL;

  EnvKV_t etc_del = { "PUB_HOSTED_URL", NULL };
  assert (Env_OK == env_apply_kvs (&etc_del, 1, NULL, &err));
  assert_str (xy_file_read (etc_mock), "");

  /* 清理临时目录并恢复全局状态 */
  Env_Test_Etc_Env_Path = NULL;
  Env_Test_Force_Shell  = -1;
  ProgMode.Scope        = saved_scope;
  ProgMode.DryRunMode   = saved_dry;
  free (etc_mock);
  free (fish_conf);
  free (fish_cfg);
  free (profile_path);
  free (bashrc_path);
}
#endif

int
main (int argc, char const *argv[])
{
  /* Child commands share stdout; never interleave their output inside a
   * buffered UTF-8 character when the test output is captured by a pipe. */
  setvbuf (stdout, NULL, _IOLBF, 0);
  if (argc == 2 && xy_streql (argv[1], "--version"))
    {
      say (Chsrc_Version);
      return 0;
    }

#ifndef XY_Build_On_Windows
  test_isolate_home ();
#endif
  xy_init ();

  br();
  chsrc_log  ("chsrc_log");
  chsrc_succ ("chsrc_succ");
  chsrc_info ("chsrc_info");
  chsrc_warn ("chsrc_warn");
  chsrc_error ("chsrc_error");
  chsrc_debug ("fw", "chsrc_debug");
  chsrc_verbose ("chsrc_verbose");

  chsrc_succ2 ("chsrc_succ2");
  chsrc_log2  ("chsrc_log2");
  chsrc_note2 ("chsrc_note2");
  chsrc_alert2 ("chsrc_alert2");
  chsrc_warn2 ("chsrc_warn2");
  chsrc_error2 ("chsrc_error2");
  chsrc_debug2 ("fw", "chsrc_debug2");
  chsrc_verbose2 ("chsrc_verbose2");

  // 现在 chsrc_check_program() 无法检测本目录文件了
  assert (chsrc_check_program ("curl"));
  assert (chsrc_check_program_quietly ("curl"));
  assert (chsrc_check_program_quietly_when_exist ("curl"));
  chsrc_ensure_program ("curl");

  chsrc_ensure_dir ("test");

  char *tmpfile_content = "Line2\n\nLine3\n";
  char *tmpfile_name = "test";
  char *tmpfile_ext = ".txt";
  char *tmpfile = "";

  FILE *tmp = chsrc_make_tmpfile (tmpfile_name, tmpfile_ext, true, &tmpfile);
  fwrite (tmpfile_content, sizeof (char), strlen (tmpfile_content), tmp);
  fclose (tmp);

  char *tmpfile_bk = xy_2strcat (tmpfile, ".bak");

  assert (xy_file_exist (tmpfile));
  chsrc_backup (tmpfile);\
  assert (chsrc_check_file (tmpfile_bk));
  remove (tmpfile_bk);

  chsrc_append_to_file ("Line4\n", tmpfile);
  assert_str (xy_file_read (tmpfile), "Line2\n\nLine3\nLine4\n");

  chsrc_prepend_to_file ("Line1 \n", tmpfile);
  assert_str (xy_file_read (tmpfile), "Line1 \nLine2\n\nLine3\nLine4\n");

  chsrc_overwrite_file ("Line999 \nLine998\nLine997\n", tmpfile);
  assert_str (xy_file_read (tmpfile), "Line999 \nLine998\nLine997\n");

  chsrc_overwrite_file ("", tmpfile);
  assert_str (xy_file_read (tmpfile), "");

  chsrc_log (xy_2strcat ("CPU arch = ", chsrc_get_cpuarch ()));
  print ("chsrc: CPU cores = ");
  println (chsrc_get_cpucore ());

  remove (tmpfile);

  // chsrc_run_in_inline_pwsh_shell ("Write-Host \"Hello from inline PowerShell\"");

#ifdef XY_Build_On_Windows
  chsrc_get_env ("OS", "windir", NULL);
  chsrc_run_as_pwsh_file ("Write-Host \"Hello from PowerShell (v7) file\"");
  chsrc_run_as_powershellv5_file ("Write-Host \"Hello from PowerShell v5 file\"");
  chsrc_run_as_powershell_file ("Write-Host \"Hello from PowerShell file\"");
#else
  Env_Test_Force_Shell = PosixShell_Bash;
  chsrc_get_env ("PWD", NULL);
  Env_Test_Force_Shell = -1;
  chsrc_run_as_sh_file ("echo Hello from sh file");
  chsrc_run_as_bash_file ("echo Hello from Bash file");
  test_env_zsh_startup_paths ();
  test_env_posix_backend ();
  if (xy_file_exist ("/dev/full"))
    assert (!chsrc_try_overwrite_file ("buffered output", "/dev/full"));
#endif

  test_env_utf16_and_reg_serializer ();

#ifndef XY_Build_On_Windows
  /* Test_Home 只能来自上面的 mkdtemp 模板，失败时保留现场便于诊断。 */
  char *rm_cmd = xy_strcat (3, "rm -rf -- \"", Test_Home, "\"");
  assert (0 == system (rm_cmd));
  free (rm_cmd);
#endif

  chsrc_run ("echo " Chsrc_Version " test pass!", RunOpt_Dont_Notify_On_Success);
  return 0;
}
