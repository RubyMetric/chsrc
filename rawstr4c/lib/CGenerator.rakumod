#!/usr/bin/env raku
# ---------------------------------------------------------------
# File Name     : CGenerator.rakumod
# File Authors  : Aoran Zeng <ccmywish@qq.com>
# Contributors  :  Nul None  <nul@none.org>
# Created On    : <2025-07-12>
# Last Modified : <2025-07-13>
#
# Generates C code from raw string
# ---------------------------------------------------------------

unit module CGenerator;

use ConfigParser;

class CharConverter {

  method convert-char($char, $mode) {
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
      default { die "Unknown translation mode: $mode"; }
    }
  }

  method convert-content($content, $mode) {
    my $result = "";
    for $content.comb -> $char {
      $result ~= self.convert-char($char, $mode);
    }
    return $result;
  }
}


class VariableNameGenerator {

  method generate($global-config, $section-config, $title) {

    # 检查 name-literally
    my $name-literally = $section-config.get('name-literally');
    if $name-literally.is-bool() && $name-literally.as-bool() {
      return $section-config.get('name', $title.lc).as-string();
    }

    my $prefix = $global-config.get('prefix', '_rawstr4c').as-string();
    my $language = $section-config.get('language').as-string();
    my $postfix = self.resolve-postfix($global-config, $language);

    my $keep-prefix = $section-config.get('keep-prefix', 'true').as-bool();
    my $keep-postfix = $section-config.get('keep-postfix', 'true').as-bool();

    my $name = $section-config.get('name', $title.lc).as-string();
    $name = $name.subst(/\s+/, '_', :g);

    # 组装变量名
    my $var-name = "";
    $var-name ~= $prefix if $keep-prefix && $prefix;
    $var-name ~= "_" if $var-name && $name;
    $var-name ~= $name if $name;
    $var-name ~= "_" if $var-name && $postfix && $keep-postfix;
    $var-name ~= $postfix if $postfix && $keep-postfix;

    return $var-name || "unnamed_var";
  }

  method resolve-postfix($global-config, $language) {
    my $postfix = $global-config.get('postfix');

    if $postfix.is-mode() && $postfix.as-mode() eq 'use-language' {
      return $language ?? 'in_' ~ $language !! '';
    }
    # 如果不是模式，那就是用户给了一个具体的字符串
    return $postfix.as-string();
  }
}
