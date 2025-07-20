# ---------------------------------------------------------------
# SPDX-License-Identifier: GPL-3.0-or-later
# ---------------------------------------------------------------
# File Name     : Version.rakumod
# File Authors  : Aoran Zeng <ccmywish@qq.com>
# Contributors  :  Nul None  <nul@none.org>
# Created On    : <2025-07-14>
# Last Modified : <2025-07-16>
# ---------------------------------------------------------------

unit module Rawstr4c::Version;

constant VERSION = "0.2.1.0";
constant RELEASE_DATE = "2025/07/16";

constant Maintain_URL = "https://github.com/RubyMetric/chsrc/blob/dev/tool/rawstr4c";
constant Maintain_URL2 = "https://gitee.com/RubyMetric/chsrc/blob/dev/tool/rawstr4c";

constant VERSION_CONTENT_FOR_-version = qq:to/EOF/
  rawstr4c {VERSION}
  Copyright (C) 2025 Aoran Zeng
  License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>
  This is free software: you are free to change and redistribute it.
  There is NO WARRANTY, to the extent permitted by law.

  Written by Aoran Zeng.
  EOF
