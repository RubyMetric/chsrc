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
use Version;

sub USAGE() {
  print qq:to/END/;
    rawstr4c: Raw String for C (GPLv3+) {Version::VERSION}-{Version::RELEASE_DATE}

    Usage: rawstr4c [options] <FILE.md|DIR>

    Arguments:
    FILE.md    Process a specific markdown file
    DIR        Process rawstr4c.md file in the given directory

    Options:
    -d|--debug    Show debug information during processing
                  Value can be [generator|parser]. Default to generator.

    -v|--version  Show version information

    -h|--help     Show this help message

    END
}


sub MAIN(
  Str  $input-path?,
  # 如果是 Str 类型，则 --debug 缺少命令行参数
  # 如果是 Any 类型，则可以直接使用 --debug，值为 True
  Any  :$debug,
  Any  :$version,
  :$d, :$v, :$h
)
{
  if ($version || $v) {
    print Version::VERSION_CONTENT_FOR_-version;
    exit(0);
  }

  if ($h) {
    USAGE;
    exit(0);
  }

  if (!$input-path) {
    USAGE;
    exit(0);
  }

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
      when 'parser' {$parser.debug;}
      default       {$generator.debug;}
    }
  }

  if ($d.defined) {
    given $d {
      when 'parser' {$parser.debug;}
      default       {$generator.debug;}
    }
  }

  $generator.generate;
}
