/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Name     : source.c
 * File Authors  : @ccmywish
 *               | @G_I_Y
 * Contributors  : @Yangmoooo
 *               | @BingChunMoLi
 *               | @Mikachu2333
 *               | @JIAFALSEDREAM
 *               |
 * Created On    : <2023-08-29>
 * Last Modified : <2026-08-20>
 *
 * 源的确定
 * ------------------------------------------------------------*/

Source_t
current_shared_source ()
{
  int depth = current_combo_stack_depth ();

  if (depth <= 0)
    return (Source_t){ NULL, NULL, NULL };

  return ProgStatus.SharedSource[depth - 1];
}

/**
 * 该函数来自 oh-my-mirrorz.py，由 @ccmywish 翻译为C语言，但功劳和版权属于原作者
 *
 * @param speed 单位为Byte/s
 */
static char *
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
  if (i <= 1 )
    {
      new = red(buf);
    }
  else
    {
      if (i == 2 && speed < 2.00) new = yellow(buf);
      else new = green(buf);
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
static void *
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

  if (in_ipv6_mode())
    {
      ipv6 = "--ipv6";
    }

  char *os_devnull = xy.os_devnull;

  /**
   * @note 我们用 —L，因为部分链接会跳转到其他地方，比如: RubyChina, npmmirror
   */
  char *curl_cmd = xy_strcat (10, "curl -qsL ", ipv6,
                                    " -o ", os_devnull,
                                    " -w \"%{http_code} %{speed_download}\" -m", time_sec,
                                    " -A ", ProgStatus.user_agent, " ", url);

  // chsrc_note2 (xy_2strcat ("测速命令 ", curl_cmd));
  char *curl_buf = xy_run (curl_cmd, 0);
  return curl_buf;
}


/**
 * @return 返回速度speed，单位为 Byte/s
 */
static double
parse_and_say_curl_result (char *curl_buf)
{
  /* 分隔两部分数据 */
  char *split = strchr (curl_buf, ' ');
  if (split) *split = '\0';

  // say(curl_buf); say(split+1);
  int  http_code = xy_str2int (curl_buf);
  double   speed = split ? xy_str2float (split+1) : 0.0;
  char *speedstr = to_human_readable_speed (speed);

  /* xy_str2int() 可能会返回0，表示转换失败 */
  if (0==http_code)
    {
      char *msg = ENGLISH ? "ERROR curl output: " : "错误 curl 输出: ";
      println (red (xy_2strcat (msg, curl_buf)));
    }
  else if (200!=http_code)
    {
      char *http_code_str = yellow (xy_2strcat ("HTTP码 ", curl_buf));
      println (xy_strcat (3, speedstr, " | ",  http_code_str));
    }
  else
    {
      println (speedstr);
    }
  return speed;
}


