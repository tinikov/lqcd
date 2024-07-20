/**
 * @file a1plus.cc
 * @author Tianchen Zhang
 * @brief A1+ projection for 4-point correlators
 * @version 1.2
 * @date 2024-07-20
 *
 */

#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <complex>
#include <valarray>

#include "arr2corr.h"
#include "dataio.h"
#include "misc.h"

void usage(char* name) {
  fprintf(stderr, "A1+ projection for 4-point correlators\n");
  fprintf(stderr,
          "USAGE: \n"
          "    %s [OPTIONS] ifname1 [ifname2 ...]\n",
          name);
  fprintf(stderr,
          "OPTIONS: \n"
          "    -n <XYZSIZE>:   Spacial size of lattice\n"
          "    -d <OFDIR>:     Directory of output files\n"
          "    [-h, --help]:   Print help\n");
}

// Custom function declaration
void a1plus(char* rawDataList[], char* a1list[], int xyzSize, int fileCountTotal);

// Main function
int main(int argc, char* argv[]) {
  // Global variables
  int xyzSize = 0;
  static const char* ofDir = NULL;
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

    fprintf(stderr, "Error: Unknown option '%s'\n", argv[0]);
    usage(programName);
    exit(1);
  }

  const int fileCountTotal = argc;
  if (fileCountTotal < 1) {
    usage(programName);
    exit(1);
  }

  // Create an array to store ofnames
  char* ofnameArr[fileCountTotal];

  for (int i = 0; i < fileCountTotal; i++) {
    ofnameArr[i] = (char*)malloc(2048 * sizeof(char));
    changePath(argv[i], ofDir, ofnameArr[i]);
  }

  // Main part for calculation
  a1plus(argv, ofnameArr, xyzSize, fileCountTotal);

  // Finalization for the ofname array
  for (int i = 0; i < fileCountTotal; i++) {
    free(ofnameArr[i]);
  }

  return 0;
}

// Custom function definition
inline COMPLX naiveSym(COMPLX* data, int x, int y, int z, int xyzSize) {
  return (CORR(data, x, y, z, xyzSize) + CORR(data, y, z, x, xyzSize) + CORR(data, z, x, y, xyzSize) +
          CORR(data, x, z, y, xyzSize) + CORR(data, z, y, x, xyzSize) + CORR(data, y, x, z, xyzSize)) /
         6.0;
}

inline COMPLX a1Sym(COMPLX* data, int x, int y, int z, int xyzSize) {
  return (naiveSym(data, x, y, z, xyzSize) + naiveSym(data, x, y, xyzSize - z, xyzSize) +
          naiveSym(data, x, xyzSize - y, z, xyzSize) + naiveSym(data, x, xyzSize - y, xyzSize - z, xyzSize) +
          naiveSym(data, xyzSize - x, y, z, xyzSize) + naiveSym(data, xyzSize - x, y, xyzSize - z, xyzSize) +
          naiveSym(data, xyzSize - x, xyzSize - y, z, xyzSize) +
          naiveSym(data, xyzSize - x, xyzSize - y, xyzSize - z, xyzSize)) /
         8.0;
}

void a1plus(char* rawDataList[], char* a1list[], int xyzSize, int fileCountTotal) {
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
          CORR(result, ix, iy, iz, xyzSize) = a1Sym(tmp, ix, iy, iz, xyzSize);
        }

    writeBin(a1list[i], arrayLength, result);
  }
}
