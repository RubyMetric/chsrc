/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

#define PL_Rust_cargo_default_download_url "https://static.crates.io/crates/windows/windows-0.62.2.crate"
#define PL_Rust_cargo_custom_download_url  "api/v1/crates/windows/0.62.2/download"

def_target(pl_rust_cargo, "rust/cargo/crate/crates");

void
pl_rust_cargo_prelude (void)
{
  chef_prep_this (pl_rust_cargo, gsr);

  chef_set_created_on   (this, "2023-08-30");
  chef_set_last_updated (this, "2025-12-31");
  chef_set_sources_last_updated (this, "2025-12-31");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 2, "@Mikachu2333", "@ccmywish");
  chef_set_sauciers (this, 1, "@happy-game");

  chef_allow_local_mode (this, FullyCan, NULL, NULL);
  chef_deny_english (this);
  chef_allow_user_define (this);


  // 以下都支持稀疏索引，我们换源时都将默认添加 `sparse+`。链接末尾的 `/` 不能缺少
  /**
   * @warning 2025-12-29:
   *   许多镜像站的 dl 字段指向 static.crates.io，因此测速链接也指向 static.crates.io，原API失效
   *   见: https://github.com/RubyMetric/chsrc/pull/330
   */
  def_sources_begin()
  {&UpstreamProvider, "https://crates.io/",                             PL_Rust_cargo_default_download_url},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/crates.io-index/", PL_Rust_cargo_default_download_url},
  {&RsProxyCN,        "https://rsproxy.cn/index/",                      "https://rsproxy.cn/" PL_Rust_cargo_custom_download_url},
  {&Ali,              "https://mirrors.aliyun.com/crates.io-index/",    "https://mirrors.aliyun.com/crates/" PL_Rust_cargo_custom_download_url},
  {&Zju,              "https://mirrors.zju.edu.cn/crates.io-index/",    PL_Rust_cargo_default_download_url},

  /* 注释原因: (2025-06-17) 镜像同步失败，多数包都不可用 */
  // {&Nju,        "https://mirror.nju.edu.cn/git/crates.io-index.git/",   FeedByPrelude},

  {&Sjtug_Zhiyuan, "https://mirrors.sjtug.sjtu.edu.cn/crates.io-index/",   "https://mirror.sjtu.edu.cn/crates.io/crates/windows/windows-0.58.0.crate"},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/crates.io-index/",PL_Rust_cargo_default_download_url},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/crates.io-index/",         PL_Rust_cargo_default_download_url},
  {&Ustc,          "https://mirrors.ustc.edu.cn/crates.io-index/",         "https://crates-io.proxy.ustclug.org/" PL_Rust_cargo_custom_download_url},

  /* 注释原因: (2025-06-17) 镜像同步失败，多数包都不可用 */
  // {&Hust,       "https://mirrors.hust.edu.cn/crates.io-index/", FeedByPrelude},

  {&Cqu,           "https://mirrors.cqu.edu.cn/crates.io-index/",  PL_Rust_cargo_default_download_url}
  def_sources_end()

}


void
pl_rust_cargo_note_get_src_default ()
{
  if (ENGLISH)
    chsrc_note2 ("No source configured in Cargo, showing default upstream source:");
  else
    chsrc_note2 ("Cargo 中未自定义源，显示默认源：");

    Source_t default_source = chsrc_yield_source (&pl_rust_cargo_target, "upstream");
    say (default_source.url);
}

void
pl_rust_cargo_note_get_src_mirror (char *url, bool sparse)
{
  chsrc_note2 (ENGLISH ? "Custom source found: " : "已找到自定义源：");
  say (xy_2strcat (url, sparse ? " (sparse)" : ""));
}

