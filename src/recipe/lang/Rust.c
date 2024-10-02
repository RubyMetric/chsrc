/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-08-30>
 * Last Modified : <2024-10-02>
 * ------------------------------------------------------------*/

static MirrorSite
RsProxyCN = {"rsproxycn", "RsProxy.cn", "字节跳动基础架构Dev Infra", "https://rsproxy.cn/", NULL};

/**
 * @update 2024-10-02
 * @note 以下都支持稀疏索引，我们换源时都将默认添加 `sparse+`
 * @note 链接末尾的 `/` 不能缺少
 */
static SourceInfo
pl_rust_sources[] = {
  {&Upstream,      "https://index.crates.io/"}, // @help 是这个吗？
  {&MirrorZ,       "https://mirrors.cernet.edu.cn/crates.io-index/"},
  {&Sjtug_Zhiyuan, "https://mirrors.sjtug.sjtu.edu.cn/crates.io-index/"},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/crates.io-index/"},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/crates.io-index/"},
  {&Ustc,          "https://mirrors.ustc.edu.cn/crates.io-index/"},
  {&RsProxyCN,     "https://rsproxy.cn/index/"},
  {&Hust,          "https://mirrors.hust.edu.cn/crates.io-index/"}
};
def_sources_n(pl_rust);


/**
 * chsrc get rust
 */
void
pl_rust_getsrc (char *option)
{
  chsrc_view_file ("~/.cargo/config.toml");
}


/**
 * @consult https://mirrors.tuna.tsinghua.edu.cn/help/crates.io-index/
 * @consult https://help.mirrors.cernet.edu.cn/crates.io-index
 *
 * chsrc set rust
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


/**
 * chsrc reset rust
 */
void
pl_rust_resetsrc (char *option)
{
  pl_rust_setsrc (SetsrcType_Reset);
}


FeatInfo
pl_rust_feat (char *option)
{
  FeatInfo fi = {0};

  fi.can_get = true;
  fi.can_reset = true;

  fi.stcan_locally = CanSemi;
  fi.locally = "可以基于本项目换源吗？请帮助确认";
  fi.can_english = false;
  fi.can_user_define = true;

  fi.note = NULL;
  return fi;
}


def_target_gsrf(pl_rust);
