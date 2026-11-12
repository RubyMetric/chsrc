/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Name     : OS.c
 * File Authors  : @ccmywish
 *               | @G_I_Y
 * Contributors  : @rocascent
 *               | @happy-game
 *               | @Mikachu2333
 *               | @NewbieXvwu
 *               |
 * Created On    : <2023-08-29>
 * Last Modified : <2026-08-19>
 *
 * chsrc OS operations
 * ------------------------------------------------------------*/

#define YesMark "✓"
#define NoMark "x"
#define HalfYesMark "⍻"

#ifdef XY_Build_On_Windows
#include <ntsecapi.h>
#include <fcntl.h>
#include <io.h>
#endif

static void
log_for_file_write (const char *filename, bool is_overwrite)
{
  char *msg = is_overwrite ? (ENGLISH ? "OVERWRITE" : "覆写") : (ENGLISH ? "WRITE" : "写入");

  xy_log_brkt (blue(App_Name), bdblue(msg), blue(filename));
}

static void
log_for_file_backup_op (const char *filename)
{
  char *msg = ENGLISH ? "BACKUP" : "备份";

  char *bak = xy_2strcat (filename, ".bak");
  xy_log_brkt (blue(App_Name), bdblue(msg), xy_strcat (3, bdyellow(filename), " -> ", bdgreen(bak)));
}

static void
log_for_check_result (const char *check_what, const char *check_type, bool exist)
{
  char *chk_msg       = NULL;
  char *not_exist_msg = NULL;
  char *exist_msg     = NULL;

  if (ENGLISH)
    {
      chk_msg       = "CHECK";
      not_exist_msg = " doesn't exist";
      exist_msg     = " exists";
    }
  else
    {
      chk_msg       = "检查";
      not_exist_msg = " 不存在";
      exist_msg     = " 存在";
    }


  if (!exist)
    {
      xy_log_brkt (App_Name, bdred (chk_msg), xy_strcat (5,
                   red (NoMark " "), check_type, " ", red (check_what), not_exist_msg));
    }
  else
    {
      xy_log_brkt (App_Name, bdgreen (chk_msg), xy_strcat (5,
                   green (YesMark " "), check_type, " ", green (check_what), exist_msg));
    }
}


static void
log_for_run_cmd_result (bool result, int exit_status, bool use_yellow_for_error)
{
  char *run_msg  = NULL;
  char *succ_msg = NULL;
  char *fail_msg = NULL;

  if (ENGLISH)
    {
      run_msg  = "RUN";
      succ_msg = YesMark " executed successfully";
      fail_msg = NoMark  " executed unsuccessfully, exit status: ";
    }
  else
    {
      run_msg  = "运行";
      succ_msg = YesMark " 命令执行成功";
      fail_msg = NoMark  " 命令执行失败，退出状态: ";
    }

  if (result)
    xy_log_brkt (green (App_Name), bdgreen (run_msg), green (succ_msg));
  else
    {
      char buf[8] = {0};
      sprintf (buf, "%d", exit_status);

      if (use_yellow_for_error)
        {
          char *log = xy_2strcat (yellow (fail_msg), bdyellow (buf));
          xy_log_brkt (yellow (App_Name), bdyellow (run_msg), log);
        }
      else
        {
          char *log = xy_2strcat (red (fail_msg), bdred (buf));
          xy_log_brkt (red (App_Name), bdred (run_msg), log);
        }
    }
}



#define Quiet_When_Exist    0x00
#define Noisy_When_Exist    0x01
#define Quiet_When_NonExist 0x00
#define Noisy_When_NonExist 0x10

/**
 * 检测二进制程序是否存在
 *
 * @param  check_cmd  检测 `prog_name` 是否存在的一段命令，一般来说，填 `prog_name` 本身即可，
 *                    但是某些情况下，需要使用其他命令绕过一些特殊情况，比如 python 这个命令在Windows上
 *                    会自动打开 Microsoft Store，需避免
 *
 * @param  prog_name  要检测的二进制程序名
 *
 */
bool
query_program_exist (char *check_cmd, char *prog_name, int mode)
{
  char *which = check_cmd;

  int status = xy_system (which);

  // char buf[32] = {0}; sprintf(buf, "错误码: %d", status);

  char *msg = ENGLISH ? "command" : "命令";

  if (0 != status)
    {
      if (mode & Noisy_When_NonExist)
        {
          // xy_warn (xy_strcat(4, "× 命令 ", progname, " 不存在，", buf));
          log_for_check_result (prog_name, msg, false);
        }
      return false;
    }
  else
    {
      if (mode & Noisy_When_Exist)
        log_for_check_result (prog_name, msg, true);
      return true;
    }
}


