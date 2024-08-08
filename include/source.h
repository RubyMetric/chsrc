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


// @ccmywish: [2023-09-05] 我只使用了不到5次，重庆大学镜像站就把我的ip封杀了，对用户来说封杀策略过严，暂时不可靠，暂时不用
//
//  Cqu           = {"cqu",     "CQU",           "重庆大学开源软件镜像站",     "https://mirrors.cqu.edu.cn/",
//                  "https://mirrors.cqu.edu.cn/speedtest/1000mb.bin"};



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
              "https://mirrors.sohu.com/deepin-cd" Big_File_deepin},

  Api7     = {"api7",    "api7.ai",  "深圳支流科技有限公司", "https://www.apiseven.com/", NULL};


// 开源社区
MirrorSite
  RubyChina = {"rubychina",    "RubyChina",    "Ruby China 社区",    "https://gems.ruby-china.com/",
               "https://gems.ruby-china.com/rubygems/gems/nokogiri-1.15.0-java.gem"}, // 9.9 MB

  NpmMirror = {"npmmirror",    "npmmirror",    "npmmirror (阿里云赞助)",    "https://npmmirror.com/",
               // 注意，这个是跳转后的地址，不确定未来会不会改变
               "https://cdn.npmmirror.com/packages/%40tensorflow/tfjs/4.10.0/tfjs-4.10.0.tgz"}, // 29MB

  GoProxyCN = {"goproxy.cn",   "Goproxy.cn",   "Goproxy.cn (七牛云赞助)",   "https://goproxy.cn/",
               "https://goproxy.cn/github.com/aws/aws-sdk-go/@v/v1.45.2.zip"},  // 30 MB

  GoProxyIO = {"goproxy.io",   "GOPROXY.IO",   "GOPROXY.IO",         "https://goproxy.io/",
               "https://goproxy.io/github.com/aws/aws-sdk-go/@v/v1.45.2.zip"},   // 30 MB

  NugetOrg   = {"nuget.org",   "NuGet Org",    "Nuget Organization", "https://www.nuget.org/",              NULL},
  EmacsChina = {"emacschina",  "EmacsChina",   "Emacs China 社区",    "https://elpamirror.emacs-china.org/", NULL};



MirrorSite
  Upstream = {"upstream",     "Upstream",     "上游默认源",     NULL,     NULL};
MirrorSite
  UserDefine = {"user",       "用户自定义",     "用户自定义",     NULL,     NULL};




typedef struct {
  const MirrorSite *mirror;
  const char *url;
} SourceInfo;


/**
 * 2024-05-25 更新
 *
 * @note
 * 下面的源，并非都实现正确，
 * BFSU 和 Tuna 以及 阿里的镜像都有问题，会循环遍历一个gem的所有版本，导致安装时间相当长
 *
 * @note 网络情况
 * 若实现正确:
 * 1. 目前北外最快最稳定
 * 2. 腾讯挺快，个别时候不稳定，但稳定时能达到4.5MB甚至以上
 * 3. tuna 有时快有时慢，不稳定，一般在3MB以下
 * 4. rubychina 有时快有时慢，快时一般在 2MB以下，慢时一般在1MB以下
 * 5. 阿里云最不稳定，经常最慢
 */
static SourceInfo
pl_ruby_sources[] = {
  {&Upstream,  "https://rubygems.org"},
  {&Huawei,    "https://mirrors.huaweicloud.com/repository/rubygems/"},
  {&Tencent,   "https://mirrors.tencent.com/rubygems/"},
  {&RubyChina, "https://gems.ruby-china.com/"}

  // {&Bfsu,      "https://mirrors.bfsu.edu.cn/rubygems/"},
  // {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/rubygems/"},
  // {&Ali,       "https://mirrors.aliyun.com/rubygems/"},
},



/**
 * 2024-04-18 更新
 *
 * Sjtug, Tuna, Lzuoss, Jlu, Bfsu, 网易，搜狐 都没有
 *
 * @note 腾讯软件源虽然有npm的名，但名存实亡
 */
