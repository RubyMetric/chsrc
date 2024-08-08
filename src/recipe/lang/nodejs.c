/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * Contributors  : Aoran Zeng <ccmywish@qq.com>
 * Created on    : <2023-09-01>
 * Last modified : <2024-08-09>
 * ------------------------------------------------------------*/

static MirrorSite
NpmMirror = {"npmmirror",    "npmmirror",    "npmmirror (阿里云赞助)",    "https://npmmirror.com/",
              // 注意，这个是跳转后的地址，不确定未来会不会改变
              "https://cdn.npmmirror.com/packages/%40tensorflow/tfjs/4.10.0/tfjs-4.10.0.tgz"}; // 29MB


/**
 * @time 2024-04-18 更新
 * @note {
 *   Sjtug, Tuna, Lzuoss, Jlu, Bfsu, 网易，搜狐 都没有
 *   腾讯软件源的npm源一直未证实是否可用
 * }
 */
static SourceInfo
pl_nodejs_sources[] = {
  {&Upstream,       NULL},
  {&NpmMirror,     "https://registry.npmmirror.com"},
  {&Huawei,        "https://mirrors.huaweicloud.com/repository/npm/"},
  {&Zju,           "https://mirrors.zju.edu.cn/npm"}
};
def_sources_n(pl_nodejs);


void
pl_nodejs_check_cmd (bool *npm_exist, bool *yarn_exist, bool *pnpm_exist)
{
  char *check_cmd = xy_str_to_quietcmd ("npm -v");
  *npm_exist = query_program_exist (check_cmd, "npm");

  check_cmd = xy_str_to_quietcmd ("yarn -v");
  *yarn_exist = query_program_exist (check_cmd, "yarn");

  check_cmd = xy_str_to_quietcmd ("pnpm -v");
  *pnpm_exist = query_program_exist (check_cmd, "pnpm");

  if (!*npm_exist && !*yarn_exist && !*pnpm_exist)
    {
      chsrc_error ("未找到 npm 或 yarn 或 pnpm 命令，请检查是否存在其一");
      exit (Exit_UserCause);
    }
}


void
pl_nodejs_getsrc (char *option)
{
  bool npm_exist, yarn_exist, pnpm_exist;
  pl_nodejs_check_cmd (&npm_exist, &yarn_exist, &pnpm_exist);

  if (npm_exist)
    {
      chsrc_run ("npm config get registry", RunOpt_Default);
    }
  if (yarn_exist)
    {
      chsrc_run ("yarn config get registry", RunOpt_Default);
    }
  if (pnpm_exist)
    {
      chsrc_run ("pnpm config get registry", RunOpt_Default);
    }
}

/**
 * NodeJS换源，参考：https://npmmirror.com/
 */
void
pl_nodejs_setsrc (char *option)
{
  bool npm_exist, yarn_exist, pnpm_exist;
  pl_nodejs_check_cmd (&npm_exist, &yarn_exist, &pnpm_exist);

  SourceInfo source;
  chsrc_yield_source (pl_nodejs);
  chsrc_confirm_source (&source);

  char *cmd = NULL;

  char *where = " ";
  if (CliOpt_Locally==true)
    {
      where = " --location project ";
    }

  if (npm_exist)
    {
      cmd = xy_strjoin (4, "npm config", where, "set registry ", source.url);
      chsrc_run (cmd, RunOpt_Default);
    }

  if (yarn_exist)
    {
      // 不再阻止换源命令输出到终端，即不再调用 xy_str_to_quietcmd()
      cmd = xy_2strjoin ("yarn config set registry ", source.url);
      chsrc_run (cmd, RunOpt_Default);
    }

  if (pnpm_exist)
    {
      cmd = xy_2strjoin ("pnpm config set registry ", source.url);
      chsrc_run (cmd, RunOpt_Default);
    }

  chsrc_say_lastly (&source, ChsrcTypeAuto);
}


def_target(pl_nodejs);
