/** ------------------------------------------------------------
 * File          : sources.h
 * License       : GPLv3
 * Authors       : Aoran Zeng <ccmywish@qq.com>
 * Created on    : <2023-08-29>
 * Last modified : <2023-09-29>
 *
 * sources:
 *
 *   镜像站与源信息
 * ------------------------------------------------------------*/

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


/**
 * 教育网镜像
 *
 * 我们目前根据 https://github.com/mirrorz-org/oh-my-mirrorz 挑选速度前10位
 */
mirror_info
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
 * @note 腾讯软件源中，有很多链接都已失效，请仔细检查
 */
mirror_info
  Ali      = {"ali",     "Ali OPSX", "阿里巴巴开源镜像站",  "https://developer.aliyun.com/mirror/",
              "https://mirrors.aliyun.com/deepin-cd" Big_File_deepin},

  Tencent  = {"tencent", "Tencent",  "腾讯软件源",         "https://mirrors.tencent.com/",
              "https://mirrors.cloud.tencent.com/debian" Big_File_debian},

  Netease  = {"netease", "Netease",  "网易开源镜像站",      "https://mirrors.163.com/",
              "https://mirrors.163.com/deepin-cd" Big_File_deepin},

  Sohu     = {"sohu",    "SOHU",     "搜狐开源镜像站",      "https://mirrors.sohu.com/",
              "https://mirrors.sohu.com/deepin-cd" Big_File_deepin},

  Api7     = {"api7",    "api7.ai",  "深圳支流科技有限公司", "https://www.apiseven.com/", NULL};

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

  NugetOrg   = {"nuget.org",    "NuGet Org",   "Nuget Organization", "https://www.nuget.org/",           NULL},
  EmacsChina = {"emacschina",  "EmacsChina",   "Emacs China 社区", "https://elpamirror.emacs-china.org/", NULL};


mirror_info*
available_mirrors[] = {
  &MirrorZ, &Tuna, &Sjtug_Zhiyuan, &Zju, &Lzuoss, &Jlu, &Bfsu, &Pku, &Bjtu, &Sustech, &Ustc, &Nju, // &Cqu,
  &Ali,  &Tencent, &Netease, &Sohu, &Api7,
  &RubyChina, &EmacsChina, &NpmMirror, &GoProxyCN, &GoProxyIO,
  // 暂不支持 &NugetOrg
};


typedef struct {
  const mirror_info* mirror;
  const char* url;
} source_info;


/**
 * 源信息
 *
 * @note 我们要求每个源最好情况下
 *  1. 至少有一个教育网镜像
 *  2. 至少有一个商业公司或开源社区维护的镜像
 */


/**
 * 2023-09-15 更新
 *
 * @note
 * 下面的源，并不都实现正确，
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
static source_info
pl_ruby_sources[] = {
  {&Tencent,   "https://mirrors.tencent.com/rubygems/"},
  {&RubyChina, "https://gems.ruby-china.com/"},

  // {&Bfsu,      "https://mirrors.bfsu.edu.cn/rubygems/"},
  // {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/rubygems/"},
  // {&Ali,       "https://mirrors.aliyun.com/rubygems/"},
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
 * @note 缺少教育网或开源社区软件源
 */
pl_php_sources[] = {
  {&Ali,            "https://mirrors.aliyun.com/composer/"},
  {&Tencent,        "https://mirrors.tencent.com/composer/"}
},



/**
 * 2023-09-27 更新
 *
 * @note 目前只有唯一一个源
 */
