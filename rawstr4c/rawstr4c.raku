#!/usr/bin/env raku
# ---------------------------------------------------------------
# File Name     : rawstr4c.raku
# File Authors  : Aoran Zeng <ccmywish@qq.com>
# Contributors  :  Nul None  <nul@none.org>
# Created On    : <2025-07-12>
# Last Modified : <2025-07-12>
#
# Generate raw strings for C programming language
# ---------------------------------------------------------------

unless @*ARGS {
  die "Usage: rawstr4c <FILE.md>\n";
}

my $markdown-file = @*ARGS[0];

# 确保文件存在
unless $markdown-file.IO.e {
  die "Error: File '$markdown-file' not found.\n";
}

# 解析配置信息
sub parse-config($content) {
  my %config;

  for $content.lines -> $line {
    # say "Processing line: $line";
    if $line ~~ /^ '-' \s* 'translate' \s* '=' \s* '`' ':' (<[a..z\-]>+) '`' / {
      %config<translation> = ~$0;
      say "Global translation mode: " ~ %config<translation>;
      last;
    }
  }

  return %config;
}

# 根据转换模式处理字符
sub convert-char($char, $mode) {
  my $byte = $char.encode('UTF-8')[0];

  given $mode {
    when 'octal' {
      return "\\" ~ sprintf("%03o", $byte);
    }
    when 'hex' {
      return "\\x" ~ sprintf("%02x", $byte);
    }
    when 'escape' {
      # 只转义必要的字符
      given $char {
        when '"'  { return '\\"'; }
        when "'"  { return "\\'"; }
        when '\\' { return '\\\\'; }
        when "\n" { return '\\n'; }
        when "\t" { return '\\t'; }
        when "\r" { return '\\r'; }
        when "\0" { return '\\0'; }
        default   { return $char; }
      }
    }
    default {
      die "Unknown translation mode: $mode";
    }
  }
}

my $content = $markdown-file.IO.slurp;

my %config = parse-config($content);
my $translation-mode = %config<translation> // 'octal';

my $in-code-block = False;
for $content.lines -> $line {
  if $line ~~ /^ '```' / {
    # 遇到代码块的开始或结束标记
    $in-code-block = !$in-code-block;
    next; # 跳过代码块标记行
  }

  if $in-code-block {
    # 如果在代码块内部，处理每一行
    for $line.comb -> $char {
      print convert-char($char, $translation-mode);
    }
    print convert-char("\n", $translation-mode); # 处理换行符
  }
}
