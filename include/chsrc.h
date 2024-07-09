/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File          : chsrc.h
 * Authors       : Aoran Zeng <ccmywish@qq.com>
 *               | Heng Guo   <2085471348@qq.com>
 * Contributors  : Peng Gao   <gn3po4g@outlook.com>
 *               |
 * Created on    : <2023-08-29>
 * Last modified : <2024-07-09>
 *
 * chsrc 头文件
 * ------------------------------------------------------------*/

#include "xy.h"
#include "source.h"

#define App_Name "chsrc"

#define Exit_UserCause    1
#define Exit_Unsupported  2
#define Exit_MatinerIssue 3
#define Exit_FatalBug     4
#define Exit_FatalUnkownError  5

#define chsrc_log(str)   xy_log(App_Name,str)
#define chsrc_succ(str)  xy_succ(App_Name,str)
#define chsrc_info(str)  xy_info(App_Name,str)
#define chsrc_warn(str)  xy_warn(App_Name,str)
#define chsrc_error(str) xy_error(App_Name,str)

#define chsrc_succ_remarkably(str)    xy_succ_remarkably(App_Name,"成功",str);
#define chsrc_infolog_remarkably(str) xy_info_remarkably(App_Name,"LOG",str);
#define chsrc_info_remarkably(str)    xy_info_remarkably(App_Name,"提示",str);
#define chsrc_note_remarkably(str)    xy_warn_remarkably(App_Name,"提示",str);
#define chsrc_warn_remarkably(str)    xy_warn_remarkably(App_Name,"警告",str);
#define chsrc_error_remarkably(str)   xy_error_remarkably(App_Name,"错误",str);

void
chsrc_check_remarkably (const char *check_what, const char *check_type, bool exist)
{
  if (!exist)
    {
      xy_log_remarkably (App_Name, xy_str_to_bold (xy_str_to_red ("检查")),
                         xy_strjoin (5, xy_str_to_red ("x "), check_type, " ", xy_str_to_red (check_what), " 不存在"));
    }
  else
    {
      xy_log_remarkably (App_Name, xy_str_to_bold (xy_str_to_green ("检查")),
                        xy_strjoin (5, xy_str_to_green ("√ "), check_type, " ", xy_str_to_green (check_what), " 存在"));
    }
}


bool Cli_Option_IPv6 = false;
bool Cli_Option_Locally = false;
bool Cli_Option_InEnglish = false;


bool
is_url (const char *str)
{
  return (xy_str_start_with (str, "http://") || xy_str_start_with (str, "https://"));
}


/**
 * 检测二进制程序是否存在
 *
 * @param  check_cmd  检测 `prog_name` 是否存在的一段命令，一般来说，填 `prog_name` 本身即可，
 *                    但是某些情况下，需要使用其他命令绕过一些特殊情况，比如 python 这个命令在Windows上
 *                    会自动打开 Microsoft Store，需避免
 *
 * @param  prog_name   要检测的二进制程序名
 */
bool
query_program_exist (char *check_cmd, char *prog_name)
{
  char *which = check_cmd;

  int ret = system(which);

  // char buf[32] = {0}; sprintf(buf, "错误码: %d", ret);

  if (0 != ret)
    {
      // xy_warn (xy_strjoin(4, "× 命令 ", progname, " 不存在，", buf));
      chsrc_check_remarkably (prog_name, "命令", false);
      return false;
    }
  else
    {
      chsrc_check_remarkably (prog_name, "命令", true);
      return true;
    }
}


/**
 * @note 此函数只能对接受 --version 选项的程序有效
 */
bool
chsrc_check_program (char *prog_name)
{
  char *quiet_cmd = xy_str_to_quietcmd (xy_2strjoin (prog_name, " --version"));
  return query_program_exist (quiet_cmd, prog_name);
}


/**
 * @note 此函数具有强制性，检测不到就直接退出
 */
