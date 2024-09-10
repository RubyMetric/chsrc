/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors   : Aoran Zeng <ccmywish@qq.com>
 * Contributors   :  Mr. Will  <mr.will.com@outlook.com>
 * Created On     : <2023-08-30>
 * Major Revision :      2
 * Last Modified  : <2024-09-10>
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
  {&Upstream,      "https://registry.npmjs.org/"},
  {&NpmMirror,     "https://registry.npmmirror.com"},
  {&Huawei,        "https://mirrors.huaweicloud.com/repository/npm/"},
  {&Zju,           "https://mirrors.zju.edu.cn/npm"}
};
def_sources_n(pl_nodejs);


void
pl_nodejs_check_cmd (bool *npm_exist, bool *yarn_exist, bool *pnpm_exist)
{
  *npm_exist  = chsrc_check_program ("npm");
  *yarn_exist = chsrc_check_program ("yarn");
  *pnpm_exist = chsrc_check_program ("pnpm");

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
      // TODO
    }
  if (yarn_exist)
    {
      // TODO
    }
  if (pnpm_exist)
    {
      // TODO
    }
}

/**
 * chsrc set nodejs
 */
void
pl_nodejs_setsrc (char *option)
{
  bool npm_exist, yarn_exist, pnpm_exist;
  pl_nodejs_check_cmd (&npm_exist, &yarn_exist, &pnpm_exist);

  chsrc_yield_source_and_confirm (pl_nodejs);

  if (npm_exist)
    {
      // TODO
    }

  if (yarn_exist)
    {
      // TODO
    }

  if (pnpm_exist)
    {
      // TODO
    }

  chsrc_conclude (&source, ChsrcTypeAuto);
}


FeatInfo
pl_nodejs_feat (char *option)
{
  FeatInfo fi = {0};

  fi.can_get = true;
  fi.can_reset = true;

  fi.stcan_locally = CanFully;
  fi.locally = "支持 npm (From v0.1.7); 支持 yarn v2 (chsrc v0.1.8.1); 支持 pnpm (chsrc v0.1.8.2)";
  fi.can_english = false;
  fi.can_user_define = true;

  return fi;
}

def_target_gsf (pl_nodejs);