/**
 * @brief 生成用于 “检测一个程序是否存在” 的命令，该内部函数由 chsrc_check_program() 家族调用
 *
 * @note
 *   1. Unix 中，'where' 命令仅在 Zsh 中可以使用，sh 和 Bash 中均无法使用，因为其并非二进制程序
 *   2. 因部分 Linux 发行版中没有 'which' 和 'whereis' 命令，使用 'command -v' 代替
 */
static char *
cmd_to_check_program (char *prog_name)
{
  char *check_tool = xy.on_windows ?  "where " : "command -v ";

  char *quiet_cmd = xy_quiet_cmd (xy_2strcat (check_tool, prog_name));

  return quiet_cmd;
}


/**
 * @brief 通过 `调用程序名 --version` 的方式检测程序是否存在
 *
 * @deprecated 因存在以下三个问题弃用：
 *
 *  1. 该程序得到直接执行，可能不太安全 (虽然基本不可能)
 *  2. 有一些程序启动速度太慢，即使只调用 --version，也依旧会花费许多时间，比如 mvn
 *  3. 有些程序并不支持 --version 选项 (虽然基本不可能)
 */
XY_Deprecate_This("Use cmd_to_check_program() instead")
static char *
cmd_to_check_program2 (char *prog_name)
{
  char *quiet_cmd = xy_quiet_cmd (xy_2strcat (prog_name, " --version"));
  return quiet_cmd;
}


/**
 * @brief 检测程序是否存在
 *
 * @note
 *  1. 一般只在 recipe 中使用，显式检测每一个需要用到的 program
 *  2. 无论存在与否，*均输出检测信息*
 *
 */
bool
chsrc_check_program (char *prog_name)
{
  return query_program_exist (cmd_to_check_program(prog_name), prog_name, Noisy_When_Exist|Noisy_When_NonExist);
}

/**
 * @brief 检测程序是否存在
 *
 * @note
 *  1. 此函数没有强制性，只返回检查结果
 *  2. 无论存在与否，*均不输出检测信息*
 */
bool
chsrc_check_program_quietly (char *prog_name)
{
  return query_program_exist (cmd_to_check_program(prog_name), prog_name, Quiet_When_Exist|Quiet_When_NonExist);
}

/**
 * @brief 检测程序是否存在
 *
 * @note 存在时不输出检测信息，不存在时才输出检测信息
 *
 */
bool
chsrc_check_program_quietly_when_exist (char *prog_name)
{
  return query_program_exist (cmd_to_check_program(prog_name), prog_name, Quiet_When_Exist|Noisy_When_NonExist);
}


/**
 * @brief 确保程序一定存在
 *
 * @note
 *  1. 此函数具有强制性，检测不到就直接退出
 *  2. 存在时不输出检测信息，不存在时才输出检测信息
 *
 */
void
chsrc_ensure_program (char *prog_name)
{
  bool exist = query_program_exist (cmd_to_check_program(prog_name), prog_name, Quiet_When_Exist|Noisy_When_NonExist);
  if (exist)
    {
      // OK, nothing should be done
    }
  else
    {
      char *msg1 = ENGLISH ? "not found " : "未找到 ";
      char *msg2 = ENGLISH ? " command, please check for existence" : " 命令，请检查是否存在";
      chsrc_error (xy_strcat (3, msg1, prog_name, msg2));
      exit (Exit_UserCause);
    }
}


bool
chsrc_check_file (char *path)
{
  char *msg = ENGLISH ? "file" : "文件";
  if (xy_file_exist (path))
    {
      log_for_check_result (path, msg, true);
      return true;
    }
  else
    {
      log_for_check_result (path, msg, false);
      return false;
    }
}



bool
chsrc_is_root (void)
{
#ifdef XY_Build_On_Windows
  BOOL is_admin = FALSE;
  SID_IDENTIFIER_AUTHORITY nt_authority = SECURITY_NT_AUTHORITY;
  PSID admin_group = NULL;

  if (AllocateAndInitializeSid (&nt_authority, 2,
                                SECURITY_BUILTIN_DOMAIN_RID,
                                DOMAIN_ALIAS_RID_ADMINS,
                                0, 0, 0, 0, 0, 0, &admin_group))
    {
      if (!CheckTokenMembership (NULL, admin_group, &is_admin))
        {
          is_admin = FALSE;
        }
      FreeSid (admin_group);
    }
  return (is_admin != FALSE);
#else
  return (0 == geteuid ());
#endif
}


void
chsrc_ensure_root ()
{
  if (chsrc_is_root ())
    return;

  char *msg = NULL;
  if (xy.on_windows)
    {
      msg = ENGLISH ? "Please run the terminal as Administrator to ensure the necessary permissions"
                    : "请以管理员身份运行终端来保证必要的权限";
    }
  else
    {
      msg = ENGLISH ? "Use sudo before the command or switch to root to ensure the necessary permissions"
                    : "请在命令前使用 sudo 或切换为root用户来保证必要的权限";
    }
  chsrc_error (msg);
  exit (Exit_UserCause);
}


