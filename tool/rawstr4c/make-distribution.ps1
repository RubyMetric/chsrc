# ---------------------------------------------------------------
# SPDX-License-Identifier: GPL-3.0-or-later
# ---------------------------------------------------------------
# Script Name    : make-distribution.ps1
# Script Authors : Aoran Zeng <ccmywish@qq.com>
# Created On     : <2025-07-21>
# Last Modified  : <2025-07-21>
#
# Make rawstr4c distribution file (tar.gz)
#
# Usage:
#
#           .\make-distribution.ps1 [-upload]
# ---------------------------------------------------------------

param(
  [switch]$upload
)

$files = @()

# 获取相对路径的函数
function Get-RelativePath($item) {
  # K:\chsrc\tool\rawstr4c
  # K:\chsrc\tool\rawstr4c\our-dist-file
  # 注意我们必须要删掉这里的\，这就是 +1 的原因
  return $item.FullName.Substring((Get-Location).Path.Length + 1)
}

# 添加 lib 目录（排除 .precomp）
$files += Get-ChildItem lib -Recurse | Where-Object { $_.FullName -notlike "*\.precomp*" } | ForEach-Object { Get-RelativePath $_ }

# 添加其他目录
$files += Get-ChildItem bin, doc, test -Recurse | ForEach-Object { Get-RelativePath $_ }

# 添加根目录文件
$rootFiles = @('META6.json', 'LICENSE', 'README.md')
foreach ($file in $rootFiles) {
  if (Test-Path $file) {
    $files += $file
  }
}

# 打包
$archiveName = "rawstr4c-$(Get-Date -Format 'yyyyMMdd').tar.gz"
$files | tar -czf $archiveName -T -

Write-Host "打包完成: $archiveName" -ForegroundColor Green
Write-Host "提示: 可指定 -upload 直接上传" -ForegroundColor Green

# 如果指定了 upload 参数，则上传到 fez
if ($upload) {
  Write-Host "正在上传到 fez..." -ForegroundColor Yellow

  try {
    fez upload --file $archiveName
    Write-Host "上传成功!" -ForegroundColor Green
  } catch {
    Write-Error "上传失败: $_"
    exit 1
  }
}

