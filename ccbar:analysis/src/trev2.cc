/**
 * @file trev2.cc
 * @author Tianchen Zhang
 * @brief Time reversal for 2-point correlators
 * @version 1.1
 * @date 2024-02-13
 *
 */

#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <complex>
#include <valarray>

#include "dataio.h"
#include "misc.h"

void usage(char* name) {
  fprintf(stderr, "Time reversal for 2-point correlators\n");
  fprintf(stderr,
          "USAGE: \n"
          "    %s [OPTIONS] ifname1 [ifname2 ...]\n",
          name);
  fprintf(stderr,
          "OPTIONS: \n"
          "    -n <TSIZE>:       Temporal size of lattice\n"
          "    -d <OFDIR>:       Directory of output files\n"
          "    [-p] <PREFIX>:    Prefix for output files\n"
          "    [-t]:             Also save a txt file (add \"txt.\" prefix)\n"
          "    [-h, --help]:     Print help\n");
}

// Custom function declaration
void timeReverse2pt(char* rawDataList[], char* tr2ptList[], int tSize,
                    int fileCountTotal);

// Main function
int main(int argc, char* argv[]) {
  // Global variables
  int tSize = 0;
  static const char* ofDir = NULL;
  static const char* ofPrefix = NULL;
  bool isAddPrefix = false;
  bool isSaveTxt = false;
  char programName[128];
  strncpy(programName, basename(argv[0]), 127);
  argc--;
  argv++;

  // Read options (order irrelevant)
  while (argc > 0 && argv[0][0] == '-') {
    // -h and --help: show usage
    if (strcmp(argv[0], "-h") == 0 || strcmp(argv[0], "--help") == 0) {
      usage(programName);
      exit(0);
    }

    // -n: tSize
    if (strcmp(argv[0], "-n") == 0) {
      tSize = atoi(argv[1]);  // atoi(): convert ASCII string to integer
      if (!tSize) {
        usage(programName);
        exit(1);
      }
      argc -= 2;
      argv += 2;
      continue;
    }

    // -d: directory for output file
    if (strcmp(argv[0], "-d") == 0) {
      ofDir = argv[1];
      if (ofDir == NULL) {
        usage(programName);
        exit(1);
      }
      argc -= 2;
      argv += 2;
      continue;
    }

    // -p: prefix for output file
    if (strcmp(argv[0], "-p") == 0) {
      ofPrefix = argv[1];
      isAddPrefix = true;
      argc -= 2;
      argv += 2;
      continue;
    }

    // -t: save txt
    if (strcmp(argv[0], "-t") == 0) {
      isSaveTxt = true;
      argc--;
      argv++;
      continue;
    }

    fprintf(stderr, "Error: Unknown option '%s'\n", argv[0]);
    usage(programName);
    exit(1);
  }

  const int fileCountTotal = argc;  // # of data files
  if (fileCountTotal < 1) {
    usage(programName);
    exit(1);
  }

  // Create an array to store ofnames
  char* ofnameArr[fileCountTotal];

  if (isAddPrefix) {
    for (int i = 0; i < fileCountTotal; i++) {
      char stmp[2048];
      ofnameArr[i] = (char*)malloc(2048 * sizeof(char));
      addPrefix(argv[i], ofPrefix, stmp);
      changePath(stmp, ofDir, ofnameArr[i]);
    }
  } else {
    for (int i = 0; i < fileCountTotal; i++) {
      ofnameArr[i] = (char*)malloc(2048 * sizeof(char));
      changePath(argv[i], ofDir, ofnameArr[i]);
    }
  }

  // Main part for calculation
  timeReverse2pt(argv, ofnameArr, tSize, fileCountTotal);

  if (isSaveTxt) {
    for (int i = 0; i < fileCountTotal; i++) {
      char txttmp[2048];
      addSuffix(ofnameArr[i], "txt", txttmp);
      bin2txt(ofnameArr[i], txttmp, tSize);
    }
  }

  // Finalization for the string arrays
  for (int i = 0; i < fileCountTotal; i++) {
    free(ofnameArr[i]);
  }

  return 0;
}

// Custom function definition
void timeReverse2pt(char* rawDataList[], char* tr2ptList[], int tSize,
                    int fileCountTotal) {
  for (int i = 0; i < fileCountTotal; i++) {
    COMPLX raw[tSize], data[tSize];
    for (int j = 0; j < tSize; j++) raw[j] = data[j] = 0.0;

    readBin(rawDataList[i], tSize, raw);

    for (int j = 0; j < tSize; j++)
      data[j] = (raw[j] + raw[(tSize - j) % tSize]) * 0.5;

    writeBin(tr2ptList[i], tSize, data);
  }
}