#define RunOpt_Default                0x0000  // 默认若命令运行失败，直接退出
#define RunOpt_Dont_Notify_On_Success 0x0010  // 运行成功不提示用户，只有运行失败时才提示用户
#define RunOpt_No_Last_New_Line       0x0100  // 不输出最后的空行
#define RunOpt_Dont_Abort_On_Failure  0x1000  // 命令运行失败也不退出

static void
chsrc_run (const char *cmd, int run_option)
{
  if (ProgStatus.chsrc_run_faas)
    {
      run_option |= RunOpt_Dont_Notify_On_Success|RunOpt_No_Last_New_Line;
    }
  else
    {
      if (ENGLISH)
        xy_log_brkt (blue (App_Name), bdblue ("RUN"), blue (cmd));
      else
        xy_log_brkt (blue (App_Name), bdblue ("运行"), blue (cmd));
    }

  if (in_dry_run_mode())
    {
      return; // Dry Run 此时立即结束，并不真正执行
    }

  int status = xy_system (cmd);
  bool use_yellow_for_error = (run_option & RunOpt_Dont_Abort_On_Failure) != 0;

  if (0==status)
    {
      if (! (RunOpt_Dont_Notify_On_Success & run_option))
        {
          log_for_run_cmd_result (true, status, use_yellow_for_error);
        }
    }
  else
    {
      log_for_run_cmd_result (false, status, use_yellow_for_error);
      if (! (run_option & RunOpt_Dont_Abort_On_Failure))
        {
          char *msg = ENGLISH ? "Fatal error, forced end" : "关键错误，强制结束";
          chsrc_error (msg);
          exit (Exit_ExternalError);
        }
    }

  if (! (RunOpt_No_Last_New_Line & run_option))
    {
      br();
    }
}


static void
chsrc_run_as_a_service (const char *cmd)
{
  int run_option = RunOpt_Default;
  ProgStatus.chsrc_run_faas = true;
    run_option |= RunOpt_Dont_Notify_On_Success|RunOpt_No_Last_New_Line;
    chsrc_run (cmd, run_option);
  ProgStatus.chsrc_run_faas = false;
}


/**
 * @brief 以纯粹的方式直接运行命令，不做任何多余处理，
 *        命令执行前显示给用户，并保留所有输出给用户
 *
 * @return 返回命令的退出状态
 */
int
chsrc_run_directly (const char *cmd)
{
  if (ENGLISH)
    xy_log_brkt (blue (App_Name), bdblue ("RUN"), blue (cmd));
  else
    xy_log_brkt (blue (App_Name), bdblue ("运行"), blue (cmd));

  if (in_dry_run_mode())
    {
      return 0; // Dry Run 此时立即结束，并不真正执行
    }
  int status = xy_system (cmd);
  return status;
}


#ifdef XY_Build_On_Windows
/**
 * @brief 使用加密安全随机源生成十六进制字符串
 *
 * @param[in] nbytes 随机字节数，上限 64，越界时终止程序
 *
 * @return 长度为 2 * nbytes 的随机字符串，调用者需要 free
 */
static char *
_chsrc_random_hex_string (size_t nbytes)
{
  unsigned char rnd[64];
  if (nbytes > sizeof (rnd))
    {
      chsrc_error2 (CHINESE
        ? "随机字符串的随机字节数不能超过 64"
        : "The random byte count for a random string must not exceed 64");
      abort ();
    }

  if (!RtlGenRandom (rnd, (ULONG) nbytes))
    {
      chsrc_error2 (CHINESE
        ? "无法生成随机字符串"
        : "Unable to generate a random string");
      exit (Exit_ExternalError);
    }

  char *out = xy_malloc0 (2 * nbytes + 1);
  static const char hex_digits[] = "0123456789abcdef";
  for (size_t i = 0; i < nbytes; i++)
    {
      out[2 * i]     = hex_digits[rnd[i] >> 4];
      out[2 * i + 1] = hex_digits[rnd[i] & 0x0F];
    }
  out[2 * nbytes] = '\0';
  return out;
}
#endif


/**
 * @brief 在系统临时目录创建一个临时文件
 *
 * @param[in]  filename    文件名，不包含后缀名
 * @param[in]  postfix     后缀名，需要自己加 '.'
 * @param[in]  loud        创建成功时是否提示用户
 * @param[out] tmpfilepath 生成的临时文件名，非 Windows 可以为 NULL
 *
 * @return 返回一个 FILE*，调用者需要关闭该文件
 */
