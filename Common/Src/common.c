#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "common.h"
#include "common_fun.h"

double Avg(double *x, int len)
{
	int i;
	double tmp = 0.0f;
	for(i=0; i<len; i++)
	{
		tmp += x[i];
	}
	tmp /= (double)i;
	return tmp;
}

int BitAdd(short bit)
{
	int count = 0;
	while (bit)
	{
		++count;
		bit &= (bit - 1);
	}
	return count;
}

double Var(double *x, int len)
{
	int i;
	double avg;
	double avg2;
	double *x2 = NULL;
	if (NULL == (x2 = malloc(sizeof(double) * len))) return -1;
	avg = Avg(x, len);
	for(i=0; i<len; i++)
	{
		x2[i] = x[i] * x[i];
	}
	avg2 = Avg(x2, len);
	free(x2);
	return (avg2 - avg * avg);
}

void Swap(double *x, double *y)
{
	double tmp;
	tmp = *x;
	*x = *y;
	*y = tmp;
}

int OverflowCheckInt16(short x)
{
	if (x == INT16_MAX || x == INT16_MIN)
	{
		return 1;
	}
	else
	{	
		return 0;
	}
}

int OverflowCheckStd(double *x, int len)
{
	for (int i = 0; i < len; i++)
	{
		if (fabs(x[i] - 1.0f) < MIN_FLOAT_DIFF || fabs(x[i] + 1.0f) < MIN_FLOAT_DIFF)
		{
			return -1;
		}
	}
	return 0;
}
 
int NextPow2(int x)
{
	return (int)pow(2,(double)((int)(log2((double)x) + 1)));
}

int VectorMaxIdx(double *x, int len)
{
	int max_idx = 0;
	int i = 0;
	double max_val = 0;
	for (i = 0; i < len; i++)
	{
		if (max_val <= x[i])
		{
			max_val = x[i];
			max_idx = i;
		}
	}
	return max_idx;
}

double VectorMax(double *x, int len)
{
	int max_idx = 0;
	int i = 0;
	double max_val = 0;
	for (i = 0; i < len; i++)
	{
		if (max_val <= x[i])
		{
			max_val = x[i];
			max_idx = i;
		}
	}
	return max_val;
}

/*Shall not be used for inplace fliplr!!!!*/
void VectorFliplr(double *in, double *out, int len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		out[i] = in[len - i - 1];/*in & out shall not be the same point*/
	}
}

double VectorInnerProduct(double *x, double *y, int len)
{
	double tmp = 0.0f;
	int i;
	for (i = 0; i < len; i++)
	{
		tmp += (x[i] * y[i]);
	}
	return tmp;
}

void MaxtrixHdmdProduct(double *I1, double *I2, double *O, int len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		O[i] = I1[i] * I2[i];
	}
}

void MatrixAdd(double *I1, double *I2, double *O, int len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		O[i] = I1[i] + I2[i];
	}
}

/*Support inplace constant product!!!*/
void MatrixConstProduct(double c, double* I, double* O, int len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		O[i] = I[i] * c;
	}
}

void MatrixTranspose(double *mtx, int m, int n)  
{  
	int i,next,pre,cur;
	double tmp;
	for(i=0; i<m*n; ++i)  
	{  
		next = (i%n)*m + i/n;  
		while(next > i) 
		{
			next = (next%n)*m + next/n;
		}    
		if(next == i)    
		{
			tmp = mtx[i];
			cur = i;
			pre = (i%m)*n + i/m;  
			while(pre != i)
			{
				mtx[cur] = mtx[pre];
				cur = pre;
				pre = (pre%m)*n + pre/m; 
			}
			mtx[cur] = tmp;
		}  
	}  
}  





