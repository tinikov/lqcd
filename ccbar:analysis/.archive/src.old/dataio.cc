/**
 * @file dataio.cc
 * @author Tianchen Zhang
 * @brief
 * @version 1.1
 * @date 2024-02-13
 *
 */

#include "dataio.h"

#include <complex>
#include <valarray>

#include "alias.h"

void readBin(const char* ifname, int arrayLength, DOUBLE* data) {
  FILE* fp = fopen(ifname, "rb");
  if (fp == NULL) {
    perror(ifname);
    exit(1);
  }

  fread(data, sizeof(DOUBLE), arrayLength, fp);
  fclose(fp);
}
void readBin(const char* ifname, int arrayLength, COMPLX* data) {
  FILE* fp = fopen(ifname, "rb");
  if (fp == NULL) {
    perror(ifname);
    exit(1);
  }

  fread(data, sizeof(COMPLX), arrayLength, fp);
  fclose(fp);
}
void readBin(const char* ifname, int arrayLength, DVARRAY& data) {
  FILE* fp = fopen(ifname, "rb");
  if (fp == NULL) {
    perror(ifname);
    exit(1);
  }

  fread(&data[0], sizeof(DOUBLE), arrayLength, fp);
  fclose(fp);
}
void readBin(const char* ifname, int arrayLength, CVARRAY& data) {
  FILE* fp = fopen(ifname, "rb");
  if (fp == NULL) {
    perror(ifname);
    exit(1);
  }

  fread(&data[0], sizeof(COMPLX), arrayLength, fp);
  fclose(fp);
}

void writeBin(const char* ofname, int arrayLength, const DOUBLE* data) {
  FILE* fp = fopen(ofname, "wb");
  if (fp == NULL) {
    perror(ofname);
    exit(1);
  }

  fwrite(data, sizeof(DOUBLE), arrayLength, fp);
  fclose(fp);
}
void writeBin(const char* ofname, int arrayLength, const COMPLX* data) {
  FILE* fp = fopen(ofname, "wb");
  if (fp == NULL) {
    perror(ofname);
    exit(1);
  }

  fwrite(data, sizeof(COMPLX), arrayLength, fp);
  fclose(fp);
}
void writeBin(const char* ofname, int arrayLength, const DVARRAY& data) {
  FILE* fp = fopen(ofname, "wb");
  if (fp == NULL) {
    perror(ofname);
    exit(1);
  }

  fwrite(&data[0], sizeof(DOUBLE), arrayLength, fp);
  fclose(fp);
}
void writeBin(const char* ofname, int arrayLength, const CVARRAY& data) {
  FILE* fp = fopen(ofname, "wb");
  if (fp == NULL) {
    perror(ofname);
    exit(1);
  }

  fwrite(&data[0], sizeof(COMPLX), arrayLength, fp);
  fclose(fp);
}

void bin2txt(const char* binfname, const char* txtfname, int arrayLength) {
  COMPLX data[arrayLength];
  for (int i = 0; i < arrayLength; i++) {
    data[i] = 0.0;
  }

  FILE* ifp = fopen(binfname, "rb");
  if (ifp == NULL) {
    perror(binfname);
    exit(1);
  }

  fread(data, sizeof(COMPLX), arrayLength, ifp);
  fclose(ifp);

  FILE* ofp = fopen(txtfname, "w");
  if (ofp == NULL) {
    perror(txtfname);
    exit(1);
  }

  for (int i = 0; i < arrayLength; i++) {
    fprintf(ofp, "%d %1.16e %1.16e\n", i, data[i].real(), data[i].imag());
  }
  fclose(ofp);
}

void keepReal(CVARRAY& data, DVARRAY& realData, int arrayLength) {
  for (int i = 0; i < arrayLength; i++) {
    realData[i] = data[i].real();
  }
}

void keepImag(CVARRAY& data, DVARRAY& imagData, int arrayLength) {
  for (int i = 0; i < arrayLength; i++) {
    imagData[i] = data[i].imag();
  }
}

void varryNorm(CVARRAY& data, DVARRAY& normData, int arrayLength) {
  for (int i = 0; i < arrayLength; i++) {
    normData[i] = sqrt(norm(data[i]));
  }
}
