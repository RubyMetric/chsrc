/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Name     :  mirror.c
 * File Authors  :  Aoran Zeng   <ccmywish@qq.com>
 *               |   Heng Guo    <2085471348@qq.com>
 * Contributors  : Shengwei Chen <414685209@qq.com>
 *               |  Jialin Lyu   <jialinlvcn@aliyun.com>
 *               |  Mikachu2333  <mikachu.23333@zohomail.com>
 *               |
 * Created On    : <2023-08-29>
 * Last Modified : <2025-07-11>
 *
 * 通用镜像站
 * ------------------------------------------------------------*/

#define Big_File_ubuntu     "/18.04/ubuntu-18.04.6-desktop-amd64.iso"       // 2.3 GB
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
  IS_GeneralMirrorSite,
  "mirrorz", "MirrorZ",  "校园网联合镜像站(MirrorZ)", "https://mirrors.cernet.edu.cn/",
  {SKIP, "功能特殊无法测速，跳过", "SKIP because of its special function", NULL, ROUGH}
},

Tuna =
{
  IS_GeneralMirrorSite,
  "tuna", "TUNA", "清华大学开源软件镜像站", "https://mirrors.tuna.tsinghua.edu.cn/",
  {NotSkip, NA, NA, "https://mirrors.tuna.tsinghua.edu.cn/speedtest/1000mb.bin", ROUGH}
},

/**
 * @note 2025-3-17 SJTUG 共设两台服务器。思源服务器同步新镜像，致远服务器兼容原 SJTU 镜像站。
 * @note 有些target（例如flathub）思源站的兼容性不好，可以考虑将两个服务器分开测试
 */

Sjtug_Zhiyuan =
{
  IS_GeneralMirrorSite,
  "sjtu-zy", "SJTUG-zhiyuan", "上海交通大学致远镜像站", "https://mirrors.sjtug.sjtu.edu.cn/",
  {NotSkip, NA, NA, "https://mirrors.sjtug.sjtu.edu.cn/ctan" Big_File_ctan, ROUGH}
},

Sjtug_Siyuan =
{
  IS_GeneralMirrorSite,
  "sjtu-sy", "SJTUG-siyuan", "上海交通大学思源镜像站", "https://mirror.sjtu.edu.cn/",
  {NotSkip, NA, NA, "https://mirror.sjtu.edu.cn/ctan" Big_File_ctan, ROUGH}
},

Zju =
{
  IS_GeneralMirrorSite,
  "zju", "ZJU", "浙江大学开源软件镜像站", "https://mirrors.zju.edu.cn/",
  {NotSkip, NA, NA, "https://mirrors.zju.edu.cn/ubuntu-releases" Big_File_ubuntu, ROUGH}
},

Lzuoss =
{
  IS_GeneralMirrorSite,
  "lzu", "LZUOSS", "兰州大学开源社区镜像站", "https://mirror.lzu.edu.cn/",
  {NotSkip, NA, NA, "https://mirror.lzu.edu.cn/CTAN" Big_File_ctan, ROUGH}
},

Jlu =
{
  IS_GeneralMirrorSite,
  "jlu", "JLU", "吉林大学开源镜像站", "https://mirrors.jlu.edu.cn/",
  {NotSkip, NA, NA, "https://mirrors.jlu.edu.cn/_static/speedtest.bin", ROUGH}
},

Bfsu =
{
  IS_GeneralMirrorSite,
  "bfsu", "BFSU", "北京外国语大学开源软件镜像站", "https://mirrors.bfsu.edu.cn/",
  {NotSkip, NA, NA, "https://mirrors.bfsu.edu.cn/speedtest/1000mb.bin", ROUGH}
},

Pku =
{
  IS_GeneralMirrorSite,
  "pku", "PKU", "北京大学开源镜像站", "https://mirrors.pku.edu.cn/",
  {NotSkip, NA, NA, "https://mirrors.pku.edu.cn/ubuntu-releases" Big_File_ubuntu, ROUGH}
},

