/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(os_opensuse);

void
os_opensuse_prelude ()
{
  use_this(os_opensuse);

  chef_set_created_on   (this, "2023-09-17");
  chef_set_last_updated (this, "2025-08-10");
  chef_set_sources_last_updated (this, "2025-06-20");

  chef_set_authors (this, 1, "Heng Guo", "2085471348@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_sous_chefs (this, 0);
  chef_set_contributors (this, 1,
    "Aoran Zeng", "ccmywish@qq.com");

  // chef_allow_get();
  chef_allow_set();
  // chef_allow_reset();

  chef_allow_local_mode (this, CanNot, NULL, NULL);
  chef_forbid_english(this);
  chef_forbid_user_define(this);

  chef_set_note(this, NULL, NULL);

  def_sources_begin()
  {&upstream,     "https://download.opensuse.org/", DelegateToUpstream},
  {&Ali,          "https://mirrors.aliyun.com/opensuse", DelegateToMirror},
  {&Volcengine,   "https://mirrors.volces.com/opensuse", DelegateToMirror},
  {&Bfsu,         "https://mirrors.bfsu.edu.cn/opensuse", DelegateToMirror},
  {&Ustc,         "https://mirrors.ustc.edu.cn/opensuse", DelegateToMirror},
  {&Tuna,         "https://mirrors.tuna.tsinghua.edu.cn/opensuse", DelegateToMirror},
  {&Tencent,      "https://mirrors.tencent.com/opensuse", DelegateToMirror}
  def_sources_end()
}


/**
 * @consult https://mirrors.tuna.tsinghua.edu.cn/help/opensuse/
 */
void
os_opensuse_setsrc (char *option)
{
  // chsrc_ensure_root ();

  use_this(os_opensuse);
  Source_t source = chsrc_yield_source_and_confirm (this, option);

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
