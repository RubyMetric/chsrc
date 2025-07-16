#!/usr/bin/env raku
# ---------------------------------------------------------------
# Copyright © 2025-2025 Aoran Zeng
# SPDX-License-Identifier: GPL-3.0-or-later
# ---------------------------------------------------------------
# File Name     : rawstr4c.raku
# File Authors  : Aoran Zeng <ccmywish@qq.com>
# Contributors  :  Nul None  <nul@none.org>
# Created On    : <2025-07-12>
# Last Modified : <2025-07-16>
#
# rawstr4c:
#
#             Raw strings for C programming language
#
# ---------------------------------------------------------------

use Parser;
use Generator;

sub USAGE() {
  usage;
}

sub usage() {
  print q:to/END/;
    Usage: rawstr4c [--debug] [--help] <FILE.md|DIR>

    Arguments:
      FILE.md    Process a specific markdown file
      DIR        Process rawstr4c.md file in the given directory

    Options:
      --debug    Show debug information during processing
                 Value can be [generator|parser]. Default to generator.

      --help     Show this help message

    Error: Unknown option or invalid arguments provided.
    END
}


sub MAIN(
  # 一定要声明为必选，强制用户输入，未输入时直接进入 USAGE
  Str  $input-path,
  # 如果是 Str 类型，则 --debug 缺少命令行参数
  # 如果是 Any 类型，则可以直接使用 --debug，值为 True
  Any  :$debug,
)
{
  my $markdown-file;

  if $input-path.IO.d {
    $markdown-file = $input-path.IO.add("rawstr4c.md");
    unless $markdown-file.e {
      # 也可以 warn
      note "Error: No 'rawstr4c.md' file found in directory '$input-path'";
      exit(1);
    }
  }
  elsif $input-path.IO.f {
    $markdown-file = $input-path.IO;
  } else {
    note "Error: '$input-path' is neither a file nor a directory";
    exit(1);
  }

  my $parser = Parser::Parser.new($markdown-file.Str);
  $parser.parse;

  my $generator = Generator::Generator.new($parser);

  if ($debug.defined) {
    given $debug {
      when 'parser' {
        $parser.debug;
      }
      default {
        $generator.debug;
      }
    }
  }
  $generator.generate;
}
