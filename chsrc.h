/** ------------------------------------------------------------
 * File          : chsrc.h
 * License       : GPLv3
 * Authors       : Aoran Zeng <ccmywish@qq.com>
 * Created on    : <2023-08-29>
 * Last modified : <2023-09-15>
 *
 * chsrc:
 *
 *   chsrc.c 头文件
 * ------------------------------------------------------------*/

#include "xy.h"
#include "sources.h"

/* 辅助函数 */
int
dblary_maxidx(double* array, int size)
{
  double maxval = array[0];
  int maxidx = 0;

  for (int i=1; i<size; i++) {
    if (array[i]>maxval) {
      maxval = array[i];
      maxidx = i;
    }
  }
  return maxidx;
}

/**
 * 用于告知用户 chsrc 所执行的操作
 */
void
chsrc_logcmd (const char* cmd)
{
  xy_info(xy_2strjoin("chsrc: 运行 ", cmd));
}

void
chsrc_runcmd (const char* cmd)
{
  chsrc_logcmd(cmd);
  system(cmd);
}

/**
 * 用于 _setsrc 函数
 */
void
chsrc_say_selection (source_info* source)
{
  xy_info (xy_strjoin(5, "chsrc: 选中镜像站: ", source->mirror->abbr, " (", source->mirror->code, ")"));
}

void
chsrc_say_thanks (source_info* source)
{
  xy_success(xy_2strjoin("chsrc: 感谢镜像提供方: ", source->mirror->name));
}


/* Target Info */
typedef struct {
  void (*setfn)(char* option);
  void (*getfn)(char* option);
  source_info* sources;
  size_t       sources_n;
} target_info;

#define def_target_info(t) target_info t##_target = {t##_setsrc, t##_getsrc, t##_sources, t##_sources_n}
