#ifndef XCORR_H_
#define XCORR_H_

int CrossCorr(double* x, int xlen, double* y, int ylen, double* c, int clen, int maxlag);

int LagEst(double *x, int xlen, double *y, int ylen, int *lag);

int VectorAlign(double* x, int xlen, double* y, int ylen, double* out, int out_len);

int FirWiener(double*x, int xlen, double*y, int ylen, double* w, int N);

int WienerResid(double*x, double*y, double* out, int len, int order);

int FFTFilt(double*x, int xlen, double*y, int ylen, double* w, int N);

#endif
