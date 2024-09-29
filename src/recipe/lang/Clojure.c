/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-10>
 * Last Modified : <2024-08-09>
 * ------------------------------------------------------------*/

/**
 * @time 2023-09-10 更新
 * @note 不太好换，且用户可能不多，我们暂时只给用户提供文档
 */
static SourceInfo
pl_clojure_sources[] = {
  {&Upstream,       NULL},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/help/clojars/"},
  {&Ustc,          "https://mirrors.ustc.edu.cn/help/clojars.html"}
};
def_sources_n(pl_clojure);


void
pl_clojure_setsrc (char *option)
{
  chsrc_yield_source_and_confirm (pl_clojure);

  chsrc_note2 ("抱歉，Clojure换源较复杂，您可手动查阅并换源:");
  puts (source.url);
  chsrc_conclude (&source, SetsrcType_Manual);
}

def_target_s (pl_clojure);
