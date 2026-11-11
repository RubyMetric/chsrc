/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_dish(wr_winget, "winget");

void
wr_winget_prepare ()
{
  chef_prep_this_dish (wr_winget, gsr);

  chef_set_recipe_created_on   (this, "2024-06-07");
  chef_set_recipe_last_updated (this, "2025-08-17");

  chef_set_chefs (this, 1, "@ccmywish");
  chef_set_sauciers (this, 1, "@Mikachu2333");

  chef_set_scope_cap (this, ProjectScope, ScopeCap_Unable);
  chef_set_scope_cap (this, UserScope,    ScopeCap_Able_And_Implemented);
  chef_set_scope_cap (this, SystemScope,  ScopeCap_Unable);
  chef_set_default_scope (this, UserScope);

  chef_deny_english(this);
  chef_allow_user_define(this);

  def_sources_begin()
  {&UpstreamProvider, "https://cdn.winget.microsoft.com/cache",    NULL},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/winget-source", NULL},
  {&Ustc,             "https://mirrors.ustc.edu.cn/winget-source", NULL},
  {&Nju,              "https://mirror.nju.edu.cn/winget-source",   NULL},
  {&Nyist,            "https://mirror.nyist.edu.cn/winget-source", NULL},
  {&Xjtu,             "https://mirrors.xjtu.edu.cn/winget-source", NULL}
  def_sources_end()
}


void
wr_winget_getsrc (char *option)
{
  chsrc_run ("winget source list", RunOpt_Default);
}


/**
 * @brief 查询某个 winget source 是否已存在
 *
 * @note 不能用 `winget source remove` 的返回值判断：源不存在时 winget 返回
 *       一个非零退出码，而旧实现依赖“连删两次”来绕过，导致源本就不存在时
 *       第二次 remove 报错并中断，`add` 根本执行不到。
 */
static bool
wr_winget_source_exists (const char *name)
{
  char *cmd = xy_strcat (3, "winget source list ", name);
  int status = xy_run_get_status (cmd);
  free (cmd);
  return status == 0;
}


/**
 * @consult https://mirrors.ustc.edu.cn/help/winget-source.html
 */
void
wr_winget_setsrc (char *option)
{
  chsrc_use_this_source (wr_winget);

  /* 幂等：只有确实存在时才删除，避免“源不存在”把流程打断 */
  if (wr_winget_source_exists ("winget"))
    {
      chsrc_run ("winget source remove winget", RunOpt_Dont_Abort_On_Failure);
    }
  else
    {
      chsrc_note2 (CHINESE ? "winget 源不存在，跳过删除"
                           : "winget source does not exist, skip removal");
    }

  chsrc_run (xy_strcat (3, "winget source add winget ", source.url, " --trust-level trusted"), RunOpt_Default);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


void
wr_winget_resetsrc (char *option)
{
  chsrc_run ("winget source reset winget", RunOpt_Default);

  chsrc_determine_chgtype (ChgType_Reset);
  chsrc_conclude (NULL);
}
