/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * ------------------------------------------------------------*/

def_target(pl_rust_cargo, "rust/cargo/crate/crates");

void
pl_rust_cargo_prelude (void)
{
  use_this(pl_rust_cargo);
  chef_allow_gsr(pl_rust_cargo);

  chef_set_created_on   (this, "2023-08-30");
  chef_set_last_updated (this, "2025-07-22");
  chef_set_sources_last_updated (this, "2025-06-18");

  chef_set_authors (this, 1, "Aoran Zeng", "ccmywish@qq.com");
  chef_set_chef (this, NULL, NULL);
  chef_set_cooks (this, 0);
  chef_set_contributors (this, 1,
    "Mikachu2333", "mikachu.23333@zohomail.com");


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
  chsrc_view_file ("~/.cargo/config.toml");
}


/**
 * @consult https://mirrors.tuna.tsinghua.edu.cn/help/crates.io-index/
 * @consult https://help.mirrors.cernet.edu.cn/crates.io-index
 */
void
pl_rust_cargo_setsrc (char *option)
{
  use_this_source(pl_rust_cargo);

  char *content = RAWSTR_pl_rust_cargo_config;

  content = xy_str_gsub (content, "@url@", source.url);

  chsrc_note2 (xy_strjoin (3, "请手动写入以下内容到 ", xy_normalize_path ("~/.cargo/config.toml"), " 文件中:"));
  println (content);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


void
pl_rust_cargo_resetsrc (char *option)
{
  pl_rust_cargo_setsrc (option);
}
