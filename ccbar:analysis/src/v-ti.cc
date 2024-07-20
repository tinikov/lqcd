/**
 * @file v-ti.cc
 * @author Tianchen Zhang
 * @brief Central potential (time-independent version)
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
  fprintf(stderr, "Central potential (time-independent version)\n");
  fprintf(stderr,
          "USAGE: \n"
          "    %s [OPTIONS] prev_V prev_PS\n",
          name);
  fprintf(stderr,
          "OPTIONS: \n"
          "    -l <LENGTH>:       Array length\n"
          "    -mps <M_PS>:       M_PS (LUnit)\n"
          "    -mv <M_V>:         M_V (LUnit)\n"
          "    -mc <MC>:          charm quark mass (LUnit)\n"
          "    -ov0 <OFNAMEV0>:   ofname of v0\n"
          "    -ovs <OFNAMEVS>:   ofname of vs\n"
          "    [-h, --help]:      Print help\n");
}

// Main function
int main(int argc, char* argv[]) {
  // Global variables
  int arrayLength = 0;
  DOUBLE mPS = 0.0;
  DOUBLE mV = 0.0;
  DOUBLE mc = 0.0;
  static const char* ofnameV0 = NULL;
  static const char* ofnameVs = NULL;
  char programName[128];
  strncpy(programName, basename(argv[0]), 127);
  argc--;
  argv++;

  // Read options (order irrelevant)
  while (argc > 0 && argv[0][0] == '-')  // Read options (order irrelevant)
  {
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

    // -mps: PS channel charmonium mass
    if (strcmp(argv[0], "-mps") == 0) {
      mPS = atof(argv[1]);  // atof(): convert ASCII string to float
      if (mPS == 0.0) {
        usage(programName);
        exit(1);
      }
      argc -= 2;
      argv += 2;
      continue;
    }

    // -mv: V channel charmonium mass
    if (strcmp(argv[0], "-mv") == 0) {
      mV = atof(argv[1]);  // atof(): convert ASCII string to float
      if (mV == 0.0) {
        usage(programName);
        exit(1);
      }
      argc -= 2;
      argv += 2;
      continue;
    }

    // -mc: charm quark mass
    if (strcmp(argv[0], "-mc") == 0) {
      mc = atof(argv[1]);  // atof(): convert ASCII string to float
      if (mc == 0.0) {
        usage(programName);
        exit(1);
      }
      argc -= 2;
      argv += 2;
      continue;
    }

    // -ov0: ofnameV0
    if (strcmp(argv[0], "-ov0") == 0) {
      ofnameV0 = argv[1];
      if (ofnameV0 == NULL) {
        usage(programName);
        exit(1);
      }
      argc -= 2;
      argv += 2;
      continue;
    }

    // -ovs: ofnameVs
    if (strcmp(argv[0], "-ovs") == 0) {
      ofnameVs = argv[1];
      if (ofnameVs == NULL) {
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

  CVARRAY prev_v(arrayLength), ppotps(arrayLength), v0(arrayLength),
      vs(arrayLength);
  prev_v = ppotps = v0 = vs = 0.0;

  readBin(argv[0], arrayLength, prev_v);
  readBin(argv[1], arrayLength, ppotps);

  v0 = 1 / (4.0 * mc) * (3.0 * prev_v + ppotps) + 1 / 4.0 * (3.0 * mV + mPS) -
       2.0 * mc;
  vs = 1 / mc * (prev_v - ppotps) + (mV - mPS);

  writeBin(ofnameV0, arrayLength, v0);
  writeBin(ofnameVs, arrayLength, vs);

  return 0;
}