pl_nodejs_sources[] = {
  {&Upstream,       NULL},
  {&NpmMirror,     "https://registry.npmmirror.com"},
  {&Huawei,        "https://mirrors.huaweicloud.com/repository/npm/"},
  {&Zju,           "https://mirrors.zju.edu.cn/npm"}
},


/**
 * 2024-05-24 更新
 *
 * 参考: https://help.mirrors.cernet.edu.cn/CPAN/
 */
pl_perl_sources[] = {
  {&Upstream,       NULL},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/CPAN/"},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/CPAN/"},
  {&Bjtu,          "https://mirror.bjtu.edu.cn/cpan/"},
  {&Hust,          "https://mirrors.hust.edu.cn/CPAN/"},
  {&Ali,           "https://mirrors.aliyun.com/CPAN/"},
  {&Lzuoss,        "https://mirror.lzu.edu.cn/CPAN/"}
},



/**
 * 2024-04-18 更新
 *
 * @note 缺少教育网或开源社区软件源
 */
pl_php_sources[] = {
  {&Upstream,       NULL},
  {&Ali,           "https://mirrors.aliyun.com/composer/"},
  {&Tencent,       "https://mirrors.tencent.com/composer/"},
  {&Huawei,        "https://mirrors.huaweicloud.com/repository/php/"}
},



/**
 * 2023-09-27 更新
 *
 * @note 目前只有唯一一个源
 */
pl_lua_sources[] = {
  {&Upstream,       NULL},
  {&Api7,          "https://luarocks.cn"},
},


/**
 * 2024-04-18 更新
 *
 * @note 缺少教育网软件源
 */
pl_go_sources[] = {
  {&Upstream,       NULL},
  {&GoProxyCN,     "https://goproxy.cn"},
  {&Ali,           "https://mirrors.aliyun.com/goproxy/"},
  {&Huawei,        "https://mirrors.huaweicloud.com/goproxy/"},
  {&GoProxyIO,     "https://goproxy.io"}
},


/**
 * 2024-05-24 更新
 *
 * @note 缺少商业公司或开源社区软件源
 *
 * @note 以下都支持稀疏索引，我们换源时都将默认添加 `sparse+`
 */
pl_rust_sources[] = {
  {&Upstream,       NULL},
  {&Sjtug_Zhiyuan, "https://mirrors.sjtug.sjtu.edu.cn/crates.io-index/"},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/crates.io-index/"},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/crates.io-index/"},
  {&Ustc,          "https://mirrors.ustc.edu.cn/crates.io-index/"},
  {&Hust,          "https://mirrors.hust.edu.cn/crates.io-index/"}
},


/**
 * 2024-04-18 更新
 *
 * @note 缺少教育网或开源社区软件源
 */
pl_java_sources[] = {
  {&Upstream,       NULL},
  {&Ali,           "https://maven.aliyun.com/repository/public/"},
  {&Huawei,        "https://mirrors.huaweicloud.com/repository/maven/"},
  {&Netease,       "http://mirrors.163.com/maven/repository/maven-public/"} // 网易的24小时更新一次
},


/**
 * 2023-09-10 更新
 *
 * @note 不太好换，且用户可能不多，我们暂时只给用户提供文档
 */
pl_clojure_sources[] = {
  {&Upstream,       NULL},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/help/clojars/"},
  {&Ustc,          "https://mirrors.ustc.edu.cn/help/clojars.html"}
},


/**
 * 2024-04-18 更新
 *
 * @note 暂时未实现该换源功能，可参照
 *    https://mirrors.huaweicloud.com/mirrorDetail/5ebf85de07b41baf6d0882ab?mirrorName=nuget&catalog=language
 */
pl_dotnet_sources[] = {
  {&Upstream,       NULL},
  {&NugetOrg,      "https://www.nuget.org/api/v2/"},
  {&Huawei,        "https://mirrors.huaweicloud.com/repository/nuget/v3"}
},



/**
 * 2023-09-10 更新
 *
 * @note 我们这里挑选的必须也支持 Flutter
 *
 * 我们将会在setsrc函数中生成
 * 1. https://mirrors.tuna.tsinghua.edu.cn/dart-pub
 * 2. https://mirrors.tuna.tsinghua.edu.cn/flutter
 */
