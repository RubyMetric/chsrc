/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Name     : core.c
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 *               |  Heng Guo  <2085471348@qq.com>
 * Contributors  :  Peng Gao  <gn3po4g@outlook.com>
 *               | Happy Game <happygame10124@gmail.com>
 *               | Yangmoooo  <yangmoooo@outlook.com>
 *               |
 * Created On    : <2023-08-29>
 * Last Modified : <2025-06-19>
 *
 * chsrc framework
 * ------------------------------------------------------------*/

#if defined(__STDC__) && __STDC_VERSION__ < 201112L
#   error "chsrc requires C11 or later, please use a new compiler which at least supports C11"
#endif

#if defined(__STDC__) && __STDC_VERSION__ < 201710L
#   warning "chsrc recommends a C17 or later compiler"
#endif

#include "xy.h"
#include "struct.h"
#include "mirror.c"

#define App_Name "chsrc"

static int chsrc_get_cpucore ();

bool ProgMode_CMD_Measure = false;
bool ProgMode_CMD_Reset   = false;

bool ProgMode_Target_Group = false;
int  ProgMode_Leader_Selected_Index = -1;

/* 此时 chsrc_run() 不再是recipe中指定要运行的一个外部命令，而是作为一个功能实现的支撑 */
bool ProgMode_Run_as_a_Service = false;

enum ChgType_t
{
  ChgType_Auto,
  ChgType_Reset,
  ChgType_SemiAuto,
  ChgType_Manual,
  ChgType_Untested
};

enum ChgType_t ProgMode_ChgType = ChgType_Auto;


/* 命令行选项 */
bool CliOpt_IPv6      = false;
bool CliOpt_Locally   = false;
bool CliOpt_InEnglish = false;
bool CliOpt_DryRun    = false;
bool CliOpt_NoColor   = false;

/**
 * -local 的含义是启用 *项目级* 换源
 *
 * 每个 target 对 [-local] 的支持情况可使用 | chsrc ls <target> | 来查看
 *
 *
 * 1. 默认不使用该选项时，含义是 *全局* 换源，
 *
 *    全局分为 (1)系统级 (2)用户级
 *
 *    大多数第三方配置软件往往默认进行的是 *用户级* 的配置。所以 chsrc 首先将尝试使用 *用户级* 配置
 *
 * 2. 若不存在 *用户级* 的配置，chsrc 将采用 *系统级* 的配置
 *
 * 3. 最终效果本质由第三方软件决定，如 poetry 默认实现的就是项目级的换源
 */

#define Exit_OK               0
#define Exit_Fatal            1
#define Exit_Unknown          2
#define Exit_Unsupported      3
#define Exit_UserCause        4
#define Exit_MaintainerCause  5
#define Exit_ExternalError    6

#define chsrc_log(str)     xy_log(App_Name,str)
#define chsrc_succ(str)    xy_succ(App_Name,str)
#define chsrc_info(str)    xy_info(App_Name,str)
#define chsrc_warn(str)    xy_warn(App_Name,str)
#define chsrc_error(str)   xy_error(App_Name,str)
#define chsrc_debug(str)   xy_warn(App_Name "(DEBUG)",str)
#define chsrc_verbose(str) xy_info(App_Name "(VERBOSE)",str)

#define red(str)      xy_str_to_red(str)
#define blue(str)     xy_str_to_blue(str)
#define green(str)    xy_str_to_green(str)
#define yellow(str)   xy_str_to_yellow(str)
#define purple(str)   xy_str_to_purple(str)
#define bold(str)     xy_str_to_bold(str)
#define bdred(str)    xy_str_to_bold(xy_str_to_red(str))
#define bdblue(str)   xy_str_to_bold(xy_str_to_blue(str))
#define bdgreen(str)  xy_str_to_bold(xy_str_to_green(str))
#define bdyellow(str) xy_str_to_bold(xy_str_to_yellow(str))
#define bdpurple(str) xy_str_to_bold(xy_str_to_purple(str))

// 2系列都是带有括号的
#define chsrc_succ2(str)    xy_succ_brkt(App_Name,CliOpt_InEnglish?"SUCCEED":"成功",str)
#define chsrc_log2(str)     xy_info_brkt(App_Name,"LOG",str)
#define chsrc_warn2(str)    xy_warn_brkt(App_Name,CliOpt_InEnglish?"WARN":"警告",str)
#define chsrc_error2(str)   xy_error_brkt(App_Name,CliOpt_InEnglish?"ERROR":"错误",str)
#define chsrc_debug2(str)   xy_warn_brkt(App_Name,"DEBUG",str)
#define chsrc_verbose2(str) xy_info_brkt(App_Name,"VERBOSE",str)

void
chsrc_note2 (const char *str)
{
  char *msg = CliOpt_InEnglish ? "NOTE" : "提示";
  xy_log_brkt (yellow(App_Name), bdyellow(msg), yellow(str));
}

void
chsrc_log_write (const char *filename)
{
  char *msg = CliOpt_InEnglish ? "WRITE" : "写入";

  xy_log_brkt (blue(App_Name), bdblue(msg), blue(filename));
}

