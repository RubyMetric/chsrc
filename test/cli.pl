#!/usr/bin/env perl
# ---------------------------------------------------------------
# Test File     : cli.pl
# Test Authors  : @ccmywish
# Contributors  : @Mikachu2333
# Created On    : <2024-06-05>
# Last Modified : <2026-10-28>
#
# 测试 chsrc 可执行文件
#
#    $ perl test/cli.pl
#
#    $ perl test/cli.pl fastcheck
# ---------------------------------------------------------------

=encoding utf8

本文件可以使用

    $ podchecker .test\cli.pl

来检查 Pod 语法

    $ pod2html .\test\cli.pl > CLI.html

来生成 HTML 文件

    $ pod2markdown .\test\cli.pl > CLI.md

来生成 Markdown 文件

=over

=item C<=end> 的时候前面必须给一个空行，否则 podchecker 不认

=item 每一个 Pod block 必须用 C<=cut> 来终结，否则 VS Code 会把后续内容全部认为是 Pod

=back
=cut


use v5.42;
use utf8;
# v5.38 还不用强制 utf8，现在代码里（包括注释）只要有其他字符，都必须强制使用 utf8
use File::Temp qw(tempdir);
use Test::More;
# `` 执行命令以后，返回的全部是字节流，而不是字符串，
# chsrc 已经输出的是 UTF-8 字符串了，所以我们在这里需要
# 设置该选项，将这些字节流编码为 UTF-8 才能得到我们想要的结果
use open qw(:std :encoding(UTF-8));

=begin comment

注意:

    `./chsrc`

在 Windows 上也是可以正常执行的，Perl 应该是内部进行了转换。然而，下面这一行却会报错：

    `./chsrc get -no-color 2>&1`

2>&1重定向虽然在 Windows CMD 中是正确的，但是在 Perl 中执行，却会反而把 ./chsrc 的问题给报出来。
因此，我们需要对执行的命令进行处理

=end comment
=cut

my $CHSRC = ($^O eq 'MSWin32') ? '.\chsrc' : './chsrc';

my $version_str_cn = qr|chsrc .*\n\nCopyright .*\n许可证 GPLv3\+：GNU GPL 第 3 版或更高版本|;
like `$CHSRC -v`,         $version_str_cn,  'chsrc -v';
like `$CHSRC --version`,  $version_str_cn,  'chsrc --version';
like `$CHSRC version`,    $version_str_cn,  'chsrc version';

my $version_str_en = qr|chsrc .*\n\nCopyright .*\nLicense GPLv3\+: GNU GPL version 3 or later|;
like `$CHSRC -v -en`,     $version_str_en,  'chsrc -v -en';
like `$CHSRC -en -v`,     $version_str_en,  'chsrc -en -v';
like `$CHSRC --version -en`,  $version_str_en,  'chsrc --version -en';
like `$CHSRC -en --version`,  $version_str_en,  'chsrc -en --version';
like `$CHSRC version -en`,    $version_str_en,  'chsrc version -en';

my $help_str = qr/^   (help|list|get|set|reset)/m;
like `$CHSRC -h`,     $help_str,    'chsrc -h';
like `$CHSRC --help`, $help_str,    'chsrc --help';
like `$CHSRC help`,   $help_str,    'chsrc help';
like `$CHSRC`,        $help_str,    'chsrc';


=begin comment

测试 chsrc list

=end comment
=cut
my $list_str = qr/mirrorz\s*MirrorZ\s*.*\n\s*tuna\s*TUNA/;
like `$CHSRC ls`,            $list_str,    'chsrc ls';
like `$CHSRC list mirrors`,  $list_str,    'chsrc list mirrors';
like `$CHSRC list os`,    qr/netbsd\s*openbsd/,   'chsrc list os';
like `$CHSRC list ware`,  qr/brew\s*homebrew/,   'chsrc list ware';


=begin comment

测试 chsrc get

=end comment
=cut
my $get_null = qr/chsrc: 请提供想要查看源的菜品名/u;
like `$CHSRC get -no-color 2>&1`,  $get_null,    'chsrc get -no-color';

my $fake_dish_name = qr/暂不支持的换源菜品/;
like `$CHSRC get fake_dish_name 2>&1`,  $fake_dish_name, 'chsrc get fake_dish_name';





=begin comment

测试 combo dish

=end comment
=cut

my $URL = 'https://abc.com/';

# uv combo
my $set_uv_abcd = qr/该套餐的所有子菜品都不支持你指定的镜像站/;
like `$CHSRC set uv abcd 2>&1`,  $set_uv_abcd, 'chsrc set uv abcd';