pl_dart_sources[] = {
  {&Upstream,       NULL},
  // {&Sjtug_Zhiyuan, "https://mirror.sjtu.edu.cn/"}, // 不确定SJTUG的flutter镜像给的对不对
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/"},
  {&Tencent,       "https://mirrors.cloud.tencent.com/"},
  {&Nju,           "https://mirror.nju.edu.cn/"}
},



/**
 * 2023-09-10 更新
 */
pl_haskell_sources[] = {
  {&Upstream,       NULL},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/hackage"},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/hackage"},
  {&Nju,           "https://mirror.nju.edu.cn/hackage"},
  {&Ustc,          "https://mirrors.ustc.edu.cn/hackage"}
},


/**
 * 2023-09-15 更新
 */
pl_ocaml_sources[] = {
  {&Upstream,       NULL},
  {&Sjtug_Zhiyuan, "https://mirrors.sjtug.sjtu.edu.cn/git/opam-repository.git"}
},




/**
 * 2023-09-04 更新
 *
 * @note 以下注释的，是不含有bioconductor的镜像站，
 *       我们在换cran的同时，也直接帮助用户换bioconductor
 */
pl_r_sources[] = {
  {&Upstream,       NULL},
  {&Sjtug_Zhiyuan, "https://mirrors.sjtug.sjtu.edu.cn/cran/"},
  // {&Ali,           "https://mirrors.aliyun.com/CRAN/"},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/CRAN/"},
  // {&Sustech,       "https://mirrors.sustech.edu.cn/CRAN"},
  // {&Bfsu,          "https://mirrors.bfsu.edu.cn/CRAN/"},
  // {&Bjtu,          "https://mirror.bjtu.edu.cn/cran/"},
},


/**
 * 2023-09-05 更新
 *
 * @note 缺少商业公司或开源社区软件源
 */
pl_julia_sources[] = {
  {&Upstream,       NULL},
  {&Sjtug_Zhiyuan, "https://mirrors.sjtug.sjtu.edu.cn/julia"},
  {&Pku,           "https://mirrors.pku.edu.cn/julia"},
  {&Nju,           "https://mirror.nju.edu.cn/julia"}
};





/**
 * 2024-06-12 更新
 */
static SourceInfo
os_ubuntu_sources[] = {
  {&Upstream,       NULL},
  {&Ali,           "https://mirrors.aliyun.com/ubuntu"},
  {&Volcengine,    "https://mirrors.volces.com/ubuntu"},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/ubuntu"},
  {&Ustc,          "https://mirrors.ustc.edu.cn/ubuntu"},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/ubuntu"},
  {&Tencent,       "https://mirrors.tencent.com/ubuntu"},
  {&Huawei,        "https://mirrors.huaweicloud.com/ubuntu"},
  {&Netease,       "https://mirrors.163.com/ubuntu"},
  {&Sohu,          "https://mirrors.sohu.com/ubuntu"}
},


/**
 * 2023-09-29 更新
 *
 * @note: 实际上镜像站里的内容和Ubuntu的不太一样
 */
os_mint_sources[] = {
  {&Upstream,       NULL},
  {&MirrorZ,       "https://mirrors.cernet.edu.cn/linuxmint/"},
  {&Ali,           "http://mirrors.aliyun.com/linuxmint-packages/"},
  {&Netease,       "https://mirrors.163.com/linuxmint/packages/"},
},


/**
 * 2023-09-06 更新
 *
 * TODO: 1. 源并不完整，且未经测试是否有效
 */
os_deepin_sources[] = {
  {&Upstream,       NULL},
  {&Ali,           "https://mirrors.aliyun.com/deepin"},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/deepin"},
  {&Ustc,          "https://mirrors.ustc.edu.cn/deepin"},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/deepin"},
  {&Tencent,       "https://mirrors.tencent.com/deepin"},
  {&Netease,       "https://mirrors.163.com/deepin"},
  {&Sohu,          "https://mirrors.sohu.com/deepin"}
},


/**
 * 2024-06-12 更新
 */
