/**
 * @file prev.cc
 * @author Tianchen Zhang
 * @brief Pre-potential: [▽^2 C(r,t)]/C(r,t)
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

#include "correlator.h"
#include "dataio.h"
#include "misc.h"

void usage(char* name) {
  fprintf(stderr, "Pre-potential: [▽^2 C(r,t)]/C(r,t)\n");
  fprintf(stderr,
          "USAGE: \n"
          "    %s [OPTIONS] ifname1 [ifname2 ...]\n",
          name);
  fprintf(stderr,
          "OPTIONS: \n"
          "    -n <XYZSIZE>:     Spacial size of lattice\n"
          "    -d <OFDIR>:       Directory of output files\n"
          "    [-p] <PREFIX>:    Prefix for output files\n"
          "    [-h, --help]:     Print help\n");
}

// Custom function declaration
void prePotential(char* rawDataList[], char* ppotList[], int xyzSize,
                  int fileCountTotal);

// Main function
int main(int argc, char* argv[]) {
  // Global variables
  int xyzSize = 0;
  static const char* ofDir = NULL;
  static const char* ofPrefix = NULL;
  bool isAddPrefix = false;
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

    // -n: xyzSize
    if (strcmp(argv[0], "-n") == 0) {
      xyzSize = atoi(argv[1]);  // atoi(): convert ASCII string to integer
      if (!xyzSize) {
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
  prePotential(argv, ofnameArr, xyzSize, fileCountTotal);

  // Finalization for the string arrays
  for (int i = 0; i < fileCountTotal; i++) {
    free(ofnameArr[i]);
  }

  return 0;
}

// Custom function definition
void prePotential(char* rawDataList[], char* ppotList[], int xyzSize,
                  int fileCountTotal) {
  int arrayLength = int(pow(xyzSize, 3));

  for (int i = 0; i < fileCountTotal; i++) {
    COMPLX tmp[arrayLength], result[arrayLength];
    for (int j = 0; j < arrayLength; j++)  // Initialize the empty arrays
    {
      tmp[j] = result[j] = 0.0;
    }

    readBin(rawDataList[i], arrayLength, tmp);

    for (int ix = 0; ix < xyzSize; ix++)
      for (int iy = 0; iy < xyzSize; iy++)
        for (int iz = 0; iz < xyzSize; iz++) {
          CORR(result, ix, iy, iz, xyzSize) =
              (CORR(tmp, ix + 1, iy, iz, xyzSize) +
               CORR(tmp, ix - 1, iy, iz, xyzSize) +
               CORR(tmp, ix, iy + 1, iz, xyzSize) +
               CORR(tmp, ix, iy - 1, iz, xyzSize) +
               CORR(tmp, ix, iy, iz + 1, xyzSize) +
               CORR(tmp, ix, iy, iz - 1, xyzSize) -
               6.0 * CORR(tmp, ix, iy, iz, xyzSize)) /
              CORR(tmp, ix, iy, iz, xyzSize);
        }

    writeBin(ppotList[i], arrayLength, result);
  }
}