FILE *
chsrc_make_tmpfile (const char *filename, const char *postfix, bool loud, char **tmpfilepath)
{
  char *tmpfile = NULL;
  FILE *f = NULL;

#ifdef XY_Build_On_Windows
  /**
   * Windows 上自行生成不可预测的临时文件名，并通过 CREATE_NEW 原子创建。
   *
   * GetTempFileNameW() 不支持指定后缀；若先创建、删除再用目标后缀重建，
   * 删除和重新创建之间会产生竞态。因此这里直接生成随机文件名。
   */
  /* 与 chsrc_make_tmpdir() 相同：返回值 MAX_PATH + 1 表示缓冲区不足，路径不保证以 NUL 结尾。 */
  WCHAR temp_dir[MAX_PATH + 2] = {0};
  DWORD temp_dir_len = GetTempPathW (MAX_PATH + 1, temp_dir);

  if (0 == temp_dir_len || temp_dir_len >= MAX_PATH + 1)
    {
      chsrc_error2 (CHINESE
        ? "无法获取系统临时目录"
        : "Unable to get system temp directory");
      exit (Exit_ExternalError);
    }

  char *temp_path = xy_wstr_to_utf8 (temp_dir);
  if (!temp_path)
    {
      chsrc_error2 (CHINESE
        ? "无法转换系统临时目录"
        : "Unable to convert the system temp directory");
      exit (Exit_ExternalError);
    }

  /*
   * CREATE_NEW 保证文件名已存在时不会覆盖原文件。
   * 极小概率随机名碰撞时重新生成名字。
   */
  for (int attempt = 0; attempt < 8; attempt++)
    {
      char *hex = _chsrc_random_hex_string (8);
      char *path = xy_strcat (6, temp_path, "chsrc_", hex, "_", filename, postfix);
      free (hex);

      WCHAR *wpath = xy_utf8_to_wstr (path);
      if (!wpath)
        {
          free (path);
          chsrc_error2 (CHINESE
            ? "无法转换临时文件路径"
            : "Unable to convert the temporary file path");
          exit (Exit_ExternalError);
        }

      HANDLE h = CreateFileW (
        wpath,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        CREATE_NEW,
        FILE_ATTRIBUTE_NORMAL,
        NULL
      );

      if (INVALID_HANDLE_VALUE == h)
        {
          DWORD err = GetLastError ();

          free (wpath);
          free (path);

          if (ERROR_FILE_EXISTS == err || ERROR_ALREADY_EXISTS == err)
            continue;

          char *err_s = xy_int2str ((int) err);
          char *msg = CHINESE
            ? xy_2strcat ("无法创建临时文件，错误码: ", err_s)
            : xy_2strcat ("Unable to create temporary file, error: ", err_s);

          free (err_s);
          chsrc_error2 (msg);
          exit (Exit_ExternalError);
        }

      int fd = _open_osfhandle ((intptr_t) h, _O_RDWR | _O_TEXT);
      if (-1 == fd)
        {
          CloseHandle (h);
          DeleteFileW (wpath);
          free (wpath);
          free (path);

          chsrc_error2 (CHINESE
            ? "无法创建临时文件"
            : "Unable to create temporary file");
          exit (Exit_ExternalError);
        }

      f = _fdopen (fd, "w+");
      if (!f)
        {
          /*
           * _open_osfhandle() 成功后 fd 拥有 HANDLE，
           * 因此这里只需关闭 fd，不能再次 CloseHandle(h)。
           */
          _close (fd);
          DeleteFileW (wpath);
          free (wpath);
          free (path);

          chsrc_error2 (CHINESE
            ? "无法打开临时文件"
            : "Unable to open temporary file");
          exit (Exit_ExternalError);
        }

      tmpfile = path;
      free (wpath);
      break;
    }
  free (temp_path);

  if (!f)
    {
      chsrc_error2 (CHINESE
        ? "无法创建具有唯一名称的临时文件"
        : "Unable to create a uniquely named temporary file");
      exit (Exit_ExternalError);
    }

#else
  /**
   * 非 Windows 平台使用 mkstemps() 创建临时文件
   * 这是 Unix 系统上常见的扩展接口（非 POSIX 标准），可以指定后缀名
   */
  tmpfile = xy_strcat (
    5, "/tmp/", "chsrc_tmp_", filename, "_XXXXXX", postfix);

  size_t postfix_len = strlen (postfix);

  /* mkstemps() 会原子性地创建文件并返回文件描述符 */
  int fd = mkstemps (tmpfile, postfix_len);

  if (-1 == fd)
    {
      char *msg = CHINESE
        ? xy_2strcat ("无法创建临时文件: ", tmpfile)
        : xy_2strcat ("Unable to create temporary file: ", tmpfile);

      chsrc_error2 (msg);
      exit (Exit_ExternalError);
    }

  f = fdopen (fd, "w+");

  if (!f)
    {
      close (fd);  /* 关闭文件描述符以避免泄漏 */

      char *msg = CHINESE
        ? xy_2strcat ("无法打开临时文件: ", tmpfile)
        : xy_2strcat ("Unable to open temporary file: ", tmpfile);

      chsrc_error2 (msg);
      exit (Exit_ExternalError);
    }
#endif

  if (loud)
    {
      char *msg = CHINESE
        ? xy_2strcat ("已创建临时文件: ", tmpfile)
        : xy_2strcat ("Temporary file created: ", tmpfile);

      chsrc_succ2 (msg);
    }

  /**
   * 允许生成文件后不了解其文件名，调用者只了解 FILE*
   * 这样的话，其实是无法删除该文件的，但是生成在 /tmp 目录下我们恰好可以不用清理
   * 但是在 Windows 上，就没有办法了，所以我们禁止在 Windows 上不指定返回出的临时文件名
   */
  if (xy.on_windows && !tmpfilepath)
    {
      fclose (f);
      xy_file_remove (tmpfile);
      free (tmpfile);

      chsrc_error2 ("在 Windows 上，创建临时文件时必须指定返回的临时文件名");
      xy_unreached ();
    }

  if (tmpfilepath)
    {
      *tmpfilepath = xy_normalize_path (xy_strdup (tmpfile));
    }

  free (tmpfile);

  return f;
}