static int
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
static void
measure_speed_for_every_source (Source_t sources[], int size, double speed_records[])
{
  // bool get_measured[size]; /* 是否真正执行了测速 */
  int get_measured_n = 0;     /* 测速了几个        */
  char *measure_msgs[size];

  double speed = 0.0;

  for (int i=0; i<size; i++)
    {
      Source_t src = sources[i];

      SourceProvider_t *provider = src.provider;
      ProviderSpeedMeasureInfo_t psmi = provider->psmi;

      bool provider_skip = psmi.skip;

      bool has_dedicated_speed_url = false;

      /**
       * 存在两类测速链接
       * 1. 有*专用测速链接*时，我们选专用，这是精准测速
       * 2. 若无，我们用*镜像站整体测速链接*来进行代替，
       *      若是专用镜像站，则是精准测速
       *      若是通用镜像站，则是模糊测速
       */
      const char *provider_speed_url = psmi.url;
      const char *dedicated_speed_url = src.speed_measure_url;

      /* 最终用来测速的 URL */
      char *url = NULL;

      if (!provider_skip && !provider_speed_url)
      /* 没有声明跳过，但是却没有提供 URL，这是维护者维护时出了纰漏，我们软处理 */
        {
          char *msg1 = ENGLISH ? "Maintainers don't offer " : "维护者未提供 ";
          char *msg2 = ENGLISH ? " mirror site's speed measure link, so skip it" : " 镜像站测速链接，跳过该站点（需修复）";
          chsrc_warn (xy_strcat (3, msg1, provider->code, msg2));
          speed = 0;

          speed_records[i] = speed;
          // get_measured[i] = false;
          measure_msgs[i] = NULL;
        }
      else if (!provider_skip && provider_speed_url)
        {
          if (hp_is_url (provider_speed_url))
            {
              url = xy_strdup (provider_speed_url);
              chsrc_debug ("m", xy_2strcat ("使用镜像站整体测速链接: ", url));
            }
        }
      else if (provider_skip)
        {
          /* Provider 被声明为跳过测速，下方判断精准测速链接有无提供，若也没有提供，将会输出跳过原因  */
        }

      if (dedicated_speed_url)
        {
          if (hp_is_url (dedicated_speed_url))
            {
              url = xy_strdup (dedicated_speed_url);
              has_dedicated_speed_url = true;
              chsrc_debug ("m", xy_2strcat ("使用专用测速链接: ", url));
            }
          else
            {
              /* 防止维护者没填，这里有一些脏数据，我们软处理：假装该链接URL不存在 */
              has_dedicated_speed_url = false;
              chsrc_debug ("m", xy_2strcat ("专用测速链接为脏数据，请修复: ", provider->name));
            }
        }


      if (provider_skip && !has_dedicated_speed_url)
        {
          if (xy_streql ("upstream", provider->code))
            {
              /* 上游源不测速，但不置0，因为要避免这么一种情况: 可能其他镜像站测速都为0，最后反而选择了该 upstream */
              speed = -1024*1024*1024;
              if (!src.url)
                {
                  psmi.skip_reason_ZH = "缺乏对上游默认源进行测速的URL，请帮助补充";
                  psmi.skip_reason_EN = "Lack of URL to measure upstream default source provider, please help to add";
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

          const char *msg = ENGLISH ? provider->abbr : provider->name;
          const char *skip_reason = ENGLISH ? psmi.skip_reason_EN : psmi.skip_reason_ZH;
          if (NULL==skip_reason)
            {
              skip_reason = ENGLISH ? "SKIP for no reason" : "无理由跳过";
            }
          measure_msgs[i] = xy_strcat (4, faint("  x "), msg, " ", yellow(faint(skip_reason)));
          println (measure_msgs[i]);

          /* 下一位 */
          continue;
        }

      /* 此时，一定获得了一个用于测速的链接 */
      if (url)
        {
          const char *msg = ENGLISH ? provider->abbr : provider->name;

          bool is_accurate = false;
          if (has_dedicated_speed_url)
            {
              is_accurate = true;
            }
          else if (provider->psmi.accurate)
            {
              is_accurate = true;
            }

          char *accurate_msg = CHINESE ? (is_accurate ? bdblue(faint("[精准测速]")) :  faint("[模糊测速]"))
                                       : (is_accurate ? bdblue(faint("[accurate]")) : faint("[rough]"));

          if (xy_streql ("upstream", provider->code))
            {
              measure_msgs[i] = xy_strcat (7, faint("  ^ "), msg, " (", src.url, ") ", accurate_msg, faint(" ... "));
            }
          else
            {
              measure_msgs[i] = xy_strcat (5, faint("  - "), msg, " ", accurate_msg, faint(" ... "));
            }

          print (measure_msgs[i]);
          fflush (stdout);

          char *curl_result = measure_speed_for_url (url);
          double speed = parse_and_say_curl_result (curl_result);
          speed_records[i] = speed;

          /* 释放 url 内存 */
          if (url) free (url);
        }
      else
        {
          xy_unreached();
        }
    }
}



/**
 * 自动测速选择镜像站和源
 */
Source_t
dish_select_fastest_source (Dish_t *dish)
{
  Source_t *sources = dish->sources;
  size_t size = dish->sources_n;
  char *dish_name = dish->aliases;

  /* 使用缓存 */
  if (chsrc_in_dish_group_mode())
    {
      if (current_combo_dish()->all_sub_dishes_use_same_source)
        {
          Source_t shared = current_shared_source();
          if (shared.provider && shared.url)
            {
              char *msg = CHINESE ? "使用上次测速的最快源"
                                  : "Using last measured fastest source";
              xy_log_brkt (App_Name, bdpurple (CHINESE ? "测速" : "MEASURE"), msg);
              return shared;
            }
        }
    }

  /* reset 时选择默认源 */
  if (chsrc_in_reset_mode())
    return sources[0];

  if (!in_dry_run_mode())
  {
    char *msg = CHINESE ? "进行中" : "working";
    xy_log_brkt (App_Name, bdpurple (CHINESE ? "测速" : "MEASURE"), msg);
    br();
  }

  if (0==size || 1==size)
    {
      char *msg1 = CHINESE ? "当前 " : "Currently ";
      char *msg2 = CHINESE ? " 无任何可用源，请联系维护者: chsrc issue" : "No any source, please contact maintainers: chsrc issue";
      chsrc_error (xy_strcat (3, msg1, dish_name, msg2));
      exit (Exit_MaintainerCause);
    }

  if (in_dry_run_mode())
  /* Dry Run 时，跳过测速 */
    {
      return sources[1]; /* 原则第一个源 */
    }

  bool only_one = false;
  if (2==size) only_one = true;

  /** --------------------------------------------- */
  bool exist_curl = chsrc_check_program_quietly_when_exist ("curl");
  if (!exist_curl)
    {
      char *msg = CHINESE ? "没有curl命令，无法测速"
                          : "No curl, unable to measure speed";
      chsrc_error (msg);
      exit (Exit_UserCause);
    }

  if (xy.on_windows)
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
          char *msg = CHINESE ? "你使用的是Cygwin构建的curl，该版本的curl存在bug，请改用其他版本的curl"     : "You're using curl built by Cygwin which has a bug! Please use another curl!";
          chsrc_error (msg);
          exit (Exit_UserCause);
        }
    }
  /* --------------------------------------------- */

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

  int fastest_idx = get_max_ele_idx_in_dbl_ary (speed_records, size);
  Source_t source = sources[fastest_idx];

  if (only_one)
    {
      char *msg1 = CHINESE ? "镜像站提示: " : "NOTICE  mirror site: ";
      char   *is = CHINESE ? " 是 " : " is ";
      char *msg2 = CHINESE ? " 目前唯一可用镜像站，感谢他们的慷慨支持"
                           : "'s ONLY mirror available currently, thanks for their generous support";
      const char *name = CHINESE ? source.mirror->name
                                 : source.mirror->abbr;
      println (xy_strcat (5, msg1, bdgreen(name), green(is), green(dish_name), green(msg2)));
    }
  else
    {
      char *msg = CHINESE ? "最快镜像站: " : "FASTEST mirror site: ";
      const char *name = CHINESE ? source.mirror->name
                                 : source.mirror->abbr;
      println (xy_2strcat (msg, green(name)));
    }

  // https://github.com/RubyMetric/chsrc/pull/71
  if (in_measure_mode())
    {
      char *msg = ENGLISH ? "URL of above source: " : "镜像源地址: ";
      println (xy_2strcat (msg, green(source.url)));
    }


  /* 缓存 */
  if (chsrc_in_dish_group_mode())
    {
      if (current_combo_dish()->all_sub_dishes_use_same_source)
        {
          ProgStatus.SharedSource[current_combo_stack_depth() - 1] = source;
        }
    }

  return source;
}



