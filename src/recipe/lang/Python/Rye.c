/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nul None  <nul@none.org>
 * Created On    : <2024-12-06>
 * Last Modified : <2024-12-06>
 *
 * 由于Rye已经有后继uv了，所以我们不把该管理器纳入Python group中
 * ------------------------------------------------------------*/

char *
pl_python_find_rye_config ()
{
  char *buf = xy_run ("rye config --show-path", 0);
  char *rye_config = xy_normalize_path (buf);

  return rye_config;
}

/**
 * chsrc get rye
 */
void
pl_python_rye_getsrc (char *option)
{
  char *rye_config = pl_python_find_rye_config ();
  chsrc_note2 (xy_strjoin (3, "请查看 ", rye_config, " 配置文件中的 [[sources]] 节内容"));
}


/**
 * @consult https://github.com/RubyMetric/chsrc/issues/127
 * @consult recipe Java
 *
 * chsrc set rye
 */
void
pl_python_rye_setsrc (char *option)
{
  chsrc_yield_source_and_confirm (pl_python);

  const char *file = xy_strjoin (7,
    "[[sources]]\n",
    "name = \"", source.mirror->abbr, "\"\n",
    "url = \"",  source.url,          "\"");

  char *rye_config = pl_python_find_rye_config ();
  chsrc_note2 (xy_strjoin (3, "请在您的 Rye 配置文件 ", rye_config, " 中添加:"));
  println (file);

  chsrc_determine_chgtype (ChgType_Manual);
  chsrc_conclude (&source);
}


/**
 * chsrc reset rye
 */
void
pl_python_rye_resetsrc (char *option)
{
  pl_python_rye_setsrc (option);
}


/**
 * chsrc ls rye
 */
Feature_t
pl_python_rye_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = true;

  f.cap_locally = true;
  f.cap_locally_explain = NULL;

  f.can_english = false;
  f.can_user_define = true;

  f.note = NULL;
  return f;
}

// def_target_gsrf(pl_python_rye);
Target_t pl_python_rye_target = {def_target_inner_gsrf(pl_python_rye),def_target_sourcesn(pl_python)};