void
chsrc_ensure_program (char *prog_name)
{
  char *quiet_cmd = xy_str_to_quietcmd (xy_2strjoin (prog_name, " --version"));
  bool exist = query_program_exist (quiet_cmd, prog_name);
  if (exist)
    {
      // OK, nothing should be done
    }
  else
    {
      chsrc_error (xy_strjoin (3, "未找到 ", prog_name, " 命令，请检查是否存在"));
      exit (Exit_UserCause);
    }
}


bool
chsrc_check_file (char *path)
{
  if (xy_file_exist (path))
    {
      chsrc_check_remarkably (path, "文件", true);
      return true;
    }
  else
    {
      chsrc_check_remarkably (path, "文件", false);
      return false;
    }
}


/**
 * 用于 _setsrc 函数，检测用户输入的镜像站code，是否存在于该target可用源中
 *
 * @param  target  目标名
 * @param  input   如果用户输入 default 或者 def，则选择第一个源
 */
#define find_mirror(s, input) query_mirror_exist(s##_sources, s##_sources_n, (char*)#s+3, input)
int
query_mirror_exist (SourceInfo *sources, size_t size, char *target, char *input)
{
  if (is_url (input))
    {
      chsrc_error ("暂不支持对该软件使用用户自定义源，请联系开发者询问原因或请求支持");
      exit (Exit_Unsupported);
    }

  if (0==size || 1==size)
    {
      chsrc_error (xy_strjoin (3, "当前 ", target, " 无任何可用源，请联系维护者"));
      exit (Exit_MatinerIssue);
    }

  if (2==size)
    {
      chsrc_succ (xy_strjoin (4, sources[1].mirror->name, " 是 ", target, " 目前唯一可用镜像站，感谢他们的慷慨支持"));
    }

  if (xy_streql ("reset", input))
    {
      puts ("将重置为上游默认源");
      return 0; // 返回第1个，因为第1个是上游默认源
    }

  if (xy_streql ("first", input))
    {
      puts ("将使用维护团队测速第一的源");
      return 1; // 返回第2个，因为第1个是上游默认源
    }

  int idx = 0;
  SourceInfo source = sources[0];

  bool exist = false;
  for (int i=0; i<size; i++)
    {
      source = sources[i];
      if (xy_streql (source.mirror->code, input))
        {
          idx = i;
          exist = true;
          break;
        }
    }
  if (!exist)
    {
      chsrc_error (xy_strjoin (3, "镜像站 ", input, " 不存在"));
      chsrc_error (xy_2strjoin ("查看可使用源，请使用 chsrc list ", target));
      exit (Exit_UserCause);
    }
  return idx;
}


/**
 * 该函数来自 oh-my-mirrorz.py，由 @ccmywish 翻译为C语言，但功劳和版权属于原作者
 */
char *
to_human_readable_speed (double speed)
{
  char *scale[] = {"Byte/s", "KByte/s", "MByte/s", "GByte/s", "TByte/s"};
  int i = 0;
  while (speed > 1024.0)
  {
    i += 1;
    speed /= 1024.0;
  }
  char *buf = xy_malloc0 (64);
  sprintf (buf, "%.2f %s", speed, scale[i]);

  char *new = NULL;
  if (i <= 1 ) new = xy_str_to_red (buf);
  else
    {
      if (i == 2 && speed < 2.00) new = xy_str_to_yellow (buf);
      else new = xy_str_to_green (buf);
    }
  return new;
}


/**
 * 测速代码参考自 https://github.com/mirrorz-org/oh-my-mirrorz/blob/master/oh-my-mirrorz.py
 * 功劳和版权属于原作者，由 @ccmywish 修改为C语言，并做了额外调整
 *
 * @return 返回测得的速度，若出错，返回-1
 */
