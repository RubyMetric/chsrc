/* --------------------------------------------------------------
* File          : chsrc.h
* Authors       : Aoran Zeng <ccmywish@qq.com>
* Created on    : <2023-08-29>
* Last modified : <2023-08-30>
*
* chsrc:
*
*   chsrc.c 头文件
* -------------------------------------------------------------*/

#include <stdio.h>
#include "helper.h"

typedef struct {
  const char* abbr;
  const char* name;
} mirror_info;

// 教育网
// 我们目前根据 https://github.com/mirrorz-org/oh-my-mirrorz 挑选速度前10位
//
mirror_info
  Tuna          = {"TUNA",     "清华大学开源软件镜像站"},
  Sjtug_Zhiyuan = {"SJTUG-zhiyuan", "上海交通大学致远镜像站"},
  Zju           = {"ZJU",      "浙江大学开源软件镜像站"},
  Lzuoss        = {"LZUOSS",   "兰州大学开源社区镜像站"},
  Jlu           = {"JLU",      "吉林大学开源镜像站"},
  Bfsu          = {"BFSU",     "北京外国语大学开源软件镜像站"},
  Pku           = {"PKU",      "北京大学开源镜像站"},
  Bjtu          = {"BJTU",     "北京交通大学自由与开源软件镜像站"},
  Sustech       = {"SUSTech",  "南方科技大学开源软件镜像站"},
  Ustc          = {"USTC",     "中国科学技术大学开源镜像站"};

// 大型公司
mirror_info
  Ali      = {"Ali OPSX", "阿里巴巴开源镜像站"},
  Tencent  = {"Tencent",  "腾讯软件源"},
  Netease  = {"Netease",  "网易开源镜像站"},
  Sohu     = {"SOHU",     "搜狐开源镜像站"};

// 开源社区
mirror_info
  RubyChina = {"RubyChina", "Ruby China 社区源"};


typedef struct {
  const mirror_info* mirror;
  const char* url;
} source_info;


/**
 * 源信息
 *
 * 我们要求每个源至少有一个教育网镜像，至少有一个商业公司或开源社区维护的镜像
 */


/**
 * 2023-08-29 更新
 *
 * 速度已经过测试，目前北外最快，Ruby China 的源慢了一半
 */
static source_info
pl_ruby_sources[] = {
  {&Bfsu,      "https://mirrors.bfsu.edu.cn/rubygems/"},
  {&Ali,       "https://mirrors.aliyun.com/rubygems/"},
  {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/rubygems/"},
  {&Tencent,   "https://mirrors.tencent.com/rubygems/"},
  {&RubyChina, "https://gems.ruby-china.com"},
},


/**
 * 2023-08-29 更新
 *
 * 不要添加Zju，浙大的pypi在校外访问会自动转向Tuna
 *
 * TODO: 速度只经过简单测试，请Python用户协助，待更新完整列表，如阿里，腾讯，其他大学镜像
 */
pl_python_sources[] = {
  {&Sjtug_Zhiyuan, "https://mirror.sjtu.edu.cn/pypi/web/simple"},
  {&Tuna,          "https://pypi.tuna.tsinghua.edu.cn/simple"},
  {&Lzuoss,        "https://mirror.lzu.edu.cn/pypi/web/simple"},
  {&Jlu,           "https://mirrors.jlu.edu.cn/pypi//web/simple"},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/pypi/web/simple"}
},



/**
 * 2023-08-30 更新
 *
 * Sjtug, Tuna, Lzuoss, Jlu, Bfsu, 网易，搜狐 都没有
 *
 * 腾讯软件源虽然有npm的名，但名存实亡
 */
pl_nodejs_sources[] = {
  {&Ali,           "https://registry.npmmirror.com"},
  {&Zju,           "https://mirrors.zju.edu.cn/docs/npm/"}
},


/**
 * 2023-08-30 更新
 *
 * 参考：https://help.mirrors.cernet.edu.cn/CPAN/
 *
 * Jlu 吉林大学没有该源
 *
 * TODO: 速度未经测试，请Perl用户协助
 */
pl_perl_sources[] = {
  {&Ali,           "https://mirrors.aliyun.com/CPAN/"},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/CPAN/"},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/CPAN/"},
  {&Bjtu,          "https://mirror.bjtu.edu.cn/cpan/"},
  {&Lzuoss         "https://mirror.lzu.edu.cn/CPAN/"}
};


static source_info
os_ubuntu_sources[] = {
  {&Ali,       "https://mirrors.aliyun.com/ubuntu/"},
  {&Bfsu,      "https://mirrors.bfsu.edu.cn/ubuntu/"},
  {&Ustc,      "https://mirrors.ustc.edu.cn/ubuntu/"},
  {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/ubuntu/"},
  {&Tencent,   "https://mirrors.tencent.com/ubuntu/"},
  {&Netease,   "https://mirrors.163.com/ubuntu/"},
  {&Sohu,      "https://mirrors.sohu.com/ubuntu/"},
};


/* 函数签名 */
bool does_the_program_exist (char* check_cmd, char* progname);
