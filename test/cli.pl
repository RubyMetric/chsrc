#!/usr/bin/env perl
# ---------------------------------------------------------------
# Test File     : cli.pl
# Test Authors  : Aoran Zeng <ccmywish@qq.com>
# Created On    : <2024-06-05>
# Last Modified : <2024-09-04>
#
#   测试 chsrc 可执行文件
# ---------------------------------------------------------------

use v5.38;
use Test::More;

my $version_str = qr|Written by Aoran Zeng, Heng Guo and contributors|;
like `./chsrc -v`,         $version_str,  'chsrc -v';
like `./chsrc --version`,  $version_str,  'chsrc --version';
like `./chsrc version`,    $version_str,  'chsrc version';

my $help_str = qr/^(help|list|get|set)/m;
like `./chsrc -h`,     $help_str,    'chsrc -h';
like `./chsrc --help`, $help_str,    'chsrc --help';
like `./chsrc help`,   $help_str,    'chsrc help';
like `./chsrc`,        $help_str,    'chsrc';

=begin
测试 chsrc list
=cut
my $list_str = qr/mirrorz\s*MirrorZ\s*.*/;
like `./chsrc ls`,            $list_str,    'chsrc ls';
like `./chsrc list mirrors`,  $list_str,    'chsrc list mirrors';
like `./chsrc list os`,    qr/debian\s*ubuntu/,   'chsrc list os';
like `./chsrc list ware`,  qr/brew\s*homebrew/,   'chsrc list ware';


=begin
测试 chsrc get
=cut
my $get_null = qr/chsrc: 请您提供想要查看源的目标名/;
like `./chsrc get -no-color 2>&1`,  $get_null,    'chsrc get -no-color';

my $fake_target_name = qr/暂不支持的换源目标/;
like `./chsrc get fake_target_name 2>&1`,  $fake_target_name, 'chsrc get fake_target_name';

my $get_ruby = qr/gem sources/;
like `./chsrc get ruby`,   $get_ruby,  'chsrc get ruby';



=begin
测试 chsrc measure
=cut
my $measure_ruby = qr/  - Ruby China/;
like `./chsrc measure ruby`,  $measure_ruby,  'chsrc measure ruby';



=begin
测试 chsrc set 以及 chsrc reset
=cut
my $set_ruby = "换源完成，感谢镜像提供方";
my $set_ruby_abcd = "";
my $set_ruby_first = "换源完成，感谢镜像提供方";
my $reset_ruby = "将重置为上游默认源.*Upstream";
my $set_ruby_tencent = "换源完成，感谢镜像提供方.*腾讯软件源";
my $set_ruby_locally = "bundle config --local";


like `./chsrc set ruby`,         qr/$set_ruby/,         'chsrc set ruby';
# like `./chsrc set ruby abcd`,    qr/$set_ruby_abcd/,   'chsrc set ruby abcd';
like `./chsrc set ruby first`,   qr/$set_ruby_first/,   'chsrc set ruby first';
like `./chsrc reset ruby`,       qr/$reset_ruby/s,      'chsrc reset ruby';
like `./chsrc set ruby tencent`, qr/$set_ruby_tencent/, 'chsrc set ruby tencent';
like `./chsrc set -local ruby tencent`,  qr/$set_ruby_locally/, 'chsrc set -local ruby tencent';


done_testing;