my $set_uv_url = qr/该套餐不支持用户自定义源/;
like `$CHSRC set uv $URL 2>&1`,  $set_uv_url, "chsrc set uv $URL";

# js combo
my $set_js_url = qr/食谱更新.*\n食谱更新.*\n食谱更新/;
# 如果有 npm, yarn, pnpm 这三个子菜品，再打开此测试
# like `$CHSRC set js $URL`,  $set_js_url, "chsrc set js $URL";

my $measure_js = qr/js 由以下3个子菜品组成.*\nnpm\nyarn\npnpm/;
like `$CHSRC measure -no-color js`, $measure_js, 'chsrc measure -no-color js';

my $list_js = qr/食谱创建.*食谱创建.*食谱创建.*/s;
like `$CHSRC list -no-color js`, $list_js, 'chsrc list -no-color js';

like `$CHSRC set -scope 2>&1`,                 qr/缺少参数/,       'chsrc set -scope missing arg';


=begin comment

测试 -scope 参数校验与环境变量型 recipes (rustup / pub / flutter / nvm / brew)

=end comment
=cut

like `$CHSRC set -scope 2>&1`,                 qr/缺少参数/,       'chsrc set -scope missing arg';
like `$CHSRC set -scope=project rustup 2>&1`,  qr/不支持项目级换源/, 'chsrc set -scope=project rustup rejected';
like `$CHSRC reset -scope=project rustup 2>&1`,qr/不支持项目级换源/, 'chsrc reset -scope=project rustup rejected';
like `$CHSRC ls -en -no-color rustup`,         qr/system scope.*supported and implemented/, 'chsrc ls -en rustup shows system scope';

if ($^O ne 'MSWin32') {
    my $tmp_home = tempdir (CLEANUP => 1);
    local $ENV{HOME} = $tmp_home;

    # 未知 shell 精确拒绝执行
    {
        local $ENV{SHELL} = '/usr/bin/nu';
        like `$CHSRC set rustup tuna 2>&1`, qr/无法精确确认当前 Shell 类型/, 'unknown shell set rejected';
        like `$CHSRC get rustup 2>&1`,      qr/无法精确确认当前 Shell 类型/, 'unknown shell get rejected';
    }

    # Bash 下 5 个迁移 recipe 的幂等写入、无双斜杠、持久化查看与重置
    {
        local $ENV{SHELL} = '/bin/bash';
        open my $fh, '>', "$tmp_home/.bashrc" or die $!;
        print {$fh} "# user header\n";
        close $fh;

        like `$CHSRC set -en -no-color rustup tuna`, qr/Thanks to the mirror site.*TUNA/s, 'rustup set tuna';
        open my $rc1, '<', "$tmp_home/.bashrc" or die $!;
        my $content1 = do { local $/; <$rc1> };
        close $rc1;

        # 重复执行幂等，文件内容与重启提示不重复产生
        my $out2 = `$CHSRC set -en -no-color rustup tuna`;
        unlike $out2, qr/source ~\/\.bashrc/, 'rustup repeat set skips restart hint';
        open my $rc2, '<', "$tmp_home/.bashrc" or die $!;
        my $content2 = do { local $/; <$rc2> };
        close $rc2;
        is $content2, $content1, 'rustup repeat set is byte-for-byte idempotent';

        like `$CHSRC get -no-color rustup`, qr|RUSTUP_DIST_SERVER=https://mirrors\.tuna\.tsinghua\.edu\.cn/rustup|, 'rustup get persisted';

        # Homebrew URL 无双斜杠
        like `$CHSRC set -en -no-color brew tuna`, qr/Thanks to the mirror site.*TUNA/s, 'brew set tuna';
        my $brew_get = `$CHSRC get -no-color brew`;
        like   $brew_get, qr|HOMEBREW_BREW_GIT_REMOTE=https://mirrors\.tuna\.tsinghua\.edu\.cn/git/homebrew/brew\.git|, 'brew url clean single slash';
        unlike $brew_get, qr|tsinghua\.edu\.cn//|, 'brew url has no double slash';

        # Pub / Flutter / nvm 基本闭环
        like `$CHSRC set -en -no-color pub tuna`,     qr/TUNA/, 'pub set tuna';
        like `$CHSRC set -en -no-color flutter tuna`, qr/TUNA/, 'flutter set tuna';
        like `$CHSRC set -en -no-color nvm tuna`,     qr/TUNA/, 'nvm set tuna';
        like `$CHSRC get -no-color nvm`,              qr|NVM_NODEJS_ORG_MIRROR=https://mirrors\.tuna\.tsinghua\.edu\.cn/nodejs-release|, 'nvm get persisted';

        # 危险字符注入拦截
        like `$CHSRC set rustup 'https://evil.org/\$inj' 2>&1`, qr/不安全字符/, 'dangerous char in custom url rejected';

        # 重置全部变量并确认清空
        like `$CHSRC reset -en -no-color rustup`,  qr/reset to the upstream/, 'rustup reset';
        like `$CHSRC reset -en -no-color brew`,    qr/reset to the upstream/, 'brew reset';
        like `$CHSRC reset -en -no-color pub`,     qr/reset to the upstream/, 'pub reset';
        like `$CHSRC reset -en -no-color flutter`, qr/reset to the upstream/, 'flutter reset';
        like `$CHSRC reset -en -no-color nvm`,     qr/reset to the upstream/, 'nvm reset';
        # 断言"持久化已清空"时，同时清掉父进程可能继承来的同名变量，
        # 否则 chsrc_get_env 的 getenv 回退会把会话值打印出来，使结果依赖运行环境
        {
            local $ENV{RUSTUP_UPDATE_ROOT} = undef;
            local $ENV{RUSTUP_DIST_SERVER} = undef;
            like `$CHSRC get -no-color rustup`, qr/^RUSTUP_UPDATE_ROOT=\nRUSTUP_DIST_SERVER=$/m, 'rustup empty after reset';
        }
    }

    # Fish 下 conf.d/chsrc.fish 写入与 config.fish 旧配置一次性清理
    {
        local $ENV{SHELL} = '/usr/bin/fish';
        mkdir "$tmp_home/.config";
        mkdir "$tmp_home/.config/fish";
        open my $ffh, '>', "$tmp_home/.config/fish/config.fish" or die $!;
        print {$ffh} "# keep comment\nset -x PUB_HOSTED_URL https://old.mirror\n";
        close $ffh;

        like `$CHSRC set -en -no-color pub tuna`, qr/TUNA/, 'fish pub set tuna';
        ok -f "$tmp_home/.config/fish/conf.d/chsrc.fish", 'fish conf.d/chsrc.fish created';

        open my $oldf, '<', "$tmp_home/.config/fish/config.fish" or die $!;
        my $old_fish_txt = do { local $/; <$oldf> };
        close $oldf;
        unlike $old_fish_txt, qr/PUB_HOSTED_URL/, 'legacy PUB_HOSTED_URL cleaned from config.fish';
        like   $old_fish_txt, qr/# keep comment/, 'unrelated comment preserved in config.fish';
    }
}





