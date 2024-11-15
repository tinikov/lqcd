#!/usr/bin/env python3

import warnings

warnings.filterwarnings("ignore", category=RuntimeWarning)

import argparse
import numpy as np
from iminuit import Minuit
from iminuit.cost import LeastSquares

parser = argparse.ArgumentParser(prog="fit-mqq", description="Fit hadron masses")
parser.add_argument(
    "-t", "--tsize", type=int, required=True, help="temporal size of lattice"
)
parser.add_argument(
    "-c", "--cutoff", type=np.float64, required=True, help="lattice cutoff"
)
parser.add_argument(
    "--ftype", choices=["exp", "csh"], required=True, help="fit function: exp or csh"
)
parser.add_argument(
    "-r",
    "--range",
    type=int,
    nargs=2,
    required=True,
    metavar=("MIN", "MAX"),
    help="fit range: [nt_min, nt_max]",
)
parser.add_argument("ifname", nargs="+", type=str, help="file list for fit")
args = parser.parse_args()

tmin = args.range[0]
tmax = args.range[1]
t_size = args.tsize
if tmin >= tmax or tmin < 0 or tmax > t_size:
    print("Please check the range for fit! ")
    exit(1)
tsites = np.arange(0, t_size, 1)
fitsites = np.arange(tmin, tmax, 1)
fileCountTotal = len(args.ifname)


# Define functions for fit
def expFit(n, A, M):
    return A * np.exp(-M * n)


def coshFit(n, A, M):
    return A * np.cosh(M * (n - t_size / 2))


para = {
    "A": 0.01,
    "M": 1.0,
}

A_arr, M_arr, chisq_arr = [], [], []
file_index = 0

for ifname in args.ifname:
    rawdata = np.fromfile(ifname, dtype=np.float64)
    assert rawdata.size == t_size * 2
    rawdata = rawdata.reshape(t_size, 2)

    file_index += 1

    corr = rawdata[:, 0]
    err = rawdata[:, 1]

    fitdata = rawdata[tmin:tmax]
    fitcorr = fitdata[:, 0]
    fiterr = fitdata[:, 1]

    if args.ftype == "exp":
        least_squares = LeastSquares(fitsites, fitcorr, fiterr, expFit)  # type: ignore
    if args.ftype == "csh":
        least_squares = LeastSquares(fitsites, fitcorr, fiterr, coshFit)  # type: ignore
    m = Minuit(least_squares, **para)  # type: ignore
    m.migrad()

    A_arr.append(m.values["A"])
    M_arr.append(m.values["M"])
    chisq_arr.append(m.fval)

# degree of freedom: (# of data) - (# of parameters) - 1
df = tmax - tmin - 2 - 1

A_arr = np.array(A_arr)
M_arr = np.array(M_arr)
chisq_arr = np.array(chisq_arr) / df

A_mean = np.mean(A_arr)
M_mean = np.mean(M_arr)
chisq_mean = np.mean(chisq_arr)

A_var = np.sqrt(np.var(A_arr) * (fileCountTotal - 1) / fileCountTotal)
M_var = np.sqrt(np.var(M_arr) * (fileCountTotal - 1) / fileCountTotal)
chisq_var = np.sqrt(np.var(chisq_arr) * (fileCountTotal - 1) / fileCountTotal)

cutoff = args.cutoff * 1000

# print("##  A      = {} ± {}".format(A_mean, A_var))
print(f"##  Fit range: [{tmin}, {tmax}]")
print(f"##  M(LU)  = {M_mean} ± {M_var}")
print(f"##  M      = {M_mean * cutoff} ± {M_var * cutoff}")
print(f"##  χ^2/df = {chisq_mean} ± {chisq_var}")