void
pl_rust_cargo_getsrc (char *option)
{
  char *cargo_config_file = xy_normalize_path ("~/.cargo/config.toml");

  char *raw_content = xy_file_read (cargo_config_file);
  char *formatted_content = xy_str_gsub (raw_content, " ", "");
  formatted_content = xy_str_gsub (formatted_content, "'", "\"");

  XyStrFindResult_t result_has_mirror = xy_str_find (formatted_content, "replace-with");
  if (result_has_mirror.found)
    {
      char *mirror_name = xy_str_next_nonempty_line (formatted_content + result_has_mirror.end + 1);
      mirror_name = xy_str_delete_prefix (mirror_name, "=\"");
      mirror_name = xy_str_delete_suffix (mirror_name, "\"");

      XyStrFindResult_t result_mirror = xy_str_find (formatted_content, xy_strcat (3, "[source.", mirror_name, "]"));
      if (!result_mirror.found)
        {
          pl_rust_cargo_note_get_src_default();
          return;
        }
      char *mirror_url = xy_str_next_nonempty_line (formatted_content + result_mirror.end + 1);
      mirror_url = xy_str_delete_prefix (mirror_url, "registry=\"");
      mirror_url = xy_str_delete_suffix (mirror_url, "\"");
      if (xy_str_find (mirror_url, "sparse+").found)
        {
          pl_rust_cargo_note_get_src_mirror (xy_str_delete_prefix (mirror_url, "sparse+"), true);
        }
    }
  else
    {
      pl_rust_cargo_note_get_src_default();
    }
}


void
pl_write_rust_config (const char *path, const char *url)
{
  remove (path);
  char *content = RAWSTR_pl_rust_cargo_config;
  content = xy_str_gsub (content, "@url@", url);
  chsrc_overwrite_file (content, path);
}

/**
 * @consult https://mirrors.tuna.tsinghua.edu.cn/help/crates.io-index/
 * @consult https://help.mirrors.cernet.edu.cn/crates.io-index
 */
void
pl_rust_cargo_setsrc (char *option)
{
  chsrc_ensure_program ("cargo");

  chsrc_use_this_source (pl_rust_cargo);

  char *default_content = RAWSTR_pl_rust_cargo_config;
  char *cargo_config_dir = "~/.cargo/";
  char *cargo_config_file = xy_2strcat (cargo_config_dir, "config.toml");

  chsrc_ensure_dir (cargo_config_dir);

  cargo_config_file = xy_normalize_path (cargo_config_file);

  if (xy_file_exist (cargo_config_file))
    {
      chsrc_backup (cargo_config_file);

      char *raw_content = xy_file_read (cargo_config_file);

      XyStrFindResult_t result_has_mirror = xy_str_find (raw_content, "replace-with");
      if (!result_has_mirror.found)
        {
          pl_write_rust_config (cargo_config_file, source.url);
          goto finish;
        }

      char *mirror_name = xy_str_next_nonempty_line (raw_content + result_has_mirror.end + 1);
      mirror_name = xy_str_gsub (mirror_name, " ", "");
      mirror_name = xy_str_gsub (mirror_name, "'", "\"");
      mirror_name = xy_str_delete_prefix (mirror_name, "=\"");
      mirror_name = xy_str_delete_suffix (mirror_name, "\"");

      XyStrFindResult_t result_mirror = xy_str_find (raw_content, xy_strcat (3, "[source.", mirror_name, "]"));
      if (!result_mirror.found)
        {
          pl_write_rust_config (cargo_config_file, source.url);
          goto finish;
        }

      char *mirror_url = xy_str_next_nonempty_line (raw_content + result_mirror.end + 1);
      mirror_url = xy_str_gsub (mirror_url, " ", "");
      if (!xy_str_find (mirror_url, "registry").found)
        {
          pl_write_rust_config (cargo_config_file, source.url);
          goto finish;
        }
      mirror_url = xy_str_delete_prefix (mirror_url, "registry=\"");
      mirror_url = xy_str_delete_suffix (mirror_url, "\"");

      char *final_content = xy_str_gsub (raw_content, mirror_url, xy_2strcat ("sparse+", source.url));
      chsrc_overwrite_file (final_content, cargo_config_file);
      goto finish;
    }

    pl_write_rust_config (cargo_config_file, source.url);
    goto finish;

finish:
  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


void
pl_rust_cargo_resetsrc (char *option)
{
  pl_rust_cargo_setsrc (option);
}