os_debian_sources[] = {
  {&Upstream,       NULL},
  {&Ali,           "https://mirrors.aliyun.com/debian"},
  {&Volcengine,    "https://mirrors.volces.com/debian"},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/debian"},
  {&Ustc,          "https://mirrors.ustc.edu.cn/debian"},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/debian"},
  {&Tencent,       "https://mirrors.tencent.com/debian"},
  {&Netease,       "https://mirrors.163.com/debian"},
  {&Sohu,          "https://mirrors.sohu.com/debian"}
},


/**
 * 2023-09-02 更新
 *
 * TODO: 1. 源并不完整，且未经测试是否有效
 */
os_fedora_sources[] = {
  {&Upstream,       NULL},
  {&Ali,           "https://mirrors.aliyun.com/fedora"},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/fedora"},
  {&Ustc,          "https://mirrors.ustc.edu.cn/fedora"},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/fedora"},
  {&Tencent,       "https://mirrors.tencent.com/fedora"},
  {&Netease,       "https://mirrors.163.com/fedora"},
  {&Sohu,          "https://mirrors.sohu.com/fedora"}
},


/**
 * 2024-06-12 更新
 */
os_kali_sources[] = {
  {&Upstream,       NULL},
  {&Ali,           "https://mirrors.aliyun.com/kali"},
  {&Volcengine,    "https://mirrors.volces.com/kali"},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/kali"},
  {&Ustc,          "https://mirrors.ustc.edu.cn/kali"},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/kali"},
  {&Tencent,       "https://mirrors.tencent.com/kali"},
  {&Huawei,        "https://mirrors.huaweicloud.com/kali"},
  {&Netease,       "https://mirrors.163.com/kali"},
  {&Sohu,          "https://mirrors.sohu.com/kali"}
},


/**
 * 2024-07-03 更新
 * @note 不要给后面加 / ，因为ARM情况下，还要额外加一个 arm 后缀
 */
os_arch_sources[] = {
  {&Upstream,       NULL},
  {&Ali,           "https://mirrors.aliyun.com/archlinux"},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/archlinux"},
  {&Ustc,          "https://mirrors.ustc.edu.cn/archlinux"},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/archlinux"},
  {&Tencent,       "https://mirrors.tencent.com/archlinux"},
  {&Huawei,        "https://mirrors.huaweicloud.com/archlinux"}, // 不支持 archlinuxcn
  {&Netease,       "https://mirrors.163.com/archlinux"},         // archlinuxcn 的URL和其他镜像站不同
  // {&Sohu,          "https://mirrors.sohu.com/archlinux"}       // 不支持 archlinuxcn
},


/**
 * 2024-07-03 更新
 *
 * @note 根据 GitHub:@zheng7fu2 建议，拆分 archlinuxcn 出来
 */
os_archlinuxcn_sources[] = {
  {&Upstream,      "https://repo.archlinuxcn.org/"},
  {&Ali,           "https://mirrors.aliyun.com/archlinuxcn/"},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/archlinuxcn/"},
  {&Ustc,          "https://mirrors.ustc.edu.cn/archlinuxcn/"},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/archlinuxcn/"},
  {&Tencent,       "https://mirrors.cloud.tencent.com/archlinuxcn/"},
  {&Netease,       "https://mirrors.163.com/archlinux-cn/"}
},


/**
 * 2023-09-05 更新
 *
 * TODO: 1. 源并不完整，且未经测试是否有效
 */
os_gentoo_sources[] = {
  {&Upstream,       NULL},
  {&Ali,           "mirrors.aliyun.com"},
  {&Bfsu,          "mirrors.bfsu.edu.cn"},
  {&Ustc,          "mirrors.ustc.edu.cn"},
  {&Tuna,          "mirrors.tuna.tsinghua.edu.cn"},
  {&Tencent,       "mirrors.tencent.com"},
  {&Netease,       "mirrors.163.com"},
  {&Sohu,          "mirrors.sohu.com"}
},


/**
 * 2024-06-12 更新
 */
os_opensuse_sources[] = {
  {&Upstream,       NULL},
  {&Ali,            "https://mirrors.aliyun.com/opensuse"},
  {&Volcengine,     "https://mirrors.volces.com/opensuse"},
  {&Bfsu,           "https://mirrors.bfsu.edu.cn/opensuse"},
  {&Ustc,           "https://mirrors.ustc.edu.cn/opensuse"},
  {&Tuna,           "https://mirrors.tuna.tsinghua.edu.cn/opensuse"},
  {&Tencent,        "https://mirrors.tencent.com/opensuse"},
  {&Netease,        "https://mirrors.163.com/opensuse"},
  {&Sohu,           "https://mirrors.sohu.com/opensuse"}
},


