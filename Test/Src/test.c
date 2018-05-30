#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "hwtest.h"

int main()
{                    
	aec_test_t aec_rslt[AEC_PCM_NUM];
	char *file[AEC_PCM_NUM] = {"test1_16k_ovfl.wav","test2_16k2.wav"};
	unsigned int fs[AEC_PCM_NUM] = {16000, 16000};
	unsigned int channels[AEC_PCM_NUM] = {3, 3};
	unsigned int nbits[AEC_PCM_NUM] = { 16, 16 };
	unsigned int hd_len[AEC_PCM_NUM] = {44, 44};
	unsigned int data_size[AEC_PCM_NUM] = {96044, 96044};
	short mic[AEC_PCM_NUM] = {0x0001, 0x0001};
	short ref[AEC_PCM_NUM] = {0x0002, 0x0002};
	if (0 != TestProc(&aec_rslt[0], file[0], fs[0], channels[0], nbits[0], hd_len[0], data_size[0], mic[0], ref[0], 1)) return -1;
	if (0 != TestProc(&aec_rslt[1], file[1], fs[1], channels[1], nbits[1], hd_len[1], data_size[1], mic[1], ref[1], 1)) return -1;
	return 0;
}


