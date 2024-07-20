/**
 * @file alias.h
 * @author Tianchen Zhang
 * @brief Define convenient aliases for double, complex and valarray types.
 * @version 1.1
 * @date 2024-02-13
 *
 */

#ifndef CCBAR_SRC_ALIAS_H_
#define CCBAR_SRC_ALIAS_H_

#include <complex>
#include <valarray>

using DOUBLE = double;
using COMPLX = std::complex<double>;
using DVARRAY = std::valarray<double>;
using CVARRAY = std::valarray<std::complex<double> >;

#endif  // CCBAR_SRC_ALIAS_H_
