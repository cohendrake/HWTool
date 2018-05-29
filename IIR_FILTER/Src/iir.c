#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "iir.h"

//IIR filter
void IIRFilter(double* io, int len, double* iir_coef, double oscale)
{
	double tmp[IIR_MAX_ORDER] = { 0.0f };
	int i,j;
	double *a = iir_coef + IIR_NUM_ORDER;
	double *b = iir_coef;
	for(i=0; i<len; i++)
	{
		for(j=1; j<IIR_MAX_ORDER; j++)
		{
			io[i] +=  -1.0f * a[MIN(j,IIR_DEN_ORDER)] * tmp[j];
		}
		tmp[0] = io[i];
		io[i] = 0.0f;
		for(j=0; j<IIR_MAX_ORDER; j++)
		{
			io[i] +=  b[MIN(j,IIR_NUM_ORDER)] * tmp[j];
		}
		memmove(&tmp[1], &tmp[0], sizeof(tmp[0])*(IIR_MAX_ORDER - 1));
		io[i] = io[i] * oscale;		
	}
}


//IIR cascade
void IIRFilterCascade(double* io, int len, double (*iir_coef_table)[IIR_TOTAL_ORDER], double* scale)
{
	int i;
	for(i=0; i<IIR_SECTION; i++)
	{
		IIRFilter(io, len, iir_coef_table[i], scale[i]);
	}
}