double
test_speed_url (const char *url)
{
  char *time_sec = "6";

  /* 现在我们切换至跳转后的链接来测速，不再使用下述判断
  if (xy_str_start_with(url, "https://registry.npmmirror"))
    {
      // 这里 npmmirror 跳转非常慢，需要1~3秒，所以我们给它留够至少8秒测速时间，否则非常不准
      time_sec = "10";
    }
  */

  char *ipv6 = ""; // 默认不启用

  if (Cli_Option_IPv6==true) {
    ipv6 = "--ipv6";
  }

  // 我们用 —L，因为Ruby China源会跳转到其他地方
  // npmmirror 也会跳转
  char *curl_cmd = xy_strjoin (7, "curl -qsL ", ipv6,
                                  " -o " xy_os_devnull,
                                  " -w \"%{http_code} %{speed_download}\" -m", time_sec ,
                                  " -A chsrc/" Chsrc_Version "  ", url);

  // chsrc_info (xy_2strjoin ("测速命令 ", curl_cmd));

  char *buf = xy_run (curl_cmd, 0, NULL);
  // 如果尾部有换行，删除
  buf = xy_str_strip (buf);

  // 分隔两部分数据
  char *split = strchr (buf, ' ');
  if (split) *split = '\0';

  // puts(buf); puts(split+1);
  int http_code = atoi (buf);
  double speed  = atof (split+1);
  char *speedstr = to_human_readable_speed (speed);

  if (200!=http_code)
    {
      char* httpcodestr = xy_str_to_yellow (xy_2strjoin ("HTTP码 ", buf));
      puts (xy_strjoin (3, speedstr, " | ",  httpcodestr));
    }
  else
    {
      puts (speedstr);
    }
  return speed;
}


int
get_max_ele_idx_in_dbl_ary (double *array, int size)
{
  double maxval = array[0];
  int maxidx = 0;

  for (int i=1; i<size; i++)
    {
      if (array[i]>maxval)
        {
          maxval = array[i];
          maxidx = i;
        }
    }
  return maxidx;
}

/**
 * 自动测速选择镜像站和源
 */
#define auto_select(s) auto_select_(s##_sources, s##_sources_n, (char*)#s+3)
int
auto_select_ (SourceInfo *sources, size_t size, const char *target)
{
  if (0==size || 1==size)
    {
      chsrc_error (xy_strjoin (3, "当前 ", target, " 无任何可用源，请联系维护者: chsrc issue"));
      exit (Exit_MatinerIssue);
    }

  bool onlyone = false;
  if (2==size) onlyone = true;

  char *check_curl = xy_str_to_quietcmd ("curl --version");
  bool  exist_curl = query_program_exist (check_curl, "curl");
  if (!exist_curl)
    {
      chsrc_error ("没有curl命令，无法测速");
      exit (Exit_UserCause);
    }

  double speeds[size];
  double speed = 0.0;
  for (int i=0;i<size;i++)
  {
    SourceInfo src = sources[i];
    const char* url = src.mirror->__bigfile_url;
    if (NULL==url)
      {
        if (xy_streql ("upstream", src.mirror->code))
          {
            continue; // 上游默认源不测速
          }
        else
          {
            chsrc_warn (xy_strjoin (3, "开发者未提供 ",  src.mirror->code, " 镜像站测速链接，跳过该站点"));
            speed = 0;
          }
      }
    else
      {
        printf ("%s", xy_strjoin (3, "测速 ", src.mirror->site , " ... "));
        fflush (stdout);
        speed = test_speed_url (url);
      }
    speeds[i] = speed;
  }
  int fastidx = get_max_ele_idx_in_dbl_ary (speeds, size);

  if (onlyone)
    chsrc_succ (xy_strjoin (4, sources[fastidx].mirror->name, " 是 ", target, " 目前唯一可用镜像站，感谢他们的慷慨支持"));
  else
    puts (xy_2strjoin ("最快镜像站: ", xy_str_to_green (sources[fastidx].mirror->name)));

  return fastidx;
}


#define use_specific_mirror_or_auto_select(input, s) \
  (NULL!=(input)) ? find_mirror(s, input) : auto_select(s)



bool
source_is_upstream (SourceInfo *source)
{
  return xy_streql (source->mirror->code, "upstream");
}

bool
source_is_userdefine (SourceInfo *source)
{
  return xy_streql (source->mirror->code, "user");
}

bool
source_has_empty_url (SourceInfo *source)
{
  return source->url == NULL;
}

/**
 * 用户*只可能*通过下面三种方式来换源，无论哪一种都会返回一个 SourceInfo 出来
 *
 * 1. 用户指定 MirrorCode
 * 2. 用户什么都没指定 (将测速选择最快镜像)
 * 3. 用户给了一个 URL
 *
 * @dependency 变量 option
 * @dependency 变量 source
 */
