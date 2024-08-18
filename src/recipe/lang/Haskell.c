/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  :  Nil Null  <nil@null.org>
 * Created On    : <2023-09-10>
 * Last Modified : <2024-08-15>
 * ------------------------------------------------------------*/

/**
 * @time 2023-09-10 更新
 */
static SourceInfo
pl_haskell_sources[] = {
  {&Upstream,       NULL},
  {&Tuna,          "https://mirrors.tuna.tsinghua.edu.cn/hackage"},
  {&Bfsu,          "https://mirrors.bfsu.edu.cn/hackage"},
  {&Nju,           "https://mirror.nju.edu.cn/hackage"},
  {&Ustc,          "https://mirrors.ustc.edu.cn/hackage"}
};
def_sources_n(pl_haskell);


void
pl_haskell_setsrc (char *option)
{
  chsrc_yield_source_and_confirm (pl_haskell);

  char *file = xy_strjoin (3, "repository mirror\n"
                              "  url: ", source.url,
                            "\n  secure: True");

  char *config = NULL;
  if (xy_on_windows)
    {
      config = xy_uniform_path ("~/AppData/Roaming/cabal/config");
    }
  else
    {
      config = "~/.cabal/config";
    }

  chsrc_note2 (xy_strjoin (3, "请向 ", config, " 中手动添加:"));
  puts (file); br();

  config = xy_uniform_path ("~/.stack/config.yaml");
  file = xy_strjoin (3, "package-indices:\n"
                       "  - download-prefix: ", source.url,
                     "\n    hackage-security:\n"
                       "        keyids:\n"
                       "        - 0a5c7ea47cd1b15f01f5f51a33adda7e655bc0f0b0615baa8e271f4c3351e21d\n"
                       "        - 1ea9ba32c526d1cc91ab5e5bd364ec5e9e8cb67179a471872f6e26f0ae773d42\n"
                       "        - 280b10153a522681163658cb49f632cde3f38d768b736ddbc901d99a1a772833\n"
                       "        - 2a96b1889dc221c17296fcc2bb34b908ca9734376f0f361660200935916ef201\n"
                       "        - 2c6c3627bd6c982990239487f1abd02e08a02e6cf16edb105a8012d444d870c3\n"
                       "        - 51f0161b906011b52c6613376b1ae937670da69322113a246a09f807c62f6921\n"
                       "        - 772e9f4c7db33d251d5c6e357199c819e569d130857dc225549b40845ff0890d\n"
                       "        - aa315286e6ad281ad61182235533c41e806e5a787e0b6d1e7eef3f09d137d2e9\n"
                       "        - fe331502606802feac15e514d9b9ea83fee8b6ffef71335479a2e68d84adc6b0\n"
                       "        key-threshold: 3\n"
                       "        ignore-expiry: no");

  chsrc_note2 (xy_strjoin (3, "请向 ", config, " 中手动添加:"));
  puts (file);
  chsrc_conclude (&source, ChsrcTypeManual);
}

def_target_s (pl_haskell);
