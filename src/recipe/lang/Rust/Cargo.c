/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng  <ccmywish@qq.com>
 * Contributors  : Mikachu2333 <mikachu.23333@zohomail.com>
 *               |
 * Created On    : <2023-08-30>
 * Last Modified : <2025-07-11>
 * ------------------------------------------------------------*/

static SourceProvider_t pl_rust_cargo_upstream =
{
  def_upstream, "https://crates.io/",
  {NotSkip, NA, NA, "https://crates.io/api/v1/crates/windows/0.58.0/download", ACCURATE}
  /* 跳转为: https://static.crates.io/crates/windows/windows-0.58.0.crate */
};


/**
 * @update 2025-06-18
 *
 * @note 以下都支持稀疏索引，我们换源时都将默认添加 `sparse+`
 * @note 链接末尾的 `/` 不能缺少
 *
 * @note 2025-06-17: 经验证，南京大学、华中科大的镜像同步失败，实际情况下多数包都不可用
 */
static Source_t pl_rust_cargo_sources[] =
{
  {&pl_rust_cargo_upstream,  "https://github.com/rust-lang/crates.io-index/",
                              NULL},

  {&MirrorZ,       "https://mirrors.cernet.edu.cn/crates.io-index/",
                    NULL},
  {&RsProxyCN,     "https://rsproxy.cn/index/",
                    NULL},
  {&Ali,           "https://mirrors.aliyun.com/crates.io-index/",
                   "https://mirrors.aliyun.com/crates/api/v1/crates/windows/0.58.0/download"},

  {&Zju,           "https://mirrors.zju.edu.cn/crates.io-index/",
                    NULL},

  // {&Nju,        "https://mirror.nju.edu.cn/git/crates.io-index.git/",
  //                NULL},

  {&Sjtug_Zhiyuan, "https://mirrors.sjtug.sjtu.edu.cn/crates.io-index/",
                    NULL},

  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/crates.io-index/",
                    NULL},

  {&Bfsu,          "https://mirrors.bfsu.edu.cn/crates.io-index/",
                    NULL},

  {&Ustc,          "https://mirrors.ustc.edu.cn/crates.io-index/",
                   "https://crates-io.proxy.ustclug.org/api/v1/crates/windows/0.58.0/download"},

  // {&Hust,       "https://mirrors.hust.edu.cn/crates.io-index/",
  //                NULL},

  {&Cqu,           "https://mirrors.cqu.edu.cn/crates.io-index/",
                    NULL}
};
def_sources_n(pl_rust_cargo);


/**
 * chsrc get cargo
 */
void
pl_rust_cargo_getsrc (char *option)
{
  chsrc_view_file ("~/.cargo/config.toml");
}


/**
 * @consult https://mirrors.tuna.tsinghua.edu.cn/help/crates.io-index/
 * @consult https://help.mirrors.cernet.edu.cn/crates.io-index
 *
 * chsrc set cargo
 */
void
pl_rust_cargo_setsrc (char *option)
{
  chsrc_yield_source_and_confirm (pl_rust_cargo);

  const char* file = xy_strjoin (3,
    "[source.crates-io]\n"
    "replace-with = 'mirror'\n\n"

    "[source.mirror]\n"
    "registry = \"sparse+", source.url, "\"");

  chsrc_note2 (xy_strjoin (3, "请您手动写入以下内容到 ", xy_normalize_path ("~/.cargo/config.toml"), " 文件中:"));
  p(file);

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}


/**
 * chsrc reset cargo
 */
void
pl_rust_cargo_resetsrc (char *option)
{
  pl_rust_cargo_setsrc (option);
}


Feature_t
pl_rust_cargo_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = true;

  f.cap_locally = PartiallyCan;
  f.cap_locally_explain = "可以基于本项目换源吗？请帮助确认";
  f.can_english = false;
  f.can_user_define = true;

  f.note = NULL;
  return f;
}


def_target_gsrf(pl_rust_cargo);