#define chsrc_yield_source(for_what) \
  if (is_url (option)) \
    { \
      SourceInfo __tmp = { &UserDefine, option }; \
      source = __tmp; \
    } \
  else \
    { \
      int __index = use_specific_mirror_or_auto_select (option, for_what); \
      source = for_what##_sources[__index]; \
    }



#define split_between_source_changing_process   puts ("--------------------------------")

/**
 * 用于 _setsrc 函数
 *
 * 1. 告知用户选择了什么源和镜像
 * 2. 对选择的源和镜像站进行一定的校验
 */
void
chsrc_confirm_source (SourceInfo *source)
{
  // 由于实现问题，我们把本应该独立出去的默认上游源，也放在了可以换源的数组中，而且放在第一个
  // chsrc 已经规避用户使用未实现的 `chsrc reset`
  // 但是某些用户可能摸索着强行使用 chsrc set target upstream，从而执行起该禁用的功能，
  // 之所以禁用，是因为有的 reset 我们并没有实现，我们在这里阻止这些邪恶的用户
  if (source_is_upstream (source) && source_has_empty_url (source))
    {
      chsrc_error ("暂未对该软件实现重置");
      exit (Exit_Unsupported);
    }
  else if (source_has_empty_url (source))
    {
      chsrc_error ("该源URL不存在，请向开发团队提交bug");
      exit (Exit_FatalBug);
    }
  else
    {
      puts (xy_strjoin (5, "选中镜像站: ", xy_str_to_green (source->mirror->abbr), " (", xy_str_to_green (source->mirror->code), ")"));
    }

  split_between_source_changing_process;
}


#define ChsrcTypeAuto     "auto"
#define ChsrcTypeReset    "reset"
#define ChsrcTypeSemiAuto "semiauto"
#define ChsrcTypeManual   "manual"
#define ChsrcTypeUntested "untested"

/**
 * @param source    可为NULL
 * @param last_word 5种选择：ChsrcTypeAuto | ChsrcTypeReset | ChsrcTypeSemiAuto | ChsrcTypeManual | ChsrcTypeUntested
 */
void
chsrc_say_lastly (SourceInfo *source, const char *last_word)
{
  split_between_source_changing_process;

  if (xy_streql (ChsrcTypeAuto, last_word))
    {
      if (source)
        {
          if (source_is_userdefine (source))
            {
              chsrc_log ("全自动换源完成; 邀您参与贡献，帮助其他人使用该URL换源: chsrc issue");
            }
          else
            {
              chsrc_log (xy_2strjoin ("全自动换源完成，感谢镜像提供方: ", xy_str_to_purple (source->mirror->name)));
            }
        }
      else
        {
          chsrc_log ("全自动换源完成");
        }
    }
  else if (xy_streql (ChsrcTypeReset, last_word))
    {
      // source_is_upstream (source)
      chsrc_log (xy_str_to_purple ("已重置为上游默认源"));
    }
  else if (xy_streql (ChsrcTypeSemiAuto, last_word))
    {
      if (source)
        {
          if (source_is_userdefine (source))
            {
              chsrc_log ("半自动换源完成，仍需按上述提示手工操作; 邀您参与贡献，帮助其他人使用该URL换源: chsrc issue");
            }
          else
            {
              chsrc_log (xy_2strjoin ("半自动换源完成，仍需按上述提示手工操作，感谢镜像提供方: ", xy_str_to_purple (source->mirror->name)));
            }
        }
      else
        {
          chsrc_log ("半自动换源完成，仍需按上述提示手工操作");
        }
      chsrc_warn ("若您有完全自动化的换源方案，邀您帮助: chsrc issue");
    }
  else if (xy_streql (ChsrcTypeManual, last_word))
    {
      if (source)
        {
          if (source_is_userdefine (source))
            {
              chsrc_log ("因实现约束需按上述提示手工操作; 邀您参与贡献，帮助其他人使用该URL换源: chsrc issue");
            }
          else
            {
              chsrc_log (xy_2strjoin ("因实现约束需按上述提示手工操作，感谢镜像提供方: ", xy_str_to_purple (source->mirror->name)));
            }
        }
      else
        {
          chsrc_log ("因实现约束需按上述提示手工操作");
        }
      chsrc_warn ("若您有完全自动化的换源方案，邀您帮助: chsrc issue");
    }
  else if (xy_streql (ChsrcTypeUntested, last_word))
    {
      if (source)
        {
          if (source_is_userdefine (source))
            {
              chsrc_log ("邀您参与贡献，帮助其他人使用该URL换源: chsrc issue");
            }
          else
            {
              chsrc_log (xy_2strjoin ("感谢镜像提供方: ", xy_str_to_purple (source->mirror->name)));
            }
        }
      else
        {
          chsrc_log ("自动换源完成");
        }
      chsrc_warn ("该换源步骤已实现但未经测试或存在任何反馈，请报告使用情况: chsrc issue");
    }
  else
    {
      puts (last_word);
    }
}