static bool
source_is_upstream (Source_t *source)
{
  return xy_streql (source->mirror->code, "upstream");
}

static bool
source_is_userdefine (Source_t *source)
{
  return xy_streql (source->mirror->code, "user");
}

static bool
source_has_empty_url (Source_t *source)
{
  return source->url == NULL;
}


/**
 * @brief 在 combo dish 中为当前 sub dish 解析源
 *
 * 用户只提供一个 mirror code，
 *
 * 1. 若当前 sub dish 拥有该 code，直接使用它
 * 2. 若1不成立，而某个兄弟 sub dish 拥有该 code，说明该 code 仅属于另一类源，
 *    当前 sub dish 应自动测速选择其最快镜像
 * 3. 若1和2都不成立，则直接报错
 */
Source_t
subdish_select_source_by_mirror_code (Dish_t *dish, char *mirror_code)
{
  if (dish_has_source_from_mirror (dish, mirror_code))
    return dish_select_source_by_mirror_code (dish, mirror_code);

  if (subdish_sibling_has_source_from_mirror (dish, mirror_code))
    {
      chsrc_alert2 (CHINESE ? "当前子菜品没有来自所请求镜像站的源，尝试测速选择最快源"
                            : "Current sub dish has no source from the requested mirror, try to auto select the fastest source");
      return dish_select_fastest_source (dish);
    }


  chsrc_error (CHINESE ? "所有的子菜品均没有来自所请求镜像站的源"
                       : "All sub dishes have no source from the requested mirror");
  exit (Exit_UserCause);
}



