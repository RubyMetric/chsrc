/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng    <ccmywish@qq.com>
 *               | Heng Guo      <2085471348@qq.com>
 * Contributors  : Shengwei Chen <414685209@qq.com>
 *               |
 * Created On    : <2023-08-29>
 * Last Modified : <2024-09-14>
 *
 * 镜像站与换源信息
 * ------------------------------------------------------------*/

typedef struct MirrorSite_t {
  const char *code; // 用于用户指定镜像站
  const char *abbr; // 需要使用镜像站的英文名时，用这个代替，因为大部分镜像站没有提供正式的英文名
  const char *name; // 镜像站中文名
  const char *site;
  const char *__bigfile_url;  // 用于对该镜像站测速
} MirrorSite;

// #define Big_File_ubuntu  "/indices/md5sums.gz"  这个是错的
#define Big_File_ctan       "/systems/texlive/Images/texlive.iso"           // 4.8GB
#define Big_File_debian     "/ls-lR.gz"                                     // 13.9MB
#define Big_File_archlinux  "/iso/latest/archlinux-x86_64.iso"              // 800MB 左右
#define Big_File_deepin     "/20.9/deepin-desktop-community-20.9-amd64.iso" // 4GB 左右


/**
 * 教育网镜像
 *
 * 我们目前根据 https://github.com/mirrorz-org/oh-my-mirrorz 挑选速度前10位
 */
MirrorSite
MirrorZ       = {"mirrorz", "MirrorZ",       "MirrorZ 校园网镜像站",     "https://mirrors.cernet.edu.cn/",  NULL},

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
 */
MirrorSite
Ali = {
  "ali", "Ali OPSX Public", "阿里巴巴开源镜像站(公网)",
  "https://developer.aliyun.com/mirror/",
  "https://mirrors.aliyun.com/deepin-cd" Big_File_deepin},

/*
// https://mirrors.cloud.aliyuncs.com/
Ali_ECS_VPC = {
  "ali-ECS-VPC", "Ali OPSX ECS VPC", "阿里巴巴开源镜像站(ECS VPC网络)",
  "https://developer.aliyun.com/mirror/",
  "https://mirrors.cloud.aliyuncs.com/deepin-cd" Big_File_deepin},

// https://mirrors.aliyuncs.com/
Ali_ECS_classic = {
  "ali-ECS", "Ali OPSX ECS", "阿里巴巴开源镜像站(ECS 经典网络)",
  "https://developer.aliyun.com/mirror/",
  "https://mirrors.aliyuncs.com/deepin-cd" Big_File_deepin},
*/

Tencent = {
  "tencent", "Tencent Public",  "腾讯软件源(公网)",  "https://mirrors.tencent.com/",
  "https://mirrors.cloud.tencent.com/debian" Big_File_debian},

/*
Tencent_Intra = {
  "tencent-intra", "Tencent Intranet",  "腾讯软件源(内网)", "https://mirrors.tencent.com/",
  "https://mirrors.cloud.tencentyun.com/debian" Big_File_debian},
*/

Huawei = {"huawei",  "Huawei Cloud", "华为开源镜像站",  "https://mirrors.huaweicloud.com/",
          "https://mirrors.huaweicloud.com/debian" Big_File_debian },

Volcengine = {
  "volc",  "Volcengine", "火山引擎开源软件镜像站(公网)",
  "https://developer.volcengine.com/mirror/",
  "https://mirrors.volces.com/debian" Big_File_debian },
/*
Volceengine_Intra = {
  "volc-intra",  "Volcengine Intranet", "火山引擎开源软件镜像站(内网)",
  "https://developer.volcengine.com/mirror/",
  "https://mirrors.ivolces.com/debian" Big_File_debian },
*/

Netease  = {
  "netease", "Netease", "网易开源镜像站",
  "https://mirrors.163.com/",
  "https://mirrors.163.com/deepin-cd" Big_File_deepin},

Sohu = {
  "sohu", "SOHU", "搜狐开源镜像站",
  "https://mirrors.sohu.com/",
  "https://mirrors.sohu.com/deepin-cd" Big_File_deepin};


MirrorSite
  Upstream = {"upstream", "Upstream",  "上游默认源",  NULL,  NULL};
MirrorSite
  UserDefine = {"user",   "用户自定义",  "用户自定义",  NULL,  NULL};


typedef struct SourceInfo_t {
  const MirrorSite *mirror;
  const char *url;
} SourceInfo;

#define def_sources_n(t) const size_t t##_sources_n = xy_arylen(t##_sources)

enum StatusCan {
  CanNot,
  CanFully,
  CanSemi
};

/* Target Feature Info */
typedef struct FeatInfo_t {
  bool can_get;
  bool can_reset;       // 有的reset不是暂时没有实现，而是现在的实现根本就无法重置

  bool can_english;

  bool can_user_define; // 用户自定义换源URL

  enum StatusCan stcan_locally;
  char *locally;

  char *note;
} FeatInfo;

/* Target Info */
typedef struct TargetInfo_t {
  void (*getfn)   (char *option);
  void (*setfn)   (char *option);
  void (*resetfn) (char *option);

  FeatInfo (*featfn)  (char *option);

  SourceInfo *sources;
  size_t      sources_n;
} TargetInfo;


#define def_target_inner_s(t)    NULL,       t##_setsrc, NULL,         NULL
#define def_target_inner_gs(t)   t##_getsrc, t##_setsrc, NULL,         NULL
#define def_target_inner_gsr(t)  t##_getsrc, t##_setsrc, t##_resetsrc, NULL
#define def_target_inner_gsf(t)  t##_getsrc, t##_setsrc, NULL,         t##_feat
#define def_target_inner_gsrf(t) t##_getsrc, t##_setsrc, t##_resetsrc, t##_feat

#define def_target_sourcesn(t)   t##_sources, t##_sources_n

// 大部分target还不支持reset，所以暂时先默认设置为NULL来过渡
#define def_target(t)      TargetInfo t##_target = {def_target_inner_gs(t),def_target_sourcesn(t)}
#define def_target_gs(t)   TargetInfo t##_target = {def_target_inner_gs(t),def_target_sourcesn(t)}
#define def_target_gsr(t)  TargetInfo t##_target = {def_target_inner_gsr(t),def_target_sourcesn(t)}
#define def_target_gsf(t)  TargetInfo t##_target = {def_target_inner_gsf(t),def_target_sourcesn(t)}
#define def_target_gsrf(t) TargetInfo t##_target = {def_target_inner_gsrf(t),def_target_sourcesn(t)}
#define def_target_s(t)    TargetInfo t##_target = {def_target_inner_s(t),def_target_sourcesn(t)}
