/**
 * @file norm.cc
 * @author Tianchen Zhang
 * @brief Normalizaion for 4-point correlators
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

void
usage(char* name)
{
  fprintf(stderr, "Normalizaion for 4-point correlators\n");
  fprintf(stderr,
          "USAGE: \n"
          "    %s [OPTIONS] ifname1 [ifname2 ...]\n",
          name);
  fprintf(stderr,
          "OPTIONS: \n"
          "    -n <XYZSIZE>:     Spacial size of lattice\n"
          "    -d <OFDIR>:       Directory of output files\n"
          "    [-h, --help]:     Print help\n");
}

// Custom function declaration
void
naiveNorm(char* rawDataList[], char* nnlist[], int xyzSize, int fileCountTotal);
void
l2Norm(char* rawDataList[], char* l2list[], int xyzSize, int fileCountTotal);

// Main function
int
main(int argc, char* argv[])
{
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
      xyzSize = atoi(argv[1]); // atoi(): convert ASCII string to integer
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

  const int fileCountTotal = argc; // # of data files
  if (fileCountTotal < 1) {
    usage(programName);
    exit(1);
  }

  // Create arrays to store ofnames
  char *nnNameArr[fileCountTotal], *l2NameArr[fileCountTotal];
  for (int i = 0; i < fileCountTotal; i++) {
    char stmpnn[2048], stmpl2[2048];
    nnNameArr[i] = (char*)malloc(2048 * sizeof(char));
    l2NameArr[i] = (char*)malloc(2048 * sizeof(char));
    addPrefix(argv[i], "nn", stmpnn);
    changePath(stmpnn, ofDir, nnNameArr[i]);
    addPrefix(argv[i], "l2", stmpl2);
    changePath(stmpl2, ofDir, l2NameArr[i]);
  }

  // Main part for calculation
  naiveNorm(argv, nnNameArr, xyzSize, fileCountTotal);
  l2Norm(argv, l2NameArr, xyzSize, fileCountTotal);

  // Finalization for the string arrays
  for (int i = 0; i < fileCountTotal; i++) {
    free(nnNameArr[i]);
    free(l2NameArr[i]);
  }

  return 0;
}

// Custom function definition
void
naiveNorm(char* rawDataList[], char* nnlist[], int xyzSize, int fileCountTotal)
{
  int arrayLength = int(pow(xyzSize, 3));

  for (int i = 0; i < fileCountTotal; i++) {
    COMPLX tmp[arrayLength], result[arrayLength];

    for (int j = 0; j < arrayLength; j++) // Initialize the empty arrays
    {
      tmp[j] = result[j] = 0.0;
    }

    readBin(rawDataList[i], arrayLength, tmp);

    for (int j = 0; j < arrayLength; j++) // Compute C_n(t) = C(t)/C(0)
    {
      result[j] = tmp[j] / tmp[0];
    }

    writeBin(nnlist[i], arrayLength, result);
  }
}

void
l2Norm(char* rawDataList[], char* l2list[], int xyzSize, int fileCountTotal)
{
  int arrayLength = int(pow(xyzSize, 3));

  for (int i = 0; i < fileCountTotal; i++) {
    COMPLX tmp[arrayLength], result[arrayLength];
    DOUBLE normFact = 0.0;

    for (int j = 0; j < arrayLength; j++) // Initialize the empty arrays
    {
      tmp[j] = result[j] = 0.0;
    }

    readBin(rawDataList[i], arrayLength, tmp);

    for (int j = 0; j < arrayLength; j++) {
      normFact += norm(tmp[j]);
    }

    normFact = sqrt(normFact);

    for (int j = 0; j < arrayLength; j++) // C_n(t) = C(t)/\sqrt(\sum_{C^2})
    {
      result[j] = tmp[j] / normFact;
    }

    writeBin(l2list[i], arrayLength, result);
  }
}
