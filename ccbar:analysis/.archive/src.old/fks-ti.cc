/**
 * @file fks-ti.cc
 * @author Tianchen Zhang
 * @brief F_{KS} (time-independent version)
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
  fprintf(stderr, "F_{KS} (time-independent version)\n");
  fprintf(stderr,
          "USAGE: \n"
          "    %s [OPTIONS] ppotV ppotPS\n",
          name);
  fprintf(stderr,
          "OPTIONS: \n"
          "    -l <LENGTH>:       Array length\n"
          "    -m <MDIFF>:        (M_V - M_PS) (LUnit)\n"
          "    -o <OFNAME>:       ofname of F_KS\n"
          "    [-h, --help]:      Print help\n");
}

// Main function
int main(int argc, char* argv[]) {
  // Global variables
  int arrayLength = 0;
  DOUBLE mdiff = 0.0;
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

    // -m: mdiff
    if (strcmp(argv[0], "-m") == 0) {
      mdiff = atof(argv[1]);  // atof(): convert ASCII string to float
      if (mdiff == 0.0) {
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
  if (argc != 2) {
    usage(programName);
    exit(1);
  }

  CVARRAY ppotv(arrayLength), ppotps(arrayLength), fks(arrayLength);
  ppotv = ppotps = fks = 0.0;

  readBin(argv[0], arrayLength, ppotv);
  readBin(argv[1], arrayLength, ppotps);

  fks = -(ppotv - ppotps) / mdiff;

  writeBin(ofname, arrayLength, fks);

  return 0;
}
