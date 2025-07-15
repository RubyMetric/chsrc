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
my enum ConfigItemValueType <String Mode Boolean>;

#| 配置项的值
my class ConfigItemValue {
  has ConfigItemValueType $.type;
  has Str                 $.raw-value;
  has Any                 $.parsed-value;

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

  has %!items;

  # 如果非要在程序内部中调用，而不是直接从 Markdown 文件中读取出来
  # 一定要记得 $raw-value 用的是 rawstr4c 的语法！也就是说，这里一定是一个字符串
  method set($k, $raw-value) {
    %!items{$k} = ConfigItemValue.new($raw-value);
  }

  method get($k, $default = Nil) {
    return %!items{$k} // ($default ?? ConfigItemValue.new($default) !! ConfigItemValue.new(''));
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
my class Section {

  has Str $.title;
  has Int $.level;
  has Config $.config;
  has Str $.codeblock is rw;
  has Section $.parent is rw;
  has Section @.children;


  method new($title, $level, $config = Config.new(), Section $parent?) {
    self.bless(:$title, :level($level), :config($config), :$parent, :children([]));
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
  has IO::Path $.input-file is rw;
  #| 所有sections的扁平数组，已经是深度遍历的了
  has Section @!sections;

  method new($input-file) {
    self.bless(:$input-file);
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
    my $content = $.input-file.slurp;
    my @lines = $content.lines;

    my $current-section;
    my $in-codeblock = False;

    # 在代码块中的 raw string
    my $rawstr = "";

    # 无论有没有具体的 root 信息 (比如所处理的文件第一行就是标题)，
    # 都创建一个 root section (level 0)
    my $root-config = Config.new();
    $current-section = Section.new("", 0, $root-config);
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
        my $new-section = Section.new($title, $level, Config.new());
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
      if self.parse-config-item-line($line, $current-section.config) {
        next;
      }

      # Step3: 开始处理 codeblock
      if $line ~~ /^ '```' (.*)? / {
        if $in-codeblock {
          $in-codeblock = False;
        } else {
          $in-codeblock = True;
          my $lang = ~($0 // '');
          if $lang && $current-section && !$current-section.config.exist('language') {
            $current-section.config.set('language', $lang);
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
      my $has-config = $section.config.keys ?? "有配置" !! "无配置";
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
      return "{$prefix}- {$title} (level {$section.level})";
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
