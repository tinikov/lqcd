/**
 * @file jre.cc
 * @author Tianchen Zhang
 * @brief Jackknife resampling for raw data
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
  fprintf(stderr, "Jackknife resampling for raw data\n");
  fprintf(stderr,
          "USAGE: \n"
          "    %s [OPTIONS] ifname1 ifname2 [ifname3 ...]\n",
          name);
  fprintf(stderr,
          "OPTIONS: \n"
          "    -l <LENGTH>:      Length of data arrays\n"
          "    -d <OFDIR>:       Directory of output files\n"
          "    [-v]:             Calculate variance for each sample\n"
          "    [-h, --help]:     Print help\n");
}

// Custom function declaration
void jackknifeResample(char* rawDataList[], char* sampleList[], int arrayLength, int fileCountTotal);
void jackknifeResampleWithVar(char* rawDataList[], char* sampleList[], int arrayLength, int fileCountTotal);

// Main function
int main(int argc, char* argv[]) {
  // Global variables
  int arrayLength = 0;
  static const char* ofDir = NULL;
  bool isSaveVar = false;
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

    // -v: calculate the variance
    if (strcmp(argv[0], "-v") == 0) {
      isSaveVar = true;
      argc--;
      argv++;
      continue;
    }

    fprintf(stderr, "Error: Unknown option '%s'\n", argv[0]);
    usage(programName);
    exit(1);
  }

  const int fileCountTotal = argc;  // # of data files
  if (fileCountTotal < 2) {
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
  if (isSaveVar) {
    jackknifeResampleWithVar(argv, ofnameArr, arrayLength, fileCountTotal);
  } else {
    jackknifeResample(argv, ofnameArr, arrayLength, fileCountTotal);
  }

  // Finalization for the string arrays
  for (int i = 0; i < fileCountTotal; i++) {
    free(ofnameArr[i]);
  }

  return 0;
}

// Custom function definition
void jackknifeResample(char* rawDataList[], char* sampleList[], int arrayLength, int fileCountTotal) {
  CVARRAY sum(arrayLength), value(arrayLength);
  sum = value = 0.0;

  // First round: Get sum of all data
  for (int i = 0; i < fileCountTotal; i++) {
    CVARRAY tmp(arrayLength);
    tmp = 0.0;
    readBin(rawDataList[i], arrayLength, tmp);

    sum += tmp;
  }

  // Second round: Generate jackknife resampled data and save files
  for (int i = 0; i < fileCountTotal; i++) {
    CVARRAY tmp(arrayLength);
    tmp = 0.0;
    readBin(rawDataList[i], arrayLength, tmp);

    value = (sum - tmp) / (fileCountTotal - 1.0);

    writeBin(sampleList[i], arrayLength, value);
  }
}

void jackknifeResampleWithVar(char* rawDataList[], char* sampleList[], int arrayLength, int fileCountTotal) {
  DVARRAY sum(arrayLength), sumSquare(arrayLength), value(arrayLength), var(arrayLength);
  sum = sumSquare = value = var = 0.0;

  // First round: Get sum and sum^2 of all data
  for (int i = 0; i < fileCountTotal; i++) {
    CVARRAY tmp(arrayLength);
    tmp = 0.0;
    readBin(rawDataList[i], arrayLength, tmp);

    DVARRAY rtmp(arrayLength);
    rtmp = 0.0;
    keepReal(tmp, rtmp, arrayLength);
    // varryNorm(tmp, rtmp, arrayLength);

    sum += rtmp;
    sumSquare += rtmp * rtmp;
  }

  // Second round: Generate the Jackknife sampled data and calculate the
  // variance
  // Also, save files to sampleList[]
  for (int i = 0; i < fileCountTotal; i++) {
    CVARRAY tmp(arrayLength);
    tmp = 0.0;
    readBin(rawDataList[i], arrayLength, tmp);

    DVARRAY rtmp(arrayLength);
    rtmp = 0.0;
    keepReal(tmp, rtmp, arrayLength);
    // varryNorm(tmp, rtmp, arrayLength);

    value = (sum - rtmp) / (fileCountTotal - 1.0);
    // About this variance, please refer to eq.(7.37) on P.383, Montvay LQCD
    // book
    var =
        sqrt(((sumSquare - rtmp * rtmp) / DOUBLE(fileCountTotal - 1.0) - value * value) / DOUBLE(fileCountTotal - 2.0));

    CVARRAY result(arrayLength);
    result = 0.0;

    for (int j = 0; j < arrayLength; j++) {
      result[j].real(value[j]);
      result[j].imag(var[j]);
    }

    writeBin(sampleList[i], arrayLength, result);
  }
}
