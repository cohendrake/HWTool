#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "common.h"
#include "common_fun.h"
#include "fftw3.h"
#include "xcorr.h"

int CrossCorr(double* x, int xlen, double* y, int ylen, double* c, int clen, int maxlag)
{
	fftw_plan plan1, plan2;
	double *in = NULL;
	fftw_complex *out = NULL;
	int max_len, full_len, half_len,i;

	max_len = MAX(xlen, ylen);
	if (clen != 2 * maxlag + 1)
	{
		return -1;
	}

	full_len = NextPow2(2 * max_len - 1);
	half_len = full_len / 2 + 1;

	if (NULL == (in = (double *)fftw_alloc_real(full_len * 2 *sizeof(double))))
	{
		return -1;
	}

	if (NULL == (out = (fftw_complex *)fftw_alloc_complex(half_len * 2 * sizeof(fftw_complex))))
	{
		return -1;
	}

	for (i = 0; i < full_len * 2; i++)
	{
		if (i < xlen)
		{
			in[i] = x[i];
		}
		else if (i >= full_len && i < full_len + ylen)
		{
			in[i] = y[i - full_len];
		}
		else
		{
			in[i] = 0.0f;
		}
	}

	int n[] = { full_len };
	int *inembed = n;
	int *onembed = n;
	int istride = 1;
	int ostride = 1;
	int idist = full_len;
	int odist = half_len;

	//FFT
	plan1 = fftw_plan_many_dft_r2c(1, n, 2, in, inembed, istride, idist, out, onembed, ostride, odist, FFTW_ESTIMATE);
	fftw_execute(plan1);

	// X * conj(Y)
	double real, imag;
	for (i = 0; i < half_len; i++)
	{
		real = out[i][0] * out[i + half_len][0] + out[i][1] * out[i + half_len][1];
		imag = out[i][1] * out[i + half_len][0] - out[i][0] * out[i + half_len][1];
		out[i][0] = real;
		out[i][1] = imag;
	}

	//IFFT
	plan2 = fftw_plan_many_dft_c2r(1, n, 1, out, onembed, ostride, odist, in, inembed, istride, idist, FFTW_ESTIMATE);
	fftw_execute(plan2);
	for (i = 0; i < full_len * 2; i++)
	{
		in[i] /= full_len;
	}/*normalize*/

	memset(c, 0, sizeof(c[0]) * clen);
	if (maxlag <= max_len - 1)
	{
		memmove(c + maxlag, in, sizeof(in[0]) * (maxlag + 1));
		memmove(c, in + full_len - maxlag, sizeof(in[0]) * maxlag);
	}
	else
	{
		memmove(c + maxlag, in, sizeof(in[0]) * max_len);
		memmove(c + maxlag - max_len + 1, in + full_len - max_len + 1, sizeof(in[0]) * (max_len - 1));
	}


	fftw_destroy_plan(plan1);
	fftw_destroy_plan(plan2);
	fftw_free(in);
	fftw_free(out);
	return 0;
}

int LagEst(double* x, int xlen, double* y, int ylen, int* lag)
{
	int clen, maxlag;
	double *a, *b, *c;
	maxlag = MAX(xlen, ylen) - 1;
	clen = 2 * maxlag + 1; 
	if (NULL == (c = (double*)malloc(sizeof(double) * clen)))
	{
		return -1;
	}
	a = x;
	b = y;
	if (CrossCorr(a, xlen, b, ylen, c, clen, maxlag) != 0) return -1 ;
	*lag = VectorMaxIdx(c, clen) - maxlag;
	free(c);
	return 0;
}


int VectorAlign(double* x, int xlen, double* y, int ylen, double* out, int out_len)
{
	int lag;
	if (xlen != out_len)
	{
		return -1;
	}
	if (LagEst(x, xlen, y, ylen, &lag) == 0)
	{
		memset(out, 0, sizeof(out[0]) * out_len);
		if (lag > 0)
		{
			memmove(out + lag, y, sizeof(y[0]) * MIN(out_len - lag, ylen));
		}
		else
		{
			memmove(out, y - lag, sizeof(y[0]) * MIN(out_len , ylen + lag));
		}
		return 0;
	}
	else
	{
		return -1;
	}
}

