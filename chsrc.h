/** ------------------------------------------------------------
 * File          : chsrc.h
 * License       : GPLv3
 * Authors       : Aoran Zeng <ccmywish@qq.com>
 * Created on    : <2023-08-29>
 * Last modified : <2023-09-10>
 *
 * chsrc:
 *
 *   chsrc.c 头文件
 * ------------------------------------------------------------*/

#include "xy.h"

typedef struct {
  const char* code; // 用于用户指定镜像站
  const char* abbr;
  const char* name;
  const char* site;
  const char* __bigfile_url;  // 用于对该镜像站测速
} mirror_info;

// #define Big_File_ubuntu  "/indices/md5sums.gz"  这个是错的
#define Big_File_ctan       "/systems/texlive/Images/texlive.iso"           // 4.8GB
#define Big_File_debian     "/ls-lR.gz"                                     // 13.9 MB
#define Big_File_archlinux  "/iso/latest/archlinux-x86_64.iso"              // 800MB 左右
#define Big_File_deepin     "/20.9/deepin-desktop-community-20.9-amd64.iso" // 4GB左右


// 教育网
// 我们目前根据 https://github.com/mirrorz-org/oh-my-mirrorz 挑选速度前10位
//
mirror_info
  MirrorZ       = {"mirrorz", "MirrorZ",       "MirrorZ校园网镜像站",       "https://mirrors.cernet.edu.cn/",  NULL},

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

// 速度暂时处于10位以后，但是目前可用的源
  Nju           = {"nju",     "NJU",           "南京大学开源镜像站",         "https://mirrors.nju.edu.cn/",
                   "https://mirrors.nju.edu.cn/archlinux" Big_File_archlinux};


// @ccmywish: 2023-09-05 我只使用了不到5次，重庆大学镜像站就把我的ip封杀了，对用户来说封杀策略过严，暂时不可靠，暂时不用
//
//  Cqu           = {"cqu",     "CQU",           "重庆大学开源软件镜像站",     "https://mirrors.cqu.edu.cn/",
//                  "https://mirrors.cqu.edu.cn/speedtest/1000mb.bin"};


// 大型公司
// 注意，腾讯软件源中，有很多链接都已失效，请仔细检查
mirror_info
  Ali      = {"ali",     "Ali OPSX", "阿里巴巴开源镜像站",  "https://developer.aliyun.com/mirror/",
              "https://mirrors.aliyun.com/deepin-cd" Big_File_deepin},

  Tencent  = {"tencent", "Tencent",  "腾讯软件源",         "https://mirrors.tencent.com/",
              "https://mirrors.cloud.tencent.com/debian" Big_File_debian},

  Netease  = {"netease", "Netease",  "网易开源镜像站",      "https://mirrors.163.com/",
              "https://mirrors.163.com/deepin-cd" Big_File_deepin},

  Sohu     = {"sohu",    "SOHU",     "搜狐开源镜像站",      "https://mirrors.sohu.com/",
              "https://mirrors.sohu.com/deepin-cd" Big_File_deepin};


// 开源社区
mirror_info
  RubyChina = {"rubychina",    "RubyChina",    "Ruby China 社区",    "https://gems.ruby-china.com/",
               "https://gems.ruby-china.com/rubygems/gems/nokogiri-1.15.0-java.gem"}, // 9.9 MB

  NpmMirror = {"npmmirror",    "npmmirror",    "npmmirror (阿里云赞助)",    "https://npmmirror.com/",
               // 注意，这个是跳转后的地址，不确定未来会不会改变
               "https://cdn.npmmirror.com/packages/%40tensorflow/tfjs/4.10.0/tfjs-4.10.0.tgz"}, // 29MB

  GoProxyCN = {"goproxy.cn",   "Goproxy.cn",   "Goproxy.cn (七牛云赞助)",   "https://goproxy.cn/",
               "https://goproxy.cn/github.com/aws/aws-sdk-go/@v/v1.45.2.zip"},  // 30 MB

  GoProxyIO = {"goproxy.io",   "GOPROXY.IO",   "GOPROXY.IO",         "https://goproxy.io/",
               "https://goproxy.io/github.com/aws/aws-sdk-go/@v/v1.45.2.zip"},   // 30 MB

  NugetOrg  = {"nuget.org",    "NuGet Org",    "Nuget Organization", "https://www.nuget.org/",       NULL};


