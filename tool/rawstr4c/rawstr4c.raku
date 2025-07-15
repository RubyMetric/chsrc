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

use lib 'lib';
use Parser;
use Generator;

sub MAIN(
  # 必须声明为可选，否则一直报奇怪的错
  Str $input-path?,
  Bool :$debug = False,  #= --debug
  Bool :$help            #= --help
)
{
  if $help || !$input-path {
    print q:to/END/;
      Usage: rawstr4c <FILE.md|DIR> [--debug]

      Arguments:
        FILE.md    Process a specific markdown file
        DIR        Process rawstr4c.md file in the given directory

      Options:
        --debug    Show debug information during processing
        --help     Show this help message
      END
    # exit($help ?? 0 !! 1);
    exit(0);
  }

  my $markdown-file;

  if $input-path.IO.d {
    $markdown-file = $input-path.IO.add("rawstr4c.md");
    unless $markdown-file.e {
      die "Error: No 'rawstr4c.md' file found in directory '$input-path'";
    }
  }
  elsif $input-path.IO.f {
    $markdown-file = $input-path.IO;
  } else {
    die "Error: '$input-path' is neither a file nor a directory";
  }

  my $parser = Parser::Parser.new($markdown-file.Str);
  $parser.parse;
  $parser.debug-print-summary if $debug;

  Generator::Generator.new($parser).generate;
}
