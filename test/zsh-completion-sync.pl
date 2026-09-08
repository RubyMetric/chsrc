#!/usr/bin/env perl
# -----------------------------------------------------------------------------
# SPDX-License-Identifier: GPL-3.0-or-later
#
# Check that the native Zsh completion contains every alias of every dish
# registered in src/recipe/menu.c.  Internal sources-only dishes are not menu
# entries and therefore must not be offered to users.
# -----------------------------------------------------------------------------

use strict;
use warnings;
use FindBin qw($Bin);
use File::Find qw(find);

my $root = "$Bin/..";
my $menu_path = "$root/src/recipe/menu.c";
my $completion_path = "$root/tool/completion/_chsrc";

open my $menu_fh, '<', $menu_path or die "Cannot read $menu_path: $!\n";
my @registered;
my $category;
while (my $line = <$menu_fh>) {
    if ($line =~ /^#define add\(t\).*&((?:pl|os|wr))_##t##_dish/) {
        $category = $1;
        next;
    }
    if ($line =~ /^#undef add/) {
        undef $category;
        next;
    }
    if (defined $category && $line =~ /\badd\s*\(\s*([A-Za-z0-9_]+)\s*\)/) {
        push @registered, "${category}_$1";
    }
}
close $menu_fh;

my %aliases_for;
find(
    sub {
        return unless -f $_ && /\.c\z/;
        open my $fh, '<', $File::Find::name
            or die "Cannot read $File::Find::name: $!\n";
        local $/;
        my $source = <$fh>;
        close $fh;
        while ($source =~ /def_(?:combo_)?dish\s*\(\s*([A-Za-z0-9_]+)\s*,\s*"([^"]+)"\s*\)/g) {
            $aliases_for{$1} = [split m{/}, $2];
        }
    },
    "$root/src/recipe"
);

my @errors;
my %expected;
my %menu_category = (pl => 'lang', os => 'os', wr => 'ware');
for my $dish (@registered) {
    if (!exists $aliases_for{$dish}) {
        push @errors, "registered dish $dish has no def_dish/def_combo_dish declaration";
        next;
    }
    my ($prefix) = $dish =~ /^(pl|os|wr)_/;
    for my $alias (@{$aliases_for{$dish}}) {
        $expected{$alias} = $menu_category{$prefix};
    }
}

open my $completion_fh, '<', $completion_path
    or die "Cannot read $completion_path: $!\n";
my %completed;
my $completion_category;
while (my $line = <$completion_fh>) {
    if ($line =~ /^dishes_(lang|os|ware)=\(/) {
        $completion_category = $1;
        next;
    }
    if (defined $completion_category && $line =~ /^\)/) {
        undef $completion_category;
        next;
    }
    if (defined $completion_category && $line =~ /^\s*'([^':]+):/) {
        $completed{$1} = $completion_category;
    }
}
close $completion_fh;

for my $alias (sort keys %expected) {
    if (!exists $completed{$alias}) {
        push @errors, "completion is missing registered dish alias '$alias'";
    } elsif ($completed{$alias} ne $expected{$alias}) {
        push @errors, "completion puts '$alias' in $completed{$alias}, expected $expected{$alias}";
    }
}

for my $alias (sort keys %completed) {
    push @errors, "completion contains unregistered dish alias '$alias'"
        unless exists $expected{$alias};
}

die join("\n", @errors) . "\n" if @errors;
print "Zsh completion dish aliases match the registered menus.\n";