mirror_info*
available_mirrors[] = {
  &MirrorZ, &Tuna, &Sjtug_Zhiyuan, &Zju, &Lzuoss, &Jlu, &Bfsu, &Pku, &Bjtu, &Sustech, &Ustc, &Nju, // &Cqu,
  &Ali,  &Tencent, &Netease, &Sohu,
  &RubyChina, &NpmMirror, &GoProxyCN, &GoProxyIO
  // 暂不支持 &NugetOrg
};


typedef struct {
  const mirror_info* mirror;
  const char* url;
} source_info;


/**
 * 源信息
 *
 * @note 我们要求每个源至少有一个教育网镜像，至少有一个商业公司或开源社区维护的镜像
 */


/**
 * 2023-09-02 更新
 *
 * @note 目前北外最快，Ruby China 的源慢了一半
 */
static source_info
pl_ruby_sources[] = {
  {&Bfsu,      "https://mirrors.bfsu.edu.cn/rubygems/"},
  {&Ali,       "https://mirrors.aliyun.com/rubygems/"},
  {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/rubygems/"},
  {&Tencent,   "https://mirrors.tencent.com/rubygems/"},
  // Ruby China 给的链接是不带后缀 / 的，不知道是否带后缀会影响
  // 如有问题请报告
  {&RubyChina, "https://gems.ruby-china.com/"},
},


/**
 * 2023-09-10 更新
 *
 * @note 不要添加Zju，浙大的pypi在校外访问会自动转向Tuna
 */
pl_python_sources[] = {
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/pypi/web/simple"},
  {&Lzuoss,        "https://mirror.lzu.edu.cn/pypi/web/simple"},
  {&Jlu,           "https://mirrors.jlu.edu.cn/pypi/web/simple"},
  {&Sjtug_Zhiyuan, "https://mirror.sjtu.edu.cn/pypi/web/simple"},
  {&Tuna,          "https://pypi.tuna.tsinghua.edu.cn/simple"},
  {&Ali,           "https://mirrors.aliyun.com/pypi/simple/"},
  {&Tencent,       "https://mirrors.cloud.tencent.com/pypi/simple"}
  // {&Netease,    "https://mirrors.163.com/.help/pypi.html"} // 不用，24小时更新一次
},



/**
 * 2023-09-09 更新
 *
 * Sjtug, Tuna, Lzuoss, Jlu, Bfsu, 网易，搜狐 都没有
 *
 * @note 腾讯软件源虽然有npm的名，但名存实亡
 */
pl_nodejs_sources[] = {
  {&NpmMirror,     "https://registry.npmmirror.com"},
  {&Zju,           "https://mirrors.zju.edu.cn/npm"}
},


/**
 * 2023-09-05 更新
 *
 * 参考: https://help.mirrors.cernet.edu.cn/CPAN/
 */
pl_perl_sources[] = {
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/CPAN/"},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/CPAN/"},
  {&Bjtu,          "https://mirror.bjtu.edu.cn/cpan/"},
  {&Ali,           "https://mirrors.aliyun.com/CPAN/"},
  {&Lzuoss,        "https://mirror.lzu.edu.cn/CPAN/"}
},


/**
 * 2023-09-04 更新
 *
 * @note 以下注释的，是不含有bioconductor的镜像站，
 *       我们在换cran的同时，也直接帮助用户换bioconductor
 */
pl_r_sources[] = {
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
  {&Sjtug_Zhiyuan, "https://mirrors.sjtug.sjtu.edu.cn/julia"},
  {&Pku,           "https://mirrors.pku.edu.cn/julia"},
  {&Nju,           "https://mirror.nju.edu.cn/julia"}
},


/**
 * 2023-09-04 更新
 *
 * @note 缺少教育网软件源
 */
pl_go_sources[] = {
  {&GoProxyCN,    "https://goproxy.cn"},
  {&Ali,          "https://mirrors.aliyun.com/goproxy/"},
  {&GoProxyIO,    "https://goproxy.io"}
},


