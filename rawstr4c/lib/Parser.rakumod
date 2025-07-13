#!/usr/bin/env raku
# ---------------------------------------------------------------
# File Name     : Parser.rakumod
# File Authors  : Aoran Zeng <ccmywish@qq.com>
# Contributors  :  Nul None  <nul@none.org>
# Created On    : <2025-07-12>
# Last Modified : <2025-07-13>
#
# rawstr4c.md parsing
# ---------------------------------------------------------------

unit module Parser;

#| 不能用 Bool，只能用 Boolean
my enum ConfigValueType <String Mode Boolean>;

#| 配置项的值
my class ConfigValue {
  has ConfigValueType $.type;
  has $.raw-value;
  has $.parsed-value;

  method new($raw-text) {
    my $type;
    my $parsed;

    given $raw-text {
      when /^ ':' (.+) $/ {
        # 模式值 :mode
        $type = Mode;
        $parsed = ~$0;
      }
      when /^ ('true'|'false'|'yes'|'no') $/ {
        # 特殊字面量 - true/false/yes/no 都是 literal
        $type = Boolean;
        $parsed = ~$0 ~~ /^('true'|'yes')$/ ?? True !! False;
      }
      default {
        # 普通字符串
        $type = String;
        $parsed = $raw-text;
      }
    }

    self.bless(:$type, :raw-value($raw-text), :parsed-value($parsed));
  }

  method as-string() {
    return $.parsed-value.Str;
  }

  method as-bool() {
    given $.type {
      when Boolean { return $.parsed-value; }
      when String  {
        # 尝试将字符串解析为布尔值
        return $.parsed-value ~~ /^('true'|'yes')$/;
      }
      default { return False; }
    }
  }

  # 获取模式值（去掉冒号前缀）
  method as-mode() {
    return $.type == Mode ?? $.parsed-value !! $.raw-value;
  }

  # 类型检查方法
  method is-mode() { return $.type == Mode; }
  method is-bool() { return $.type == Boolean; }
  method is-string() { return $.type == String; }
}



#| 包含所有 config items 的容器
my class Config {

  has %.items;

  method set($k, $raw-value) {
    %.items{$k} = ConfigValue.new($raw-value);
  }

  method get($k, $default = Nil) {
    return %.items{$k} // ($default ?? ConfigValue.new($default) !! ConfigValue.new(''));
  }

  method exist($k) {
    return %.items{$k}:exists;
  }

  # 配置项名称
  # @danger: 把这个函数命名为 items，会让我的机器蓝屏.....
  method keys() {
    return %.items.keys;
  }
}

#|( 仅存在两个域:

  1. Global dom
  2. Section dom

我们要求，在 Global dom 里，只存在配置，不存在 code block. 而 code block 只能在 Section dom 中存在。

因此，Parser 解析完毕后将包含:
  - $global-config
  - @sections (多个 $section)

一个 $section 是 Hash，其包含:
  - title
  - level
  - raw-string
  - config
)
class Parser {
  has $.global-config;
  has @.sections;

  method new() {
    self.bless(
      global-config => Config.new(),
      sections => []
    );
  }

  # 配置项所在行 -> 解析为配置项
  method parse-config-item-line($line, $section-config) {
    # 语法: - key = `value`
    if $line ~~ /^ '-' \s* (<[a..z\-]>+) \s* '=' \s* '`' (.+?) '`' / {
      my $key = ~$0;
      my $value = ~$1;
      $section-config.set($key, $value);
      return True;
    }
    return False;
  }

  method parse($content) {
    my @lines = $content.lines;

    my $current-section;
    my $current-section-config = Config.new();
    my $in-global = True;
    my $in-code-block = False;

    # 在代码块中的 raw string
    my $rawstr = "";


    # 开始遍历
    for @lines -> $line {

      # Step1: 记录层次 level
      #
      # @note 我们要避免，在代码块中也有 # 字符，比如在代码块里写的是 shell 脚本
      if !$in-code-block && $line ~~ /^ '#' ('#'*) \s* (.+) / {
        my $level = 1 + $0.chars;
        my $title = ~$1;

        # 只有匹配到下一个标题时，才说明前一个 section 已经结束，此时才有机会存下来
        # Global dom 里是没有 raw string 的，所以被这里的条件排除掉了
        if $rawstr && $current-section && $current-section<title> {
          $current-section<raw-string> = $rawstr;
          $current-section<config> = $current-section-config;
          @.sections.push: $current-section;
        }

        $rawstr = "";

        if $level == 1 {
          $in-global = True;
          $current-section = {};
          $current-section-config = Config.new();
        } else {
          $in-global = False;
          $current-section = {
            title => $title,
            level => $level,
          };
          $current-section-config = Config.new();
        }
        next;
      }

      # Step2: 处理配置项
      if $in-global {
        if self.parse-config-item-line($line, $.global-config) {
          next;
        }
      } elsif $current-section {
        if self.parse-config-item-line($line, $current-section-config) {
          next;
        }
      }

      # Step3: 开始处理raw string
      if $line ~~ /^ '```' (.*)? / {
        if $in-code-block {
          $in-code-block = False;
        } else {
          $in-code-block = True;
          my $lang = ~($0 // '');
          if $lang && $current-section && !$current-section-config.exist('language') {
            $current-section-config.set('language', $lang);
          }
        }
        next;
      }

      # 代码块里的内容统统进来
      if $in-code-block {
        $rawstr ~= $line ~ "\n";
      }
    }

    # 遍历结束, 这意味着文件已经阅读完毕，最后一个section还没存，现在存它
    if $rawstr && $current-section && $current-section<title> {
      $current-section<raw-string> = $rawstr;
      $current-section<config> = $current-section-config;
      @.sections.push: $current-section;
    }
  }

  method debug-info() {
    say "Global config:";
    for $.global-config.keys.sort -> $item {
      my $value = $.global-config.get($item);
      say "  $item = {$value.as-string} (type: {$value.type})";
    }
    say "";

    say "Found " ~ @.sections.elems ~ " sections:";
    for @.sections -> $section {
      say "Section: " ~ $section<title>;
    }
    say "";
  }
}
