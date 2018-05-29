#ifndef COMMON_FUN_H_
#define COMMON_FUN_H_

double Avg(double *x, int len);

int BitAdd(short bit);

double Var(double *x, int len);

void Swap(double *x, double *y);

int OverflowCheckInt16(short x);

int OverflowCheckStd(double *x, int len);

int NextPow2(int x);

int VectorMaxIdx(double *x, int len);

double VectorMax(double *x, int len);

void VectorFliplr(double *in, double *out, int len);

double VectorInnerProduct(double *x, double *y, int len);

void MaxtrixHdmdProduct(double *I1, double *I2, double *O, int len);

void MatrixAdd(double *I1, double *I2, double *O, int len);

void MatrixConstProduct(double c, double *I, double *O, int len);

void MatrixTranspose(double *mtx, int m, int n);
#endif
