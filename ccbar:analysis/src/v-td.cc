/**
 * @file v-td.cc
 * @author Tianchen Zhang
 * @brief Vcc and Vspin (time-dependent version)
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

#include "dataio.h"
#include "misc.h"

void usage(char* name) {
  fprintf(stderr, "Central potential (time-dependent version)\n");
  fprintf(stderr,
          "USAGE: \n"
          "    %s [OPTIONS] CV(t-1) CV(t+1) CPS(t-1) CPS(t+1) ppotV ppotPS\n",
          name);
  fprintf(stderr,
          "OPTIONS: \n"
          "    -n <XYZSIZE>:      Spacial size of lattice\n"
          "    -mc <MASS>:        Kinetic mass of charm quark\n"
          "    -ov0 <OFNAMEV0>:   ofname of v0\n"
          "    -ovs <OFNAMEVS>:   ofname of vs\n"
          "    [-h, --help]:      Print help\n");
}

// Main function
int main(int argc, char* argv[]) {
  // Global variables
  int xyzSize = 0;
  DOUBLE mc = 0.0;
  static const char* ofnameV0 = NULL;
  static const char* ofnameVs = NULL;
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
    if (strcmp(argv[0], "-l") == 0) {
      xyzSize = atoi(argv[1]);  // atoi(): convert ASCII string to integer
      if (!xyzSize) {
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
  if (argc != 6) {
    usage(programName);
    exit(1);
  }

  int arrayLength = int(pow(xyzSize, 3));

  CVARRAY v0(arrayLength), vs(arrayLength);
  v0 = vs = 0.0;

  std::vector<CVARRAY> data;
  for (int i = 0; i < 6; i++) {
    CVARRAY tmp(arrayLength);
    tmp = 0.0;
    readBin(argv[i], arrayLength, tmp);
    data.push_back(tmp);
  }

  v0 = 1 / (4.0 * mc) * (3 * data[4] + data[5]) -
       1 / 4.0 * (3 * (log(data[1]) - log(data[0])) / 2.0 + (log(data[3]) - log(data[2])) / 2.0) - 2 * mc;
  vs = 1.0 / mc * (data[4] - data[5]) - (log(data[1] / data[3]) - log(data[0] / data[2])) / 2.0;

  writeBin(ofnameVs, arrayLength, v0);
  writeBin(ofnameV0, arrayLength, vs);

  return 0;
}