/**
 * @brief 为该 dish 确定最终将使用的源
 *
 * 用户*只可能*通过下面5种方式来换源，无论哪一种都会返回一个 Source_t 出来
 *
 *   1. 用户指定了一个 mirror code，即 chsrc set <dish> <code>
 *   2. 用户指定了一个 URL，        即 chsrc set <dish> https://url
 *   3. 用户什么都没指定，          即 chsrc set <dish>
 *   4. 用户正在重置源，            即 chsrc reset <dish>
 */
Source_t
chsrc_yield_source (Dish_t *dish, char *option)
{
  /**
   * 防止某些意外时刻 _setsrc() 等函数会被直接调，但此时 _prepare() 还没有执行过
   * 我们在这里卡一道，确保 _prepare() 被调用
   *
   * 目前可能出现这种情况的时候：组换源的时候，sub dishes 的 _setsrc() 被直接调用
   */
  if (!dish->prepared) dish->preparefn();

  Source_t source;
  char *user_defined_url = NULL;
  char *mirror_code = NULL;

  if (hp_is_url (option))
    {
      user_defined_url = option;
    }
  else if (option)
    {
      mirror_code = option;
    }

  /**
   * @note
   *   这里的逻辑需要厘清: 这个函数本身，只会被 _setsrc() 调用，而
   *   combo dish 自身是没有 _setsrc() 的！所以进入到这里来的，必
   *   定是 combo dish 的 sub dish 的 _setsrc()
   */
  if (user_defined_url)
    {
      if (!(dish->can_user_define))
        {
          char *en_msg = "Using user-defined sources for this dish is not supported.";
          char *zh_msg = dish->user_define_cap_explain ? xy_2strcat ("暂不支持对该菜品使用用户自定义源，", dish->user_define_cap_explain) : "暂不支持对该菜品使用用户自定义源";
          chsrc_error (ENGLISH ? en_msg : zh_msg);
          exit (Exit_Unsupported);
        }
      Source_t tmp = { &UserDefinedProvider, user_defined_url };
      source = tmp;
    }
  else if (mirror_code)
    {
      if (chsrc_in_dish_group_mode())
        {
          source = subdish_select_source_by_mirror_code (dish, mirror_code);
        }
      else
        {
          source = dish_select_source_by_mirror_code (dish, mirror_code);
        }
    }
  else
    {
      source = dish_select_fastest_source (dish);
    }
  return source;
}




#define hr() say ("--------------------------------");


/**
 * 用于 _setsrc 函数
 *
 * 1. 告知用户选择了什么源和镜像
 * 2. 对选择的源和镜像站进行一定的校验
 */
void
chsrc_confirm_source (Source_t *source)
{
  // 由于实现问题，我们把本应该独立出去的上游默认源，也放在了可以换源的数组中，而且放在第一个
  // chsrc 已经规避用户使用未实现的 `chsrc reset`
  // 但是某些用户可能摸索着强行使用 chsrc set dish upstream，从而执行起该禁用的功能，
  // 之所以禁用，是因为有的 reset 我们并没有实现，我们在这里阻止这些邪恶的用户
  if (source_is_upstream (source) && source_has_empty_url (source))
    {
      char *msg = ENGLISH ? "Not implement `reset` for the dish yet" : "暂未对该菜品实现重置";
      chsrc_error (msg);
      exit (Exit_Unsupported);
    }
  else if (source_has_empty_url (source))
    {
      char *msg = ENGLISH ? "URL of the source doesn't exist, please report a bug to the dev team" : \
                                     "该源URL不存在，请向维护团队提交bug";
      chsrc_error (msg);
      exit (Exit_MaintainerCause);
    }
  else
    {
      char *msg = ENGLISH ? "SELECT  mirror site: " : "选中镜像站: ";
      say (xy_strcat (5, msg, green (source->mirror->abbr), " (", green (source->mirror->code), ")"));
    }

  hr();
}


