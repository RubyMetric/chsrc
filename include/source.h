/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright © 2023-2024 Aoran Zeng, Heng Guo
 * -------------------------------------------------------------
 * File          : source.h
 * Authors       : Aoran Zeng    <ccmywish@qq.com>
 *               | Heng Guo      <2085471348@qq.com>
 * Contributors  : Shengwei Chen <414685209@qq.com>
 *               |
 * Created on    : <2023-08-29>
 * Last modified : <2024-08-09>
 *
 * 镜像站与换源信息
 * ------------------------------------------------------------*/

typedef struct {
  const char *code; // 用于用户指定镜像站
  const char *abbr;
  const char *name;
  const char *site;
  const char *__bigfile_url;  // 用于对该镜像站测速
} MirrorSite;

// #define Big_File_ubuntu  "/indices/md5sums.gz"  这个是错的
#define Big_File_ctan       "/systems/texlive/Images/texlive.iso"           // 4.8GB
#define Big_File_debian     "/ls-lR.gz"                                     // 13.9 MB
#define Big_File_archlinux  "/iso/latest/archlinux-x86_64.iso"              // 800MB 左右
#define Big_File_deepin     "/20.9/deepin-desktop-community-20.9-amd64.iso" // 4GB左右


/**
 * 教育网镜像
 *
 * 我们目前根据 https://github.com/mirrorz-org/oh-my-mirrorz 挑选速度前10位
 */
MirrorSite
MirrorZ       = {"mirrorz", "MirrorZ",       "MirrorZ 校园网镜像站",       "https://mirrors.cernet.edu.cn/",  NULL},

Tuna          = {"tuna",    "TUNA",          "清华大学开源软件镜像站",     "https://mirrors.tuna.tsinghua.edu.cn/",
                 "https://mirrors.tuna.tsinghua.edu.cn/speedtest/1000mb.bin"},

Sjtug_Zhiyuan = {"sjtu",    "SJTUG-zhiyuan", "上海交通大学致远镜像站",     "https://mirrors.sjtug.sjtu.edu.cn/",
                 "https://mirrors.sjtug.sjtu.edu.cn/ctan" Big_File_ctan},

Zju           = {"zju",     "ZJU",           "浙江大学开源软件镜像站",     "https://mirrors.zju.edu.cn/",
                 "https://mirrors.zju.edu.cn/debian" Big_File_debian},

Lzuoss        = {"lzu",     "LZUOSS",        "兰州大学开源社区镜像站",     "https://mirror.lzu.edu.cn/",
                 "https://mirror.lzu.edu.cn/CTAN" Big_File_ctan},

Jlu           = {"jlu",     "JLU",           "吉林大学开源镜像站",         "https://mirrors.jlu.edu.cn/",
                 "https://mirrors.jlu.edu.cn/_static/speedtest.bin"},

Bfsu          = {"bfsu",    "BFSU",          "北京外国语大学开源软件镜像站","https://mirrors.bfsu.edu.cn/",
                 "https://mirrors.bfsu.edu.cn/speedtest/1000mb.bin"},

Pku           = {"pku",     "PKU",           "北京大学开源镜像站",         "https://mirrors.pku.edu.cn/",
                 "https://mirrors.pku.edu.cn/debian" Big_File_debian},

Bjtu          = {"bjtu",    "BJTU",          "北京交通大学自由与开源软件镜像站", "https://mirror.bjtu.edu.cn/",
                 "https://mirror.bjtu.edu.cn/archlinux" Big_File_archlinux},

Sustech       = {"sustech", "SUSTech",       "南方科技大学开源软件镜像站",  "https://mirrors.sustech.edu.cn/",
                 "https://mirrors.sustech.edu.cn/site/speedtest/1000mb.bin"},

Ustc          = {"ustc",    "USTC",          "中国科学技术大学开源镜像站",  "https://mirrors.ustc.edu.cn/",
                 "https://mirrors.ustc.edu.cn/CTAN" Big_File_ctan},

Hust          = {"hust",    "HUST",          "华中科技大学开源镜像站",     "https://mirrors.hust.edu.cn/",
                 "https://mirrors.hust.edu.cn/debian" Big_File_debian},

// 速度暂时处于10位以后，但是目前可用的源
Nju           = {"nju",     "NJU",           "南京大学开源镜像站",         "https://mirrors.nju.edu.cn/",
                 "https://mirrors.nju.edu.cn/archlinux" Big_File_archlinux};

/**
 * @note by:ccmywish {
 *   [2023-09-05] 我只使用了不到5次，重庆大学镜像站就把我的ip封杀了，对用户来说封杀策略过严，暂时不可靠，暂时不用
 * }
 */
//  Cqu       = {"cqu",     "CQU",           "重庆大学开源软件镜像站",     "https://mirrors.cqu.edu.cn/",
//               "https://mirrors.cqu.edu.cn/speedtest/1000mb.bin"};



/**
 * 商业公司提供的源
 *
 * @warning 腾讯软件源中，有很多链接都已失效，请仔细检查
 */
MirrorSite
  Ali      = {"ali",     "Ali OPSX", "阿里巴巴开源镜像站",  "https://developer.aliyun.com/mirror/",
              "https://mirrors.aliyun.com/deepin-cd" Big_File_deepin},

  Tencent  = {"tencent", "Tencent",  "腾讯软件源",         "https://mirrors.tencent.com/",
              "https://mirrors.cloud.tencent.com/debian" Big_File_debian},

  Huawei   = {"huawei",  "Huawei Cloud", "华为开源镜像站",  "https://mirrors.huaweicloud.com/",
              "https://mirrors.huaweicloud.com/debian" Big_File_debian },

  Volcengine = {"volc",  "Volcengine",   "火山引擎开源软件镜像站", "https://developer.volcengine.com/mirror/",
              "https://mirrors.volces.com/debian" Big_File_debian },

  Netease  = {"netease", "Netease",  "网易开源镜像站",      "https://mirrors.163.com/",
              "https://mirrors.163.com/deepin-cd" Big_File_deepin},

  Sohu     = {"sohu",    "SOHU",     "搜狐开源镜像站",      "https://mirrors.sohu.com/",
              "https://mirrors.sohu.com/deepin-cd" Big_File_deepin};

// 开源社区
MirrorSite
  NugetOrg   = {"nuget.org",   "NuGet Org",    "Nuget Organization", "https://www.nuget.org/",  NULL};

MirrorSite
  Upstream = {"upstream",     "Upstream",     "上游默认源",     NULL,     NULL};
MirrorSite
  UserDefine = {"user",       "用户自定义",     "用户自定义",     NULL,     NULL};




typedef struct {
  const MirrorSite *mirror;
  const char *url;
} SourceInfo;



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


#define def_sources_n(t) const size_t t##_sources_n = xy_arylen(t##_sources)


def_sources_n(pl_clojure);