Bjtu =
{
  IS_GeneralMirrorSite,
  "bjtu", "BJTU", "北京交通大学自由与开源软件镜像站", "https://mirror.bjtu.edu.cn/",
  {NotSkip, NA, NA, "https://mirror.bjtu.edu.cn/archlinux" Big_File_archlinux, ROUGH}
},

Sustech =
{
  IS_GeneralMirrorSite,
  "sustech", "SUSTech", "南方科技大学开源软件镜像站", "https://mirrors.sustech.edu.cn/",
  {NotSkip, NA, NA, "https://mirrors.sustech.edu.cn/site/speedtest/1000mb.bin", ROUGH}
},

Ustc =
{
  IS_GeneralMirrorSite,
  "ustc", "USTC", "中国科学技术大学开源镜像站", "https://mirrors.ustc.edu.cn/",
  {NotSkip, NA, NA, "https://mirrors.ustc.edu.cn/ubuntu-releases" Big_File_ubuntu, ROUGH}
},

Hust =
{
  IS_GeneralMirrorSite,
  "hust", "HUST", "华中科技大学开源镜像站", "https://mirrors.hust.edu.cn/",
  {NotSkip, NA, NA, "https://mirrors.hust.edu.cn/ubuntu-releases" Big_File_ubuntu, ROUGH}
},

// 速度暂时处于10位以后或者无测速报告，但是目前可用的源

Iscas =
{
  IS_GeneralMirrorSite,
  "iscas", "ISCAS", "中科院软件所智能软件研究中心开源镜像站", "https://mirror.iscas.ac.cn/",
  {NotSkip, NA, NA, "https://mirror.iscas.ac.cn/ubuntu-releases" Big_File_ubuntu, ROUGH}
},

Scau =
{
  IS_GeneralMirrorSite,
  "scau", "SCAU", "华南农业大学开源软件镜像站", "https://mirrors.scau.edu.cn/",
  {NotSkip, NA, NA, "https://mirrors.scau.edu.cn/ubuntu-releases" Big_File_ubuntu, ROUGH}
},

NJTech =
{
  IS_GeneralMirrorSite,
  "njtech", "NJTech", "南京工业大学开源软件镜像站", "https://mirrors.njtech.edu.cn/",
  {NotSkip, NA, NA, "https://mirrors.njtech.edu.cn/ubuntu-releases" Big_File_ubuntu, ROUGH}
},

Nyist =
{
  IS_GeneralMirrorSite,
  "nyist", "NYIST", "南阳理工学院开源软件镜像站", "https://mirror.nyist.edu.cn/",
  {NotSkip, NA, NA, "https://mirror.nyist.edu.cn/ubuntu-releases" Big_File_ubuntu, ROUGH}
},

Sdu =
{
  IS_GeneralMirrorSite,
  "sdu", "SDU", "山东大学镜像站", "https://mirrors.sdu.edu.cn/",
  {NotSkip, NA, NA, "https://mirrors.sdu.edu.cn/ubuntu-releases" Big_File_ubuntu, ROUGH}
},

Cqupt =
{
  IS_GeneralMirrorSite,
  "cqupt", "CQUPT", "重庆邮电大学开源镜像站", "https://mirrors.cqupt.edu.cn/",
  {NotSkip, NA, NA, "https://mirrors.cqupt.edu.cn/ubuntu-releases" Big_File_ubuntu, ROUGH}
},

Nju =
{
  IS_GeneralMirrorSite,
  "nju", "NJU", "南京大学开源镜像站", "https://mirrors.nju.edu.cn/",
  {NotSkip, NA, NA, "https://mirrors.nju.edu.cn/archlinux" Big_File_archlinux, ROUGH}
},

/**
 * @note 2023-09-05 封杀策略过严，谨慎使用
 */
Cqu =
{
  IS_GeneralMirrorSite,
  "cqu", "CQU", "重庆大学开源软件镜像站", "https://mirrors.cqu.edu.cn/",
  {NotSkip, NA, NA, "https://mirrors.cqu.edu.cn/ubuntu-releases" Big_File_ubuntu, ROUGH}
};