Source_t
chsrc_yield_source_and_confirm (Dish_t *dish, char *option)
{
  Source_t source = chsrc_yield_source(dish, option);
  chsrc_confirm_source(&source);
  return source;
}


void
chsrc_determine_chgtype (ChgType_t type)
{
  ProgStatus.chgtype =  chsrc_in_reset_mode() ? ChgType_Reset : type;
}


#define MSG_EN_PUBLIC_URL "If the URL you specify is a public service, you are invited to contribute: chsrc issue"
#define MSG_ZH_PUBLIC_URL "若您指定的URL为公有服务，邀您参与贡献: chsrc issue"

#define MSG_EN_FULLY_AUTO "Fully-Auto changed source. "
#define MSG_ZH_FULLY_AUTO "全自动换源完成"

#define MSG_EN_SEMI_AUTO  "Semi-Auto changed source. "
#define MSG_ZH_SEMI_AUTO  "半自动换源完成"

#define MSG_EN_THANKS     "Thanks to the mirror site: "
#define MSG_ZH_THANKS     "感谢镜像提供方: "

#define MSG_EN_BETTER     "If you have a better source changing method , please help: chsrc issue"
#define MSG_ZH_BETTER     "若您有更好的换源方案，邀您帮助: chsrc issue"

#define MSG_EN_CONSTRAINT "Implementation constraints require manual operation according to the above prompts. "
#define MSG_ZH_CONSTRAINT "因实现约束需按上述提示手工操作"

#define MSG_EN_STILL      "Still need to operate manually according to the above prompts. "
#define MSG_ZH_STILL      "仍需按上述提示手工操作"

#define thank_mirror(msg) say(xy_2strcat(msg,purple(ENGLISH?source->mirror->abbr:source->mirror->name)))

/**
 * @param source 可为NULL
 *
 * @dependency @gvar:ProgStatus.chgtype
 */
