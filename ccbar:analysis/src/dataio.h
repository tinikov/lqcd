/**
 * @file dataio.h
 * @author Tianchen Zhang
 * @brief Deal with binary data.
 *        Provide 5 functions:
 *        void readBin(): Read data from binary file;
 *        void writeBin(): Write data to binary file;
 *        void keepReal(): Keep the real part of each element in complex valarray;
 *        void keepImag(): Keep the imaginary of each element in complex valarray;
 *        void varryNorm(): Calculate the norm of each element in complex valarray
 * @version 1.2
 * @date 2024-07-20
 *
 */

#ifndef CCBAR_SRC_DATAIO_H_
#define CCBAR_SRC_DATAIO_H_

#include <complex>
#include <valarray>

#include "alias.h"

/**
 * @brief Read data from binary file
 *
 * @param ifname Input file name of the data file
 * @param arrayLength Total of double/complex numbers
 * @param data The pointer of the array that contains the data to be read
 */
void readBin(const char* ifname, int arrayLength, DOUBLE* data);
void readBin(const char* ifname, int arrayLength, COMPLX* data);
void readBin(const char* ifname, int arrayLength, DVARRAY& data);
void readBin(const char* ifname, int arrayLength, CVARRAY& data);

/**
 * @brief Write data to binary file
 *
 * @param ofname Output file name of the data file
 * @param arrayLength Total of double/complex numbers
 * @param data The pointer of the array that contains the data to be written to
 * file
 */
void writeBin(const char* ofname, int arrayLength, const DOUBLE* data);
void writeBin(const char* ofname, int arrayLength, const COMPLX* data);
void writeBin(const char* ofname, int arrayLength, const DVARRAY& data);
void writeBin(const char* ofname, int arrayLength, const CVARRAY& data);

/**
 * @brief Keep the real part of each element in complex valarray
 *
 * @param data original data (complex valarray)
 * @param realData real part of the valarray (double valarray)
 * @param arrayLength Total of complex numbers
 */
void keepReal(CVARRAY& data, DVARRAY& realData, int arrayLength);

/**
 * @brief Keep the imaginary part of each element in complex valarray
 *
 * @param data original data (complex valarray)
 * @param imagData imaginary part of the valarray (double valarray)
 * @param arrayLength Total of complex numbers
 */
void keepImag(CVARRAY& data, DVARRAY& imagData, int arrayLength);

/**
 * @brief Calculate the norm of each element in complex valarray
 *
 * @param data original data (complex valarray)
 * @param normData norm of the valarray (double valarray)
 * @param arrayLength Total of complex numbers
 */
void varryNorm(CVARRAY& data, DVARRAY& normData, int arrayLength);
#endif
