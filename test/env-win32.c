/* SPDX-License-Identifier: GPL-3.0-or-later
 * Win32 registry regressions with an in-memory Reg*W boundary.
 * Real backup files are checked; no user registry values or broadcasts change. */
#define Chsrc_Version "chsrc-env-win32-tester"
#define CHSRC_TESTING
#include "../src/framework/base.c"
#include "../src/framework/OS.c"
#include "../src/framework/file-update.c"

#ifdef XY_Build_On_Windows
static int Test_Write_Count, Test_Fail_Write = -1, Test_Broadcasts;
static bool Test_Fail_Rollback;
static struct { wchar_t *name; DWORD type, size; BYTE *data; } Test_Values[32];

static int
test_value_index (LPCWSTR name)
{
  for (int i = 0; i < 32; i++)
    if (Test_Values[i].name && 0 == _wcsicmp (Test_Values[i].name, name)) return i;
  return -1;
}

static LONG WINAPI
test_open_key (HKEY root, LPCWSTR path, DWORD options, REGSAM access, PHKEY result)
{
  assert (HKEY_CURRENT_USER == root && 0 == wcscmp (path, L"Environment"));
  *result = (HKEY) (uintptr_t) 1;
  return ERROR_SUCCESS;
}

static LONG WINAPI
test_create_key (HKEY root, LPCWSTR path, DWORD reserved, LPWSTR cls, DWORD options,
                 REGSAM access, const SECURITY_ATTRIBUTES *security, PHKEY result,
                 LPDWORD disposition)
{
  *disposition = REG_OPENED_EXISTING_KEY;
  return test_open_key (root, path, options, access, result);
}

static LONG WINAPI
test_close_key (HKEY key) { return ERROR_SUCCESS; }

static LONG WINAPI
test_query_value (HKEY key, LPCWSTR name, LPDWORD reserved, LPDWORD type,
                  LPBYTE data, LPDWORD size)
{
  int i = test_value_index (name);
  if (i < 0) return ERROR_FILE_NOT_FOUND;
  if (type) *type = Test_Values[i].type;
  DWORD cap = *size;
  *size = Test_Values[i].size;
  if (!data) return ERROR_SUCCESS;
  if (cap < *size) return ERROR_MORE_DATA;
  memcpy (data, Test_Values[i].data, *size);
  return ERROR_SUCCESS;
}

static LONG WINAPI
test_delete_value (HKEY key, LPCWSTR name)
{
  int i = test_value_index (name);
  if (i < 0) return ERROR_FILE_NOT_FOUND;
  free (Test_Values[i].name);
  free (Test_Values[i].data);
  memset (&Test_Values[i], 0, sizeof Test_Values[i]);
  return ERROR_SUCCESS;
}

static LONG WINAPI
test_set_value (HKEY key, LPCWSTR name, DWORD reserved, DWORD type,
                const BYTE *data, DWORD size)
{
  if (Test_Fail_Write >= 0)
    {
      int call = Test_Write_Count++;
      if (call == Test_Fail_Write || (Test_Fail_Rollback && call > Test_Fail_Write))
        return ERROR_ACCESS_DENIED;
    }
  int i = test_value_index (name);
  if (i < 0)
    {
      for (i = 0; i < 32 && Test_Values[i].name; i++);
      assert (i < 32);
      Test_Values[i].name = _wcsdup (name);
    }
  free (Test_Values[i].data);
  Test_Values[i].data = xy_malloc0 (size);
  memcpy (Test_Values[i].data, data, size);
  Test_Values[i].size = size;
  Test_Values[i].type = type;
  return ERROR_SUCCESS;
}

static LRESULT WINAPI
test_broadcast (HWND window, UINT message, WPARAM wparam, LPARAM lparam,
                UINT flags, UINT timeout, PDWORD_PTR result)
{
  Test_Broadcasts++;
  return 1;
}

#define RegSetValueExW test_set_value
#define RegOpenKeyExW test_open_key
#define RegCreateKeyExW test_create_key
#define RegCloseKey test_close_key
#define RegQueryValueExW test_query_value
#define RegDeleteValueW test_delete_value
#define SendMessageTimeoutW test_broadcast
#include "../src/framework/env.c"
#undef RegSetValueExW
#undef SendMessageTimeoutW

