/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-11>
 * Last Modified : <2024-08-15>
 * ------------------------------------------------------------*/

/**
 * @update 2023-09-11
 * @note 目前只有一个源
 */
static Source_t wr_flathub_sources[] =
{
  {&UpstreamProvider,  NULL},
  {&Sjtug_Zhiyuan,    "https://mirror.sjtu.edu.cn/flathub"},
};
def_sources_n(wr_flathub);


/**
 * @consult https://mirrors.sjtug.sjtu.edu.cn/docs/flathub
 */
void
wr_flathub_setsrc (char *option)
{
  chsrc_yield_source_and_confirm (wr_flathub);

  chsrc_note2 ("若出现问题，可先调用以下命令:");
  char *note = xy_strjoin (3,
    "wget ", source.url, "/flathub.gpg\n"
    "flatpak remote-modify --gpg-import=flathub.gpg flathub"
  );
  puts (note);

  char *cmd = xy_2strjoin ("flatpak remote-modify flathub --url=", source.url);
  chsrc_run (cmd, RunOpt_Default);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}

def_target_s (wr_flathub);
