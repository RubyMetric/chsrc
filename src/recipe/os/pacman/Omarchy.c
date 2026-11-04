/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

#define OS_Pacman_Omarchy_Conf "/etc/pacman.conf"

#define OS_Omarchy_Upstream "https://pkgs.omarchy.org"
#define OS_Omarchy_CN       "https://pkgs.omarchy.cn"

/* 仅取 [omarchy] 段内容，供通道检测与写后校验复用 */
#define OS_Omarchy_Section_Cmd \
  "awk '/^\\[omarchy\\]/{f=1;next} /^\\[/{f=0} f' " OS_Pacman_Omarchy_Conf

/* 仅重写 [omarchy] 段: 删除段内全部 Server 行后，再按 target / fallback 写入 */
#define OS_Omarchy_Awk_Rewrite \
  "/^\\[omarchy\\]/{print; print \"Server = \" target; " \
  "if (fallback != \"\") print \"Server = \" fallback; insec=1; next} " \
  "/^\\[/{insec=0} " \
  "insec && /^[[:space:]]*Server[[:space:]]*=/{next} " \
  "{print}"


/** ------------------------------------------------------------
 * omarchy dish (换 OPR 专属仓库)
 *
 * Omarchy 的 Arch 主仓库由 Omarchy 按通道管理：stable/rc 使用快照镜像
 * （stable-mirror/rc-mirror.omarchy.org），edge 使用滚动镜像（mirror.omarchy.org）。
 * 本 dish 只负责 [omarchy] 段 (OPR)；OPR 的官方中国镜像由阿里云 CDN 提供。
 * ------------------------------------------------------------*/

/* 测速链接固定用 stable 的 omarchy.db：同通道 cn/org 内容一致，三通道体积相当(约 55-61 KB)，不代表当前通道 */
static MirrorSite_t OmarchyCN =
{
  IS_DedicatedMirrorSite,
  "omarchycn", "Omarchy Ali", "Omarchy 阿里云镜像源", "https://pkgs.omarchy.cn/",
  {NotSkip, NA, NA, "https://pkgs.omarchy.cn/stable/x86_64/omarchy.db", ACCURATE}
};

def_dish(os_omarchy, "omarchy");

void
os_omarchy_prepare ()
{
  chef_prep_this_dish (os_omarchy, gsr);

  chef_set_recipe_created_on   (this, "2026-09-04");
  chef_set_recipe_last_updated (this, "2026-11-04");

  chef_set_chefs (this, 1, "@yayoinoyume");
  chef_set_sauciers (this, 0);

  chef_set_os_scope (this);
  chef_deny_english(this);
  chef_allow_user_define(this);

  chef_set_note (this,
    "仅更换 OPR 源；Arch 主仓库请勿使用 chsrc set arch。"
    "Omarchy 4.5+ 中国区 Arch 镜像：将 /etc/omarchy/region 写为 cn，再运行 omarchy-refresh-pacman；它会覆盖本菜品设置，之后重跑 chsrc set omarchy omarchycn 即可。",
    "Only switches the OPR source; do not use chsrc set arch for the Arch main repositories. "
    "For China on Omarchy 4.5+, write cn to /etc/omarchy/region and run omarchy-refresh-pacman; it overwrites this dish's setting, so rerun chsrc set omarchy omarchycn afterward.");

  def_sources_begin()
  {&UpstreamProvider, OS_Omarchy_Upstream, DelegateToUpstream},
  {&OmarchyCN,        OS_Omarchy_CN,       DelegateToMirror}
  def_sources_end()
}


void
os_omarchy_getsrc (char *option)
{
  chsrc_view_file (OS_Pacman_Omarchy_Conf);
}


