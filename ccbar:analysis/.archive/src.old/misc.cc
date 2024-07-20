/**
 * @file misc.cc
 * @author Tianchen Zhang
 * @brief
 * @version 1.1
 * @date 2024-02-13
 *
 */

#include "misc.h"

#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void addPrefix(const char* origPath, const char* prefix, char* newPath) {
  char stmp[1024], dir[1024], base[512], pre[512];

  strncpy(pre, prefix, 511);
  strncpy(stmp, origPath, 1023);
  strncpy(dir, dirname(stmp), 1023);
  strncpy(stmp, origPath, 1023);
  strncpy(base, basename(stmp), 511);

  snprintf(newPath, 2048, "%s/%s.%s", dir, pre, base);
}

void addSuffix(const char* origPath, const char* suffix, char* newPath) {
  char stmp[1024], dir[1024], base[512], suf[512];

  strncpy(suf, suffix, 511);
  strncpy(stmp, origPath, 1023);
  strncpy(dir, dirname(stmp), 1023);
  strncpy(stmp, origPath, 1023);
  strncpy(base, basename(stmp), 511);

  snprintf(newPath, 2048, "%s/%s.%s", dir, base, suf);
}

void changePath(const char* origPath, const char* tarDir, char* newPath) {
  char stmp[1024], dir[1024], base[1024];

  strncpy(dir, tarDir, 1023);
  strncpy(stmp, origPath, 1023);
  strncpy(base, basename(stmp), 1023);

  snprintf(newPath, 2048, "%s/%s", dir, base);
}
