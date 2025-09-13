/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(pl_rust_cargo, "rust/cargo/crate/crates");

void
pl_rust_cargo_prelude (void)
{
  chef_prep_this (pl_rust_cargo, gsr);

  chef_set_created_on   (this, "2023-08-30");
  chef_set_last_updated (this, "2025-09-13");
  chef_set_sources_last_updated (this, "2025-06-18");

  chef_set_chef (this, NULL);
  chef_set_cooks (this, 1, "@ccmywish");
  chef_set_sauciers (this, 2, "@Mikachu2333", "@happy-game");

  chef_allow_local_mode (this, PartiallyCan, "可以基于本项目换源吗？请帮助确认", "Can it change sources based on this project? Please help confirm");
  chef_forbid_english (this);
  chef_allow_user_define(this);


  // 以下都支持稀疏索引，我们换源时都将默认添加 `sparse+`。链接末尾的 `/` 不能缺少
  def_sources_begin()
  {&UpstreamProvider, "https://crates.io/",                             FeedByPrelude},
  {&MirrorZ,          "https://mirrors.cernet.edu.cn/crates.io-index/", FeedByPrelude},
  {&RsProxyCN,        "https://rsproxy.cn/index/",                      FeedByPrelude},
  {&Ali,              "https://mirrors.aliyun.com/crates.io-index/",    FeedByPrelude},
  {&Zju,              "https://mirrors.zju.edu.cn/crates.io-index/",    FeedByPrelude},

  /* 注释原因: (2025-06-17) 镜像同步失败，多数包都不可用 */
  // {&Nju,        "https://mirror.nju.edu.cn/git/crates.io-index.git/",   FeedByPrelude},

  {&Sjtug_Zhiyuan, "https://mirrors.sjtug.sjtu.edu.cn/crates.io-index/",   FeedByPrelude},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/crates.io-index/",FeedByPrelude},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/crates.io-index/",         FeedByPrelude},
  {&Ustc,          "https://mirrors.ustc.edu.cn/crates.io-index/",         FeedByPrelude},

  /* 注释原因: (2025-06-17) 镜像同步失败，多数包都不可用 */
  // {&Hust,       "https://mirrors.hust.edu.cn/crates.io-index/", FeedByPrelude},

  {&Cqu,           "https://mirrors.cqu.edu.cn/crates.io-index/",  FeedByPrelude}
  def_sources_end()

  chef_set_sources_speed_measure_url_with_postfix (this, "api/v1/crates/windows/0.58.0/download");
}


void
pl_rust_cargo_getsrc (char *option)
{
  char *cargo_config_file = xy_normalize_path ("~/.cargo/config.toml");
  
  if (xy_file_exist (cargo_config_file))
    {
      // 尝试提取 [source.mirror] 下的 registry URL
      char *grep_cmd = xy_str_gsub ("grep -A1 '\\[source\\.mirror\\]' '@f@' | grep 'registry' | cut -d'\"' -f2", "@f@", cargo_config_file);
      chsrc_ensure_program ("grep");
      chsrc_ensure_program ("cut");
      
      char *mirror_url;
      int status = xy_run_get_stdout (grep_cmd, &mirror_url);
      char *stripped_url = (mirror_url) ? xy_str_strip(mirror_url) : "";
      
      if (0 == status && stripped_url && strstr(stripped_url, "http"))
        {
          // 找到配置的镜像源，如果存在 sparse+ 前缀则去除
          char *clean_url = (strstr(stripped_url, "sparse+")) ? 
                           stripped_url + 7 : stripped_url;
          say (clean_url);
        }
      else
        {
          // 配置文件存在但没有找到镜像源配置，显示默认上游源
          if (ENGLISH)
            chsrc_note2 ("Config file exists but no mirror source found, showing default upstream source:");
          else
            chsrc_note2 ("配置文件存在但未找到镜像源配置，显示默认上游源：");

          Source_t default_source = chsrc_yield_source (&pl_rust_cargo_target, "upstream");
          say (default_source.url);
        }
    }
  else
    {
      // 配置文件不存在，显示默认上游源
      if (ENGLISH)
        chsrc_note2 ("No source configured in Cargo, showing default upstream source:");
      else
        chsrc_note2 ("Cargo 中未配置源，显示默认上游源：");

      Source_t default_source = chsrc_yield_source (&pl_rust_cargo_target, "upstream");
      say (default_source.url);
    }
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

  char *cargo_config_dir = "~/.cargo/";
  char *cargo_config_file = xy_2strcat (cargo_config_dir, "config.toml");
  
  chsrc_ensure_dir (cargo_config_dir);
  
  cargo_config_file = xy_normalize_path (cargo_config_file);
  
  if (xy_file_exist (cargo_config_file))
    {
      chsrc_backup (cargo_config_file);
    }

  char *content = RAWSTR_pl_rust_cargo_config;
  content = xy_str_gsub (content, "@url@", source.url);

  if (xy_file_exist (cargo_config_file))
    {
      char *check_cmd = xy_str_gsub (RAWSTR_pl_rust_cargo_check_config, "@f@", cargo_config_file);
      chsrc_ensure_program ("grep");
      int status = chsrc_run_directly (check_cmd);
      
      if (0 == status)
        {
#if defined(XY_Build_On_macOS) || defined(XY_Build_On_BSD)
          char *sed_cmd = "sed -i '' ";
#else
          char *sed_cmd = "sed -i ";
#endif
          
          char *update_cmd = xy_str_gsub (RAWSTR_pl_rust_cargo_update_replace_with, "@sed@", sed_cmd);
          update_cmd = xy_str_gsub (update_cmd, "@f@", cargo_config_file);
          chsrc_run (update_cmd, RunOpt_Default);
          
          update_cmd = xy_str_gsub (RAWSTR_pl_rust_cargo_update_registry, "@sed@", sed_cmd);
          update_cmd = xy_str_gsub (update_cmd, "@f@", cargo_config_file);
          update_cmd = xy_str_gsub (update_cmd, "@url@", source.url);
          chsrc_run (update_cmd, RunOpt_Default);
        }
      else
        {
          chsrc_append_to_file ("\n", cargo_config_file);
          chsrc_append_to_file (content, cargo_config_file);
        }
    }
  else
    {
      chsrc_append_to_file (content, cargo_config_file);
    }

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


void
pl_rust_cargo_resetsrc (char *option)
{
  pl_rust_cargo_setsrc (option);
}
