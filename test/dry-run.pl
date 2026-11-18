#!/usr/bin/env perl
# SPDX-License-Identifier: GPL-3.0-or-later
# 在临时配置和假命令中验证 Dry Run；可单独运行，也可由 cli.pl 加载。
use v5.32;
use utf8;
use Test::More;
use File::Temp qw(tempdir);
use open qw(:std :encoding(UTF-8));

my $chsrc = $^O eq 'MSWin32' ? '.\\chsrc' : './chsrc';
my $root = tempdir (CLEANUP => 1);
my $bin = "$root/bin";
my $cargo_home = "$root/cargo-home";
mkdir $bin or die "$bin: $!";
mkdir $cargo_home or die "$cargo_home: $!";

for my $program (qw(npm yarn pnpm cargo)) {
    my $path = "$bin/$program" . ($^O eq 'MSWin32' ? '.bat' : '');
    open (my $fh, '>:raw', $path) or die "$path: $!";
    if ($^O eq 'MSWin32') {
        print $fh "\@echo off\r\n";
        print $fh qq{if "%~1"=="--version" (echo 1.22.22 & exit /b 0)\r\n} if $program eq 'yarn';
        print $fh "echo executed>>\"%CHSRC_DRY_MARKER%\"\r\nexit /b 0\r\n";
    } else {
        print $fh "#!/bin/sh\n";
        print $fh qq{if [ "\$1" = --version ]; then echo 1.22.22; exit 0; fi\n} if $program eq 'yarn';
        print $fh "printf executed >> \"\$CHSRC_DRY_MARKER\"\nexit 0\n";
    }
    close $fh or die "$path: $!";
    chmod 0755, $path unless $^O eq 'MSWin32';
}

sub dry_cli {
    my ($args, $expected_status, $pattern) = @_;
    my ($command, $arguments) = split / /, $args, 2;
    my $output = `$chsrc $command -no-color $arguments 2>&1`;
    my $status = $? >> 8;
    utf8::decode ($output);
    is ($status, $expected_status, "$args: exit status");
    like ($output, $pattern, "$args: output");
    return $output;
}

{
    local $ENV{PATH} = $bin . ($^O eq 'MSWin32' ? ';' : ':') . $ENV{PATH};
    local $ENV{CARGO_HOME} = $cargo_home;
    local $ENV{CHSRC_DRY_MARKER} = "$root/executed";

    my $output = dry_cli ('set -dry npm', 0, qr/npm config set registry.*Dry Run: 未做任何修改/s);
    unlike ($output, qr/全自动换源完成/, 'set -dry npm: no false success');
    dry_cli ('reset -dry npm', 0, qr{https://registry\.npmjs\.org/.*Dry Run: 未做任何修改}s);

    for my $command (qw(get g measure mea m cesu ce c list ls issue)) {
        dry_cli ("$command -dry npm", 2, qr/-dry 仅适用于 `set` 和 `reset` 命令/);
    }
    for my $command (qw(version ver -v --version)) {
        dry_cli ("$command -dry", 0, qr/Copyright/);
    }
    for my $command (qw(help h -h --help)) {
        dry_cli ("$command -dry", 0, qr/选项:/);
    }

    my $combo = dry_cli ('set -dry js', 0, qr/复用 Dry Run 中已选定的源/);
    my @registries = $combo =~ m{(?:set registry|set npmRegistryServer) (https?://\S+)}g;
    is (scalar @registries, 3, 'combo previews all three registries');
    is_deeply (\@registries, [('https://registry.npmmirror.com/') x 3],
               'combo reuses the same source without speed measurement');

    my $config = "$cargo_home/config.toml";
    my @configs = (
        "[build]\njobs = 2\n",
        "[source.crates-io]\nreplace-with = \"tuna\"\n",
        "[source.crates-io]\nreplace-with = \"tuna\"\n[source.tuna]\nother = 1\n",
        "[source.crates-io]\nreplace-with = \"tuna\"\n[source.tuna]\nregistry = \"sparse+https://example.invalid/index/\"\n",
    );
    for my $i (0 .. $#configs) {
        open (my $fh, '>:raw', $config) or die "$config: $!";
        print $fh $configs[$i];
        close $fh or die "$config: $!";
        dry_cli ('set -dry cargo', 0, qr/Dry Run: 未做任何修改/);
        ok (-f $config, "cargo branch $i: config survives");
        my $after = '';
        if (open (my $read, '<:raw', $config)) {
            local $/;
            $after = <$read>;
            close $read;
        }
        is ($after, $configs[$i], "cargo branch $i: bytes unchanged");
        ok (! -e "$config.bak", "cargo branch $i: no backup created");
    }
    unlink $config or die "$config: $!";
    dry_cli ('set -dry cargo', 0, qr/Dry Run: 未做任何修改/);
    ok (! -e $config, 'cargo dry run does not create a missing config');

    local $ENV{CARGO_HOME} = "$root/missing-cargo-home";
    dry_cli ('set -dry cargo', 0, qr/Dry Run: 未做任何修改/);
    ok (! -e $ENV{CARGO_HOME}, 'cargo dry run does not create a missing directory');
    ok (! -e $ENV{CHSRC_DRY_MARKER}, 'dry run never executes package manager mutations');
}

done_testing unless caller;
1;
