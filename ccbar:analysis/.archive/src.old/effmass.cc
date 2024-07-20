/**
 * @file effmass.cc
 * @author Tianchen Zhang
 * @brief Effective masses for charmonium
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
  fprintf(stderr,
          "Effective masses for charmonium (ofname: exp.xxx and csh.xxx)\n");
  fprintf(stderr,
          "USAGE: \n"
          "    %s [OPTIONS] ifname1 [ifname2 ...]\n",
          name);
  fprintf(stderr,
          "OPTIONS: \n"
          "    -n <TSIZE>:       Temporal size of lattice\n"
          "    -d <OFDIR>:       Directory of output files\n"
          "    [-t]:             Also save a txt file (add \".txt\" suffix)\n"
          "    [-h, --help]:     Print help\n");
}

// Custom function declaration
void expMass(char* rawDataList[], char* expList[], int tSize,
             int fileCountTotal);
void cshMass(char* rawDataList[], char* cshList[], int tSize,
             int fileCountTotal);

// Main function
int main(int argc, char* argv[]) {
  // Global variables
  int tSize = 0;
  static const char* ofDir = NULL;
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

  // Initialization
  const int fileCountTotal = argc;  // # of data files
  if (fileCountTotal < 1) {
    usage(programName);
    exit(1);
  }

  // Create arrays to store ofnames
  char *expNameArr[fileCountTotal], *cshNameArr[fileCountTotal];
  for (int i = 0; i < fileCountTotal; i++) {
    char stmp[2048];

    expNameArr[i] = (char*)malloc(2048 * sizeof(char));
    addPrefix(argv[i], "exp", stmp);
    changePath(stmp, ofDir, expNameArr[i]);

    cshNameArr[i] = (char*)malloc(2048 * sizeof(char));
    addPrefix(argv[i], "csh", stmp);
    changePath(stmp, ofDir, cshNameArr[i]);
  }

  // Main part for calculation
  expMass(argv, expNameArr, tSize, fileCountTotal);
  cshMass(argv, cshNameArr, tSize, fileCountTotal);

  if (isSaveTxt) {
    for (int i = 0; i < fileCountTotal; i++) {
      char txttmp[2048];

      addSuffix(expNameArr[i], "txt", txttmp);
      bin2txt(expNameArr[i], txttmp, tSize);

      addSuffix(cshNameArr[i], "txt", txttmp);
      bin2txt(cshNameArr[i], txttmp, tSize);
    }
  }

  // Finalization for the string arrays
  for (int i = 0; i < fileCountTotal; i++) {
    free(expNameArr[i]);
    free(cshNameArr[i]);
  }

  return 0;
}

// Custom function definition
void expMass(char* rawDataList[], char* expList[], int tSize,
             int fileCountTotal) {
  for (int i = 0; i < fileCountTotal; i++) {
    COMPLX raw[tSize], effmass[tSize];
    for (int j = 0; j < tSize; j++) {
      raw[j] = 0.0;
      effmass[j] = 0.0;
    }
    readBin(rawDataList[i], tSize, raw);

    for (int j = 0; j < tSize; j++) {
      effmass[j].real(log(raw[j].real() / raw[(j + 1) % tSize].real()));
    }

    writeBin(expList[i], tSize, effmass);
  }
}

DOUBLE
cshMassCal(int t1, int t2, DOUBLE corr1, DOUBLE corr2, int tSize) {
#define JMAX 100
#define M0 0.001
#define M1 10.0
#define MACC 1.0e-12
#define coshtype(m) \
  (corr1 / corr2 -  \
   cosh((m) * (tSize / 2.0 - t1)) / cosh((m) * (tSize / 2.0 - t2)))

  DOUBLE dm, f, fmid, mmid, mass;

  f = coshtype(M0);
  fmid = coshtype(M1);
  if (f * fmid >= 0.0) {
    fprintf(stderr, "Root must be bracketed for bisection in RTBIS\n");
    return NAN;
  }
  mass = f < 0.0 ? (dm = M1 - M0, M0) : (dm = M0 - M1, M1);
  for (int j = 1; j <= JMAX; j++) {
    mmid = mass + (dm *= 0.5);
    fmid = coshtype(mmid);
    if (fmid <= 0.0) mass = mmid;
    if (fabs(dm) < MACC || fmid == 0.0) return mass;
  }
  fprintf(stderr, "Too many bisections in RTBIS");
  return 0.0;
}

void cshMass(char* rawDataList[], char* cshList[], int tSize,
             int fileCountTotal) {
  for (int i = 0; i < fileCountTotal; i++) {
    COMPLX raw[tSize], effmass[tSize];
    for (int j = 0; j < tSize; j++) {
      raw[j] = 0.0;
      effmass[j] = 0.0;
    }
    readBin(rawDataList[i], tSize, raw);

    for (int j = 0; j < tSize; j++) {
      int t1 = j;
      int t2 = (j + 1) % tSize;
      effmass[j].real(
          cshMassCal(t1, t2, raw[t1].real(), raw[t2].real(), tSize));
    }

    writeBin(cshList[i], tSize, effmass);
  }
}