/**
 * 2023-09-05 更新
 *
 * @note 缺少商业公司或开源社区软件源
 *
 * @note 以下都支持稀疏索引，我们换源时都将默认添加 `sparse+`
 */
pl_rust_sources[] = {
  {&Sjtug_Zhiyuan, "https://mirrors.sjtug.sjtu.edu.cn/crates.io-index/"},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/crates.io-index/"},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/crates.io-index/"},
  {&Ustc,          "https://mirrors.ustc.edu.cn/crates.io-index/"},
},


/**
 * 2023-09-10 更新
 *
 * @note 缺少教育网或开源社区软件源
 */
pl_java_sources[] = {
  {&Ali,           "https://maven.aliyun.com/repository/public/"},
  {&Netease,       "http://mirrors.163.com/maven/repository/maven-public/"} // 网易的24小时更新一次
},


/**
 * 2023-08-30 更新
 *
 * @note 暂时未实现该换源功能
 *
 * @note 当前仅有一个源，缺少教育网或开源社区软件源
 */
pl_dotnet_sources[] = {
  {&NugetOrg,      "https://www.nuget.org/api/v2/"}
},



/**
 * 2023-09-10 更新
 */
pl_dart_sources[] = {
  {&Sjtug_Zhiyuan, "https://mirror.sjtu.edu.cn/dart-pub"},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/dart-pub"},
  {&Tencent,       "https://mirrors.cloud.tencent.com/dart-pub/"},
  {&Nju,           "https://mirror.nju.edu.cn/dart-pub"},
},



/**
 * 2023-09-04 更新
 *
 * @note 缺少教育网或开源社区软件源
 */
pl_php_sources[] = {
  {&Ali,            "https://mirrors.aliyun.com/composer/"},
  {&Tencent,        "https://mirrors.tencent.com/composer/"}
};



/**
 * 2023-09-01 更新
 *
 * TODO: 1. 源并不完整，且未经测试是否有效
 */