/**
 * 2024-04-18 更新
 *
 * TODO: 源并不完整，且未经测试是否有效
 */
os_msys2_sources[] = {
  {&Upstream,       NULL},
  {&Ali,            "https://mirrors.aliyun.com/msys2"},
  {&Bfsu,           "https://mirrors.bfsu.edu.cn/msys2"},
  {&Ustc,           "https://mirrors.ustc.edu.cn/msys2"},
  {&Tuna,           "https://mirrors.tuna.tsinghua.edu.cn/msys2"},
  {&Tencent,        "https://mirrors.tencent.com/msys2"},
  {&Huawei,         "https://mirrors.huaweicloud.com/msys2"},
  {&Netease,        "https://mirrors.163.com/msys2"},
  {&Sohu,           "https://mirrors.sohu.com/msys2"}
},


/**
 * 2024-06-12 更新
 */
os_rocky_sources[] = {
  {&Upstream,        NULL},
  {&Ali,            "https://mirrors.aliyun.com/rockylinux"},
  {&Volcengine,     "https://mirrors.volces.com/rockylinux"},
  {&Sjtug_Zhiyuan,  "https://mirror.sjtu.edu.cn/rocky"},
  {&Sustech,        "https://mirrors.sustech.edu.cn/rocky-linux"},
  {&Zju,            "https://mirrors.zju.edu.cn/rocky"},
  {&Lzuoss,         "https://mirror.lzu.edu.cn/rocky"},
  {&Sohu,           "https://mirrors.sohu.com/Rocky"},
  {&Netease,        "https://mirrors.163.com/rocky"}
},


/**
 * 2024-06-12 更新
 */
os_alma_sources[] = {
  {&Upstream,       NULL},
  {&Ali,           "https://mirrors.aliyun.com/almalinux"},
  {&Volcengine,    "https://mirrors.volces.com/almalinux"},
  {&Sjtug_Zhiyuan, "https://mirrors.sjtug.sjtu.edu.cn/almalinux"},
  {&Zju,           "https://mirrors.zju.edu.cn/almalinux"},
  {&Nju,           "https://mirror.nju.edu.cn/almalinux"},

},


/**
 * 2024-04-18 更新
 */
os_alpine_sources[] = {
  {&Upstream,       NULL},
  {&Tuna,           "https://mirrors.tuna.tsinghua.edu.cn/alpine"},
  {&Sjtug_Zhiyuan,  "https://mirrors.sjtug.sjtu.edu.cn/alpine"},
  {&Sustech,        "https://mirrors.sustech.edu.cn/alpine"},
  {&Zju,            "https://mirrors.zju.edu.cn/alpine"},
  {&Lzuoss,         "https://mirror.lzu.edu.cn/alpine"},
  {&Ali,            "https://mirrors.aliyun.com/alpine"},
  {&Tencent,        "https://mirrors.cloud.tencent.com/alpine"},
  {&Huawei,         "https://mirrors.huaweicloud.com/alpine"}
},


/**
 * 2023-09-24 更新
 */
os_void_sources[] = {
  {&Upstream,       NULL},
  {&Tuna,           "https://mirrors.tuna.tsinghua.edu.cn/voidlinux"},
  {&Sjtug_Zhiyuan,  "https://mirror.sjtu.edu.cn/voidlinux"},
  {&Bfsu,           "https://mirrors.bfsu.edu.cn/voidlinux"}
},


/**
 * 2023-09-29 更新
 */
os_solus_sources[] = {
  {&Upstream,       NULL},
  {&Tuna,           "https://mirrors.tuna.tsinghua.edu.cn/solus/packages/shannon/eopkg-index.xml.xz"},
  {&Bfsu,           "https://mirrors.bfsu.edu.cn/solus/packages/shannon/eopkg-index.xml.xz"},
  {&Nju,            "https://mirror.nju.edu.cn/solus/packages/shannon/eopkg-index.xml.xz"}
},