void
chsrc_log_backup (const char *filename)
{
  char *msg = CliOpt_InEnglish ? "BACKUP" : "备份";

  char *bak = xy_2strjoin (filename, ".bak");
  xy_log_brkt (blue(App_Name), bdblue(msg), xy_strjoin (3, bdyellow(filename), " -> ", bdgreen(bak)));
}

#define YesMark "✓"
#define NoMark "x"
#define HalfYesMark "⍻"

/**
 * @translation Done
 */
void
log_check_result (const char *check_what, const char *check_type, bool exist)
{
  char *chk_msg       = NULL;
  char *not_exist_msg = NULL;
  char *exist_msg     = NULL;

  if (CliOpt_InEnglish)
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
      xy_log_brkt (App_Name, bdred (chk_msg), xy_strjoin (5,
                   red (NoMark " "), check_type, " ", red (check_what), not_exist_msg));
    }
  else
    {
      xy_log_brkt (App_Name, bdgreen (chk_msg), xy_strjoin (5,
                   green (YesMark " "), check_type, " ", green (check_what), exist_msg));
    }
}


/**
 * @translation Done
 */
void
log_cmd_result (bool result, int exit_status)
{
  char *run_msg  = NULL;
  char *succ_msg = NULL;
  char *fail_msg = NULL;

  if (CliOpt_InEnglish)
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
      char *log = xy_2strjoin (red (fail_msg), bdred (buf));
      xy_log_brkt (red (App_Name), bdred (run_msg), log);
    }
}