/**
 * @brief 在系统临时目录下创建一个本进程独占、名字不可预测的目录
 *
 * @details
 *   Windows 上用 RtlGenRandom 生成名字，POSIX 上用 mkdtemp；无论哪个平台，目录都必须由本进程
 *   创建成功（重名换名重试），绝不复用已存在的目录。可预测的名字（如 PID、操作序号）可能被
 *   提前占位，或以 junction / symlink 重定向，使后续写入落到攻击者指定的位置
 *
 * @param[in]  prefix  目录名前缀（UTF-8）
 * @param[out] out_err 失败原因，可为 NULL
 *
 * @return 目录路径，调用者需要 free；失败返回 NULL
 */
char *
chsrc_make_tmpdir (const char *prefix, char **out_err)
{
#ifdef XY_Build_On_Windows
  /*
   * GetTempPathW() 成功时返回不含结尾 NUL 的长度；缓冲区不足时返回所需长度（含 NUL），
   * 最大为 MAX_PATH + 1。故返回值等于 MAX_PATH + 1 一定是失败而非有效长度：若接受它，
   * temp_dir 可能没有 NUL 结尾，xy_wstr_to_utf8() 会越界读取。
   */
  WCHAR temp_dir[MAX_PATH + 2] = {0};
  DWORD tlen = GetTempPathW (MAX_PATH + 1, temp_dir);
  if (0 == tlen || tlen >= MAX_PATH + 1)
    {
      if (out_err)
        {
          *out_err = ENGLISH
            ? xy_strdup ("Failed to locate the system temp directory (GetTempPathW)")
            : xy_strdup ("无法获取系统临时目录路径");
        }
      return NULL;
    }

  char *temp_path = xy_wstr_to_utf8 (temp_dir);
  if (!temp_path)
    {
      if (out_err)
        {
          *out_err = ENGLISH
            ? xy_strdup ("Failed to convert the system temp directory to UTF-8")
            : xy_strdup ("无法将系统临时目录转换为 UTF-8");
        }
      return NULL;
    }

  for (int attempt = 0; attempt < 8; attempt++)
    {
      char *hex = _chsrc_random_hex_string (8);
      char *name = xy_strcat (3, prefix, "-", hex);
      char *path = xy_2strcat (temp_path, name);
      free (hex);

      WCHAR *wpath = xy_utf8_to_wstr (path);
      if (!wpath)
        {
          free (path);
          free (name);
          free (temp_path);
          if (out_err)
            {
              *out_err = ENGLISH
                ? xy_strdup ("Failed to convert the temporary directory path to UTF-16")
                : xy_strdup ("无法将临时目录路径转换为 UTF-16");
            }
          return NULL;
        }

      if (CreateDirectoryW (wpath, NULL))
        {
          free (wpath);
          free (name);
          free (temp_path);
          return path;
        }

      DWORD err = GetLastError ();
      free (wpath);
      free (path);
      if (ERROR_ALREADY_EXISTS != err)
        {
          char *err_s = xy_int2str ((int) err);
          if (out_err)
            {
              *out_err = ENGLISH
                ? xy_strcat (4, "Failed to create temporary directory `", name, "` (Win32 error ", err_s)
                : xy_strcat (4, "无法创建临时目录 `", name, "`，错误码: ", err_s);
            }
          free (err_s);
          free (name);
          free (temp_path);
          return NULL;
        }
      free (name);

      /* 名字已被占位（预先放置的目录或 junction），换一个随机名重试 */
    }
  free (temp_path);

  if (out_err)
    {
      *out_err = ENGLISH
        ? xy_strdup ("Failed to create a uniquely named temporary directory")
        : xy_strdup ("无法创建具有唯一名称的临时目录");
    }
  return NULL;
#else
  const char *tmp = getenv ("TMPDIR");
  if (!tmp || '\0' == tmp[0])
    tmp = "/tmp";

  char *tmpl = xy_strcat (4, tmp, "/", prefix, "-XXXXXX");
  if (!mkdtemp (tmpl))
    {
      if (out_err)
        {
          *out_err = ENGLISH
            ? xy_strcat (3, "Failed to create temporary directory `", tmpl, "`")
            : xy_strcat (3, "无法创建临时目录 `", tmpl, "`");
        }
      free (tmpl);
      return NULL;
    }
  return tmpl;
#endif
}


