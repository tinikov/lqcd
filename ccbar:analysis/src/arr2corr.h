/**
 * @file arr2corr.h
 * @author Tianchen Zhang
 * @brief
 * @version 1.2
 * @date 2024-07-20
 *
 */

#ifndef CCBAR_SRC_ARR2CORR_H_
#define CCBAR_SRC_ARR2CORR_H_

#include <complex>
#include <valarray>

#include "alias.h"

inline DOUBLE& CORR(DOUBLE* data, int x, int y, int z, int xyzSize) {
  x = (x + xyzSize) % xyzSize;
  y = (y + xyzSize) % xyzSize;
  z = (z + xyzSize) % xyzSize;
  DOUBLE& corrReturn = data[x + xyzSize * (y + xyzSize * z)];
  return corrReturn;
}

inline COMPLX& CORR(COMPLX* data, int x, int y, int z, int xyzSize) {
  x = (x + xyzSize) % xyzSize;
  y = (y + xyzSize) % xyzSize;
  z = (z + xyzSize) % xyzSize;
  COMPLX& corrReturn = data[x + xyzSize * (y + xyzSize * z)];
  return corrReturn;
}

inline DOUBLE& CORR(DVARRAY& data, int x, int y, int z, int xyzSize) {
  x = (x + xyzSize) % xyzSize;
  y = (y + xyzSize) % xyzSize;
  z = (z + xyzSize) % xyzSize;
  DOUBLE& corrReturn = data[x + xyzSize * (y + xyzSize * z)];
  return corrReturn;
}

inline COMPLX& CORR(CVARRAY& data, int x, int y, int z, int xyzSize) {
  x = (x + xyzSize) % xyzSize;
  y = (y + xyzSize) % xyzSize;
  z = (z + xyzSize) % xyzSize;
  COMPLX& corrReturn = data[x + xyzSize * (y + xyzSize * z)];
  return corrReturn;
}

#endif
