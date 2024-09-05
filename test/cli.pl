#!/usr/bin/env perl
# ---------------------------------------------------------------
# Test File     : cli.pl
# Test Authors  : Aoran Zeng <ccmywish@qq.com>
# Created On    : <2024-06-05>
# Last Modified : <2024-09-05>
#
#   测试 chsrc 可执行文件
# ---------------------------------------------------------------

use v5.38;
use Test::More;

my $version_str = qr|chsrc .*\nCopyright .*\nLicense GPLv3\+: GNU GPL version 3 or later|;
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
my $list_str = qr/mirrorz\s*MirrorZ\s*.*\ntuna\s*TUNA/;
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


if ($ARGV[0] eq 'fastcheck') {
  say "Fast checking, done testing.";
  done_testing;
  exit 0;
}

my $has_ruby = system 'ruby -v';
if ($has_ruby == 0) {
  say "Ruby exists. Go on testing.";
} else {
  say "No Ruby. End testing.";
  done_testing;
  exit 0;
}

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
my $set_ruby_abcd      = qr/镜像站.*不存在/;
my $set_ruby_first     = qr/全自动换源完成, 感谢镜像提供方/;
my $set_ruby           = qr/全自动换源完成, 感谢镜像提供方/;
my $reset_ruby         = qr/将重置为上游默认源.*Upstream/s;
my $set_ruby_rubychina = qr/Ruby China 社区/;
my $set_ruby_locally   = qr/bundle config --local/;

like `./chsrc set ruby abcd 2>&1`,      $set_ruby_abcd,      'chsrc set ruby abcd';
like `./chsrc set ruby first`,          $set_ruby_first,     'chsrc set ruby first';
like `./chsrc set ruby`,                $set_ruby,           'chsrc set ruby';
like `./chsrc reset ruby`,              $reset_ruby,         'chsrc reset ruby';
like `./chsrc set ruby rubychina`,      $set_ruby_rubychina, 'chsrc set ruby rubychina';
like `./chsrc set -local ruby first`,   $set_ruby_locally,   'chsrc set -local ruby first';


done_testing;