/**
 * 2023-09-29 更新
 */
os_linuxlite_sources[] = {
  {&Upstream,       NULL},
  {&Sjtug_Zhiyuan,  "https://mirrors.sjtug.sjtu.edu.cn/linuxliteos/"}
},


/**
 * 2023-09-29 更新
 */
os_trisquel_sources[] = {
  {&Upstream,       NULL},
  {&Ali,            "https://mirrors.aliyun.com/trisquel/"},
  {&MirrorZ,        "https://mirrors.cernet.edu.cn/trisquel/"},
  {&Nju,            "https://mirror.nju.edu.cn/trisquel/"},
  {&Ustc,           "https://mirrors.ustc.edu.cn/trisquel/"}
},


/**
 * 2023-09-29 更新
 */
os_raspberrypi_sources[] = {
  {&Upstream,       NULL},
  {&MirrorZ,        "https://help.mirrors.cernet.edu.cn/raspberrypi/"},
  {&Tuna,           "https://mirrors.tuna.tsinghua.edu.cn/raspberrypi/"},
  {&Bfsu,           "https://mirrors.bfsu.edu.cn/raspberrypi/"},
  {&Ustc,           "https://mirrors.ustc.edu.cn/raspberrypi/"},
  {&Sjtug_Zhiyuan,  "https://mirrors.sjtug.sjtu.edu.cn/raspberrypi/"},
  {&Sustech,        "https://mirrors.sustech.edu.cn/raspberrypi/"}
},


/**
 * 2024-06-20 更新
 */
