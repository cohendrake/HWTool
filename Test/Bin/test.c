#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "hwtest.h"

int main()
{                    
	//double M[12] = {1,0,0,1,0,0,1,0,0,1,0,0};
	//int i;
	//int m = 4;
	//int n = 3;
	//MatrixTranspose(M, m, n);
	//for(i=0; i<m*n; i++)
	//{
	//	fprintf(stdout, "%.6f\n",M[i]);
	//}

	//IIRFilterCascade(M, m*n, IIR_HP200_COEF, IIR_HP200_SCALE);

	//double x[5] = { 1,2,1,2,1 };
	//double z[5] = { 0.0 };
	//double y[10] = { 0,1,2,3,0,1,2,3,0,0 };
	//double c[5] = { 0.0 };
	//double w[5] = { 1, 0, 0 ,0 ,0 };
	//int lag;
	//if (CrossCorr(x, 5, x, 5, c, 5, 2) == 0)
	//{
	//	for (i = 0; i < 5; i++)
	//	{
	//		fprintf(stdout,"XCORR:%.6f\n",c[i]);
	//	}
	//}

	//if (LagEst(x, 5, y, 10, &lag) == 0)
	//{
	//	fprintf(stdout,"Lag:%d\n",lag);
	//}

	//if (VectorAlign(x, 5, y, 10, z, 5) == 0)
	//{
	//	for (i = 0; i < 5; i++)
	//	{
	//		fprintf(stdout, "Align:%.6f\n", z[i]);
	//	}
	//}

	//if (FirWiener(x, 5, y, 10, w, 5) == 0)
	//{
	//	for (i = 0; i < 5; i++)
	//	{
	//		fprintf(stdout, "Wiener:%.6f\n", w[i]);
	//	}
	//}

	//pcmprop_t pcmprop;
	//PcmInit(&pcmprop, "test16.wav", 16000, 1, 16, 44, 56);
	//if (PcmRead(&pcmprop) == 0)
	//{
	//	for (i = 0; i < (int)pcmprop.glen; i++)
	//	{
	//		fprintf(stdout, "Wav: %.6f\n", pcmprop.g[i]);
	//	}
	//}
	//PcmDestroy(&pcmprop);

	//double w[5] = { 1, 2 };
	//double x[10] = { 1,2,3,4,5,6,7,8,9,10 };
	//double y[10] = { 0.0 };
	//if (FFTFilt(x, 10, y, 10, w, 2) == 0)
	//{
	//	for (int i = 0; i < 10; i++)
	//	{
	//		fprintf(stdout, "FFTfilt: %.6f\n", y[i]);
	//	}
	//}
	aec_test_t aec_rslt[AEC_PCM_NUM];
	char *file[AEC_PCM_NUM] = {"test1_16k2.wav","test2_16k2.wav"};
	unsigned int fs[AEC_PCM_NUM] = {16000, 16000};
	unsigned int channels[AEC_PCM_NUM] = {3, 3};
	unsigned int nbits[AEC_PCM_NUM] = { 16, 16 };
	unsigned int hd_len[AEC_PCM_NUM] = {44, 44};
	unsigned int data_size[AEC_PCM_NUM] = {96044, 96044};
	short mic[AEC_PCM_NUM] = {0x0001, 0x0001};
	short ref[AEC_PCM_NUM] = {0x0002, 0x0002};
	TestProc(&aec_rslt[0], file[0], fs[0], channels[0], nbits[0], hd_len[0], data_size[0], mic[0], ref[0], 1);
	TestProc(&aec_rslt[1], file[1], fs[1], channels[1], nbits[1], hd_len[1], data_size[1], mic[1], ref[1], 1);
	return 0;
}


