/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * 由于Rye已经有后继uv了，所以我们不把该管理器纳入Python group中
 * ------------------------------------------------------------*/

def_target(pl_python_rye, "rye");

void
pl_python_rye_prelude (void)
{
  chef_prep_this (pl_python_rye, gsr);

  chef_set_created_on   (this, "2024-12-06");
  chef_set_last_updated (this, "2025-08-09");
  chef_set_sources_last_updated (this, "2025-08-09");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@ccmywish");
  chef_set_contributors (this, 0);

  chef_allow_local_mode (this, FullyCan, "支持项目级配置", "Supports project-level configuration");
  chef_allow_english(this);
  chef_allow_user_define(this);

  // 使用 pl_python_group 的源
  this->sources = pl_python_group_target.sources;
  this->sources_n = pl_python_group_target.sources_n;
}

char *
pl_python_find_rye_config ()
{
  char *buf = xy_run ("rye config --show-path", 0);
  char *rye_config = xy_normalize_path (buf);

  return rye_config;
}



void
pl_python_rye_getsrc (char *option)
{
  char *rye_config = pl_python_find_rye_config ();
  chsrc_note2 (xy_strcat (3, "请查看 ", rye_config, " 配置文件中的 [[sources]] 节内容"));
}


/**
 * @consult https://github.com/RubyMetric/chsrc/issues/127
 */
void
pl_python_rye_setsrc (char *option)
{
  /* 并不在 Python group 中，所以不考虑 target group 情况，仅使用 Python group 提供的源 */
  Source_t source = chsrc_yield_source_and_confirm (&pl_python_group_target, option);

  const char *content = RAWSTR_pl_python_rye_config;

  content = xy_str_gsub (content, "@1@", source.mirror->abbr);
  content = xy_str_gsub (content, "@2@", source.url);

  char *rye_config = pl_python_find_rye_config ();
  chsrc_note2 (xy_strcat (3, "请在配置文件 ", rye_config, " 中添加:"));
  println (content);

  chsrc_determine_chgtype (ChgType_Manual);
  chsrc_conclude (&source);
}


void
pl_python_rye_resetsrc (char *option)
{
  pl_python_rye_setsrc (option);
}
