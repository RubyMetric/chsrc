/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : Heng Guo <2085471348@qq.com>
 * Contributors   : Aoran Zeng <ccmywish@qq.com>
 * Created On     : <2023-09-17>
 * Major Revision :      1
 * Last Modified  : <2025-08-09>
 * ------------------------------------------------------------*/

/**
 * @update 2025-06-20
 */
static Source_t os_opensuse_sources[] =
{
  {&UpstreamProvider,  NULL, NULL},
  {&Ali,              "https://mirrors.aliyun.com/opensuse",  DelegateToMirror},
  {&Volcengine,       "https://mirrors.volces.com/opensuse",  DelegateToMirror},
  {&Bfsu,             "https://mirrors.bfsu.edu.cn/opensuse", DelegateToMirror},
  {&Ustc,             "https://mirrors.ustc.edu.cn/opensuse", DelegateToMirror},
  {&Tuna,             "https://mirrors.tuna.tsinghua.edu.cn/opensuse", DelegateToMirror},
  {&Tencent,          "https://mirrors.tencent.com/opensuse", DelegateToMirror},
  // {&Tencent_Intra, "https://mirrors.tencentyun.com/opensuse", DelegateToMirror},

  /* 不启用原因：过慢 */
  // {&Netease,          "https://mirrors.163.com/opensuse",   DelegateToMirror},
  /* 不启用原因：过慢 */
  // {&Sohu,             "https://mirrors.sohu.com/opensuse",  DelegateToMirror}
};
def_sources_n(os_opensuse);


/**
 * @consult https://mirrors.tuna.tsinghua.edu.cn/help/opensuse/
 */
void
os_opensuse_setsrc (char *option)
{
  // chsrc_ensure_root ();

  chsrc_yield_source_and_confirm (os_opensuse);

  while (1) {
    chsrc_note2 ("请选择你的操作系统为:");
    printf ("%s",
      "1. openSUSE Leap\n"
      "2. openSUSE Tumbleweed\n"
      "\n"
      "==> ");

    int choice = 0;

    /* 接受到一个数字时返回1，非法为0，流结束为-1 */
    if (scanf ("%d", &choice) != 1)
      {
        /* 清除输入缓冲区 */
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
        chsrc_error2 ("输入无效！请输入数字");
        continue;
      }
    if (choice == 1)
      {
        char *script = xy_str_gsub (RAWSTR_os_openSUSE_leap_in_bash, "@url@", source.url);
        chsrc_run_as_bash_file (script);
        break;
      }
    if (choice == 2)
      {
        char *script = xy_str_gsub (RAWSTR_os_openSUSE_tumbleweed_in_bash, "@url@", source.url);
        chsrc_run_as_bash_file (script);
        break;
      }
    else
      {
        chsrc_error2 ("无效的选择，请输入 1 或 2 ");
      }
  }

  chsrc_determine_chgtype (ChgType_Untested);
  chsrc_conclude (&source);
}


Feature_t
os_opensuse_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = false;
  f.can_reset = false;

  f.cap_locally = CanNot;
  f.cap_locally_explain = NULL;
  f.can_english = false;
  f.can_user_define = false;

  f.note = NULL;
  return f;
}


def_target_sf(os_opensuse);