void
chsrc_ensure_root ()
{
  char *euid = getenv ("$EUID");
  if (NULL==euid)
    {
      char *buf = xy_run ("id -u", 0, NULL);
      if (0!=atoi(buf)) goto not_root;
      else return;
    }
  else
    {
      if (0!=atoi(euid)) goto not_root;
      else return;
    }
not_root:
  chsrc_error ("请在命令前使用 sudo 或切换为root用户来保证必要的权限");
  exit (Exit_UserCause);
}


#define RunOpt_Default           0x0000  // 默认若命令运行失败，直接退出
#define RunOpt_No_Note_On_Sccess 0x0010  // 运行成功不提示用户，只有运行失败时才提示用户
#define RunOpt_No_Last_New_Line  0x0100  // 不输出最后的空行
#define RunOpt_No_Exit_On_Error  0x1000  // 命令运行失败也不退出

static void
chsrc_run (const char *cmd, int run_option)
{
  xy_info_remarkably (App_Name, "运行", cmd);
  int status = system (cmd);
  if (0==status)
    {
      if (! (RunOpt_No_Note_On_Sccess & run_option))
        {
          xy_succ_remarkably (App_Name, "运行", "命令执行成功");
        }
    }
  else
    {
      char buf[8] = {0};
      sprintf (buf, "%d", status);
      char *str = xy_2strjoin ("命令执行失败，返回码 ", buf);
      xy_error_remarkably (App_Name, "运行", str);
      if (! (run_option & RunOpt_No_Exit_On_Error))
        {
          chsrc_error ("关键错误，强制结束");
          exit (Exit_FatalUnkownError);
        }
    }

  if (! (RunOpt_No_Last_New_Line & run_option))
    {
      puts ("");
    }
}


static void
chsrc_view_file (const char *path)
{
  char *cmd = NULL;
  path = xy_uniform_path (path);
  if(xy_on_windows)
    {
      cmd = xy_2strjoin ("type ", path);
    }
  else
    {
      cmd = xy_2strjoin ("cat ", path);
    }
  chsrc_run (cmd, RunOpt_No_Note_On_Sccess|RunOpt_No_Last_New_Line);
}

static void
chsrc_ensure_dir (const char *dir)
{
  dir = xy_uniform_path (dir);
  char *mkdir_cmd = NULL;

  if (xy_on_windows)
    {
      // 也可以用 opendir() #include <dirent.h>
      DWORD attr = GetFileAttributesA(dir);

      if (attr == INVALID_FILE_ATTRIBUTES)
        {
          // Q: 我们应该报错吗？
          // chsrc_error ("目录路径错误");
          // exit (Exit_MatinerIssue);
        }
      else
        {
          if (attr & FILE_ATTRIBUTE_DIRECTORY)
            return;
        }
    }
  else
    {
      int status = system (xy_2strjoin ("test -d ", dir));
      if (0==status)
        return;
    }

  // 不存在就生成
  if (xy_on_windows)
    {
      mkdir_cmd = "md ";  // 已存在时返回 errorlevel = 1
    }
  else
    {
      mkdir_cmd = "mkdir -p ";
    }
  char *cmd = xy_2strjoin (mkdir_cmd, dir);
  cmd = xy_str_to_quietcmd (cmd);
  chsrc_run (cmd, RunOpt_No_Last_New_Line|RunOpt_No_Note_On_Sccess);
  chsrc_note_remarkably (xy_2strjoin ("目录不存在，已自动创建 ", dir));
}

