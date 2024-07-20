/**
 * @file misc.h
 * @author Tianchen Zhang
 * @brief misc = miscellaneous
 *        Provides 2 functions:
 *        void addPrefix(): Add prefix to a file name;
 *        void changePath(): Change the directory part for a file path.
 * @version 1.1
 * @date 2024-02-13
 *
 */

#ifndef CCBAR_SRC_MISC_H_
#define CCBAR_SRC_MISC_H_

#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Add prefix to a filename (preserving the original path)
 *
 * @param origPath The original path: "dir/filename"
 * @param prefix The prefix to be added: "prefix"
 * @param newPath The desired file name: "dir/prefix.filename"
 */
void
addPrefix(const char* origPath, const char* prefix, char* newPath);

/**
 * @brief Add suffix to a file (preserving the original path)
 *
 * @param origPath The original path: "dir/filename"
 * @param suffix The suffix to be added: "suffix"
 * @param newPath The desired file name: "dir/filename.suffix"
 */
void
addSuffix(const char* origPath, const char* suffix, char* newPath);

/**
 * @brief Change the directory part for a file name
 *
 * @param origPath The original path: "dir/filename"
 * @param tarDir Target directory: "tarDir"
 * @param newPath The character string generated: "tarDir/filename"
 */
void
changePath(const char* origPath, const char* tarDir, char* newPath);

#endif
