# ---------------------------------------------------------------
# SPDX-License-Identifier: GPL-3.0-or-later
# ---------------------------------------------------------------
# File Name     : Parser.rakumod
# File Authors  : Aoran Zeng <ccmywish@qq.com>
# Contributors  :  Nul None  <nul@none.org>
# Created On    : <2025-07-12>
# Last Modified : <2025-07-16>
#
# rawstr4c.md parsing
# ---------------------------------------------------------------

unit module Parser;

#| 不能用 Bool，只能用 Boolean
my enum ConfigItem's-ValueType < RS4C-Nil RS4C-String RS4C-Mode RS4C-Bool>;

#| 配置项的值
class ConfigItem's-Value {
  has ConfigItem's-ValueType $.type;
  has Str                    $.raw-value;
  has Any                    $.parsed-value;

  #| $raw-text 为 undefined 的情况只有一种，那就是内部刻意生成
  method new(Str $input-text) {
    my $type;
    my $parsed-value;

    my $raw-value = $input-text;

    # 明确区分空字符串和无值情况
    # 这种情况不可能是用户写的(并没有nil这个字面量)
    if ! $input-text.defined {
      $type = RS4C-Nil;
      $parsed-value = Nil;
      $raw-value = "<internal-rs4c-nil>"; # 一个完全用不到的值，但是由于 $.raw-value 类型是字符串，所以必须随便给一个值
    }
    else {
      # wrtd: 不要试图在这里利用 given when 统一处理未定义的值，因为会一直报错
      given $input-text {
        when /^ ':' (.+) $/ {
          # 模式值 :mode
          $type = RS4C-Mode;
          $parsed-value = ~$0;
        }
        when /^ ('true'|'false'|'yes'|'no') $/ {
          # 特殊字面量 - true/false/yes/no 都是 literal
          $type = RS4C-Bool;
          $parsed-value = ~$0 ~~ /^('true'|'yes')$/ ?? True !! False;
        }
        # 输入为空时被当做是字符串类型
        default {
          # 普通字符串
          $type = RS4C-String;
          $parsed-value = $input-text;
        }
      }
    }
    self.bless(:$type, :$raw-value, :$parsed-value);
  }

  # 获得适合调用者接受的值
  method value() {
    given $.type {
      when RS4C-Nil | RS4C-String | RS4C-Bool | RS4C-Mode  { return $.parsed-value; }
      default { die "Unknown config value type: {$.type}"; }
    }
  }

  # 这些函数防止开发者写错类型
  method nil-value() {
    return self.value if $.type == RS4C-Nil;
    die "The config value type is not RS4C-Nil, but: {$.type}";
  }

  method string-value() {
    return self.value if $.type == RS4C-String;
    die "The config value type is not RS4C-String, but: {$.type}";
  }

  method bool-value() {
    return self.value if $.type == RS4C-Bool;
    die "The config value type is not RS4C-Bool, but: {$.type}";
  }

  method mode-value() {
    return self.value if $.type == RS4C-Mode;
    die "The config value type is not RS4C-Mode, but: {$.type}";
  }


  # 类型检查方法
  method is-nil()  { return $.type == RS4C-Nil; }
  method is-mode() { return $.type == RS4C-Mode; }
  method is-bool() { return $.type == RS4C-Bool; }
  method is-string() { return $.type == RS4C-String; }
}



#| 包含所有 config items 的容器
#| 所有 get 出的值一定是 ConfigItem's-Value 实例
my class ConfigBlock {

  has %!items;

  # 如果非要在程序内部中调用，而不是直接从 Markdown 文件中读取出来
  # 一定要记得 $raw-value 用的是 rawstr4c 的语法！也就是说，这里一定是一个字符串
  method set($k, $raw-value) {
    %!items{$k} = ConfigItem's-Value.new($raw-value);
  }

  method get($k) {
    return %!items{$k};
  }

  method exist($k) {
    return %!items{$k}:exists;
  }

  # 配置项名称
  # @danger: 把这个函数命名为 items，会让我的机器蓝屏.....
  method keys() {
    return %!items.keys;
  }
}


#| 表示一个 section
class Section {

  has Str         $.title;
  has Int         $.level;
  has ConfigBlock $.configblock;
  has Str         $.codeblock is rw;
  has Section     $.parent is rw;
  has Section     @.children;


  method new($title, $level) {
    my $configblock = ConfigBlock.new();
    # parent 和 codeblock 刻意不初始化
    self.bless(:$title, :$level, :$configblock, :children([]));
  }

  method add-child($child-section) {
    $child-section.parent = self;
    @.children.push: $child-section;
  }

  method has-children() {
    return @.children.elems > 0;
  }

  # 递归获取所有后代section（深度优先遍历）
  method get-all-descendants() {
    my @descendants;
    for @.children -> $child {
      @descendants.push: $child;
      @descendants.append: $child.get-all-descendants();
    }
    return @descendants;
  }

