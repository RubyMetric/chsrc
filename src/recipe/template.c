/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2024-08-09>
 * Last Modified : <2024-09-13>
 * -------------------------------------------------------------
 * 本文件作为一个通用模板：
 *
 *    为一个【换源目标(target)】定义具体的【换源方法(recipe)】
 *
 * 模版中:
 * <target>   为该换源目标的名称
 * <category> 为该换源目标的类别，仅有3类: pl,   os, wr
 *            分别对应3个子目录:         lang, os, ware
 * ------------------------------------------------------------*/



/* 模版文件从下行正式开始 */
/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Nil Null <nil@null.org> 尼尔闹先生
 * Contributors  : Nul None <nul@none.org> 怒了馕女士
 * Created On    : <202x-01-01> https://www.yuque.com/ccmywish/blog/nil-null-and-nul-none
 * Last Modified : <202x-01-01> 请更新文件标头
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
 * @update 2024-08-09
 * @note   该target的各个源地址，该例数据为虚拟填充
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
 *
 * 用于 chsrc get <target>
 */
void
<category>_<target>_getsrc (char *option)
{
  // chsrc get <target>
}


/**
 * @required 必需
 * @consult  写明换源实现的参考地址
 *
 * 用于 chsrc set <target>
 */
void
<category>_<target>_setsrc (char *option)
{
  // 下面这行是必须的，注入source变量
  chsrc_yield_source_and_confirm (<category>_<target>);

  /* 具体的换源步骤，如调用第三方命令... */

  // 最后总结输出
  chsrc_conclude (&source, chsrc_type);
}


/**
 * @required 非必需
 *
 * 用于 chsrc reset <target>
 */
void
<category>_<target>_resetsrc (char *option)
{
  // 往往通过下述方式统一在 setsrc() 中实现
  // <category>_<target>_setsrc (ChsrcTypeReset);
}


/**
 * @required 非必需
 *
 * 用于 chsrc ls <target>
 */
FeatInfo
<category>_<target>_feat (char *option)
{
  FeatInfo fi = {0};

  fi.can_get = true;
  fi.can_reset = false;

  fi.stcan_locally = CanSemi;
  fi.locally = "具体说明是否支持项目级换源...";
  fi.can_english = false;
  fi.can_user_define = false;

  fi.note = "备注说明...";
  return fi;
}


// 定义此 target，参考 "source.h"
// 下列情形多选1
def_target(<category>_<target>);
def_target_gsr(<category>_<target>);
def_target_gsf(<category>_<target>);
def_target_gsrf(<category>_<target>);
def_target_s(<category>_<target>);
