/**
 * @file fks-td.cc
 * @author Tianchen Zhang
 * @brief F_{KS} (time-dependent version)
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
#include <vector>

#include "dataio.h"
#include "misc.h"

void usage(char* name) {
  fprintf(stderr, "F_{KS} (time-dependent version)\n");
  fprintf(stderr,
          "USAGE: \n"
          "    %s [OPTIONS] CV(t-1) CV(t+1) CPS(t-1) CPS(t+1) ppotV ppotPS\n",
          name);
  fprintf(stderr,
          "OPTIONS: \n"
          "    -l <LENGTH>:       Array length\n"
          "    -o <OFNAME>:        ofname of F_KS\n"
          "    [-h, --help]:      Print help\n");
}

// Main function
int main(int argc, char* argv[]) {
  // Global variables
  int arrayLength = 0;
  static const char* ofname = NULL;
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

    // -l: arrayLength
    if (strcmp(argv[0], "-l") == 0) {
      arrayLength = atoi(argv[1]);  // atoi(): convert ASCII string to integer
      if (!arrayLength) {
        usage(programName);
        exit(1);
      }
      argc -= 2;
      argv += 2;
      continue;
    }

    // -o: ofname
    if (strcmp(argv[0], "-o") == 0) {
      ofname = argv[1];
      if (ofname == NULL) {
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

  // Make sure of all needed syntax
  if (argc != 6) {
    usage(programName);
    exit(1);
  }

  CVARRAY ddt(arrayLength), fks(arrayLength);
  ddt = fks = 0.0;

  std::vector<CVARRAY> data;
  for (int i = 0; i < 6; i++) {
    CVARRAY tmp(arrayLength);
    tmp = 0.0;
    readBin(argv[i], arrayLength, tmp);
    data.push_back(tmp);
  }

  ddt = (log(data[1] / data[3]) - log(data[0] / data[2])) / 2.0;
  fks = (data[4] - data[5]) / ddt;

  writeBin(ofname, arrayLength, fks);

  return 0;
}