os_armbian_sources[] = {
    {&Upstream,       NULL},
    {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/armbian"},
    {&Sjtug_Zhiyuan, "https://mirror.sjtu.edu.cn/armbian"},
    {&Bfsu,          "https://mirrors.bfsu.edu.cn/armbian"},
    {&Sustech,       "https://mirrors.sustech.edu.cn/armbian"},
    {&Ustc,          "https://mirrors.ustc.edu.cn/armbian"},
    {&Nju,           "https://mirrors.nju.edu.cn/armbian"},
    {&Ali,           "https://mirrors.aliyun.com/armbian"},
},


/**
 * 2023-09-27 更新
 *
 * @note 该源需要 FreeBSD 中文社区积极参与维护
 *
 * @ccmywish: [2023-09-24] 以下三个USTC, NJU, Netease 均维护了 freebsd-pkg freebsd-ports
 * @ccmywish: [2023-09-27] 请务必保持Nju前面有至少一个镜像，原因请查看 freebsd 的换源函数
 */
os_freebsd_sources[] = {
  {&Upstream,       NULL},
  {&Ustc,      "mirrors.ustc.edu.cn"},
  {&Nju,       "mirror.nju.edu.cn"},
  {&Netease,   "mirrors.163.com"},
},


/**
 * 2023-09-05 更新
 *
 * TODO: 1. 源并不完整，且未经测试是否有效
 */
os_netbsd_sources[] = {
  {&Upstream,       NULL},
  {&Ali,       "https://mirrors.aliyun.com/pkgsrc/packages/NetBSD/"},
  {&Bfsu,      "https://mirrors.bfsu.edu.cn/pkgsrc/packages/NetBSD/"},
  {&Ustc,      "https://mirrors.ustc.edu.cn/pkgsrc/packages/NetBSD/"},
  {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/pkgsrc/packages/NetBSD/"},
  {&Tencent,   "https://mirrors.tencent.com/pkgsrc/packages/NetBSD/"},
  {&Netease,   "https://mirrors.163.com/pkgsrc/packages/NetBSD/"},
  {&Sohu,      "https://mirrors.sohu.com/pkgsrc/packages/NetBSD/"}
},


/**
 * 2023-09-02 更新
 *
 * TODO: 1. 源并不完整，且未经测试是否有效
 */
os_openbsd_sources[] = {
  {&Upstream,       NULL},
  {&Ali,       "https://mirrors.aliyun.com/OpenBSD/"},
  {&Bfsu,      "https://mirrors.bfsu.edu.cn/OpenBSD/"},
  {&Ustc,      "https://mirrors.ustc.edu.cn/OpenBSD/"},
  {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/OpenBSD/"},
  {&Tencent,   "https://mirrors.tencent.com/OpenBSD/"},
  {&Netease,   "https://mirrors.163.com/OpenBSD/"},
  {&Sohu,      "https://mirrors.sohu.com/OpenBSD/"}
},


/**
 * 2023-09-06 更新
 *
 * TODO: 1. 源并不完整，且未经测试是否有效
 */
os_openeuler_sources[] = {
  {&Upstream,       NULL},
  {&Ali,            "https://mirrors.aliyun.com/openeuler/"},
  {&Bfsu,           "https://mirrors.bfsu.edu.cn/openeuler/"},
  {&Ustc,           "https://mirrors.ustc.edu.cn/openeuler/"},
  {&Tuna,           "https://mirrors.tuna.tsinghua.edu.cn/openeuler/"},
  {&Tencent,        "https://mirrors.tencent.com/openeuler/"},
  {&Netease,        "https://mirrors.163.com/openeuler/"},
  {&Sohu,           "https://mirrors.sohu.com/openeuler/"}
},


/**
 * 2024-06-12 更新
 */
os_anolis_sources[] = {
  {&Upstream,       NULL},
  {&Ali,            "https://mirrors.aliyun.com/anolis"},
  {&Hust,           "https://mirrors.hust.edu.cn/anolis"}
},


/**
 * 2023-09-29 更新
 */
os_openkylin_sources[] = {
  {&Upstream,  "https://archive.openkylin.top/openkylin/"},
  {&Ali,       "https://mirrors.aliyun.com/openkylin/"},
  {&Netease,   "https://mirrors.163.com/openkylin/"},
},


/**
 * 2024-04-18 更新
 *
 * TODO: 未经测试是否有效
 */
os_ros_sources[] = {
  {&Upstream,       NULL},
  {&Ali,       "https://mirrors.aliyun.com"},
  {&Bfsu,      "https://mirrors.bfsu.edu.cn"},
  {&Ustc,      "https://mirrors.ustc.edu.cn"},
  {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn"},
  {&Tencent,   "https://mirrors.tencent.com"},
  {&Huawei,    "https://mirrors.huaweicloud.com"},
  {&Netease,   "https://mirrors.163.com"},
  {&Sohu,      "https://mirrors.sohu.com"}
};



/**
 * 2024-06-07 更新
 *
 * @note: 目前仅有一个源
 */
static SourceInfo
wr_winget_sources[] = {
  {&Upstream,       "https://cdn.winget.microsoft.com/cache"},
  {&Ustc,           "https://mirrors.ustc.edu.cn/winget-source"},
},


/**
 * 2023-09-10 更新
 *
 * @note 1. 这些链接将会在setsrc函数中补充完整
 *       2. 不确定 Sustech 能否工作
 */
wr_brew_sources[] = {
  {&Upstream,       NULL},
  {&Tuna,           "https://mirrors.tuna.tsinghua.edu.cn/"},
  {&Bfsu,           "https://mirrors.bfsu.edu.cn/"},
  {&Zju,            "https://mirrors.zju.edu.cn/"},
  {&Sustech,        "https://mirrors.sustech.edu.cn/"}
},


/**
 * 2024-06-08 更新
 */
wr_cocoapods_sources[] = {
  {&Upstream,       NULL},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/git/CocoaPods/Specs.git"},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/git/CocoaPods/Specs.git"}
},



/**
 * 2023-09-11 更新
 *
 * @note 目前只有一个源
 */
wr_flathub_sources[] = {
  {&Upstream,       NULL},
  {&Sjtug_Zhiyuan, "https://mirror.sjtu.edu.cn/flathub"},
},



/**
 * 2023-09-22 更新
 *
 * @note 1. 目前只有一个源
 *       2. 这些链接将会在setsrc函数中补充完整
 */
wr_nix_sources[] = {
  {&Upstream,       NULL},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/nix-channels/"}
},



/**
 * 2023-09-11 更新
 *
 * @note 目前只有一个源
 */
wr_guix_sources[] = {
  {&Upstream,       NULL},
  {&Sjtug_Zhiyuan, "https://mirror.sjtu.edu.cn/git/guix.git"}
},



/**
 * 2023-09-10 更新
 *
 * @note Emacs用户往往只需要一次性换源，只会极少次调用 chsrc，我们只给用户提供文档
 */
wr_emacs_sources[] = {
  {&Upstream,       NULL},
  {&Sjtug_Zhiyuan,  "https://mirrors.sjtug.sjtu.edu.cn/docs/emacs-elpa"},
  {&Tuna,           "https://mirrors.tuna.tsinghua.edu.cn/help/elpa/"},
  {&Bfsu,           "https://mirrors.bfsu.edu.cn/help/elpa/"},
  {&Ustc,           "https://mirrors.ustc.edu.cn/help/elpa.html"},
  {&Zju,            "https://mirrors.zju.edu.cn/docs/elpa/"},
  {&EmacsChina,     "https://elpamirror.emacs-china.org/"}
},


/**
 * 2023-09-10 更新
 *
 * @note 这些链接将会在setsrc函数中补充完整
 */
wr_anaconda_sources[] = {
  {&Upstream,       NULL},
  {&Tuna,            "https://mirrors.tuna.tsinghua.edu.cn/anaconda/"},
  {&Bfsu,            "https://mirrors.bfsu.edu.cn/anaconda/"},
  {&Zju,             "https://mirrors.zju.edu.cn/anaconda/"},
  {&Sjtug_Zhiyuan,   "https://mirror.sjtu.edu.cn/anaconda"}
},



/**
 * 2023-09-10 更新
 *
 * TODO: 1. 暂未添加商业公司源
 */
wr_tex_sources[] = {
  {&Upstream,       NULL},
  {&Sjtug_Zhiyuan, "https://mirrors.sjtug.sjtu.edu.cn/ctan/systems/texlive/tlnet"},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/CTAN/systems/texlive/tlnet"},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/CTAN/systems/texlive/tlnet"},
  {&Lzuoss,        "https://mirror.lzu.edu.cn/CTAN/systems/texlive/tlnet"},
  {&Jlu,           "https://mirrors.jlu.edu.cn/CTAN/systems/texlive/tlnet"},
  {&Sustech,       "https://mirrors.sustech.edu.cn/CTAN/systems/texlive/tlnet"}
};



#define def_sources_n(t) const size_t t##_sources_n = xy_arylen(t##_sources)

def_sources_n(pl_ruby);     def_sources_n(pl_nodejs);
def_sources_n(pl_perl);     def_sources_n(pl_php);       def_sources_n(pl_lua);
def_sources_n(pl_go);       def_sources_n(pl_rust);
def_sources_n(pl_java);     def_sources_n(pl_clojure);
def_sources_n(pl_dotnet);   def_sources_n(pl_dart);      def_sources_n(pl_haskell);
def_sources_n(pl_ocaml);
def_sources_n(pl_r);        def_sources_n(pl_julia);

def_sources_n(os_ubuntu);   def_sources_n(os_mint);
def_sources_n(os_debian);   def_sources_n(os_fedora);
def_sources_n(os_kali);     def_sources_n(os_opensuse);
def_sources_n(os_arch);     def_sources_n(os_archlinuxcn);
def_sources_n(os_msys2);    def_sources_n(os_gentoo);
def_sources_n(os_alpine);   def_sources_n(os_rocky);     def_sources_n(os_alma);
def_sources_n(os_void);
def_sources_n(os_solus);

def_sources_n(os_trisquel); def_sources_n(os_linuxlite); def_sources_n(os_raspberrypi);
def_sources_n(os_armbian);

def_sources_n(os_freebsd);  def_sources_n(os_netbsd);    def_sources_n(os_openbsd);

def_sources_n(os_ros);

def_sources_n(os_deepin);   def_sources_n(os_openeuler); def_sources_n(os_openkylin);
def_sources_n(os_anolis);

def_sources_n(wr_winget);
def_sources_n(wr_brew);     def_sources_n(wr_cocoapods);

def_sources_n(wr_flathub);
def_sources_n(wr_nix);      def_sources_n(wr_guix);
def_sources_n(wr_tex);      def_sources_n(wr_emacs);
def_sources_n(wr_anaconda);
