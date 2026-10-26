/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_dish(wr_huggingface_cli, "huggingface-cli/huggingface/hf");



static MirrorSite_t
HFMirror = {
  IS_DedicatedMirrorSite,
  "hfmirror", "HF-Mirror", "HuggingFace 镜像站", "https://hf-mirror.com/"
};



void
wr_huggingface_cli_prepare ()
{
  chef_prep_this_dish (wr_huggingface_cli, gsr);

  chef_set_recipe_created_on   (this, "2026-10-26");
  chef_set_recipe_last_updated (this, "2026-10-26");

  chef_set_chefs (this, 1, "@Mikachu2333");
  chef_set_sauciers (this, 1, "@Mikachu2333");

  chef_set_scope_cap (this, ProjectScope, ScopeCap_Unable);
  chef_set_scope_cap (this, UserScope,    ScopeCap_Able_And_Implemented);
  chef_set_scope_cap (this, SystemScope,  ScopeCap_Unable);
  chef_set_default_scope (this, UserScope);

  chef_deny_english(this);
  chef_allow_user_define(this);

  def_sources_begin()
  {&UpstreamProvider, "https://huggingface.co", "https://huggingface.co/deepseek-ai/DeepSeek-V4.1-Flash-1017/resolve/main/model.safetensors.index.json"},
  {&HFMirror,         "https://hf-mirror.com",  "https://hf-mirror.com/api/resolve-cache/models/deepseek-ai/DeepSeek-V4.1-Flash-1017/1bae0e1088993b9b82de08f59de5be5a34e42170/model.safetensors.index.json?download=true"}
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
  chsrc_use_this_source (wr_huggingface_cli);

#ifdef XY_Build_On_Windows

  char *cmd = xy_strcat (3, "setx HF_ENDPOINT \"", source.url, "\"");
  chsrc_run (cmd, RunOpt_Dont_Notify_On_Success|RunOpt_No_Last_New_Line);

#else

  char *w = xy_strcat (3, "export HF_ENDPOINT=\"", source.url, "\"\n");

  char *bashrc = xy_bashrc;
  if (xy_file_exist (bashrc))
    {
      chsrc_backup (bashrc);
      chsrc_append_to_file (w, bashrc);
    }

  char *zshrc = xy_zshrc;
  if (xy_file_exist (zshrc))
    {
      chsrc_backup (zshrc);
      chsrc_append_to_file (w, zshrc);
    }

  char *fishrc = xy_fishrc;
  if (xy_file_exist (fishrc))
    {
      char *w = xy_strcat (3, "set -x HF_ENDPOINT ", source.url, "\n");

      chsrc_backup (fishrc);
      chsrc_append_to_file (w, fishrc);
    }
#endif

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
  chsrc_alert2 ("请重启终端使huggingface-cli环境变量生效");
}

void
wr_huggingface_cli_resetsrc (char *option)
{
  wr_huggingface_cli_setsrc (option);
}
