/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Name     :  mirror.c
 * File Authors  :  Aoran Zeng   <ccmywish@qq.com>
 *               |   Heng Guo    <2085471348@qq.com>
 * Contributors  : Shengwei Chen <414685209@qq.com>
 *               |
 * Created On    : <2023-08-29>
 * Last Modified : <2024-11-21>
 *
 * 通用镜像站
 * ------------------------------------------------------------*/

#define Big_File_ubuntu     "/24.04/ubuntu-24.04.1-desktop-amd64.iso"       // 5.8 GB
#define Big_File_ctan       "/systems/texlive/Images/texlive.iso"           // 4.8 GB
#define Big_File_archlinux  "/iso/latest/archlinux-x86_64.iso"              // 800 MB
#define Big_File_deepin     "/20.9/deepin-desktop-community-20.9-amd64.iso" // 4 GB

/**
 * 教育网镜像
 *
 * @sync https://github.com/RubyMetric/chsrc/wiki
 *
 * Wiki中的排序是根据 https://github.com/mirrorz-org/oh-my-mirrorz 挑选速度前10位
 */
MirrorSite_t MirrorZ =
{
  "mirrorz", "MirrorZ",  "MirrorZ 校园网镜像站", "https://mirrors.cernet.edu.cn/",
  {SKIP, "功能特殊无法测速，跳过", "SKIP because of its special function"}
},

Tuna =
{
  "tuna", "TUNA", "清华大学开源软件镜像站", "https://mirrors.tuna.tsinghua.edu.cn/",
  {NotSkip, NA, NA, "https://mirrors.tuna.tsinghua.edu.cn/speedtest/1000mb.bin"}
},

Sjtug_Zhiyuan =
{
  "sjtu", "SJTUG-zhiyuan", "上海交通大学致远镜像站", "https://mirrors.sjtug.sjtu.edu.cn/",
  {NotSkip, NA, NA, "https://mirrors.sjtug.sjtu.edu.cn/ctan" Big_File_ctan}
},

Zju =
{
  "zju", "ZJU", "浙江大学开源软件镜像站", "https://mirrors.zju.edu.cn/",
  {NotSkip, NA, NA, "https://mirrors.zju.edu.cn/ubuntu-releases" Big_File_ubuntu}
},

Lzuoss =
{
  "lzu", "LZUOSS", "兰州大学开源社区镜像站", "https://mirror.lzu.edu.cn/",
  {NotSkip, NA, NA, "https://mirror.lzu.edu.cn/CTAN" Big_File_ctan}
},

Jlu =
{
  "jlu", "JLU", "吉林大学开源镜像站", "https://mirrors.jlu.edu.cn/",
  {NotSkip, NA, NA, "https://mirrors.jlu.edu.cn/_static/speedtest.bin"}
},

Bfsu =
{
  "bfsu", "BFSU", "北京外国语大学开源软件镜像站", "https://mirrors.bfsu.edu.cn/",
  {NotSkip, NA, NA, "https://mirrors.bfsu.edu.cn/speedtest/1000mb.bin"}
},

Pku =
{
  "pku", "PKU", "北京大学开源镜像站", "https://mirrors.pku.edu.cn/",
  {NotSkip, NA, NA, "https://mirrors.pku.edu.cn/ubuntu-releases" Big_File_ubuntu}
},

Bjtu =
{
  "bjtu", "BJTU", "北京交通大学自由与开源软件镜像站", "https://mirror.bjtu.edu.cn/",
  {NotSkip, NA, NA, "https://mirror.bjtu.edu.cn/archlinux" Big_File_archlinux}
},

Sustech =
{
  "sustech", "SUSTech", "南方科技大学开源软件镜像站", "https://mirrors.sustech.edu.cn/",
  {NotSkip, NA, NA, "https://mirrors.sustech.edu.cn/site/speedtest/1000mb.bin"}
},

Ustc =
{
  "ustc", "USTC", "中国科学技术大学开源镜像站", "https://mirrors.ustc.edu.cn/",
  {NotSkip, NA, NA, "https://mirrors.ustc.edu.cn/ubuntu-releases" Big_File_ubuntu}
},

Hust =
{
  "hust", "HUST", "华中科技大学开源镜像站", "https://mirrors.hust.edu.cn/",
  {NotSkip, NA, NA, "https://mirrors.hust.edu.cn/ubuntu-releases" Big_File_ubuntu}
},

