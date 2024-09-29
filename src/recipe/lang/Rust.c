/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-08-30>
 * Last Modified : <2024-09-04>
 * ------------------------------------------------------------*/

static MirrorSite
RsProxyCN = {"rsproxycn", "RsProxy.cn", "字节跳动基础架构Dev Infra", "https://rsproxy.cn/", NULL};

/**
 * @time 2024-09-04 更新
 * @note 以下都支持稀疏索引，我们换源时都将默认添加 `sparse+`
 */
static SourceInfo
pl_rust_sources[] = {
  {&Upstream,       NULL},
  {&Sjtug_Zhiyuan, "https://mirrors.sjtug.sjtu.edu.cn/crates.io-index/"},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/crates.io-index/"},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/crates.io-index/"},
  {&Ustc,          "https://mirrors.ustc.edu.cn/crates.io-index/"},
  {&RsProxyCN,     "https://rsproxy.cn/index/"},
  {&Hust,          "https://mirrors.hust.edu.cn/crates.io-index/"}
};
def_sources_n(pl_rust);


void
pl_rust_getsrc (char *option)
{
  chsrc_view_file ("~/.cargo/config.toml");
}

/**
 * Rust 换源，参考：https://mirrors.tuna.tsinghua.edu.cn/help/crates.io-index/
 */
void
pl_rust_setsrc (char *option)
{
  chsrc_yield_source_and_confirm (pl_rust);

  const char* file = xy_strjoin (3,
    "[source.crates-io]\n"
    "replace-with = 'mirror'\n\n"

    "[source.mirror]\n"
    "registry = \"sparse+", source.url, "\"");

  chsrc_note2 (xy_strjoin (3, "请您手动写入以下内容到 ", xy_uniform_path ("~/.cargo/config.toml"), " 文件中:"));
  puts (file);
  chsrc_conclude (&source, SetsrcType_Manual);
}

def_target(pl_rust);
