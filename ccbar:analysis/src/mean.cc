/**
 * @file mean.cc
 * @author Tianchen Zhang
 * @brief Mean for raw data (Optional: jackknife variance)
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
  fprintf(stderr, "Mean for raw data (Optional: jackknife variance)\n");
  fprintf(stderr,
          "USAGE: \n"
          "    %s [OPTIONS] ifname1 ifname2 [ifname3 ...]\n",
          name);
  fprintf(stderr,
          "OPTIONS: \n"
          "    -l <LENGTH>:      Length of data arrays\n"
          "    -o <OFNAME>:      Name of output file\n"
          "    [-j]:             Calculate jackknife variance\n"
          "    [-jd]:            Calculate jackknife variance (input: DOUBLE)\n"
          "    [-t]:             Also save a txt file\n"
          "    [-h, --help]:     Print help\n");
}

// Custom function declaration
void arithmeticMean(char* rawDataList[], const char* result, int arrayLength,
                    int fileCountTotal);
void jackknifeMean(char* rawDataList[], const char* result, int arrayLength,
                   int fileCountTotal);
void jackknifeMeanD(char* rawDataList[], const char* result, int arrayLength,
                    int fileCountTotal);

// Main function
int main(int argc, char* argv[]) {
  // Global variables
  int arrayLength = 0;
  static const char* ofname = NULL;
  bool isJackknife = false;
  bool isJackknifeD = false;
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

    // -j: jackknife variance
    if (strcmp(argv[0], "-j") == 0) {
      isJackknife = true;
      argc--;
      argv++;
      continue;
    }

    // -jd: jackknife variance (on DOUBLE)
    if (strcmp(argv[0], "-jd") == 0) {
      isJackknifeD = true;
      argc--;
      argv++;
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

  const int fileCountTotal = argc;
  if (fileCountTotal < 2) {
    usage(programName);
    exit(1);
  }

  if (isJackknife) {
    jackknifeMean(argv, ofname, arrayLength, fileCountTotal);
  } else if (isJackknifeD) {
    jackknifeMeanD(argv, ofname, arrayLength, fileCountTotal);
  } else {
    arithmeticMean(argv, ofname, arrayLength, fileCountTotal);
  }

  if (isSaveTxt) {
    char txtfname[2048];
    addSuffix(ofname, "txt", txtfname);
    bin2txt(ofname, txtfname, arrayLength);
  }

  return 0;
}

// Custom function definition
void jackknifeMean(char* rawDataList[], const char* result, int arrayLength,
                   int fileCountTotal) {
  DVARRAY mean(arrayLength), var(arrayLength);
  mean = var = 0.0;

  for (int i = 0; i < fileCountTotal; i++) {
    CVARRAY tmp(arrayLength);
    tmp = 0.0;
    readBin(rawDataList[i], arrayLength, tmp);

    DVARRAY rtmp(arrayLength);
    rtmp = 0.0;
    keepReal(tmp, rtmp, arrayLength);
    // varryNorm(tmp, rtmp, arrayLength);

    mean += rtmp / DOUBLE(fileCountTotal);
  }

  for (int i = 0; i < fileCountTotal; i++) {
    CVARRAY tmp(arrayLength);
    tmp = 0.0;
    readBin(rawDataList[i], arrayLength, tmp);

    DVARRAY rtmp(arrayLength);
    rtmp = 0.0;
    keepReal(tmp, rtmp, arrayLength);
    // varryNorm(tmp, rtmp, arrayLength);

    var += (rtmp - mean) * (rtmp - mean);
  }

  var = sqrt(var * DOUBLE(fileCountTotal - 1) / DOUBLE(fileCountTotal));

  CVARRAY out(arrayLength);
  out = 0.0;

  for (int i = 0; i < arrayLength; i++) {
    out[i].real(mean[i]);
    out[i].imag(var[i]);
  }

  writeBin(result, arrayLength, out);
}

void jackknifeMeanD(char* rawDataList[], const char* result, int arrayLength,
                    int fileCountTotal) {
  DVARRAY mean(arrayLength), var(arrayLength);
  mean = var = 0.0;

  for (int i = 0; i < fileCountTotal; i++) {
    DVARRAY tmp(arrayLength);
    tmp = 0.0;
    readBin(rawDataList[i], arrayLength, tmp);

    mean += tmp / DOUBLE(fileCountTotal);
  }

  for (int i = 0; i < fileCountTotal; i++) {
    DVARRAY tmp(arrayLength);
    tmp = 0.0;
    readBin(rawDataList[i], arrayLength, tmp);

    var += (tmp - mean) * (tmp - mean);
  }

  var = sqrt(var * DOUBLE(fileCountTotal - 1) / DOUBLE(fileCountTotal));

  CVARRAY out(arrayLength);
  out = 0.0;

  for (int i = 0; i < arrayLength; i++) {
    out[i].real(mean[i]);
    out[i].imag(var[i]);
  }

  writeBin(result, arrayLength, out);
}

void arithmeticMean(char* rawDataList[], const char* result, int arrayLength,
                    int fileCountTotal) {
  CVARRAY mean(arrayLength);
  mean = 0.0;

  for (int i = 0; i < fileCountTotal; i++) {
    CVARRAY tmp(arrayLength);
    tmp = 0.0;
    readBin(rawDataList[i], arrayLength, tmp);

    mean += tmp / COMPLX(fileCountTotal, 0.0);
  }

  writeBin(result, arrayLength, mean);
}