// 速度暂时处于10位以后，但是目前可用的源
Iscas =
{
  "iscas", "ISCAS", "中科院软件所智能软件研究中心开源镜像站", "https://mirror.iscas.ac.cn/",
  {NotSkip, NA, NA, "https://mirror.iscas.ac.cn/ubuntu-releases" Big_File_ubuntu}
},

Scau =
{
  "scau", "SCAU", "华南农业大学开源软件镜像站", "https://mirrors.scau.edu.cn/",
  {NotSkip, NA, NA, "https://mirrors.scau.edu.cn/ubuntu-releases" Big_File_ubuntu}
},

Nju =
{
  "nju", "NJU", "南京大学开源镜像站", "https://mirrors.nju.edu.cn/",
  {NotSkip, NA, NA, "https://mirrors.nju.edu.cn/archlinux" Big_File_archlinux}
};

/**
 * @note 2023-09-05 只使用了不到5次，重庆大学镜像站就把我的ip封杀了，对用户来说封杀策略过严，暂时不可靠，暂时不用
 *
 * Cqu =
 * {
 *   "cqu", "CQU", "重庆大学开源软件镜像站", "https://mirrors.cqu.edu.cn/",
 *   {NotSkip, NA, NA, "https://mirrors.cqu.edu.cn/speedtest/1000mb.bin"}
 * };
 *
 */



/**
 * 商业公司提供的源
 */
MirrorSite_t Ali =
{
  "ali", "Ali OPSX Public", "阿里巴巴开源镜像站(公网)", "https://developer.aliyun.com/mirror/",
  {NotSkip, NA, NA, "https://mirrors.aliyun.com/deepin-cd" Big_File_deepin}
},
/*
// https://mirrors.cloud.aliyuncs.com/
Ali_ECS_VPC =
{
  "ali-ECS-VPC", "Ali OPSX ECS VPC", "阿里巴巴开源镜像站(ECS VPC网络)", "https://developer.aliyun.com/mirror/",
  {NotSkip, NA, NA, "https://mirrors.cloud.aliyuncs.com/deepin-cd" Big_File_deepin}
},

// https://mirrors.aliyuncs.com/
Ali_ECS_classic =
{
  "ali-ECS", "Ali OPSX ECS", "阿里巴巴开源镜像站(ECS 经典网络)", "https://developer.aliyun.com/mirror/",
  {NotSkip, NA, NA, "https://mirrors.aliyuncs.com/deepin-cd" Big_File_deepin}
},
*/

Tencent =
{
  "tencent", "Tencent Public", "腾讯软件源(公网)", "https://mirrors.tencent.com/",
  {NotSkip, NA, NA, "https://mirrors.cloud.tencent.com/ubuntu-releases" Big_File_ubuntu}
},
/*
Tencent_Intra =
{
  "tencent-intra", "Tencent Intranet",  "腾讯软件源(内网)", "https://mirrors.tencent.com/",
  {NotSkip, NA, NA, "https://mirrors.cloud.tencentyun.com/ubuntu-releases" Big_File_ubuntu}
},
*/

Huawei =
{
  "huawei",  "Huawei Cloud", "华为开源镜像站",  "https://mirrors.huaweicloud.com/",
  {NotSkip, NA, NA, "https://mirrors.huaweicloud.com/ubuntu-releases" Big_File_ubuntu}
},

Volcengine =
{
  "volc", "Volcengine", "火山引擎开源软件镜像站(公网)", "https://developer.volcengine.com/mirror/",
  {NotSkip, NA, NA, "https://mirrors.volces.com/ubuntu-releases" Big_File_ubuntu}
},
/*
Volceengine_Intra =
{
  "volc-intra",  "Volcengine Intranet", "火山引擎开源软件镜像站(内网)",
  "https://developer.volcengine.com/mirror/",
  "https://mirrors.ivolces.com/ubuntu-releases" Big_File_ubuntu },
*/

Netease =
{
  "netease", "Netease", "网易开源镜像站", "https://mirrors.163.com/",
  {NotSkip, NA, NA, "https://mirrors.163.com/deepin-cd" Big_File_deepin}
},

Sohu =
{
  "sohu", "SOHU", "搜狐开源镜像站", "https://mirrors.sohu.com/",
  {NotSkip, NA, NA, "https://mirrors.sohu.com/deepin-cd" Big_File_deepin}
};