pl_lua_sources[] = {
  {&Api7,            "https://luarocks.cn"},
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
 * 2023-09-10 更新
 *
 * @note 不太好换，且用户可能不多，我们暂时只给用户提供文档
 */
pl_clojure_sources[] = {
  {&Tuna,           "https://mirrors.tuna.tsinghua.edu.cn/help/clojars/"},
  {&Ustc,           "https://mirrors.ustc.edu.cn/help/clojars.html"}
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
 *
 * @note 我们这里挑选的必须也支持 Flutter
 *
 * 我们将会在setsrc函数中生成
 * 1. https://mirrors.tuna.tsinghua.edu.cn/dart-pub
 * 2. https://mirrors.tuna.tsinghua.edu.cn/flutter
 */
pl_dart_sources[] = {
  // {&Sjtug_Zhiyuan, "https://mirror.sjtu.edu.cn/"}, // 不确定SJTUG的flutter镜像给的对不对
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/"},
  {&Tencent,       "https://mirrors.cloud.tencent.com/"},
  {&Nju,           "https://mirror.nju.edu.cn/"}
},



/**
 * 2023-09-10 更新
 */
pl_haskell_sources[] = {
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/hackage"},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/hackage"},
  {&Nju,           "https://mirror.nju.edu.cn/hackage"},
  {&Ustc,          "https://mirrors.ustc.edu.cn/hackage"}
},


/**
 * 2023-09-15 更新
 */
pl_ocaml_sources[] = {
  {&Sjtug_Zhiyuan, "https://mirrors.sjtug.sjtu.edu.cn/git/opam-repository.git"}
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
  {&Ali,       "https://mirrors.aliyun.com/deepin"},
  {&Bfsu,      "https://mirrors.bfsu.edu.cn/deepin"},
  {&Ustc,      "https://mirrors.ustc.edu.cn/deepin"},
  {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/deepin"},
  {&Tencent,   "https://mirrors.tencent.com/deepin"},
  {&Netease,   "https://mirrors.163.com/deepin"},
  {&Sohu,      "https://mirrors.sohu.com/deepin"}
},


/**
 * 2023-09-01 更新
 *
 * TODO: 1. 源并不完整，且未经测试是否有效
 */
os_debian_sources[] = {
  {&Ali,       "https://mirrors.aliyun.com/debian"},
  {&Bfsu,      "https://mirrors.bfsu.edu.cn/debian"},
  {&Ustc,      "https://mirrors.ustc.edu.cn/debian"},
  {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/debian"},
  {&Tencent,   "https://mirrors.tencent.com/debian"},
  {&Netease,   "https://mirrors.163.com/debian"},
  {&Sohu,      "https://mirrors.sohu.com/debian"}
},



/**
 * 2023-09-02 更新
 *
 * TODO: 1. 源并不完整，且未经测试是否有效
 */
os_fedora_sources[] = {
  {&Ali,       "https://mirrors.aliyun.com/fedora"},
  {&Bfsu,      "https://mirrors.bfsu.edu.cn/fedora"},
  {&Ustc,      "https://mirrors.ustc.edu.cn/fedora"},
  {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/fedora"},
  {&Tencent,   "https://mirrors.tencent.com/fedora"},
  {&Netease,   "https://mirrors.163.com/fedora"},
  {&Sohu,      "https://mirrors.sohu.com/fedora"}
},


/**
 * 2023-09-02 更新
 *
 * TODO: 1. 源并不完整，且未经测试是否有效
 */
os_kali_sources[] = {
  {&Ali,       "https://mirrors.aliyun.com/kali"},
  {&Bfsu,      "https://mirrors.bfsu.edu.cn/kali"},
  {&Ustc,      "https://mirrors.ustc.edu.cn/kali"},
  {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/kali"},
  {&Tencent,   "https://mirrors.tencent.com/kali"},
  {&Netease,   "https://mirrors.163.com/kali"},
  {&Sohu,      "https://mirrors.sohu.com/kali"}
},



/**
 * 2023-09-05 更新
 *
 * TODO: 1. 源并不完整，且未经测试是否有效
 */
os_arch_sources[] = {
  {&Ali,       "https://mirrors.aliyun.com/"},
  {&Bfsu,      "https://mirrors.bfsu.edu.cn/"},
  {&Ustc,      "https://mirrors.ustc.edu.cn/"},
  {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/"},
  {&Tencent,   "https://mirrors.tencent.com/"},
  {&Netease,   "https://mirrors.163.com/"},
  {&Sohu,      "https://mirrors.sohu.com/"}
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
 * 2023-09-17 更新
 *
 * TODO: 1. 源并不完整，且未经测试是否有效
 */
os_opensuse_sources[] = {
  {&Ali,       "https://mirrors.aliyun.com/opensuse"},
  {&Bfsu,      "https://mirrors.bfsu.edu.cn/opensuse"},
  {&Ustc,      "https://mirrors.ustc.edu.cn/opensuse"},
  {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/opensuse"},
  {&Tencent,   "https://mirrors.tencent.com/opensuse"},
  {&Netease,   "https://mirrors.163.com/opensuse"},
  {&Sohu,      "https://mirrors.sohu.com/opensuse"}
},



/**
 * 2023-09-02 更新
 *
 * TODO: 1. 源并不完整，且未经测试是否有效
 */
os_msys2_sources[] = {
  {&Ali,       "https://mirrors.aliyun.com/msys2"},
  {&Bfsu,      "https://mirrors.bfsu.edu.cn/msys2"},
  {&Ustc,      "https://mirrors.ustc.edu.cn/msys2"},
  {&Tuna,      "https://mirrors.tuna.tsinghua.edu.cn/msys2"},
  {&Tencent,   "https://mirrors.tencent.com/msys2"},
  {&Netease,   "https://mirrors.163.com/msys2"},
  {&Sohu,      "https://mirrors.sohu.com/msys2"}
},



/**
 * 2023-09-24 更新
 */
os_rocky_sources[] = {
  {&Sjtug_Zhiyuan,  "https://mirror.sjtu.edu.cn/rocky"},
  {&Sustech,        "https://mirrors.sustech.edu.cn/rocky-linux"},
  {&Zju,            "https://mirrors.zju.edu.cn/rocky"},
  {&Lzuoss,         "https://mirror.lzu.edu.cn/rocky"},
  {&Sohu,           "https://mirrors.sohu.com/Rocky"},
  {&Netease,        "https://mirrors.163.com/rocky"},
  {&Ali,            "https://mirrors.aliyun.com/rockylinux"},
},



/**
 * 2023-09-24 更新
 */
os_alpine_sources[] = {
  {&Tuna,           "https://mirrors.tuna.tsinghua.edu.cn/alpine"},
  {&Sjtug_Zhiyuan,  "https://mirrors.sjtug.sjtu.edu.cn/alpine"},
  {&Sustech,        "https://mirrors.sustech.edu.cn/alpine"},
  {&Zju,            "https://mirrors.zju.edu.cn/alpine"},
  {&Lzuoss,         "https://mirror.lzu.edu.cn/alpine"},
  {&Ali,            "https://mirrors.aliyun.com/alpine"},
  {&Tencent,        "https://mirrors.cloud.tencent.com/alpine"}
},



/**
 * 2023-09-24 更新
 */
os_void_sources[] = {
  {&Tuna,           "https://mirrors.tuna.tsinghua.edu.cn/voidlinux"},
  {&Sjtug_Zhiyuan,  "https://mirror.sjtu.edu.cn/voidlinux"},
  {&Bfsu,           "https://mirrors.bfsu.edu.cn/voidlinux"}
},



/**
 * 2023-09-29 更新
 */
os_trisquel_sources[] = {
  {&Ali,            "https://mirrors.aliyun.com"},
  {&MirrorZ,        "https://mirrors.cernet.edu.cn"},
  {&Nju,            "https://mirror.nju.edu.cn"},
  {&Ustc,           "https://mirrors.ustc.edu.cn"}
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
};






/**
 * 2023-09-10 更新
 *
 * TODO: 1. 暂未添加商业公司源
 */
static source_info
wr_tex_sources[] = {
  {&Sjtug_Zhiyuan, "https://mirrors.sjtug.sjtu.edu.cn/ctan/systems/texlive/tlnet"},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/CTAN/systems/texlive/tlnet"},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/CTAN/systems/texlive/tlnet"},
  {&Lzuoss,        "https://mirror.lzu.edu.cn/CTAN/systems/texlive/tlnet"},
  {&Jlu,           "https://mirrors.jlu.edu.cn/CTAN/systems/texlive/tlnet"},
  {&Sustech,       "https://mirrors.sustech.edu.cn/CTAN/systems/texlive/tlnet"}
},


/**
 * 2023-09-10 更新
 *
 * @note Emacs用户往往只需要一次性换源，只会极少次调用 chsrc，我们只给用户提供文档
 */
wr_emacs_sources[] = {
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
 * @note 1. 这些链接将会在setsrc函数中补充完整
 *       2. 不确定 Sustech 能否工作
 */
wr_brew_sources[] = {
  {&Tuna,           "https://mirrors.tuna.tsinghua.edu.cn/"},
  {&Bfsu,           "https://mirrors.bfsu.edu.cn/"},
  {&Zju,            "https://mirrors.zju.edu.cn/"},
  {&Sustech,        "https://mirrors.sustech.edu.cn/"}
},



/**
 * 2023-09-11 更新
 *
 * @note 目前只有一个源
 */
wr_flathub_sources[] = {
  {&Sjtug_Zhiyuan,   "https://mirror.sjtu.edu.cn/flathub"},
},



/**
 * 2023-09-22 更新
 *
 * @note 1. 目前只有一个源
 *       2. 这些链接将会在setsrc函数中补充完整
 */
wr_nix_sources[] = {
  {&Bfsu,           "https://mirrors.bfsu.edu.cn/nix-channels/"}
},



/**
 * 2023-09-11 更新
 *
 * @note 目前只有一个源
 */
wr_guix_sources[] = {
  {&Sjtug_Zhiyuan,  "https://mirror.sjtu.edu.cn/git/guix.git"}
},



/**
 * 2023-09-10 更新
 *
 * @note 这些链接将会在setsrc函数中补充完整
 */
wr_anaconda_sources[] = {
  {&Tuna,            "https://mirrors.tuna.tsinghua.edu.cn/anaconda/"},
  {&Bfsu,            "https://mirrors.bfsu.edu.cn/anaconda/"},
  {&Zju,             "https://mirrors.zju.edu.cn/anaconda/"},
  {&Sjtug_Zhiyuan,   "https://mirror.sjtu.edu.cn/anaconda"}
};



#define def_target_sources_n(t) const size_t t##_sources_n = xy_arylen(t##_sources)

def_target_sources_n(pl_ruby);     def_target_sources_n(pl_python);    def_target_sources_n(pl_nodejs);
def_target_sources_n(pl_perl);     def_target_sources_n(pl_php);       def_target_sources_n(pl_lua);
def_target_sources_n(pl_go);       def_target_sources_n(pl_rust);
def_target_sources_n(pl_java);      def_target_sources_n(pl_clojure);
def_target_sources_n(pl_dotnet);   def_target_sources_n(pl_dart);      def_target_sources_n(pl_haskell);
def_target_sources_n(pl_ocaml);
def_target_sources_n(pl_r);        def_target_sources_n(pl_julia);

def_target_sources_n(os_ubuntu);   def_target_sources_n(os_debian);    def_target_sources_n(os_fedora);
def_target_sources_n(os_kali);     def_target_sources_n(os_opensuse);
def_target_sources_n(os_arch);     def_target_sources_n(os_msys2);     def_target_sources_n(os_gentoo);
def_target_sources_n(os_alpine);   def_target_sources_n(os_rocky);     def_target_sources_n(os_void);

def_target_sources_n(os_trisquel);

def_target_sources_n(os_freebsd);  def_target_sources_n(os_netbsd);    def_target_sources_n(os_openbsd);
def_target_sources_n(os_deepin);   def_target_sources_n(os_openeuler); def_target_sources_n(os_openkylin);


def_target_sources_n(wr_brew);     def_target_sources_n(wr_flathub);
def_target_sources_n(wr_nix);      def_target_sources_n(wr_guix);
def_target_sources_n(wr_tex);      def_target_sources_n(wr_emacs);
def_target_sources_n(wr_anaconda);
