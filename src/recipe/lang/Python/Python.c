/** ------------------------------------------------------------
 * SPDX-License-Identifier: GPL-3.0-or-later
 * -------------------------------------------------------------
 * File Authors  : Aoran Zeng <ccmywish@qq.com>
 * Contributors  : happy game <happygame1024@gmail.com>
 *               |
 * Created On    : <2023-09-03>
 * Last Modified : <2024-12-11>
 *
 * ------------------------------------------------------------*/

void
pl_python_getsrc (char *option)
{
  bool pdm_exist    = false,
       poetry_exist = false,
       uv_exist     = false;

  pl_python_check_unofficial_pkger (&poetry_exist, &pdm_exist, &uv_exist);

  // 交给后面检查命令的存在性
  pl_python_pip_getsrc (option);
  br();

  if (poetry_exist)
    {
      pl_python_poetry_getsrc (option);
      br();
    }

  if (pdm_exist)
    {
      pl_python_pdm_getsrc (option);
    }

  if (uv_exist)
    {
      pl_python_uv_getsrc (option);
    }
}


void
pl_python_setsrc (char *option)
{
  {
    char *msg = CliOpt_InEnglish ? "Three package managers will be replaced for you at the same time: " \
                                         "pip, Poetry, PDM. If you need to change the source independently, " \
                                         "please run independently `chsrc set <pkg-manager>`"
                                       : "将同时为您更换4个包管理器 pip, Poetry, PDM, uv 的源，若需要独立换源，请独立运行 chsrc set <pkg-manager>";
    chsrc_note2 (msg);
  }

  bool pdm_exist    = false,
       poetry_exist = false,
       uv_exist     = false;

  pl_python_check_unofficial_pkger (&poetry_exist, &pdm_exist, &uv_exist);

  chsrc_set_target_group_mode ();
  chsrc_yield_source_and_confirm (pl_python);


  // 交给后面检查命令的存在性
  pl_python_pip_setsrc (option);
  br();

  if (poetry_exist)
    {
      pl_python_poetry_setsrc (option);
      br();
    }

  if (pdm_exist)
    {
      pl_python_pdm_setsrc (option);
    }

  if (uv_exist)
    {
      pl_python_uv_setsrc (option);
    }

  chsrc_determine_chgtype (ChgType_Auto);
  chsrc_conclude (&source);
}

void
pl_python_resetsrc (char *option)
{
  pl_python_setsrc (option);
}


Feature_t
pl_python_feat (char *option)
{
  Feature_t f = {0};

  f.can_get = true;
  f.can_reset = true;

  f.cap_locally = PartiallyCan;
  f.cap_locally_explain = "Support `Poetry` & `PDM`, `uv`. No support for `pip`";
  f.can_english = false;
  f.can_user_define = true;

  return f;
}

def_target_gsrf(pl_python);
