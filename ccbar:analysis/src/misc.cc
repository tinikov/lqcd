/**
 * @file misc.cc
 * @author Tianchen Zhang
 * @brief
 * @version 1.2
 * @date 2024-07-20
 *
 */

#include "misc.h"

#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void addPrefix(const char* origPath, const char* prefix, char* newPath) {
  char stmp[2048], dir[2048], base[2048], pre[2048];

  strncpy(pre, prefix, 2047);
  strncpy(stmp, origPath, 2047);
  strncpy(dir, dirname(stmp), 2047);
  strncpy(stmp, origPath, 2047);
  strncpy(base, basename(stmp), 2047);

  snprintf(newPath, 2048, "%s/%s.%s", dir, pre, base);
}

void addSuffix(const char* origPath, const char* suffix, char* newPath) {
  char stmp[2048], dir[2048], base[2048], suf[2048];

  strncpy(suf, suffix, 2047);
  strncpy(stmp, origPath, 2047);
  strncpy(dir, dirname(stmp), 2047);
  strncpy(stmp, origPath, 2047);
  strncpy(base, basename(stmp), 2047);

  snprintf(newPath, 2048, "%s/%s.%s", dir, base, suf);
}

void changePath(const char* origPath, const char* tarDir, char* newPath) {
  char stmp[2048], dir[2048], base[2048];

  strncpy(dir, tarDir, 2047);
  strncpy(stmp, origPath, 2047);
  strncpy(base, basename(stmp), 2047);

  snprintf(newPath, 2048, "%s/%s", dir, base);
}