if ((defined $ARGV[0]) && ($ARGV[0] eq 'fastcheck')) {
    say "Fast checking, done testing.";
    done_testing;
    exit 0;
}

my $has_ruby = system 'ruby -v';
if ($has_ruby == 0) {
    say "Ruby exists. Go on testing.";
} else {
    say "No Ruby. End testing.";
    done_testing;
    exit 0;
}

my $get_ruby = qr/gem sources/;
like `$CHSRC get ruby`,   $get_ruby,  'chsrc get ruby';


=begin comment

测试 chsrc measure

=end comment
=cut
my $measure_ruby = qr/中国科学技术大学开源软件镜像/;
like `$CHSRC measure ruby`,  $measure_ruby,  'chsrc measure ruby';


=begin comment

测试 chsrc set 以及 chsrc reset

=end comment
=cut
my $set_ruby_abcd      = qr/镜像站.*不存在/;
my $set_ruby_first     = qr/全自动换源完成, 感谢镜像提供方/;
my $set_ruby           = qr/全自动换源完成, 感谢镜像提供方/;
my $reset_ruby         = qr/选中镜像站.*Upstream.*已重置为上游默认源/s;
my $set_ruby_locally   = qr/bundle config set --local/;

like `$CHSRC set ruby abcd 2>&1`,      $set_ruby_abcd,      'chsrc set ruby abcd';
like `$CHSRC set ruby first`,          $set_ruby_first,     'chsrc set ruby first';
like `$CHSRC set ruby`,                $set_ruby,           'chsrc set ruby';
like `$CHSRC reset ruby`,              $reset_ruby,         'chsrc reset ruby';
like `$CHSRC set -scope=project ruby first`,  $set_ruby_locally,  'chsrc set -scope=project ruby first';


done_testing;
