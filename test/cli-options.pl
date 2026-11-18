#!/usr/bin/env perl
use v5.32;
use utf8;
use Test::More;
use File::Temp qw(tempdir);

my $chsrc = $^O eq 'MSWin32' ? '.\\chsrc' : './chsrc';
my $bin = tempdir (CLEANUP => 1);
my $npm = "$bin/npm" . ($^O eq 'MSWin32' ? '.bat' : '');
open (my $stub, '>:raw', $npm) or die "$npm: $!";
print $stub ($^O eq 'MSWin32' ? "\@echo off\r\nexit /b 1\r\n" : "#!/bin/sh\nexit 1\n");
close $stub or die "$npm: $!";
chmod 0755, $npm unless $^O eq 'MSWin32';

{
    local $ENV{PATH} = $bin . ($^O eq 'MSWin32' ? ';' : ':') . $ENV{PATH};
    my $registry = qr{npm config set registry https://registry\.npmmirror\.com/};
    my $project = qr{npm config --location project set registry https://registry\.npmmirror\.com/};
    for my $case (
        ['--dry set npm first', 0, $registry],
        ['set --dry npm first', 0, $registry],
        ['set npm --dry first', 0, $registry],
        ['set npm first --dry', 0, $registry],
        ['reset npm --dry', 0, qr{npm config set registry https://registry\.npmjs\.org/}],
        ['set --scope project npm first --dry', 0, $project],
        ['--scope=project set npm --dry first', 0, $project],
        ['set npm first --scope project --dry', 0, $project],
        ['set npm https://example.invalid/MixedCase/Index --dry', 0,
         qr{npm config set registry https://example\.invalid/MixedCase/Index}],
        ['set npm --dry --scope', 2, qr/缺少参数/],
        ['set npm --dry --scope=invalid', 2, qr/无效的换源作用域/],
        ['--no-color list os', 0, qr/netbsd\s*openbsd/],
        ['list os --no-color', 0, qr/netbsd\s*openbsd/],
        ['--dry set npm first -- --help -d xxx', 0, $registry],
        ['--dry set npm first -- --scope project --unknown', 0, $registry],
        ['list os -- --dry --english', 0, qr/netbsd\s*openbsd/],
        ['set npm first --dry --scope -- project', 2, qr/缺少参数/],
        ['set npm first extra --dry', 2, qr/多余的位置参数/],
        ['set missing --help --unknown', 0, qr/^   help/m],
        ['help --unknown', 0, qr/^   help/m],
        ['--version --unknown', 0, qr/Copyright/],
        ['--unknown --version', 2, qr/未识别的命令行选项/],
        ['--version --help', 0, qr/Copyright/],
        ['--help --version', 0, qr/^   help/m],
        (map { [$_, 2, qr/未识别的命令行选项/] }
            qw(-dry --dry-run -scope -local -ipv6 -en -english -English -no-color -no-colour -v -version -help)),
        (map { [$_, 2, qr/不支持的命令/] } qw(h version ver)),
    ) {
        my ($args, $expected_status, $pattern) = @$case;
        my $output = `$chsrc --no-color $args 2>&1`;
        my $status = $? >> 8;
        utf8::decode ($output);
        is ($status, $expected_status, "$args: exit status");
        like ($output, $pattern, "$args: output");
    }
}

done_testing unless caller;
1;