static void
test_reg_serializer (void)
{
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
  wchar_t *dec   = xy_malloc0 ((units + 1) * sizeof (wchar_t));
  for (size_t i = 0; i < units; i++)
    {
      dec[i] = (uint16_t) reg_bytes[2 + i * 2] | ((uint16_t) reg_bytes[2 + i * 2 + 1] << 8);
    }
  char *reg_txt = xy_wstr_to_utf8 (dec);
  free (dec);
  free (reg_bytes);

  assert (NULL != strstr (reg_txt, "Windows Registry Editor Version 5.00\r\n\r\n[HKEY_CURRENT_USER\\Environment]\r\n"));
  assert (NULL != strstr (reg_txt, "\"RUSTUP_DIST_SERVER\"=\"https://a.org/path\\\\\\\"q\"\r\n"));
  assert (NULL != strstr (reg_txt, "\"NEW_ADDED_VAR\"=-\r\n"));
  assert (NULL != strstr (reg_txt, "\"EXPAND_VAR\"=hex(2):25,00,54,00,25,00,00,00\r\n"));
  free (reg_txt);
}

static void
assert_value (const char *name, const char *expected)
{
  bool found = false;
  char *value = NULL, *err = NULL;
  assert (Env_OK == env_query_persisted (name, &found, &value, &err));
  assert (!err && found == (NULL != expected));
  if (expected) assert_str (value, expected);
  free (value);
}

static void
test_roundtrip_and_case_aliases (void)
{
  char *err = NULL;
  EnvKV_t kv = { "CHSRC_TEST_URL", "https://example.org/中文" };
  ProgMode.DryRunMode = true;
  assert (Env_OK == env_apply_kvs (&kv, 1, NULL, &err));
  ProgMode.DryRunMode = false;
  assert_value (kv.name, NULL);
  assert (0 == Env_Win_Backup_Seq && 0 == Test_Broadcasts);
  assert (Env_OK == env_apply_kvs (&kv, 1, NULL, &err));
  assert_value (kv.name, kv.value);
  assert (Env_OK == env_query_persisted (kv.name, NULL, NULL, &err));
  unsigned seq = Env_Win_Backup_Seq;
  int broadcasts = Test_Broadcasts;
  EnvKV_t aliases[] = { { "chsrc_test_url", "discarded" }, kv };
  assert (Env_Unchanged == env_apply_kvs (aliases, 2, NULL, &err));
  assert (seq == Env_Win_Backup_Seq && broadcasts == Test_Broadcasts);
  kv.value = "";
  assert (Env_OK == env_apply_kvs (&kv, 1, NULL, &err));
  assert_value (kv.name, "");
  kv.value = NULL;
  assert (Env_OK == env_apply_kvs (&kv, 1, NULL, &err));
  assert_value (kv.name, NULL);
}

static void
test_combo_backups_cover_later_variables (void)
{
  ProgStatus.ComboStackDepth = 1;
  for (int i = 0; i < 2; i++)
    {
      EnvRegSnapshot_t snap = { .name = i ? "LATER_ADDED" : "FIRST_ADDED" };
      char *path = NULL, *err = NULL;
      assert (env_write_win_reg_backup (false, &snap, 1, &path, &err));
      assert (path && !err);
      char *bytes = NULL;
      size_t len = 0;
      assert (xy_file_read_bytes (path, &bytes, &len));
      assert (len > 2 && (unsigned char) bytes[0] == 0xff && (unsigned char) bytes[1] == 0xfe);
      wchar_t *wide = xy_malloc0 (len);
      memcpy (wide, bytes + 2, len - 2);
      char *text = xy_wstr_to_utf8 (wide);
      free (wide);
      assert (strstr (text, i ? "\"LATER_ADDED\"=-" : "\"FIRST_ADDED\"=-"));
      free (text);
      free (bytes);
      free (path);
    }
  ProgStatus.ComboStackDepth = 0;
}

static void
test_invalid_utf8_rejects_whole_batch (void)
{
  EnvKV_t kvs[] = { { "VALID_NEW", "valid" }, { "INVALID_NEW", "\xed\xa0\x80" } };
  char *err = NULL;
  unsigned seq = Env_Win_Backup_Seq;
  int broadcasts = Test_Broadcasts;
  assert (Env_BadValue == env_apply_kvs (kvs, 2, NULL, &err));
  assert (err);
  free (err);
  assert_value ("VALID_NEW", NULL);
  assert_value ("INVALID_NEW", NULL);
  assert (seq == Env_Win_Backup_Seq && broadcasts == Test_Broadcasts);
}