/**
 * 商业公司提供的源
 */
MirrorSite_t Ali =
{
  IS_GeneralMirrorSite,
  "ali", "Ali OPSX Public", "阿里巴巴开源镜像站(公网)", "https://developer.aliyun.com/mirror/",
  {NotSkip, NA, NA, "https://mirrors.aliyun.com/ubuntu/ls-lR.gz", ROUGH} // 31MB左右
},
/*
// https://mirrors.cloud.aliyuncs.com/
Ali_ECS_VPC =
{
  IS_GeneralMirrorSite,
  "ali-ECS-VPC", "Ali OPSX ECS VPC", "阿里巴巴开源镜像站(ECS VPC网络)", "https://developer.aliyun.com/mirror/",
  {NotSkip, NA, NA, "https://mirrors.cloud.aliyuncs.com/deepin-cd" Big_File_deepin, ROUGH}
},

// https://mirrors.aliyuncs.com/
Ali_ECS_classic =
{
  IS_GeneralMirrorSite,
  "ali-ECS", "Ali OPSX ECS", "阿里巴巴开源镜像站(ECS 经典网络)", "https://developer.aliyun.com/mirror/",
  {NotSkip, NA, NA, "https://mirrors.aliyuncs.com/deepin-cd" Big_File_deepin, ROUGH}
},
*/

Tencent =
{
  IS_GeneralMirrorSite,
  "tencent", "Tencent Public", "腾讯软件源(公网)", "https://mirrors.tencent.com/",
  {NotSkip, NA, NA, "https://mirrors.cloud.tencent.com/mariadb/mariadb-12.1.0/winx64-packages/mariadb-12.1.0-winx64-debugsymbols.zip", ROUGH} // 110MB左右
},
/*
Tencent_Intra =
{
  IS_GeneralMirrorSite,
  "tencent-intra", "Tencent Intranet",  "腾讯软件源(内网)", "https://mirrors.tencent.com/",
  {NotSkip, NA, NA, "https://mirrors.cloud.tencentyun.com/ubuntu-releases" Big_File_ubuntu, ROUGH}
},
*/

Huawei =
{
  IS_GeneralMirrorSite,
  "huawei",  "Huawei Cloud", "华为开源镜像站",  "https://mirrors.huaweicloud.com/",
  {NotSkip, NA, NA, "https://mirrors.huaweicloud.com/ubuntu-releases" Big_File_ubuntu, ROUGH}
},

Volcengine =
{
  IS_GeneralMirrorSite,
  "volc", "Volcengine", "火山引擎开源软件镜像站(公网)", "https://developer.volcengine.com/mirror/",
  {NotSkip, NA, NA, "https://mirrors.volces.com/ubuntu-releases" Big_File_ubuntu, ROUGH}
},
/*
Volceengine_Intra =
{
  IS_GeneralMirrorSite,
  "volc-intra",  "Volcengine Intranet", "火山引擎开源软件镜像站(内网)",
  "https://developer.volcengine.com/mirror/",
  "https://mirrors.ivolces.com/ubuntu-releases" Big_File_ubuntu, ROUGH},
*/


/**
 * @note 2025-06-20 网易开源镜像站的速度始终过低，不建议再使用
 */
Netease =
{
  IS_GeneralMirrorSite,
  "netease", "Netease", "网易开源镜像站", "https://mirrors.163.com/",
  {NotSkip, NA, NA, "https://mirrors.163.com/deepin-cd" Big_File_deepin, ROUGH}
},

/**
 * @note 2025-06-20 搜狐开源镜像站的速度始终过低，不建议再使用
 */
Sohu =
{
  IS_GeneralMirrorSite,
  "sohu", "SOHU", "搜狐开源镜像站", "https://mirrors.sohu.com/",
  {NotSkip, NA, NA, "https://mirrors.sohu.com/deepin-cd" Big_File_deepin, ROUGH}
};
