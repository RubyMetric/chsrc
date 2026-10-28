/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_dish(wr_huggingface_cli, "huggingface-cli/huggingface/hf");



void
wr_huggingface_cli_prepare ()
{
  chef_prep_this_dish (wr_huggingface_cli, gsr);

  chef_set_recipe_created_on   (this, "2026-10-26");
  chef_set_recipe_last_updated (this, "2026-10-28");

  chef_set_chefs (this, 1, "@Mikachu2333");
  chef_set_sauciers (this, 1, "@Mikachu2333");

  chef_set_scope_cap (this, ProjectScope, ScopeCap_Unable);
  chef_set_scope_cap (this, UserScope,    ScopeCap_Able_And_Implemented);
  chef_set_scope_cap (this, SystemScope,  ScopeCap_Able_And_Implemented);
  chef_set_default_scope (this, UserScope);

  chef_deny_english(this);
  chef_allow_user_define(this);

  def_sources_begin()
  {&UpstreamProvider, "https://huggingface.co", "https://huggingface.co/deepseek-ai/DeepSeek-V4.1-Flash-1017/resolve/main/model.safetensors.index.json"}
  def_sources_end()
}



void
wr_huggingface_cli_getsrc (char *option)
{
  chsrc_view_env ("HF_ENDPOINT", NULL);
}



void
wr_huggingface_cli_setsrc (char *option)
{
  char *repo_note = "请手动设置环境变量 HF_ENDPOINT 到您希望换源的站点，例如：\n";
  char *site_note = "HF_ENDPOINT=\"https://huggingface.co\"\n";
  char *end_note = "由于 Hugging Face 的体量极大，现有的镜像站点规模小、不保证运行及质量，因此不提供具体镜像站点，以上仅为示例。";


  char *w = xy_strcat (3, repo_note, site_note, end_note);
  say (w);

  chsrc_determine_chgtype (ChgType_Manual);
  chsrc_alert2 ("请重启终端使 HF_ENDPOINT 环境变量生效。");
}

void
wr_huggingface_cli_resetsrc (char *option)
{
  char *w = "请手动删除环境变量 HF_ENDPOINT，并重启终端。\n";
  say (w);
}