/**
 * @consult
 *   1. https://github.com/omacom/omarchy 官方 pacman.conf 模板 (仅 x86_64)
 *   2. https://pkgs.omarchy.cn/ OPR 官方中国镜像 (stable/rc/edge)
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

  if (xy_run_get_status ("grep -q '^\\[omarchy\\]' " OS_Pacman_Omarchy_Conf) != 0)
    {
      chsrc_error ("未在 " OS_Pacman_Omarchy_Conf " 中找到 [omarchy] 段");
      exit (Exit_Unsupported);
    }

  chsrc_backup (OS_Pacman_Omarchy_Conf);

  /* 从 [omarchy] 段反推当前通道，只看未注释的 Server 行，缺省为 stable */
  char *server_lines = xy_2strcat (OS_Omarchy_Section_Cmd,
                                    " | grep -E '^[[:space:]]*Server[[:space:]]*='");
  char *channel = "stable";
  if (xy_run_get_status (xy_2strcat (server_lines, " | grep -q '/edge/'")) == 0)
    channel = "edge";
  else if (xy_run_get_status (xy_2strcat (server_lines, " | grep -q '/rc/'")) == 0)
    channel = "rc";

  /* 内置源跟随当前通道；用户自定义源原样使用，不做通道替换 */
  bool is_cn   = xy_streql (source.mirror->code, OmarchyCN.code);
  bool is_user = source_is_userdefine (&source);

  /* 用户自定义 URL 会被嵌入 shell 单引号参数，awk -v 还会转义反斜杠 */
  if (is_user && (strchr (source.url, '\'') || strchr (source.url, '\\')
                  || strchr (source.url, '\n') || strchr (source.url, '\r')))
    {
      chsrc_error ("用户自定义 URL 不能包含单引号、反斜杠或换行");
      exit (Exit_UserCause);
    }

  char *target = NULL;
  if (is_user)
    target = source.url;
  else
    target = xy_strcat (4, source.url, "/", channel, "/$arch");

  /* 中国镜像后追加同通道上游兜底行，上游同步延迟时仍可安装 */
  char *fallback = is_cn ? xy_strcat (4, OS_Omarchy_Upstream, "/", channel, "/$arch") : "";

  /* 只重写 [omarchy] 段，不触碰 [core]、[archlinuxcn] 等其他段 */
  char *tmp_conf   = xy_2strcat (OS_Pacman_Omarchy_Conf, ".chsrc-tmp");
  char *awk_cmd    = xy_strcat (7, "awk -v target='", target, "' -v fallback='", fallback,
                                   "' '", OS_Omarchy_Awk_Rewrite, "' " OS_Pacman_Omarchy_Conf);
  char *write_cmd  = xy_strcat (6, awk_cmd, " > ", tmp_conf, " && mv ", tmp_conf,
                                   " " OS_Pacman_Omarchy_Conf);
  chsrc_run (write_cmd, RunOpt_Default);

  /* 写后校验: [omarchy] 段首行必须是目标 URL，中国镜像还需带同通道兜底行 */
  if (!in_dry_run_mode())
    {
      char *expected = xy_2strcat ("Server = ", target);
      char *first    = xy_run (xy_2strcat (OS_Omarchy_Section_Cmd, " | grep -m1 '^Server = '"), 1);
      bool  ok       = first && xy_streql (first, expected);

      if (ok && is_cn)
        {
          char *fallback_cmd = xy_strcat (4, OS_Omarchy_Section_Cmd, " | grep -qxF 'Server = ", fallback, "'");
          ok = xy_run_get_status (fallback_cmd) == 0;
        }

      if (!ok)
        {
          chsrc_error ("[omarchy] 段写入校验失败，请从 " OS_Pacman_Omarchy_Conf ".bak.* 恢复");
          exit (Exit_ExternalError);
        }
    }

  /* 清理本地陈旧 omarchy.db 缓存: 残留旧 sig 会导致 -Syy 验签失败 */
  chsrc_run ("rm -f /var/lib/pacman/sync/omarchy.db /var/lib/pacman/sync/omarchy.db.sig", RunOpt_Default);

  chsrc_run ("pacman -Syy", RunOpt_No_Last_New_Line);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);

  chsrc_note2 (ENGLISH
    ? xy_strcat (3, "Kept the current ", channel, " channel; the Arch main repositories are untouched.")
    : xy_strcat (3, "已保持当前 ", channel, " 通道，Arch 主仓库未改动。"));
}


void
os_omarchy_resetsrc (char *option)
{
  /* reset 模式下 chsrc_use_this_source 返回官方源，即恢复官方源的当前通道 */
  os_omarchy_setsrc (option);
}
