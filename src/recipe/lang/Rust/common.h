/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2024-10-02>
 * Last Modified : <2024-11-21>
 * ------------------------------------------------------------*/

static MirrorSite_t RsProxyCN =
{
  "rsproxycn", "RsProxy.cn", "字节跳动基础架构Dev Infra", "https://rsproxy.cn/",
  {NotSkip, NA, NA, "https://rsproxy.cn/api/v1/crates/windows/0.58.0/download"}
},
Ali =
{
    "ali", "Ali OPSX Public", "阿里巴巴开源镜像站(公网)", "https://developer.aliyun.com/mirror/",
    {NotSkip, NA, NA, "https://mirrors.aliyun.com/crates/api/v1/crates/windows/0.58.0/download"}
};

static MirrorSite_t Ustc =
{
    "ustc", "USTC", "中国科学技术大学开源镜像站", "https://mirrors.ustc.edu.cn/",
    {NotSkip, NA, NA, "https://crates-io.proxy.ustclug.org/api/v1/crates/windows/0.58.0/download"}
};

