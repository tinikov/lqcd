/**
 * @file dataio.cc
 * @author Tianchen Zhang
 * @brief
 * @version 1.2
 * @date 2024-07-20
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
