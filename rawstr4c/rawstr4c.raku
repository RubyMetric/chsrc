#!/usr/bin/env raku
# ---------------------------------------------------------------
# File Name     : rawstr4c.raku
# File Authors  : Aoran Zeng <ccmywish@qq.com>
# Contributors  :  Nul None  <nul@none.org>
# Created On    : <2025-07-12>
# Last Modified : <2025-07-12>
#
# Generate raw strings for C programming language
#
# ---------------------------------------------------------------
# 全局设置称为 Global config，其他均称为 section config
# 每一部分称为 section，我们要处理的input，都在 code block 里，我们简称为 block
# ---------------------------------------------------------------

unless @*ARGS {
  die "Usage: rawstr4c <FILE.md>\n";
}

my $markdown-file = @*ARGS[0];

unless $markdown-file.IO.e {
  die "Error: File '$markdown-file' not found.\n";
}

# 根据转换模式处理字符
sub convert-char($char, $mode) {
  given $mode {
    when 'oct' {
      my $bytes = $char.encode('UTF-8');
      return $bytes.map({ "\\" ~ sprintf("%03o", $_) }).join('');
    }
    when 'hex' {
      my $bytes = $char.encode('UTF-8');
      return $bytes.map({ "\\x" ~ sprintf("%02x", $_) }).join('');
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

# 处理字符串转换
sub process-content($content, $mode) {
  my $result = "";
  for $content.comb -> $char {
    $result ~= convert-char($char, $mode);
  }
  return $result;
}

# 生成变量名
sub generate-variable-name($global-config, $section-config, $title) {
  my $prefix = $global-config<prefix> // "_rawstr4c";
  my $postfix = $global-config<postfix> // "";

  # 处理前缀
  $prefix = $prefix.subst(/^'`'/, '').subst(/'`'$/, '');

  # 处理后缀
  if $postfix {
    $postfix = $postfix.subst(/^':'/, '');
    if $section-config<language> {
      my $lang = $section-config<language>.subst(/^'`'/, '').subst(/'`'$/, '');
      $postfix = $postfix.subst('use-language', "in_$lang");
    } else {
      $postfix = $postfix.subst('use-language', '');  # 默认为无语言
    }
  }

  # 生成的变量名称
  my $name = $section-config<name> // $title.lc;
  $name = $name.subst(/^'`'/, '').subst(/'`'$/, '');
  # 处理标题中包含的空格
  $name = $name.subst(/\s+/, '_', :g);

  my $var-name = $prefix;
  if $name {
    $var-name ~= "_" ~ $name;
  }

  if $postfix {
    $var-name ~= "_" ~ $postfix;
  }

  return $var-name;
}

#`( 真正的 main 流程开始
)
my $content = $markdown-file.IO.slurp;
my @lines = $content.lines;

my %global-config;
my @sections;
my $current-section;
my $in-global = True;
my $in-code-block = False;
my $current-code = "";

for @lines -> $line {
  # 检查标题级别 (注意，要确保不在代码块内)
  if !$in-code-block && $line ~~ /^ '#' ('#'*) \s* (.+) / {
    my $level = 1 + $0.chars;
    my $title = ~$1;

    # 如果之前有代码块，保存
    if $current-code && $current-section && $current-section<title> {
      $current-section<code> = $current-code;
      @sections.push: $current-section;
    }

    # 重置代码块
    $current-code = "";

    # 一级标题后面是全局配置，其他级别标题开始新的section
    if $level == 1 {
      $in-global = True;
      $current-section = {};
    } else {
      $in-global = False;
      $current-section = {
        title => $title,
        level => $level,
        config => {},
      };
    }
    next;
  }

  # 解析配置项
  if $line ~~ /^ '-' \s* (<[a..z\-]>+) \s* '=' \s* '`' (.+?) '`' / {
    my $key = ~$0;
    my $value = ~$1;

    if $in-global {
      %global-config{$key} = $value;
    } elsif $current-section {
      $current-section<config>{$key} = $value;
    }
    next;
  }

  # 处理代码块，即真正想要转换的 raw string
  # 检测代码块语言并记录
  if $line ~~ /^ '```' (.*)? / {
    if $in-code-block {
      # 代码块结束
      $in-code-block = False;
    } else {
      # 代码块开始，记录语言
      $in-code-block = True;
      my $lang = ~($0 // '');
      if $lang && $current-section {
        # 如果代码块指定了语言，且当前section没有language配置，则自动设置
        unless $current-section<config><language> {
          $current-section<config><language> = $lang;
        }
      }
    }
    next;
  }

  if $in-code-block {
    $current-code ~= $line ~ "\n";
  }
}

# 保存最后一个代码块
if $current-code && $current-section && $current-section<title> {
  $current-section<code> = $current-code;
  @sections.push: $current-section;
}

# 解析完毕，最后输出结果
say "Global config:";
for %global-config.kv -> $k, $v {
  say "  $k = $v";
}
say "";

say "Found " ~ @sections.elems ~ " sections:";
for @sections -> $section {
  say "Section: " ~ $section<title>;
}
say "";

for @sections -> $section {
  say "=== Section: " ~ $section<title> ~ " ===";

  # 确定转换模式 (section config 优先，否则使用 global config)
  my $translate = $section<config><translate> // %global-config<translate> // ':oct';
  $translate = $translate.subst(/^':'/, '');

  # 生成变量名
  my $var-name = generate-variable-name(%global-config, $section<config>, $section<title>);

  say "Variable name: $var-name";
  say "Translation mode: $translate";

  my $language = $section<config><language>;
  say "Language: $language";

  if $section<code> {
    say '';
    say 'char ' ~ $var-name ~ '[] = "' ~ process-content($section<code>, $translate) ~ '";';
  }
  say "\n";
}
