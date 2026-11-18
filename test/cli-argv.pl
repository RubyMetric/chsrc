#!/usr/bin/env perl
# SPDX-License-Identifier: GPL-3.0-or-later
use v5.32;
use utf8;
use Encode qw(encode);
use Test::More;

my @payload = ('--help', '--dry', '--', '--skfw sq', 'sdf', 'wiw', 'www', '',
    '他说 "你好" 😀', '日本語 한국어 𠀀', '👩🏽‍💻',
    'C:\\path with spaces\\', 'a\\"b', 'a\\\\"b', 'a""b', '"quoted"',
    "line1\nline2", "tab\tvalue");
my @bytes = map { encode ('UTF-8', $_) } @payload;
open (my $child, '-|', './cli-parser-test', 'set', 'probe', '--', @bytes) or die $!;
local $/;
my $output = <$child>;
close $child;
is ($? >> 8, 0, 'argv probe exited successfully');
my $expected = scalar (@bytes) . "\n" . join ('', map { unpack ('H*', $_) . "\n" } @bytes);
is ($output, $expected, 'preserves argument boundaries, empty arguments, quotes and UTF-8');
done_testing;
