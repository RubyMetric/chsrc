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

typedef struct {
  const char* abbr;
  const char* name;
} mirror_info;

// 教育网
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
  Ustc          = {"USTC",     "中国科学技术大学开源镜像站"},
  Wy            = {"WY",       "网易开源镜像站"},
  Sohu          = {"SOHU",     "搜狐开源镜像站"};

// 大型公司
mirror_info
  Ali      = {"Ali OPSX", "阿里巴巴开源镜像站"},
  Tencent  = {"Tencent",  "腾讯软件源"};

// 开源社区
mirror_info
  RubyChina = {"RubyChina", "Ruby China 社区源"};


typedef struct {
  const mirror_info* mirror;
  const char* url;
} source_info;

static source_info
pl_ruby_sources[] = {
  // 目前北外最快，Ruby China的源慢了一半
  {&Bfsu,      "https://mirrors.bfsu.edu.cn/rubygems/"},
  {&Ali,       "https://mirrors.aliyun.com/rubygems/"},
  {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/rubygems/"},
  {&Tencent,   "https://mirrors.tencent.com/rubygems/"},
  {&RubyChina, "https://gems.ruby-china.com"},
};

static source_info
pl_ubuntu_sources[] = {
  {&Bfsu,      "https://mirrors.bfsu.edu.cn/ubuntu/"},
  {&Ustc,      "https://mirrors.ustc.edu.cn/ubuntu/"},
  {&Ali,       "https://mirrors.aliyun.com/ubuntu/"},
  {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/ubuntu/"},
  {&Tencent,   "https://mirrors.tencent.com/ubuntu/"},
  {&Wy,        "https://mirrors.163.com/ubuntu/"},
  {&Sohu,      "https://mirrors.sohu.com/ubuntu/"},
};