  # 获取section的路径（从根到当前节点）
  method get-hierarchical-path() {
    my @path;
    my $current = self;
    while $current {
      @path.unshift: $current.title;
      $current = $current.parent;
    }
    return @path.join(" > ");
  }
}


#|(
所有内容都是 section:

  - level 0:  root section 无标题
  - level 1:  #  一级标题
  - level 2:  ## 二级标题
  - ...
)
class Parser {
  has Str $.input-file is rw;
  #| 所有sections的扁平数组，已经是深度遍历的了
  has Section @!sections;

  #| $markdown-file 必须是一个存在的 markdown 文件路径
  method new($markdown-file) {
    self.bless(:input-file($markdown-file));
  }

  # 获取根section（level 0）
  method root-section() {
    return @!sections.first({ $_.level == 0 });
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


  method parse() {
    my $content = $.input-file.IO.slurp;
    my @lines = $content.lines;

    my $current-section;
    my $in-codeblock = False;

    # 在代码块中的 raw string
    my $rawstr = "";

    # 无论有没有具体的 root 信息 (比如所处理的文件第一行就是标题)，
    # 都创建一个 root section (level 0)
    $current-section = Section.new("", 0);
    @!sections.push: $current-section;

    # 开始遍历
    my $line-count = 0;
    for @lines -> $line {
      $line-count++;

      # Step1: 处理标题，这里最重要，因为是判断上一个 section 结束的标志
      if !$in-codeblock && $line ~~ /^ '#' ('#'*) \s* (.+) / {
        my $level = 1 + $0.chars;
        my $title = ~$1;

        # 保存当前section的codeblock
        $current-section.codeblock = $rawstr;

        # 准备创建一个新的 section
        $rawstr = "";
        my $new-section = Section.new($title, $level);
        @!sections.push: $new-section;

        # 找到合适的父节点
        my $parent = self.find-parent-section($level);

        if $parent {
          $parent.add-child($new-section);
        }

        $current-section = $new-section;
        next;
      }

      # Step2: 处理配置项 (如果该行不是配置项则下一行)
      if self.parse-config-item-line($line, $current-section.configblock) {
        next;
      }

      # Step3: 开始处理 codeblock
      if $line ~~ /^ '```' (.*)? / {
        if $in-codeblock {
          $in-codeblock = False;
        } else {
          $in-codeblock = True;
          my $lang = ~($0 // '');
          if $lang && $current-section && !$current-section.configblock.exist('language') {
            $current-section.configblock.set('language', $lang);
          }
        }
        next;
      }

      # 代码块里的内容统统进来
      if $in-codeblock {
        $rawstr ~= $line ~ "\n";
      }
    }

    # 遍历结束，保存最后一个section的codeblock
    if $rawstr && $current-section {
      $current-section.codeblock = $rawstr;
    }
  }

  method find-parent-section($new-level) {
    # 从@!sections尾部向前找，找到第一个level小于new-level的section作为父节点
    for @!sections.reverse -> $section {
      if $section.level < $new-level {
        return $section;
      }
    }
    return Nil;  # 没有找到父节点，说明是 root section
  }



  # 调试方法：扁平打印所有sections
  method debug-print-sections-flatly() {
    say "====== sections ======";
    for @!sections.kv -> $i, $section {
      my $title = $section.title || "(Root)";
      my $has-config = $section.configblock.keys ?? "有配置" !! "无配置";
      my $has-code = $section.codeblock ?? "有代码" !! "无代码";
      say "  [$i] Level {$section.level}: $title - $has-config, $has-code";
    }
  }

  # 调试方法：层级打印sections
  method debug-print-sections-hierarchyly() {
    say "====== hierarchy ======";

    my $indent = 0;

    # 嵌套的格式化函数
    my sub format-section($section, $level) {
      my $prefix = '  ' x $level;
      my $title = $section.title // '(Root)';
      my $base-info = "{$prefix}- {$title} (level {$section.level})";

      my $config-info = "";
      if $section.configblock.keys {
        my @config-items;
        for $section.configblock.keys -> $key {
          my $value = $section.configblock.get($key);
          @config-items.push: "$key = {$value.raw-value}";
        }
        $config-info = "\n" ~ "{$prefix}  [" ~ @config-items.join(", ") ~ "]";
      }
      return $base-info ~ $config-info;
    }

    # 嵌套的递归打印函数
    my sub print-section-tree($section, $level) {
      say format-section($section, $level);

      if $section.has-children {
        for $section.children -> $child {
          print-section-tree($child, $level + 1);
        }
      }
    }

    my $root = self.root-section();
    print-section-tree($root, $indent);
  }

  # 调试方法：完整的调试信息打印
  method debug-print-summary() {
    self.debug-print-sections-flatly();
    self.debug-print-sections-hierarchyly();
  }
}
