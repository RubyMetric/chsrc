/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

#define OS_Pacman_Omarchy_Conf "/etc/pacman.conf"


/** ------------------------------------------------------------
 * omarchy dish (换 OPR 专属仓库)
 *
 * Omarchy 的 Arch Linux 主仓库源与 arch 完全一致，请额外使用
 * `chsrc set arch` 来更换；本 dish 只负责 [omarchy] 段 (OPR)。
 * ------------------------------------------------------------*/

def_dish(os_omarchy, "omarchy");

void
os_omarchy_prepare ()
{
  chef_prep_this_dish (os_omarchy, gsr);

  chef_set_recipe_created_on   (this, "2026-09-04");
  chef_set_recipe_last_updated (this, "2026-09-06");

  chef_set_chefs (this, 1, "@yayoinoyume");
  chef_set_sauciers (this, 0);

  chef_set_os_scope (this);
  chef_deny_english(this);
  chef_allow_user_define(this);

  chef_set_note (this,
    "Omarchy 是基于 Arch Linux 开发的发行版，除官方专属软件包仓库 (OPR) 外，其余换源与 Arch 保持一致。"
    "本菜品仅更换 OPR 源；Arch Linux 主仓库请另行 chsrc set arch，恢复官方 OPR 源请 chsrc reset omarchy。"
    "注意: omarchy-refresh-pacman 更新脚本会覆盖此配置，换源后若被还原，请重新执行",
    "Omarchy is an Arch Linux-based distro; apart from its official package repository (OPR), source switching is the same as Arch. "
    "This dish only switches the OPR source; use chsrc set arch for the Arch Linux main source, and chsrc reset omarchy to restore the official OPR source. "
    "Note: omarchy-refresh-pacman may overwrite this configuration; rerun if it reverts");

  def_sources_begin()
  {&UpstreamProvider, "https://pkgs.omarchy.org/stable/$arch", DelegateToUpstream}
  def_sources_end()
}


void
os_omarchy_getsrc (char *option)
{
  chsrc_view_file (OS_Pacman_Omarchy_Conf);
}


/**
 * @consult
 *   1. https://github.com/basecamp/omarchy 官方 pacman.conf 模板 (仅 x86_64)
 */
void
os_omarchy_setsrc (char *option)
{
  /* Omarchy 官方仅发布 x86_64 仓库，非 x86_64 无 OPR */
  char *arch = chsrc_get_cpuarch ();
  if (strncmp (arch, "x86_64", 6) != 0)
    {
      chsrc_error ("Omarchy 目前仅支持 x86_64 架构");
      exit (Exit_Unsupported);
    }

  chsrc_ensure_root ();

  chsrc_use_this_source (os_omarchy);

  chsrc_backup (OS_Pacman_Omarchy_Conf);

  /* 上游默认源基于 stable 通道，edge 通道用户换源后将切换到 stable */
  bool is_edge = xy_run_get_status ("grep -q '^Server = https://pkgs.omarchy.org/edge/' " OS_Pacman_Omarchy_Conf) == 0;
  if (is_edge && !source_is_userdefine (&source))
    chsrc_warn2 ("检测到当前使用 edge 通道，上游默认源基于 stable 通道，继续执行将切换到 stable");

  /* 幂等: 替换 [omarchy] 段的 Server 行为所选源；已是所选源则跳过 */
  char *exists_cmd = xy_strcat (3, "grep -qxF 'Server = ", source.url, "' " OS_Pacman_Omarchy_Conf);
  if (xy_run_get_status (exists_cmd) == 0)
    {
      chsrc_debug ("recipe", "所选 Server 行已存在，跳过 pacman.conf 修改");
    }
  else
    {
      char *sed_cmd = xy_strcat (4, "sed -i 's|^Server = https://pkgs.omarchy.org/.*|Server = ",
                                  source.url, "|' ", OS_Pacman_Omarchy_Conf);
      chsrc_run (sed_cmd, RunOpt_Default);
    }

  /* 清理本地陈旧 omarchy.db 缓存: 残留旧 sig 会导致 -Syy 验签失败 */
  chsrc_run ("rm -f /var/lib/pacman/sync/omarchy.db /var/lib/pacman/sync/omarchy.db.sig", RunOpt_Default);

  chsrc_run ("pacman -Syy", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);

  chsrc_note2 (ENGLISH
    ? "This dish only switches the OPR source; use chsrc set arch for the Arch Linux main source."
    : "本菜品仅更换 OPR 源；Arch Linux 主仓库请另行 chsrc set arch。");
}


void
os_omarchy_resetsrc (char *option)
{
  /* reset 模式下 chsrc_use_this_source 返回官方源，即恢复官方 stable 通道 */
  os_omarchy_setsrc (option);
}