static void
chsrc_append_to_file (const char *str, const char *file)
{
  file = xy_uniform_path (file);
  char *dir = xy_parent_dir (file);
  chsrc_ensure_dir (dir);

  char *cmd = NULL;
  if (xy_on_windows)
    {
      cmd = xy_strjoin (4, "echo ", str, " >> ", file);
    }
  else
    {
      cmd = xy_strjoin (4, "echo '", str, "' >> ", file);
    }
  chsrc_run (cmd, RunOpt_Default);
}

static void
chsrc_prepend_to_file (const char *str, const char *file)
{
  file = xy_uniform_path (file);
  char *dir = xy_parent_dir (file);
  chsrc_ensure_dir (dir);

  char *cmd = NULL;
  if (xy_on_windows)
    {
      xy_unimplement;
    }
  else
    {
      cmd = xy_strjoin (4, "sed -i '1i ", str, "' ", file);
    }
  chsrc_run (cmd, RunOpt_Default);
}

static void
chsrc_overwrite_file (const char *str, const char *file)
{
  file = xy_uniform_path (file);
  char *dir = xy_parent_dir (file);
  chsrc_ensure_dir (dir);

  char *cmd = NULL;
  if (xy_on_windows)
    {
      cmd = xy_strjoin (4, "echo ", str, " > ", file);
    }
  else
    {
      cmd = xy_strjoin (4, "echo '", str, "' > ", file);
    }
  chsrc_run (cmd, RunOpt_Default);
}

static void
chsrc_backup (const char *path)
{
  char *cmd = NULL;

  if (xy_on_bsd || xy_on_macos)
    {
      /* BSD 和 macOS 的 cp 不支持 --backup 选项 */
      cmd = xy_strjoin (5, "cp -f ", path, " ", path, ".bak");
    }
  else if (xy_on_windows)
    {
      // /Y 表示覆盖
      cmd = xy_strjoin (5, "copy /Y ", path, " ", path, ".bak" );
    }
  else
    {
      cmd = xy_strjoin (5, "cp ", path, " ", path, ".bak --backup='t'");
    }

  chsrc_run (cmd, RunOpt_No_Last_New_Line|RunOpt_No_Note_On_Sccess);
  chsrc_note_remarkably (xy_strjoin (3, "备份文件名为 ", path, ".bak"));
}


static char *
chsrc_get_cpuarch ()
{
  char *ret;
  bool exist;

  if (xy_on_windows)
    {
      xy_unimplement;
    }

  exist = chsrc_check_program ("arch");
  if (exist)
    {
      ret = xy_run ("arch", 0, NULL);
      return ret;
    }

  exist = chsrc_check_program ("uname");
  if (exist)
    {
      ret = xy_run ("uname -m", 0, NULL);
      return ret;
    }
  else
    {
      chsrc_error ("无法检测到CPU类型");
      exit (Exit_UserCause);
    }
}


/* Target Info */
typedef struct TargetInfo_t {
  void (*getfn)   (char *option);
  void (*setfn)   (char *option);
  void (*resetfn) (char *option);
  SourceInfo *sources;
  size_t      sources_n;
} TargetInfo;

// 大部分target还不支持reset，所以暂时先默认设置为NULL来过渡
#define def_target(t) TargetInfo t##_target = {t##_getsrc, t##_setsrc, NULL, t##_sources, t##_sources_n}

#define def_target_full(t) TargetInfo t##_target = {t##_getsrc, t##_setsrc, t##_resetsrc, t##_sources, t##_sources_n}

#define def_target_noget(t) TargetInfo t##_target = {NULL, t##_setsrc, NULL, t##_sources, t##_sources_n}