static void
test_shared_backup_failure_preserves_registry (void)
{
  EnvKV_t kv = { "BACKUP_FAILURE", "new" };
  char *err = NULL;
  int broadcasts = Test_Broadcasts;
  FileUpdate_Test_Fail_Commit = 0;
  assert (Env_TargetUnwritable == env_apply_kvs (&kv, 1, NULL, &err));
  FileUpdate_Test_Fail_Commit = -1;
  assert (err);
  free (err);
  assert_value (kv.name, NULL);
  assert (broadcasts == Test_Broadcasts);
  char filename[64];
  snprintf (filename, sizeof filename, "env-hkcu-%u.reg", Env_Win_Backup_Seq);
  char *path = xy_path_join (Env_Win_Backup_Dir, filename);
  assert (!xy_path_exist (path));
  free (path);
}

static void
test_registry_strings_use_bounded_shared_conversion (void)
{
  /* 未终止的 REG_SZ 必须读取完整；不能读取提供长度以外的内存。 */
  const wchar_t unterminated[] = { L'a', 0x4e2d, 0xd83e, 0xdd80 };
  assert (ERROR_SUCCESS == test_set_value ((HKEY) 1, L"RAW_STRING", 0, REG_SZ,
            (const BYTE *) unterminated, sizeof unterminated));
  assert_value ("RAW_STRING", "a中🦀");

  const wchar_t malformed[][3] = { { 0xd800, 0, 0 }, { L'a', 0, L'b' } };
  for (size_t i = 0; i < xy_c_array_len (malformed); i++)
    {
      assert (ERROR_SUCCESS == test_set_value ((HKEY) 1, L"RAW_STRING", 0, REG_SZ,
                (const BYTE *) malformed[i], sizeof malformed[i]));
      char *err = NULL, *value = NULL;
      bool found = false;
      assert (Env_AmbiguousTarget == env_query_persisted ("RAW_STRING", &found, &value, &err));
      assert (!found && !value && err);
      free (err);
      EnvRegSnapshot_t snap;
      assert (Env_OK == env_read_win_snapshot_one ((HKEY) 1, "RAW_STRING", &snap, NULL));
      assert (!snap.utf8_sz_val);
      assert (sizeof malformed[i] == snap.raw_len);
      assert (0 == memcmp (snap.raw_bytes, malformed[i], snap.raw_len));
      size_t len;
      unsigned char *bytes = env_build_reg_backup_bytes (ENV_WIN_HKCU_REGPATH, &snap, 1, &len);
      assert (bytes && len > 2);
      char *text = xy_wstr_to_utf8 ((wchar_t *) (bytes + 2));
      assert (text && strstr (text, "\"RAW_STRING\"=hex(1):"));
      free (text);
      free (bytes);
      free (snap.raw_bytes);
    }
}

static void
test_write_failure_and_rollback_failure (void)
{
  char *err = NULL;
  EnvKV_t old[] = { { "FIRST", "old" }, { "SECOND", "old" } };
  EnvKV_t desired[] = { { "FIRST", "new" }, { "SECOND", "new" } };
  assert (Env_OK == env_apply_kvs (old, 2, NULL, &err));
  for (int fail_rollback = 0; fail_rollback < 2; fail_rollback++)
    {
      Test_Write_Count = 0;
      Test_Fail_Write = 1;
      Test_Fail_Rollback = fail_rollback;
      int broadcasts = Test_Broadcasts;
      assert (Env_TargetUnwritable == env_apply_kvs (desired, 2, NULL, &err));
      Test_Fail_Write = -1;
      assert (err && strstr (err, fail_rollback ? "rollback incomplete" : "rolled back"));
      assert (strstr (err, ".reg"));
      free (err);
      err = NULL;
      assert (broadcasts == Test_Broadcasts);
      assert_value ("FIRST", fail_rollback ? "new" : "old");
      assert_value ("SECOND", "old");
    }
}

int main (void)
{
  SetErrorMode (SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
  _set_error_mode (_OUT_TO_STDERR);
  _set_abort_behavior (0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
  xy_init ();
  ProgMode.Scope = UserScope;
  ProgMode.EnglishMode = true;
  test_reg_serializer ();
  test_roundtrip_and_case_aliases ();
  test_combo_backups_cover_later_variables ();
  test_invalid_utf8_rejects_whole_batch ();
  test_shared_backup_failure_preserves_registry ();
  test_registry_strings_use_bounded_shared_conversion ();
  test_write_failure_and_rollback_failure ();
  for (int i = 0; i < 32; i++)
    {
      free (Test_Values[i].name);
      free (Test_Values[i].data);
    }
  puts ("env-win32: all test groups passed (in-memory registry)");
  return 0;
}
#else
int main (void) { puts ("env-win32: skipped on POSIX"); return 0; }
#endif
