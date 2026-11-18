#Requires -Version 7.0
# SPDX-License-Identifier: GPL-3.0-or-later
$ErrorActionPreference = 'Stop'
$binary = Join-Path (Split-Path $PSScriptRoot -Parent) 'cli-parser-test.exe'
$payload = @(
    '--help', '--dry', '--', '--skfw sq', 'sdf', 'wiw', 'www', '',
    '他说 "你好" 😀', '日本語 한국어 𠀀', '👩🏽‍💻',
    'C:\path with spaces\', 'a\"b', 'a\\"b', 'a""b', '"quoted"',
    "line1`nline2", "tab`tvalue"
)
$expected = "$($payload.Count)`n"
foreach ($value in $payload) {
    $expected += [Convert]::ToHexString([Text.Encoding]::UTF8.GetBytes($value)).ToLowerInvariant() + "`n"
}

# 结构化启动跨越真实 Windows 进程边界，不经过额外 shell。
$start = [Diagnostics.ProcessStartInfo]::new($binary)
$start.UseShellExecute = $false
$start.RedirectStandardOutput = $true
$start.RedirectStandardError = $true
foreach ($value in (@('set', 'probe', '--') + $payload)) {
    $start.ArgumentList.Add($value)
}
$process = [Diagnostics.Process]::Start($start)
$output = $process.StandardOutput.ReadToEnd().Replace("`r`n", "`n")
$errorOutput = $process.StandardError.ReadToEnd()
$process.WaitForExit()
if ($process.ExitCode -ne 0 -or $output -cne $expected) {
    throw "Structured argv round trip failed ($($process.ExitCode)): $errorOutput`n$output"
}
$process.Dispose()

# 同时验证用户实际使用的 PowerShell 原生命令传参。
$PSNativeCommandArgumentPassing = 'Standard'
$nativeArgs = @('set', 'probe', '--') + $payload
$nativeOutput = & $binary @nativeArgs
$nativeStatus = $LASTEXITCODE
$actual = ($nativeOutput -join "`n") + "`n"
if ($nativeStatus -ne 0 -or $actual -cne $expected) {
    throw "PowerShell argv round trip failed ($nativeStatus):`n$actual"
}
Write-Output 'Windows structured argv and PowerShell Unicode/quote tests passed'
