chef_set_created_on ("2023-09-06");
chef_set_authors ("Heng Guo <2085471348@qq.com>");
chef_set_contributors ("Nil Null <nil@null.org>");
chef_allow_set();
use_this;

/**
 * 似乎会弹出GUI，待确定
 */
void
os_manjaro_setsrc (char *option)
{
  chsrc_ensure_root ();
  char *cmd = "pacman-mirrors -i -c China -m rank";
  chsrc_run (cmd, RunOpt_Default);

  chsrc_run ("pacman -Syy", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (NULL);
}

def_target(os_manjaro);