int FirWiener(double*x, int xlen, double*y, int ylen, double* w, int N)
{
	double *rxx, *ryx, *a, *tmp;
	double r,e,d,q,G;
	int maxlag = N - 1;
	int len = 2 * maxlag + 1;
	int i;
	memset(w, 0, N * sizeof(double));
	if (maxlag >= xlen) /*ensure rxx is nonzero*/
	{
		return -1;
	}

	if (NULL == (rxx = (double *)malloc(sizeof(rxx[0]) * len)))
	{
		return -1;
	}

	if (NULL == (ryx = (double *)malloc(sizeof(ryx[0]) * len)))
	{
		return -1;
	}

	if (NULL == (a = (double *)malloc(sizeof(a[0]) * (maxlag + 1))))
	{
		return -1;
	}

	if (NULL == (tmp = (double *)malloc(sizeof(tmp[0]) * (maxlag + 1))))
	{
		return -1;
	}

	if (CrossCorr(x, xlen, x, xlen, rxx, len, maxlag) != 0)
	{
		return -1;
	}

	if (CrossCorr(x, xlen, y, ylen, ryx, len, maxlag) != 0)
	{
		return -1;
	}

	for (i = 0; i < maxlag + 1; i++)
	{
		rxx[i] = rxx[i + maxlag];
		if (i <= (maxlag - 1) / 2)
		{
			Swap(ryx + i, ryx + maxlag - i);
		}
	}
	memset(a, 0, sizeof(a[0]) * (maxlag + 1));
	a[0] = 1.0f;
	w[0] = ryx[0] / rxx[0];/**/
	e = rxx[0];

	for (i = 0; i < maxlag; i++)
	{
		if (i)
		{
			VectorFliplr(rxx + 1, tmp, i);
			r = rxx[i+1] + VectorInnerProduct(a + 1, tmp, i);
		}
		else
		{
			r = rxx[i+1];
		}
		G = -1.0f * r / e;

		VectorFliplr(a, tmp, i + 2);
		MatrixConstProduct(G, tmp, tmp, i + 2);
		MatrixAdd(a,tmp,a,i + 2);

		e = e * (1 - fabs(G * G));

		VectorFliplr(rxx + 1, tmp, i + 1);
		d = VectorInnerProduct(w, tmp, i + 1);

		q = (ryx[i+1] - d) / e;
		VectorFliplr(a, tmp, i + 2);
		MatrixConstProduct(q, tmp, tmp, i + 2);
		MatrixAdd(w, tmp, w, i + 2);
	}

	free(rxx);
	free(ryx);
	free(a);
	free(tmp);
	return 0;
}

int WienerResid(double*x, double*y, double* out, int len, int order)
{
	double *w;
	if (NULL == (w = (double *)malloc(sizeof(double) * order))) return -1;
	if (0 != FirWiener(x, len, y, len, w, order)) return -1;
	if (0 != FFTFilt(x, len, out, len, w, order)) return -1;
	MatrixConstProduct(-1.0f, out, out, len);
	MatrixAdd(y, out, out, len);
	free(w);
	return 0;
}

int FFTFilt(double*x, int xlen, double*y, int ylen, double* w, int N)
{
	if (xlen > ylen) return -1;
	int fralen = NextPow2(2 * N);
	int framov = fralen - N;
	int franum = xlen / framov + 1;
	int half_len = fralen / 2 + 1;
	int i, j;
	double *fra = NULL;
	double *out = NULL;
	fftw_complex *tmp = NULL;
	fftw_complex *fw = NULL;
	double real, imag;
	fftw_plan plan1, plan2;
	if (NULL == (fra = (double *)fftw_alloc_real(fralen * sizeof(double))))
	{
		return -1;
	}

	if (NULL == (out = (double *)fftw_alloc_real(fralen * sizeof(double))))
	{
		return -1;
	}

	if (NULL == (tmp = (fftw_complex *)fftw_alloc_complex(half_len * sizeof(fftw_complex))))
	{
		return -1;
	}

	if (NULL == (fw = (fftw_complex *)fftw_alloc_complex(half_len * sizeof(fftw_complex))))
	{
		return -1;
	}

	// W
	memset(fra, 0, fralen * sizeof(double));
	memmove(fra, w, sizeof(double) * N);
	plan1 = fftw_plan_dft_r2c_1d(fralen, fra, fw, FFTW_ESTIMATE);
	fftw_execute(plan1);

	//
	memset(fra, 0, fralen * sizeof(double));
	plan1 = fftw_plan_dft_r2c_1d(fralen, fra, tmp, FFTW_ESTIMATE);
	plan2 = fftw_plan_dft_c2r_1d(fralen, tmp, out, FFTW_ESTIMATE);
	
	for (i = 0; i < franum; i++)
	{
		memmove(fra, fra + framov, (fralen - framov) * sizeof(double));
		if (i < (franum - 1))
		{
			memmove(fra + fralen - framov, x + i * framov, framov * sizeof(double));
		}
		else
		{
			memmove(fra + fralen - framov, x + i * framov, (xlen - (franum - 1) * framov) * sizeof(double));
		}
		fftw_execute(plan1);
		for (j = 0; j < half_len; j++)
		{
			real = tmp[j][0] * fw[j][0] - tmp[j][1] * fw[j][1];
			imag = tmp[j][0] * fw[j][1] + tmp[j][1] * fw[j][0];
			tmp[j][0] = real;
			tmp[j][1] = imag;
		}
		fftw_execute(plan2);
		for (j = fralen - framov; j < fralen; j++)
		{
			out[j] /= (double)fralen;
		}
		if (i < (franum - 1))
		{
			memmove(y + i * framov, out + fralen - framov, framov * sizeof(double));
		}
		else
		{
			memmove(y + i * framov, out + fralen - framov, (xlen - (franum - 1) * framov) * sizeof(double));
		}

	}

	fftw_destroy_plan(plan1);
	fftw_destroy_plan(plan2);
	fftw_free(fra);
	fftw_free(tmp);
	fftw_free(out);
	fftw_free(fw);
	return 0;
}