static void
chsrc_run_as_script_file (const char *script_content, const char *filename, const char *postfix,
                          const char *cmd_prefix, const char *msg)
{
  char *tmpfile = NULL;
  FILE *f = chsrc_make_tmpfile (filename, postfix, false, &tmpfile);
  fwrite (script_content, strlen (script_content), 1, f);
  fclose (f);

  chsrc_note2 (msg);
  println (faint(script_content));
  char *cmd = xy_strcat (3, cmd_prefix, tmpfile, "\"");
  chsrc_run (cmd, RunOpt_Dont_Abort_On_Failure);
  free (cmd);
  xy_file_remove (tmpfile);
  free (tmpfile);
}


/**
 * 以 bash file.bash 的形式运行脚本内容
 */
void
chsrc_run_as_bash_file (const char *script_content)
{
  chsrc_run_as_script_file (script_content, "bash_script", ".bash", "bash \"",
                            CHINESE ? "即将执行 Bash 脚本内容:" : "The Bash script content will be executed:");
}


/**
 * 以 sh file.sh 的形式运行脚本内容
 */
void
chsrc_run_as_sh_file (const char *script_content)
{
  chsrc_run_as_script_file (script_content, "sh_script", ".sh", "sh \"",
                            CHINESE ? "即将执行 sh 脚本内容:" : "The sh script content will be executed:");
}


/**
 * 以 pwsh file.ps1 的形式运行脚本内容
 */
void
chsrc_run_as_pwsh_file (const char *script_content)
{
  chsrc_run_as_script_file (script_content, "pwsh_script", ".ps1", "pwsh -ExecutionPolicy Bypass -File \"",
                            CHINESE ? "即将执行 PowerShell (v7以上) 脚本内容:" : "The PowerShell script content will be executed:");
}


/**
 * 以 powershell file.ps1 的形式运行脚本内容
 */
void
chsrc_run_as_powershellv5_file (const char *script_content)
{
  chsrc_run_as_script_file (script_content, "psv5_script", ".ps1", "powershell -ExecutionPolicy Bypass -File \"",
                            CHINESE ? "即将执行 PowerShell v5 脚本内容:" : "The PowerShell v5 script content will be executed:");
}


/**
 * 使用 pwsh 或 旧的 powershell (v5) 运行脚本内容，优先使用 pwsh
 */
void
chsrc_run_as_powershell_file (const char *script_content)
{
  // if (chsrc_check_program_quietly_when_exist ("pwsh"))
  if (chsrc_check_program_quietly ("pwsh"))
    {
      chsrc_run_as_pwsh_file (script_content);
    }
  else
    {
      chsrc_alert2 (CHINESE ? "未检测到 PowerShell 7 及以上版本，默认使用 PowerShell v5"
                            : "PowerShell 7 or above not detected, switch to PowerShell v5");
      chsrc_run_as_powershellv5_file (script_content);
    }
}


/**
 * @param cmdline 需要自己负责转义
 *
 * @danger 需要经过 Bash 的转义，很容易出错，不要用这个函数
 */
XY_Deprecate_This("Don't use this function")
void
chsrc_run_in_inline_bash_shell (const char *cmdline)
{
  char *cmd = xy_strcat (3, "bash -c '", cmdline, "'");
  chsrc_run (cmd, RunOpt_Dont_Abort_On_Failure);
}


/**
 * @param cmdline 需要自己负责转义
 *
 * @danger 需要经过 PowerShell 的转义，很容易出错，不要用这个函数
 */
XY_Deprecate_This("Don't use this function")
void
chsrc_run_in_inline_pwsh_shell (const char *cmdline)
{
  char *cmd = xy_strcat (3, "pwsh -Command '", cmdline, "'");
  chsrc_run (cmd, RunOpt_Dont_Abort_On_Failure);
}