void
chsrc_conclude (Source_t *source)
{
  hr();

  // fprintf (stderr, "chsrc: now change type: %d\n", ProgStatus.chgtype);
  if (chsrc_in_reset_mode())
    {
      // source_is_upstream (source)
      char *msg = CHINESE ? "已重置为上游默认源"
                          : "Has been reset to the upstream default source";
      chsrc_log (purple (msg));
    }
  else if (ChgType_Auto == ProgStatus.chgtype)
    {
      if (source)
        {
          if (source_is_userdefine (source))
            {
              char *msg = ENGLISH ? MSG_EN_FULLY_AUTO      MSG_EN_PUBLIC_URL \
                                  : MSG_ZH_FULLY_AUTO ", " MSG_ZH_PUBLIC_URL;
              chsrc_log (msg);
            }
          else
            {
              char *msg = ENGLISH ? MSG_EN_FULLY_AUTO      MSG_EN_THANKS \
                                  : MSG_ZH_FULLY_AUTO ", " MSG_ZH_THANKS;
              thank_mirror (msg);
            }
        }
      else
        {
          char *msg = ENGLISH ? MSG_EN_FULLY_AUTO : MSG_ZH_FULLY_AUTO;
          chsrc_log (msg);
        }
    }
  else if (ChgType_SemiAuto == ProgStatus.chgtype)
    {
      if (source)
        {
          if (source_is_userdefine (source))
            {
              char *msg = ENGLISH ? MSG_EN_SEMI_AUTO      MSG_EN_STILL      MSG_EN_PUBLIC_URL \
                                  : MSG_ZH_SEMI_AUTO ", " MSG_ZH_STILL "。" MSG_ZH_PUBLIC_URL;
              chsrc_log (msg);
            }
          else
            {
              char *msg = ENGLISH ? MSG_EN_SEMI_AUTO      MSG_EN_STILL      MSG_EN_THANKS \
                                  : MSG_ZH_SEMI_AUTO ", " MSG_ZH_STILL "。" MSG_ZH_THANKS;
              thank_mirror (msg);
            }
        }
      else
        {
          char *msg = ENGLISH ? MSG_EN_SEMI_AUTO      MSG_EN_STILL \
                              : MSG_ZH_SEMI_AUTO ", " MSG_ZH_STILL;
          chsrc_log (msg);
        }

      char *msg = ENGLISH ? MSG_EN_BETTER : MSG_ZH_BETTER;
      chsrc_warn (msg);
    }
  else if (ChgType_Manual == ProgStatus.chgtype)
    {
      if (source)
        {
          if (source_is_userdefine (source))
            {
              char *msg = ENGLISH ? MSG_EN_CONSTRAINT      MSG_EN_PUBLIC_URL \
                                  : MSG_ZH_CONSTRAINT "; " MSG_ZH_PUBLIC_URL;
              chsrc_log (msg);
            }
          else
            {
              char *msg = ENGLISH ? MSG_EN_CONSTRAINT      MSG_EN_THANKS \
                                  : MSG_ZH_CONSTRAINT ", " MSG_ZH_THANKS;
              thank_mirror (msg);
            }
        }
      else
        {
          char *msg = ENGLISH ? MSG_EN_CONSTRAINT : MSG_ZH_CONSTRAINT;
          chsrc_log (msg);
        }
      char *msg = ENGLISH ? MSG_EN_BETTER : MSG_ZH_BETTER;
      chsrc_warn (msg);
    }
  else if (ChgType_Untested == ProgStatus.chgtype)
    {
      if (source)
        {
          if (source_is_userdefine (source))
            {
              char *msg = ENGLISH ? MSG_EN_PUBLIC_URL : MSG_ZH_PUBLIC_URL;
              chsrc_log (msg);
            }
          else
            {
              char *msg = ENGLISH ? MSG_EN_THANKS : MSG_ZH_THANKS;
              thank_mirror (msg);
            }
        }
      else
        {
          char *msg = ENGLISH ? "Auto changed source" : "自动换源完成";
          chsrc_log (msg);
        }

      char *msg = ENGLISH ? "The method hasn't been tested or has any feedback, please report usage: chsrc issue" : "该换源步骤已实现但未经测试或存在任何反馈，请报告使用情况: chsrc issue";
      chsrc_warn (msg);
    }
  else
    {
      fprintf (stderr, "chsrc: Wrong change type: %d\n", ProgStatus.chgtype);
      xy_unreached();
    }
}


/**
 * @brief 检测该 dish 是否实现了用户所指定的 scope 能力
 *
 * @note 此函数目前只支持中文
 */
void
chsrc_check_scope_capability (Dish_t *dish)
{
  ScopeCapability_t cap = ScopeCap_Unknown;

  char *msg1 = "不支持";
  char *msg2 = "换源，请使用 chsrc ls ";
  char *msg3 = " 查看支持的作用域以及默认作用域";

  char *aliases = dish->aliases;
  char *scope_name = NULL;

  if (chsrc_in_project_scope_mode())
    {
      cap = dish->scope_caps[ScopeCap_Slot_Project];

      if (cap != ScopeCap_Able_And_Implemented)
        {
          scope_name = "项目级";
          char* msg = xy_strcat (5, msg1, scope_name, msg2, aliases, msg3);
          chsrc_error (msg);
          exit (Exit_UserCause);
        }
    }
  if (chsrc_in_user_scope_mode())
    {
      cap = dish->scope_caps[ScopeCap_Slot_User];

      if (cap != ScopeCap_Able_And_Implemented)
        {
          scope_name = "用户级";
          char* msg = xy_strcat (5, msg1, scope_name, msg2, aliases, msg3);
          chsrc_error (msg);
          exit (Exit_UserCause);
        }
    }
  if (chsrc_in_system_scope_mode())
    {
      cap = dish->scope_caps[ScopeCap_Slot_System];

      if (cap != ScopeCap_Able_And_Implemented)
        {
          scope_name = "系统级";
          char* msg = xy_strcat (5, msg1, scope_name, msg2, aliases, msg3);
          chsrc_error (msg);
          exit (Exit_UserCause);
        }
    }
}
