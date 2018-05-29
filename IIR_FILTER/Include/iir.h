#ifndef IIR_H_
#define IIR_H_

#define IIR_SECTION (4)

#define IIR_DEN_ORDER (3)

#define IIR_NUM_ORDER (3)

#define IIR_TOTAL_ORDER (IIR_DEN_ORDER + IIR_NUM_ORDER)

#define IIR_MAX_ORDER ((IIR_DEN_ORDER) < (IIR_NUM_ORDER) ? (IIR_NUM_ORDER) : (IIR_DEN_ORDER))

extern double IIR_HP200_SCALE[IIR_SECTION];

extern double IIR_HP200_COEF[IIR_SECTION][IIR_TOTAL_ORDER];

void IIRFilter(double* io, int len, double* iir_coef, double oscale);

void IIRFilterCascade(double* io, int len, double (*iir_coef_table)[IIR_TOTAL_ORDER], double *scale);

#endif