static void
chsrc_view_env (const char *var1, ...)
{
  char *cmd = NULL;
  const char *var = var1;

  va_list vars;
  va_start (vars, var1);

  bool first = true;
  while (var)
    {
      if (xy.on_windows)
        {
          if (first)
            {
              cmd = xy_strcat (3, "set ", var, " ");
              first = false;
            }
          else
            {
              cmd = xy_strcat (4, cmd, "& set ", var, " ");
            }
        }
      else
       {
          if (first)
            {
              cmd = xy_strcat (5, "echo ", var, "=$", var, " ");
              first = false;
            }
          else
            {
              cmd = xy_strcat (6, cmd, "; echo ", var, "=$", var, " ");
            }
        }
      var = va_arg (vars, const char *);
    }

  va_end (vars);

  if (var1)
    {
      /**
       * 不用 chsrc_run()，因为在Windows上，set在遇到环境变量未定义时会返回非0，导致 chsrc_run() 报告运行失败
       * 这个错误过于醒目。我们应该像在 sh 一样，默默地没有输出即可，而不是报错
       */
      // chsrc_run (cmd, RunOpt_Dont_Notify_On_Success|RunOpt_No_Last_New_Line|RunOpt_Dont_Abort_On_Failure);
      int status = xy_system (cmd);
      if (status!=0) { xy_noop(); }
    }
  else
    {
      /* 必须给一个参数 */
      xy_unreached();
    }
}


static void
chsrc_view_file (const char *path)
{
  char *cmd = NULL;
  path = xy_normalize_path (path);
  if (xy.on_windows)
    {
      cmd = xy_2strcat ("type ", path);
    }
  else
    {
      cmd = xy_2strcat ("cat ", path);
    }

  chsrc_run_as_a_service (cmd);
}

static void
chsrc_ensure_dir (const char *dir)
{
  dir = xy_normalize_path (dir);

  if (xy_dir_exist (dir))
    {
      return;
    }

  // 不存在就生成
  char *mkdir_cmd = NULL;
  if (xy.on_windows)
    {
      mkdir_cmd = "md ";  // 已存在时返回 errorlevel = 1
    }
  else
    {
      mkdir_cmd = "mkdir -p ";
    }
  char *cmd = xy_2strcat (mkdir_cmd, dir);
  cmd = xy_quiet_cmd (cmd);

  chsrc_run_as_a_service (cmd);

  char *msg = ENGLISH ? "Directory doesn't exist, created automatically " : "目录不存在，已自动创建 ";
  chsrc_alert2 (xy_2strcat (msg, dir));
}


static void
chsrc_append_to_file (const char *str, const char *filename)
{
  if (in_dry_run_mode())
    {
      goto log_anyway;
    }

  char *file = xy_normalize_path (filename);
  char *dir = xy_parent_dir (file);
  chsrc_ensure_dir (dir);

  FILE *f = xy_fopen (file, "a");
  if (NULL==f)
    {
      char *msg = ENGLISH ? xy_2strcat ("Unable to open file to write: ", file)
                          : xy_2strcat ("无法打开文件以写入: ", file);
      chsrc_error2 (msg);
      exit (Exit_UserCause);
    }

  size_t len = strlen (str);

  size_t ret = fwrite (str, len, 1, f);
  if (ret != 1)
    {
      char *msg = ENGLISH ? xy_2strcat ("Write failed to ", file)
                          : xy_2strcat ("写入文件失败: ", file);
      chsrc_error2 (msg);
      exit (Exit_UserCause);
    }

  fclose (f);

log_anyway:
  /* 输出recipe指定的文件名 */
  log_for_file_write (filename, false);

  /*
  char *cmd = NULL;
  if (xy.on_windows)
    {
      cmd = xy_strcat (4, "echo ", str, " >> ", file);
    }
  else
    {
      cmd = xy_strcat (4, "echo '", str, "' >> ", file);
    }
  chsrc_run_a_service (cmd);
  */
}

/**
 * @note 本函数不会在 `str` 末尾添加换行符，所以你可能需要在 `str` 中手动添加
 */
static void
chsrc_prepend_to_file (const char *str, const char *filename)
{
  if (in_dry_run_mode())
    {
      goto log_anyway;
    }

  char *file = xy_normalize_path (filename);

  char *file_content = xy_file_read (file);
  char *content = xy_2strcat (str, file_content);

  FILE *f = xy_fopen (file, "w");

  if (f)
    {
      fwrite (content, 1, strlen (content), f);
      fclose (f);
    }
  else
    {
      chsrc_error2 ("文件打开失败");
      exit (Exit_UserCause);
    }

log_anyway:
  /* 输出recipe指定的文件名 */
  log_for_file_write (filename, false);
}

static void
chsrc_overwrite_file (const char *str, const char *filename)
{
  if (in_dry_run_mode())
    {
      goto log_anyway;
    }

  char *file = xy_normalize_path (filename);
  char *dir = xy_parent_dir (file);
  chsrc_ensure_dir (dir);

  FILE *f = xy_fopen (file, "w");
  if (NULL==f)
    {
      char *msg = ENGLISH ? xy_2strcat ("Unable to open file to overwrite: ", file)
                          : xy_2strcat ("无法打开文件以覆盖: ", file);
      chsrc_error2 (msg);
      exit (Exit_UserCause);
    }

  size_t len = strlen (str);
  size_t ret = fwrite (str, len, 1, f);
  if (ret != 1)
    {
      fclose (f);
      char *msg = ENGLISH ? xy_2strcat ("Write failed to ", file)
                          : xy_2strcat ("写入文件失败: ", file);
      chsrc_error2 (msg);
      exit (Exit_UserCause);
    }

  fclose (f);

log_anyway:
  /* 输出recipe指定的文件名 */
  log_for_file_write (filename, true);
}

