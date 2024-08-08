/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * Contributors  : Null Nil <null@nil.com>
 * Created on    : <2024-08-09>
 * Last modified : <2024-08-09>
 *
 * 该模版中:
 * <target>   为该换源目标的名称
 * <category> 为该换源目标的类别，仅有3类: pl,   os, wr
 *            分别对应3个子目录:         lang, os, ware
 * ------------------------------------------------------------*/

/**
 * 定义专服务于该target的镜像站，该例数据为虚拟填充
 */
static MirrorSite
RubyMetric = {"rbmt",                   // chsrc set <target> rbmt
              "RubyMetric",             // 该镜像站的缩写
              "RubyMetric.com",         // 该镜像站的全名
              "https://rubymetirc.com", // 镜像站首页
                                        // 镜像站某个较大的可下载物的下载链接，用于测速
              "https://rubymetirc.com/target/aws/aws-sdk-go/@v/v1.45.2.zip"};

/**
 * @time 2024-08-09 更新
 * @note 该target的各个源地址，该例数据为虚拟填充
 */
static SourceInfo
<category>_<target>_sources[] = {
  {&Upstream,      "上游地址，若维护者暂时未知，可填NULL，这个主要用于reset"},
  {&RubyMetric,    "https://rubymetirc.com/target"},
  {&RubyInstaller, "https://rubyinstaller.cn/target"},
  {&Gitee,         "https://gitee.com/RubyMetric/chsrc"},
  {&GitHub,        "https://github.com/RubyMetric/chsrc"}
};
def_sources_n(<category>_<target>);


/**
 * @required 非必需
 */
void
<category>_<target>_getsrc (char* option)
{
  // chsrc get <target>
}


/**
 * @required 必需
 * @ref      写明换源实现的参考地址
 */
void
<category>_<target>_setsrc (char* option)
{
  // chsrc set <target>

  // 下面这3行是必须的
  SourceInfo source;
  chsrc_yield_source (pl_ruby);
  chsrc_confirm_source (&source);


  /* 具体的换源步骤，如调用第三方命令... */


  // 最后总结输出
  chsrc_say_lastly (&source, chsrc_type);
}


/**
 * @required 非必需
 */
void
<category>_<target>_resetsrc (char* option)
{
  // chsrc reset <target>

  // 往往通过下述方式统一在 setsrc() 中实现
  // <category>_<name>_setsrc (ChsrcTypeReset);
}


// 定义此 target，参考 "target.h"
// 下列情形3选1
def_target_full(<category>_<target>);
def_target(<category>_<target>)
def_target_no_get(<category>_<target>)