static source_info
os_ubuntu_sources[] = {
  {&Ali,       "https://mirrors.aliyun.com/ubuntu"},
  {&Bfsu,      "https://mirrors.bfsu.edu.cn/ubuntu"},
  {&Ustc,      "https://mirrors.ustc.edu.cn/ubuntu"},
  {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/ubuntu"},
  {&Tencent,   "https://mirrors.tencent.com/ubuntu"},
  {&Netease,   "https://mirrors.163.com/ubuntu"},
  {&Sohu,      "https://mirrors.sohu.com/ubuntu"}
},


/**
 * 2023-09-06 更新
 *
 * TODO: 1. 源并不完整，且未经测试是否有效
 */
os_deepin_sources[] = {
  {&Ali,       "https://mirrors.aliyun.com/deepin/"},
  {&Bfsu,      "https://mirrors.bfsu.edu.cn/deepin/"},
  {&Ustc,      "https://mirrors.ustc.edu.cn/deepin/"},
  {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/deepin/"},
  {&Tencent,   "https://mirrors.tencent.com/deepin/"},
  {&Netease,   "https://mirrors.163.com/deepin/"},
  {&Sohu,      "https://mirrors.sohu.com/deepin/"}
},


/**
 * 2023-09-01 更新
 *
 * TODO: 1. 源并不完整，且未经测试是否有效
 */
os_debian_sources[] = {
  {&Ali,       "https://mirrors.aliyun.com/debian/"},
  {&Bfsu,      "https://mirrors.bfsu.edu.cn/debian/"},
  {&Ustc,      "https://mirrors.ustc.edu.cn/debian/"},
  {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/debian/"},
  {&Tencent,   "https://mirrors.tencent.com/debian/"},
  {&Netease,   "https://mirrors.163.com/debian/"},
  {&Sohu,      "https://mirrors.sohu.com/debian/"}
},



/**
 * 2023-09-02 更新
 *
 * TODO: 1. 源并不完整，且未经测试是否有效
 */
os_fedora_sources[] = {
  {&Ali,       "https://mirrors.aliyun.com/fedora/"},
  {&Bfsu,      "https://mirrors.bfsu.edu.cn/fedora/"},
  {&Ustc,      "https://mirrors.ustc.edu.cn/fedora/"},
  {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/fedora/"},
  {&Tencent,   "https://mirrors.tencent.com/fedora/"},
  {&Netease,   "https://mirrors.163.com/fedora/"},
  {&Sohu,      "https://mirrors.sohu.com/fedora/"}
},


/**
 * 2023-09-02 更新
 *
 * TODO: 1. 源并不完整，且未经测试是否有效
 */
os_kali_sources[] = {
  {&Ali,       "https://mirrors.aliyun.com/kali/"},
  {&Bfsu,      "https://mirrors.bfsu.edu.cn/kali/"},
  {&Ustc,      "https://mirrors.ustc.edu.cn/kali/"},
  {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/kali/"},
  {&Tencent,   "https://mirrors.tencent.com/kali/"},
  {&Netease,   "https://mirrors.163.com/kali/"},
  {&Sohu,      "https://mirrors.sohu.com/kali/"}
},



/**
 * 2023-09-02 更新
 *
 * TODO: 1. 源并不完整，且未经测试是否有效
 */
os_openbsd_sources[] = {
  {&Ali,       "https://mirrors.aliyun.com/OpenBSD/"},
  {&Bfsu,      "https://mirrors.bfsu.edu.cn/OpenBSD/"},
  {&Ustc,      "https://mirrors.ustc.edu.cn/OpenBSD/"},
  {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/OpenBSD/"},
  {&Tencent,   "https://mirrors.tencent.com/OpenBSD/"},
  {&Netease,   "https://mirrors.163.com/OpenBSD/"},
  {&Sohu,      "https://mirrors.sohu.com/OpenBSD/"}
},


/**
 * 2023-09-02 更新
 *
 * TODO: 1. 源并不完整，且未经测试是否有效
 */
os_msys2_sources[] = {
  {&Ali,       "https://mirrors.aliyun.com/msys2/"},
  {&Bfsu,      "https://mirrors.bfsu.edu.cn/msys2/"},
  {&Ustc,      "https://mirrors.ustc.edu.cn/msys2/"},
  {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/msys2/"},
  {&Tencent,   "https://mirrors.tencent.com/msys2/"},
  {&Netease,   "https://mirrors.163.com/msys2/"},
  {&Sohu,      "https://mirrors.sohu.com/msys2/"}
},


/**
 * 2023-09-05 更新
 *
 * TODO: 1. 源并不完整，且未经测试是否有效
 */
os_arch_sources[] = {
  {&Ali,       "https://mirrors.aliyun.com/archlinux/"},
  {&Bfsu,      "https://mirrors.bfsu.edu.cn/archlinux/"},
  {&Ustc,      "https://mirrors.ustc.edu.cn/archlinux/"},
  {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/archlinux/"},
  {&Tencent,   "https://mirrors.tencent.com/archlinux/"},
  {&Netease,   "https://mirrors.163.com/archlinux/"},
  {&Sohu,      "https://mirrors.sohu.com/archlinux/"}
},

/**
 * 2023-09-05 更新
 *
 * TODO: 1. 源并不完整，且未经测试是否有效
 */
os_gentoo_sources[] = {
  {&Ali,       "mirrors.aliyun.com"},
  {&Bfsu,      "mirrors.bfsu.edu.cn"},
  {&Ustc,      "mirrors.ustc.edu.cn"},
  {&Tuna,      "mirrors.tuna.tsinghua.edu.cn"},
  {&Tencent,   "mirrors.tencent.com"},
  {&Netease,   "mirrors.163.com"},
  {&Sohu,      "mirrors.sohu.com"}
},

/**
 * 2023-09-05 更新
 *
 * TODO: 1. 源并不完整，且未经测试是否有效
 */
os_netbsd_sources[] = {
  {&Ali,       "https://mirrors.aliyun.com/pkgsrc/packages/NetBSD/"},
  {&Bfsu,      "https://mirrors.bfsu.edu.cn/pkgsrc/packages/NetBSD/"},
  {&Ustc,      "https://mirrors.ustc.edu.cn/pkgsrc/packages/NetBSD/"},
  {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/pkgsrc/packages/NetBSD/"},
  {&Tencent,   "https://mirrors.tencent.com/pkgsrc/packages/NetBSD/"},
  {&Netease,   "https://mirrors.163.com/pkgsrc/packages/NetBSD/"},
  {&Sohu,      "https://mirrors.sohu.com/pkgsrc/packages/NetBSD/"}
},

/**
 * 2023-09-06 更新
 *
 * TODO: 1. 源并不完整，且未经测试是否有效
 */
os_openeuler_sources[] = {
  {&Ali,       "https://mirrors.aliyun.com/openeuler/"},
  {&Bfsu,      "https://mirrors.bfsu.edu.cn/openeuler/"},
  {&Ustc,      "https://mirrors.ustc.edu.cn/openeuler/"},
  {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/openeuler/"},
  {&Tencent,   "https://mirrors.tencent.com/openeuler/"},
  {&Netease,   "https://mirrors.163.com/openeuler/"},
  {&Sohu,      "https://mirrors.sohu.com/openeuler/"}
},

/**
 * 2023-09-06 更新
 *
 * TODO: 1. 源并不完整，且未经测试是否有效
 */
os_openkylin_sources[] = {
  {&Ali,       "https://mirrors.aliyun.com/openkylin/"},
  {&Bfsu,      "https://mirrors.bfsu.edu.cn/openkylin/"},
  {&Ustc,      "https://mirrors.ustc.edu.cn/openkylin/"},
  {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/openkylin/"},
  {&Tencent,   "https://mirrors.tencent.com/openkylin/"},
  {&Netease,   "https://mirrors.163.com/openkylin/"},
  {&Sohu,      "https://mirrors.sohu.com/openkylin/"}
}
;


/* 辅助函数 */
int
dblary_maxidx(double* array, int size)
{
  double maxval = array[0];
  int maxidx = 0;

  for (int i=1; i<size; i++) {
    if (array[i]>maxval) {
      maxval = array[i];
      maxidx = i;
    }
  }
  return maxidx;
}

/**
 * 用于告知用户 chsrc 所执行的操作
 */
void
chsrc_logcmd (const char* cmd)
{
  xy_info(xy_2strjoin("chsrc: 运行 ", cmd));
}

void
chsrc_runcmd (const char* cmd)
{
  chsrc_logcmd(cmd);
  system(cmd);
}

/**
 * 用于 _setsrc 函数
 */
void
chsrc_say_selection (source_info* source)
{
  xy_info (xy_strjoin(5, "chsrc: 选中镜像站: ", source->mirror->abbr, " (", source->mirror->code, ")"));
}

void
chsrc_say_thanks (source_info* source)
{
  xy_success(xy_2strjoin("chsrc: 感谢镜像提供方: ", source->mirror->name));
}


#define def_target_sources_n(t) const size_t t##_sources_n = xy_arylen(t##_sources)
def_target_sources_n(pl_ruby); def_target_sources_n(pl_python); def_target_sources_n(pl_nodejs);
def_target_sources_n(pl_perl); def_target_sources_n(pl_php);    def_target_sources_n(pl_go);
def_target_sources_n(pl_rust); def_target_sources_n(pl_java);   def_target_sources_n(pl_dotnet);
def_target_sources_n(pl_dart);
def_target_sources_n(pl_r);    def_target_sources_n(pl_julia);

def_target_sources_n(os_ubuntu);      def_target_sources_n(os_debian);      def_target_sources_n(os_fedora);
def_target_sources_n(os_kali);        def_target_sources_n(os_openbsd);     def_target_sources_n(os_msys2);
def_target_sources_n(os_arch);        def_target_sources_n(os_gentoo);      def_target_sources_n(os_netbsd);
def_target_sources_n(os_deepin);      def_target_sources_n(os_openeuler);   def_target_sources_n(os_openkylin);

/* Target Info */
typedef struct {
  void (*setfn)(char* option);
  void (*getfn)(char* option);
  source_info* sources;
  size_t       sources_n;
} target_info;

#define def_target_info(t) target_info t##_target = {t##_setsrc, t##_getsrc, t##_sources, t##_sources_n}
