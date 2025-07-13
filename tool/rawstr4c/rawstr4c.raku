#!/usr/bin/env raku
# ---------------------------------------------------------------
# Copyright © 2025-2025 Aoran Zeng
# SPDX-License-Identifier: GPL-3.0-or-later
# ---------------------------------------------------------------
# File Name     : rawstr4c.raku
# File Authors  : Aoran Zeng <ccmywish@qq.com>
# Contributors  :  Nul None  <nul@none.org>
# Created On    : <2025-07-12>
# Last Modified : <2025-07-14>
#
# rawstr4c:
#
#             Raw strings for C programming language
#
# Usage:
#       rawstr4c <FILE.md>  # 指定具体某文件名
#       rawstr4c <DIR>      # 使用某一目录寻找 rawstr4c.md 文件
#
# 要注意的是，该程序一次性只能处理唯一一个文件
# ---------------------------------------------------------------

use Parser;
use Generator;

sub MAIN(
  Str $input-path,
  Bool :$debug = False  #= --debug
) {
  my $markdown-file;

  if $input-path.IO.d {
    $markdown-file = $input-path.IO.add("rawstr4c.md");
    unless $markdown-file.e {
      die "Error: No 'rawstr4c.md' file found in directory '$input-path'\n";
    }
  }
  elsif $input-path.IO.f {
    $markdown-file = $input-path.IO;
  } else {
    die "Error: '$input-path' is neither a file nor a directory\n";
  }

  my $parser = Parser::Parser.new(input-file=>$markdown-file);
  $parser.parse;
  $parser.debug if $debug;

  Generator::Generator.new.generate($parser);
}
