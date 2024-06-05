#!/usr/bin/env perl
# ---------------------------------------------------------------
# File          : cli.pl
# Authors       : Aoran Zeng <ccmywish@qq.com>
# Created on    : <2024-06-05>
# Last modified : <2024-06-05>
#
#   测试 chsrc 可执行文件
# ---------------------------------------------------------------

use v5.38;
use Test::More;

my $version_str = "chsrc: Change Source \\(GPLv3\\)";
like `./chsrc -v`,        qr/$version_str/,  'chsrc -v';
like `./chsrc version`,   qr/$version_str/,  'chsrc version';
like `./chsrc --version`, qr/$version_str/,  'chsrc --version';

my $help_str = "^(help|list|get|set)";
like `./chsrc`,       qr/$help_str/m,    'chsrc';
like `./chsrc -h`,    qr/$help_str/m,    'chsrc -h';
like `./chsrc help`,  qr/$help_str/m,    'chsrc help';


=begin
测试 chsrc list
=cut
my $list_str = "^mirrorz\\s*MirrorZ.*MirrorZ 校园网镜像站";
like `./chsrc list`,  qr/$list_str/m,    'chsrc list';
like `./chsrc ls`,    qr/$list_str/m,    'chsrc ls';


=begin
测试 chsrc get
=cut
# my $get_null = "请您提供想要测速源的软件名";
# like `./chsrc get 2>1`,      qr/$get_null/,    'chsrc get';
my $get_abcd = "暂不支持的换源目标";
my $get_python = "命令.*python.*存在";
like `./chsrc get abcd`, qr/$get_abcd/,    'chsrc get abcd';
like `./chsrc get py`,   qr/$get_python/,  'chsrc get py';


=begin
测试 chsrc cesu
=cut
my $cesu_ruby = "测速 https://mirrors\\.tencent\\.com/";
like `./chsrc cesu ruby`,  qr/$cesu_ruby/,      'chsrc cesu ruby';


=begin
测试 chsrc set
=cut
my $set_ruby = "换源完成，感谢镜像提供方";
my $set_ruby_abcd = "";
my $set_ruby_first = "换源完成，感谢镜像提供方";
my $set_ruby_tencent = "换源完成，感谢镜像提供方.*腾讯软件源";
my $set_ruby_locally = "bundle config --local";

like `./chsrc set ruby`,         qr/$set_ruby/,         'chsrc set ruby';
# like `./chsrc set ruby abcd`,    qr/$set_ruby_abcd/,   'chsrc set ruby abcd';
like `./chsrc set ruby first`,   qr/$set_ruby_first/,   'chsrc set ruby first';
like `./chsrc set ruby tencent`, qr/$set_ruby_tencent/, 'chsrc set ruby tencent';
like `./chsrc set -local ruby`,  qr/$set_ruby_locally/, 'chsrc set -local ruby';

done_testing;