static void
chsrc_backup (const char *path)
{
  if (in_dry_run_mode())
    {
      goto log_anyway;
    }

  char *cmd = NULL;
  bool exist = xy_file_exist (path);

  if (!exist)
    {
      char *msg = ENGLISH ? "File doesn't exist, skip backup: " : "文件不存在,跳过备份: ";
      chsrc_alert2 (xy_2strcat (msg, path));
      return;
    }

  /* combo dish 中多个 sub dish 可能操作同一配置文件。首个 sub dish
   * 已经保留原始内容后，后续 sub dish 不应再覆盖该备份。 */
  if (chsrc_in_dish_group_mode())
    {
      int depth = ProgStatus.ComboStackDepth - 1;
      XySeq_t *backed_up_paths = ProgStatus.ComboBackedUpPaths[depth];

      for (size_t i=0; i < xy_seq_len(backed_up_paths); i++)
        {
          if (xy_streql (xy_seq_at (backed_up_paths, i), path))
            goto log_anyway;
        }

      xy_seq_push (backed_up_paths, xy_strdup (path));
    }

  if (xy.on_bsd || xy.on_macos)
    {
      /* BSD 和 macOS 的 cp 不支持 --backup 选项 */
      cmd = xy_strcat (5, "cp -f ", path, " ", path, ".bak");
    }
  else if (xy.on_windows)
    {
      /**
       * @note /Y 表示覆盖
       * @note 默认情况下会输出一个 "已复制  1个文件"
       */
      cmd = xy_strcat (5, "copy /Y ", path, " ", path, ".bak 1>nul");
    }
  else
    {
      /**
       * @see https://github.com/RubyMetric/chsrc/issues/152#issuecomment-2542673273
       *
       * busybox cp 会在 stderr 输出 unrecognized option: version
       * stderr 导入到 stdout，以便我们 xy_run() 可以接受到输出
       *
       */
      char *ver = xy_run ("cp --version 2>&1", 1);
      /* cp (GNU coreutils) 9.4 */
      if (strstr (ver, "GNU coreutils"))
        {
          cmd = xy_strcat (5, "cp ", path, " ", path, ".bak --backup='t'");
        }
      else
        {
          /* 非 GNU 的 cp 可能不支持 --backup ，如 busybox cp */
          cmd = xy_strcat (5, "cp -f ", path, " ", path, ".bak");
        }
    }

  chsrc_run_as_a_service (cmd);

log_anyway:
  log_for_file_backup_op (path);
}


/**
 * @note 检查过程中全程保持安静
 */
static char *
chsrc_get_cpuarch ()
{
  char *ret;
  char *msg;

#if XY_Build_On_Windows
  SYSTEM_INFO info;
  GetNativeSystemInfo (&info);
  WORD num = info.wProcessorArchitecture;
  switch (num)
    {
      case PROCESSOR_ARCHITECTURE_AMD64:
        ret = "x86_64"; break;
      case PROCESSOR_ARCHITECTURE_ARM64:
        ret = "aarch64"; break;
      case PROCESSOR_ARCHITECTURE_ARM:
        ret = "arm";    break;
      case PROCESSOR_ARCHITECTURE_INTEL:
        ret = "x86";    break;
      case PROCESSOR_ARCHITECTURE_IA64:
        ret = "IA-64";  break;
      case PROCESSOR_ARCHITECTURE_UNKNOWN:
      default:
        msg = ENGLISH ? "Unable to detect CPU type" : "无法检测到CPU类型";
        chsrc_error (msg);
        exit (Exit_UserCause);
    }
  return ret;
#else

  bool exist;

  exist = chsrc_check_program_quietly ("arch");
  if (exist)
    {
      ret = xy_run ("arch", 0);
      return ret;
    }

  exist = chsrc_check_program_quietly ("uname");
  if (exist)
    {
      ret = xy_run ("uname -m", 0);
      return ret;
    }
  else
    {
      msg = ENGLISH ? "Unable to detect CPU type" : "无法检测到CPU类型";
      chsrc_error (msg);
      exit (Exit_UserCause);
    }
#endif
}


static int
chsrc_get_cpucore ()
{
  int cores = 2;

#if XY_Build_On_Windows
  SYSTEM_INFO info;
  GetSystemInfo (&info);
  DWORD num = info.dwNumberOfProcessors;
  cores = (int)num;
#else
  long num = sysconf(_SC_NPROCESSORS_ONLN);
  cores = (int)num;
#endif

  return cores;
}