bool
is_url (const char *str)
{
  return (xy_str_start_with (str, "http://") || xy_str_start_with (str, "https://"));
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
 * @param  prog_name   要检测的二进制程序名
 *
 * @translation Done
 */
bool
query_program_exist (char *check_cmd, char *prog_name, int mode)
{
  char *which = check_cmd;

  int status = system (which);

  // char buf[32] = {0}; sprintf(buf, "错误码: %d", status);

  char *msg = CliOpt_InEnglish ? "command" : "命令";

  if (0 != status)
    {
      if (mode & Noisy_When_NonExist)
        {
          // xy_warn (xy_strjoin(4, "× 命令 ", progname, " 不存在，", buf));
          log_check_result (prog_name, msg, false);
        }
      return false;
    }
  else
    {
      if (mode & Noisy_When_Exist)
        log_check_result (prog_name, msg, true);
      return true;
    }
}


/**
 * @note
 *  1. 一般只在 Recipe 中使用，显式检测每一个需要用到的 program
 *  2. 无论存在与否，**均输出**
 *
 */
bool
chsrc_check_program (char *prog_name)
{
  char *quiet_cmd = xy_str_to_quietcmd (xy_2strjoin (prog_name, " --version"));
  return query_program_exist (quiet_cmd, prog_name, Noisy_When_Exist|Noisy_When_NonExist);
}

/**
 * @note
 *  1. 此函数没有强制性，只返回检查结果
 *  2. 无论存在与否，**均不输出**
 *  3. 此函数只能对接受 --version 选项的命令行程序有效
 *
 */
bool
chsrc_check_program_quietly (char *prog_name)
{
  char *quiet_cmd = xy_str_to_quietcmd (xy_2strjoin (prog_name, " --version"));
  return query_program_exist (quiet_cmd, prog_name, Quiet_When_Exist|Quiet_When_NonExist);
}

/**
 * @note 存在时不输出，不存在时才输出
 *
 */
bool
chsrc_check_program_quietly_when_exist (char *prog_name)
{
  char *quiet_cmd = xy_str_to_quietcmd (xy_2strjoin (prog_name, " --version"));
  return query_program_exist (quiet_cmd, prog_name, Quiet_When_Exist|Noisy_When_NonExist);
}


/**
 * @note
 *  1. 此函数具有强制性，检测不到就直接退出
 *  2. 检查到存在时不输出，检查到不存在时输出
 *
 */
void
chsrc_ensure_program (char *prog_name)
{
  char *quiet_cmd = xy_str_to_quietcmd (xy_2strjoin (prog_name, " --version"));
  bool exist = query_program_exist (quiet_cmd, prog_name, Quiet_When_Exist|Noisy_When_NonExist);
  if (exist)
    {
      // OK, nothing should be done
    }
  else
    {
      char *msg1 = CliOpt_InEnglish ? "not found " : "未找到 ";
      char *msg2 = CliOpt_InEnglish ? " command, please check for existence" : " 命令，请检查是否存在";
      chsrc_error (xy_strjoin (3, msg1, prog_name, msg2));
      exit (Exit_UserCause);
    }
}


bool
chsrc_check_file (char *path)
{
  char *msg = CliOpt_InEnglish ? "file" : "文件";
  if (xy_file_exist (path))
    {
      log_check_result (path, msg, true);
      return true;
    }
  else
    {
      log_check_result (path, msg, false);
      return false;
    }
}


/**
 * 用于 _setsrc 函数，检测用户输入的镜像站code，是否存在于该target可用源中
 *
 * @note 一个源Source必定来自于一个Provider，所以该函数名叫 query_provider_exist
 *
 * @param  target  目标名
 * @param  input   如果用户输入 default 或者 def，则选择第一个源
 */
#define find_mirror(s, input) query_provider_exist(s##_sources, s##_sources_n, (char*)#s+3, input)
int
query_provider_exist (Source_t *sources, size_t size, char *target, char *input)
{
  if (is_url (input))
    {
      char *msg = CliOpt_InEnglish ? "Using user-defined sources for this software is not supported at this time, please contact the developers to ask why or request support" : "暂不支持对该软件使用用户自定义源，请联系开发者询问原因或请求支持";
      chsrc_error (msg);
      exit (Exit_Unsupported);
    }

  if (0==size)
    {
      char *msg1 = CliOpt_InEnglish ? "Currently " : "当前 ";
      char *msg2 = CliOpt_InEnglish ? " doesn't have any source available. Please contact the maintainers" : " 无任何可用源，请联系维护者";
      chsrc_error (xy_strjoin (3, msg1, target, msg2));
      exit (Exit_MaintainerCause);
    }

  if (1==size)
    {
      char *msg1 = CliOpt_InEnglish ? "Currently " : "当前 ";
      char *msg2 = CliOpt_InEnglish ? " only the upstream source exists. Please contact the maintainers" : " 仅存在上游默认源，请联系维护者";
      chsrc_error (xy_strjoin (3, msg1, target, msg2));
      exit (Exit_MaintainerCause);
    }

  /* if (xy_streql ("reset", input)) 不再使用这种方式 */
  if (ProgMode_CMD_Reset)
    {
      char *msg = CliOpt_InEnglish ? "Will reset to the upstream's default source" : "将重置为上游默认源";
      say (msg);
      return 0; /* 返回第1个，因为第1个是上游默认源 */
    }

  if (2==size)
    {
      char *msg1 = CliOpt_InEnglish ? " is " : " 是 ";
      char *msg2 = CliOpt_InEnglish ? "'s ONLY mirror available currently, thanks for their generous support"
                                    : " 目前唯一可用镜像站，感谢他们的慷慨支持";
      const char *name = CliOpt_InEnglish ? sources[1].mirror->abbr
                                          : sources[1].mirror->name;
      chsrc_succ (xy_strjoin (4, name, msg1, target, msg2));
    }

  if (xy_streql ("first", input))
    {
      char *msg = CliOpt_InEnglish ? "Will use the first speedy source measured by maintainers" : "将使用维护团队测速第一的源";
      say (msg);
      return 1; /* 返回第2个，因为第1个是上游默认源 */
    }

  int idx = 0;
  Source_t src = sources[0];

  bool exist = false;
  for (int i=0; i<size; i++)
    {
      src = sources[i];
      if (xy_streql (src.mirror->code, input))
        {
          idx = i;
          exist = true;
          break;
        }
    }
  if (!exist)
    {
      {
        char *msg1 = CliOpt_InEnglish ? "Mirror site "   : "镜像站 ";
        char *msg2 = CliOpt_InEnglish ? " doesn't exist" : " 不存在";
        chsrc_error (xy_strjoin (3, msg1, input, msg2));
      }

      char *msg = CliOpt_InEnglish ? "To see available sources, use chsrc list " : "查看可使用源，请使用 chsrc list ";
      chsrc_error (xy_2strjoin (msg, target));
      exit (Exit_UserCause);
    }
  return idx;
}


/**
 * 该函数来自 oh-my-mirrorz.py，由 @ccmywish 翻译为C语言，但功劳和版权属于原作者
 *
 * @param speed 单位为Byte/s
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
  if (i <= 1 ) new = red (buf);
  else
    {
      if (i == 2 && speed < 2.00) new = yellow (buf);
      else new = green (buf);
    }
  return new;
}


/**
 * 测速代码参考自 https://github.com/mirrorz-org/oh-my-mirrorz/blob/master/oh-my-mirrorz.py
 * 功劳和版权属于原作者，由 @ccmywish 修改为C语言，并做了额外调整
 *
 * @return 返回测得的速度，若出错，返回-1
 *
 * 该函数实际原型为 char * (*)(const char*)
 */
void *
measure_speed_for_url (void *url)
{
  char *time_sec = NULL;

  time_sec = "8";

  /**
   * 现在我们切换至跳转后的链接来测速，不再使用下述判断
   *
   * if (xy_str_start_with(url, "https://registry.npmmirror"))
   *   {
   *     // 这里 npmmirror 跳转非常慢，需要1~3秒，所以我们给它留够至少8秒测速时间，否则非常不准
   *     time_sec = "10";
   *   }
   */

  char *ipv6 = ""; // 默认不启用

  if (CliOpt_IPv6==true)
    {
      ipv6 = "--ipv6";
    }

  char *os_devnull = xy_os_devnull;

  /**
   * @note 我们用 —L，因为部分链接会跳转到其他地方，比如: RubyChina, npmmirror
   */
  char *curl_cmd = xy_strjoin (8, "curl -qsL ", ipv6,
                                  " -o ", os_devnull,
                                  " -w \"%{http_code} %{speed_download}\" -m", time_sec,
                                  " -A chsrc/" Chsrc_Version "  ", url);

  // chsrc_info (xy_2strjoin ("测速命令 ", curl_cmd));

  char *curl_buf = xy_run (curl_cmd, 0);

  return curl_buf;
}


/**
 * @return 返回速度speed，单位为 Byte/s
 */
double
parse_and_say_curl_result (char *curl_buf)
{
  // 分隔两部分数据
  char *split = strchr (curl_buf, ' ');
  if (split) *split = '\0';

  // say(curl_buf); say(split+1);
     int http_code = atoi (curl_buf);
  double     speed = atof (split+1);
    char *speedstr = to_human_readable_speed (speed);

  if (200!=http_code)
    {
      char *http_code_str = yellow (xy_2strjoin ("HTTP码 ", curl_buf));
      say (xy_strjoin (3, speedstr, " | ",  http_code_str));
    }
  else
    {
      say (speedstr);
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
 * @param      sources        所有待测源
 * @param      size           待测源的数量
 * @param[out] speed_records  速度值记录，单位为Byte/s
 */
void
measure_speed_for_every_source (Source_t sources[], int size, double speed_records[])
{
    // bool get_measured[size]; /* 是否真正执行了测速 */
     int get_measured_n = 0; /* 测速了几个        */
   char *measure_msgs[size];

  double speed = 0.0;

  for (int i=0; i<size; i++)
    {
      Source_t src = sources[i];

      SourceProvider_t *provider = src.provider;
      SpeedMeasureInfo_t smi = provider->smi;

      bool skip = smi.skip;

      const char *url = smi.url;

      if (!skip && NULL==url)
        // 这种情况应当被视为bug，但是我们目前还是软处理
        {
          char *msg1 = CliOpt_InEnglish ? "Maintainers don't offer " : "维护者未提供 ";
          char *msg2 = CliOpt_InEnglish ? " mirror site's speed measure link, so skip it" : " 镜像站测速链接，跳过该站点";
          chsrc_warn (xy_strjoin (3, msg1, provider->code, msg2));
          speed = 0;

          speed_records[i] = speed;
          // get_measured[i] = false;
          measure_msgs[i] = NULL;
        }

      if (skip)
        {
          if (xy_streql ("upstream", provider->code))
            {
              /* 上游源不测速，但不置0，因为要避免这种情况: 可能其他镜像站测速都为0，最后反而选择了该 upstream */
              speed = -1024*1024*1024;
              if (!src.url)
                {
                  smi.skip_reason_CN = "上游默认源URL未知，请帮助补充";
                  smi.skip_reason_EN = "The default upstream source URL is unknown, please help to add";
                }
            }
          else if (xy_streql ("user", provider->code))
            {
              /* 代码不会执行至此 */
              speed = 1024*1024*1024;
            }
          else
            {
              /* 不测速的 Provider */
              speed = 0;
            }
          // get_measured[i] = false;
          speed_records[i] = speed;

          const char *msg = CliOpt_InEnglish ? provider->abbr : provider->name;
          const char *skip_reason = CliOpt_InEnglish ? smi.skip_reason_EN : smi.skip_reason_CN;
          if (NULL==skip_reason)
            {
              skip_reason = CliOpt_InEnglish ? "SKIP for no reason" : "无理由跳过";
            }
          measure_msgs[i] = xy_strjoin (4, "  x ", msg, " ", yellow(skip_reason));
          printf ("%s\n", measure_msgs[i]);
        }
      else
        {
          const char *msg = CliOpt_InEnglish ? provider->abbr : provider->name;

          if (xy_streql ("upstream", provider->code))
            {
              measure_msgs[i] = xy_strjoin (5, "  ^ ", msg, " (", src.url, ") ... ");
            }
          else
            {
              measure_msgs[i] = xy_strjoin (3, "  - ", msg, " ... ");
            }


          printf ("%s", measure_msgs[i]);
          fflush (stdout);

          char *url_ = xy_strdup (url);
          char *curl_result = measure_speed_for_url (url_);
          double speed = parse_and_say_curl_result (curl_result);
          speed_records[i] = speed;
        }
    }
}



/**
 * 自动测速选择镜像站和源
 */
#define auto_select_mirror(s) select_mirror_autoly(s##_sources, s##_sources_n, (char*)#s+3)
int
select_mirror_autoly (Source_t *sources, size_t size, const char *target_name)
{
  /* reset 时选择默认源 */
  if (ProgMode_CMD_Reset)
    return 0;

  if (!CliOpt_DryRun)
  {
    char *msg = CliOpt_InEnglish ? "Measuring speed in sequence" : "测速中";
    xy_log_brkt (App_Name, bdpurple (CliOpt_InEnglish ? "MEASURE" : "测速"), msg);
    br();
  }

  if (0==size || 1==size)
    {
      char *msg1 = CliOpt_InEnglish ? "Currently " : "当前 ";
      char *msg2 = CliOpt_InEnglish ? "No any source, please contact maintainers: chsrc issue" : " 无任何可用源，请联系维护者: chsrc issue";
      chsrc_error (xy_strjoin (3, msg1, target_name, msg2));
      exit (Exit_MaintainerCause);
    }

  if (CliOpt_DryRun)
  /* Dry Run 时，跳过测速 */
    {
      return 1; /* 原则第一个源 */
    }

  bool only_one = false;
  if (2==size) only_one = true;

  /** --------------------------------------------- */
  bool exist_curl = chsrc_check_program_quietly_when_exist ("curl");
  if (!exist_curl)
    {
      char *msg = CliOpt_InEnglish ? "No curl, unable to measure speed" : "没有curl命令，无法测速";
      chsrc_error (msg);
      exit (Exit_UserCause);
    }

  if (xy_on_windows)
    {
      char *curl_version = xy_run ("curl --version", 1);
      /**
       * https://github.com/RubyMetric/chsrc/issues/144
       *
       * Cygwin上，curl 的版本信息为:
       *
       *    curl 8.9.1 (x86_64-pc-cygwin)
       *
       */
      if (strstr (curl_version, "pc-cygwin"))
        {
          char *msg = CliOpt_InEnglish ? "You're using curl built by Cygwin which has a bug! Please use another curl!" : "你使用的是Cygwin构建的curl，该版本的curl存在bug，请改用其他版本的curl";
          chsrc_error (msg);
          exit (Exit_UserCause);
        }
    }
  /** --------------------------------------------- */

  /* 总测速记录值 */
  double speed_records[size];
  measure_speed_for_every_source (sources, size, speed_records);
  br();

  /* DEBUG */
  /*
  for (int i=0; i<size; i++)
    {
      printf ("speed_records[%d] = %f\n", i, speed_records[i]);
    }
  */

  int fast_idx = get_max_ele_idx_in_dbl_ary (speed_records, size);

  if (only_one)
    {
      char *msg1 = CliOpt_InEnglish ? "NOTICE  mirror site: " : "镜像站提示: ";
      char   *is = CliOpt_InEnglish ? " is " : " 是 ";
      char *msg2 = CliOpt_InEnglish ? "'s ONLY mirror available currently, thanks for their generous support"
                                    : " 目前唯一可用镜像站，感谢他们的慷慨支持";
      const char *name = CliOpt_InEnglish ? sources[fast_idx].mirror->abbr
                                          : sources[fast_idx].mirror->name;
      say (xy_strjoin (5, msg1, bdgreen(name), green(is), green(target_name), green(msg2)));
    }
  else
    {
      char *msg = CliOpt_InEnglish ? "FASTEST mirror site: " : "最快镜像站: ";
      const char *name = CliOpt_InEnglish ? sources[fast_idx].mirror->abbr
                                          : sources[fast_idx].mirror->name;
      say (xy_2strjoin (msg, green(name)));
    }

  // https://github.com/RubyMetric/chsrc/pull/71
  if (ProgMode_CMD_Measure)
    {
      char *msg = CliOpt_InEnglish ? "URL of above source: " : "镜像源地址: ";
      say (xy_2strjoin (msg, green(sources[fast_idx].url)));
    }

  return fast_idx;
}


#define use_specific_mirror_or_auto_select(input, s) \
  (NULL!=(input)) ? find_mirror(s, input) : auto_select_mirror(s)



bool
source_is_upstream (Source_t *source)
{
  return xy_streql (source->mirror->code, "upstream");
}

bool
source_is_userdefine (Source_t *source)
{
  return xy_streql (source->mirror->code, "user");
}

bool
source_has_empty_url (Source_t *source)
{
  return source->url == NULL;
}

bool
is_reset_mode ()
{
  return ProgMode_CMD_Reset;
}


/**
 * 用户*只可能*通过下面5种方式来换源，无论哪一种都会返回一个 Source_t 出来
 * option:
 *  1. 用户指定某个 Mirror Code
 *  2. NULL: 用户什么都没指定 (将测速选择最快镜像)
 *  3. 用户给了一个 URL
 *  4. ChgType_Reset
 * 选用了Leader target
 *  5. ProgMode_Leader_Selected_Index 将给出所选索引
 *
 * @dependency 变量 option
 */
#define chsrc_yield_for_the_source(for_what) \
  if (ProgMode_Target_Group==true && ProgMode_Leader_Selected_Index==-1) \
    { \
      ProgMode_Leader_Selected_Index = use_specific_mirror_or_auto_select (option, for_what); \
      source = for_what##_sources[ProgMode_Leader_Selected_Index]; \
    } \
  else if (ProgMode_Target_Group==true && ProgMode_Leader_Selected_Index!=-1) \
    { \
      source = for_what##_sources[ProgMode_Leader_Selected_Index]; \
    } \
  else if (is_url (option)) \
    { \
      Source_t __tmp = { &UserDefinedProvider, option }; \
      source = __tmp; \
    } \
  else \
    { \
      int __index = use_specific_mirror_or_auto_select (option, for_what); \
      source = for_what##_sources[__index]; \
    }

#define chsrc_yield_source(for_what) \
  Source_t source; \
  chsrc_yield_for_the_source(for_what)



#define hr() say ("--------------------------------");


/**
 * 用于 _setsrc 函数
 *
 * 1. 告知用户选择了什么源和镜像
 * 2. 对选择的源和镜像站进行一定的校验
 *
 * @translation Done
 */
#define chsrc_confirm_source confirm_source(&source)
void
confirm_source (Source_t *source)
{
  // 由于实现问题，我们把本应该独立出去的上游默认源，也放在了可以换源的数组中，而且放在第一个
  // chsrc 已经规避用户使用未实现的 `chsrc reset`
  // 但是某些用户可能摸索着强行使用 chsrc set target upstream，从而执行起该禁用的功能，
  // 之所以禁用，是因为有的 reset 我们并没有实现，我们在这里阻止这些邪恶的用户
  if (source_is_upstream (source) && source_has_empty_url (source))
    {
      char *msg = CliOpt_InEnglish ? "Not implement `reset` for the target yet" : "暂未对该目标实现重置";
      chsrc_error (msg);
      exit (Exit_Unsupported);
    }
  else if (source_has_empty_url (source))
    {
      char *msg = CliOpt_InEnglish ? "URL of the source doesn't exist, please report a bug to the dev team" : \
                                     "该源URL不存在，请向维护团队提交bug";
      chsrc_error (msg);
      exit (Exit_MaintainerCause);
    }
  else
    {
      char *msg = CliOpt_InEnglish ? "SELECT  mirror site: " : "选中镜像站: ";
      say (xy_strjoin (5, msg, green (source->mirror->abbr), " (", green (source->mirror->code), ")"));
    }

  hr();
}

#define chsrc_yield_source_and_confirm(for_what) chsrc_yield_source(for_what);chsrc_confirm_source


void
chsrc_determine_chgtype (enum ChgType_t type)
{
  ProgMode_ChgType =  is_reset_mode() ? ChgType_Reset : type;
}



#define MSG_EN_PUBLIC_URL "If the URL you specify is a public service, you are invited to contribute: chsrc issue"
#define MSG_CN_PUBLIC_URL "若您指定的URL为公有服务，邀您参与贡献: chsrc issue"

#define MSG_EN_FULLY_AUTO "Fully-Auto changed source. "
#define MSG_CN_FULLY_AUTO "全自动换源完成"

#define MSG_EN_SEMI_AUTO  "Semi-Auto changed source. "
#define MSG_CN_SEMI_AUTO  "半自动换源完成"

#define MSG_EN_THANKS     "Thanks to the mirror site: "
#define MSG_CN_THANKS     "感谢镜像提供方: "

#define MSG_EN_BETTER     "If you have a better source changing method , please help: chsrc issue"
#define MSG_CN_BETTER     "若您有更好的换源方案，邀您帮助: chsrc issue"

#define MSG_EN_CONSTRAINT "Implementation constraints require manual operation according to the above prompts. "
#define MSG_CN_CONSTRAINT "因实现约束需按上述提示手工操作"

#define MSG_EN_STILL      "Still need to operate manually according to the above prompts. "
#define MSG_CN_STILL      "仍需按上述提示手工操作"

#define thank_mirror(msg) chsrc_log(xy_2strjoin(msg,purple(CliOpt_InEnglish?source->mirror->abbr:source->mirror->name)))

/**
 * @param source 可为NULL
 *
 * @param [g]ProgMode_ChgType
 */
void
chsrc_conclude (Source_t *source)
{
  hr();

  // fprintf (stderr, "chsrc: now change type: %d\n", ProgMode_ChgType);

  if (ProgMode_CMD_Reset || ChgType_Reset == ProgMode_ChgType)
    {
      // source_is_upstream (source)
      char *msg = CliOpt_InEnglish ? "Has been reset to the upstream default source" : "已重置为上游默认源";
      chsrc_log (purple (msg));
    }
  else if (ChgType_Auto == ProgMode_ChgType)
    {
      if (source)
        {
          if (source_is_userdefine (source))
            {
              char *msg = CliOpt_InEnglish ? MSG_EN_FULLY_AUTO      MSG_EN_PUBLIC_URL \
                                           : MSG_CN_FULLY_AUTO ", " MSG_CN_PUBLIC_URL;
              chsrc_log (msg);
            }
          else
            {
              char *msg = CliOpt_InEnglish ? MSG_EN_FULLY_AUTO      MSG_EN_THANKS \
                                           : MSG_CN_FULLY_AUTO ", " MSG_CN_THANKS;
              thank_mirror (msg);
            }
        }
      else
        {
          char *msg = CliOpt_InEnglish ? MSG_EN_FULLY_AUTO : MSG_CN_FULLY_AUTO;
          chsrc_log (msg);
        }
    }
  else if (ChgType_SemiAuto == ProgMode_ChgType)
    {
      if (source)
        {
          if (source_is_userdefine (source))
            {
              char *msg = CliOpt_InEnglish ? MSG_EN_SEMI_AUTO      MSG_EN_STILL      MSG_EN_PUBLIC_URL \
                                           : MSG_CN_SEMI_AUTO ", " MSG_CN_STILL "。" MSG_CN_PUBLIC_URL;
              chsrc_log (msg);
            }
          else
            {
              char *msg = CliOpt_InEnglish ? MSG_EN_SEMI_AUTO      MSG_EN_STILL      MSG_EN_THANKS \
                                           : MSG_CN_SEMI_AUTO ", " MSG_CN_STILL "。" MSG_CN_THANKS;
              thank_mirror (msg);
            }
        }
      else
        {
          char *msg = CliOpt_InEnglish ? MSG_EN_SEMI_AUTO      MSG_EN_STILL \
                                       : MSG_CN_SEMI_AUTO ", " MSG_CN_STILL;
          chsrc_log (msg);
        }

      char *msg = CliOpt_InEnglish ? MSG_EN_BETTER : MSG_CN_BETTER;
      chsrc_warn (msg);
    }
  else if (ChgType_Manual == ProgMode_ChgType)
    {
      if (source)
        {
          if (source_is_userdefine (source))
            {
              char *msg = CliOpt_InEnglish ? MSG_EN_CONSTRAINT      MSG_EN_PUBLIC_URL \
                                           : MSG_CN_CONSTRAINT "; " MSG_CN_PUBLIC_URL;
              chsrc_log (msg);
            }
          else
            {
              char *msg = CliOpt_InEnglish ? MSG_EN_CONSTRAINT      MSG_EN_THANKS \
                                           : MSG_CN_CONSTRAINT ", " MSG_CN_THANKS;
              thank_mirror (msg);
            }
        }
      else
        {
          char *msg = CliOpt_InEnglish ? MSG_EN_CONSTRAINT : MSG_CN_CONSTRAINT;
          chsrc_log (msg);
        }
      char *msg = CliOpt_InEnglish ? MSG_EN_BETTER : MSG_CN_BETTER;
      chsrc_warn (msg);
    }
  else if (ChgType_Untested == ProgMode_ChgType)
    {
      if (source)
        {
          if (source_is_userdefine (source))
            {
              char *msg = CliOpt_InEnglish ? MSG_EN_PUBLIC_URL : MSG_CN_PUBLIC_URL;
              chsrc_log (msg);
            }
          else
            {
              char *msg = CliOpt_InEnglish ? MSG_EN_THANKS : MSG_CN_THANKS;
              thank_mirror (msg);
            }
        }
      else
        {
          char *msg = CliOpt_InEnglish ? "Auto changed source" : "自动换源完成";
          chsrc_log (msg);
        }

      char *msg = CliOpt_InEnglish ? "The method hasn't been tested or has any feedback, please report usage: chsrc issue" : "该换源步骤已实现但未经测试或存在任何反馈，请报告使用情况: chsrc issue";
      chsrc_warn (msg);
    }
  else
    {
      fprintf (stderr, "chsrc: Wrong change type: %d\n", ProgMode_ChgType);
      xy_unreached();
    }
}



void
chsrc_ensure_root ()
{
  char *euid = getenv ("$EUID");
  if (NULL==euid)
    {
      char *buf = xy_run ("id -u", 0);
      if (0!=atoi(buf)) goto not_root;
      else return;
    }
  else
    {
      if (0!=atoi(euid)) goto not_root;
      else return;
    }

  char *msg = NULL;
not_root:
  msg = CliOpt_InEnglish ? "Use sudo before the command or switch to root to ensure the necessary permissions"
                         : "请在命令前使用 sudo 或切换为root用户来保证必要的权限";
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
  if (ProgMode_Run_as_a_Service)
    {
      run_option |= RunOpt_Dont_Notify_On_Success|RunOpt_No_Last_New_Line;
    }
  else
    {
      if (CliOpt_InEnglish)
        xy_log_brkt (blue (App_Name), bdblue ("RUN"), blue (cmd));
      else
        xy_log_brkt (blue (App_Name), bdblue ("运行"), blue (cmd));
    }

  if (CliOpt_DryRun)
    {
      return; // Dry Run 此时立即结束，并不真正执行
    }

  int status = system (cmd);
  if (0==status)
    {
      if (! (RunOpt_Dont_Notify_On_Success & run_option))
        {
          log_cmd_result (true, status);
        }
    }
  else
    {
      log_cmd_result (false, status);
      if (! (run_option & RunOpt_Dont_Abort_On_Failure))
        {
          char *msg = CliOpt_InEnglish ? "Fatal error, forced end" : "关键错误，强制结束";
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
  ProgMode_Run_as_a_Service = true;
    run_option |= RunOpt_Dont_Notify_On_Success|RunOpt_No_Last_New_Line;
    chsrc_run (cmd, run_option);
  ProgMode_Run_as_a_Service = false;
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
#ifdef XY_On_Windows
      if (first)
        {
          cmd = xy_strjoin (3, "set ", var, " ");
          first = false;
        }
      else
        {
          cmd = xy_strjoin (4, cmd, "& set ", var, " ");
        }
#else
      if (first)
        {
          cmd = xy_strjoin (5, "echo ", var, "=$", var, " ");
          first = false;
        }
      else
        {
          cmd = xy_strjoin (6, cmd, "; echo ", var, "=$", var, " ");
        }
#endif
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
      int status = system (cmd);
      if (status!=0) {/* NOOP */}
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
  if (xy_on_windows)
    {
      cmd = xy_2strjoin ("type ", path);
    }
  else
    {
      cmd = xy_2strjoin ("cat ", path);
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

  chsrc_run_as_a_service (cmd);

  char *msg = CliOpt_InEnglish ? "Directory doesn't exist, created automatically " : "目录不存在，已自动创建 ";
  chsrc_note2 (xy_2strjoin (msg, dir));
}


static void
chsrc_append_to_file (const char *str, const char *filename)
{
  if (CliOpt_DryRun)
    {
      goto log_anyway;
    }

  char *file = xy_normalize_path (filename);
  char *dir = xy_parent_dir (file);
  chsrc_ensure_dir (dir);

  FILE *f = fopen (file, "a");
  if (NULL==f)
    {
      char *msg = CliOpt_InEnglish ? xy_2strjoin ("Unable to open file to write: ", file)
                                   : xy_2strjoin ("无法打开文件以写入: ", file);
      chsrc_error2 (msg);
      exit (Exit_UserCause);
    }

  size_t len = strlen (str);

  size_t ret = fwrite (str, len, 1, f);
  if (ret != 1)
    {
      char *msg = CliOpt_InEnglish ? xy_2strjoin ("Write failed to ", file)
                                   : xy_2strjoin ("写入文件失败: ", file);
      chsrc_error2 (msg);
      exit (Exit_UserCause);
    }

  fclose (f);

log_anyway:
  /* 输出recipe指定的文件名 */
  chsrc_log_write (filename);

  /*
  char *cmd = NULL;
  if (xy_on_windows)
    {
      cmd = xy_strjoin (4, "echo ", str, " >> ", file);
    }
  else
    {
      cmd = xy_strjoin (4, "echo '", str, "' >> ", file);
    }
  chsrc_run_a_service (cmd);
  */
}

static void
chsrc_prepend_to_file (const char *str, const char *filename)
{
  if (CliOpt_DryRun)
    {
      goto log_anyway;
    }

  char *file = xy_normalize_path (filename);
  char *dir = xy_parent_dir (file);
  chsrc_ensure_dir (dir);

  char *cmd = NULL;
  if (xy_on_windows)
    {
      xy_unimplemented();
    }
  else
    {
      cmd = xy_strjoin (4, "sed -i '1i ", str, "' ", file);
    }
  chsrc_run_as_a_service (cmd);

log_anyway:
  /* 输出recipe指定的文件名 */
  chsrc_log_write (filename);
}

static void
chsrc_overwrite_file (const char *str, const char *filename)
{
  if (CliOpt_DryRun)
    {
      goto log_anyway;
    }

  char *file = xy_normalize_path (filename);
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
  chsrc_run_as_a_service (cmd);

log_anyway:
  /* 输出recipe指定的文件名 */
  chsrc_log_write (filename);
}

static void
chsrc_backup (const char *path)
{
  if (CliOpt_DryRun)
    {
      goto log_anyway;
    }

  char *cmd = NULL;
  bool exist = xy_file_exist (path);

  if (!exist)
    {
      char *msg = CliOpt_InEnglish ? "File doesn't exist, skip backup: " : "文件不存在,跳过备份: ";
      chsrc_note2 (xy_2strjoin (msg, path));
      return;
    }

  if (xy_on_bsd || xy_on_macos)
    {
      /* BSD 和 macOS 的 cp 不支持 --backup 选项 */
      cmd = xy_strjoin (5, "cp -f ", path, " ", path, ".bak");
    }
  else if (xy_on_windows)
    {
      /**
       * @note /Y 表示覆盖
       * @note 默认情况下会输出一个 "已复制  1个文件"
       */
      cmd = xy_strjoin (5, "copy /Y ", path, " ", path, ".bak 1>nul");
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
          cmd = xy_strjoin (5, "cp ", path, " ", path, ".bak --backup='t'");
        }
      else
        {
          /* 非 GNU 的 cp 可能不支持 --backup ，如 busybox cp */
          cmd = xy_strjoin (5, "cp -f ", path, " ", path, ".bak");
        }
    }

  chsrc_run_as_a_service (cmd);

log_anyway:
  chsrc_log_backup (path);
}


/**
 * 检查过程中全程保持安静
 */
static char *
chsrc_get_cpuarch ()
{
  char *ret;
  char *msg;

#if XY_On_Windows
  SYSTEM_INFO info;
  GetSystemInfo (&info);
  WORD num = info.wProcessorArchitecture;
  switch (num)
    {
      case PROCESSOR_ARCHITECTURE_AMD64:
        ret = "x86_64"; break;
      case PROCESSOR_ARCHITECTURE_ARM:
        ret = "arm";    break;
      case PROCESSOR_ARCHITECTURE_INTEL:
        ret = "x86";    break;
      case PROCESSOR_ARCHITECTURE_IA64:
        ret = "IA-64";  break;
      case PROCESSOR_ARCHITECTURE_UNKNOWN:
      default:
        msg = CliOpt_InEnglish ? "Unable to detect CPU type" : "无法检测到CPU类型";
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
      msg = CliOpt_InEnglish ? "Unable to detect CPU type" : "无法检测到CPU类型";
      chsrc_error (msg);
      exit (Exit_UserCause);
    }
#endif
}


static int
chsrc_get_cpucore ()
{
  int cores = 2;

#if XY_On_Windows
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
