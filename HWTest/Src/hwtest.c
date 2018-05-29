#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "common_fun.h"
#include "common.h"
#include "iir.h"
#include "xcorr.h"
#include "pcmio.h"
#include "hwtest.h"

int TestProc(aec_test_t *rslt, char *filename, unsigned int fs, unsigned int channels, unsigned int nbits, \
	unsigned int hd_len, unsigned int data_size, short mic, short ref, int aec_flag)
{
	double w[FIR_WIENER_ORDER] = { 0.0 };
	int lag;
	unsigned int mic_ch_cnt,mic_ch, ref_ch_cnt, ref_ch;
	double *out = NULL;
	int i;
	int L = 0;
	double v1 = 0;
	double v2 = 0;
	double min_mic_energy, max_mic_energy;
	pcmprop_t pcmprop;

	if (PcmInit(&pcmprop, filename, fs, channels, nbits, hd_len, data_size, mic, ref) != 0) return -1;
	L = pcmprop.sample_per_ch;
	//Read
	if (PcmRead(&pcmprop) != 0) return -1;
	MatrixTranspose(pcmprop.g, L, pcmprop.channels);

	//Choose mic & ref channel
	rslt->mic = mic;
	rslt->ref = ref;
	rslt->channels = pcmprop.channels;
	mic_ch_cnt = 0;
	ref_ch_cnt = 0;
	if (NULL == (out = (double *)malloc(L * sizeof(double)))) return -1;
	if ((mic & ref) != 0 || BitAdd(mic | ref) > (int)pcmprop.channels || pcmprop.channels > PCM_MAX_CH) return -1;

	for (i = 0; i < (int)pcmprop.channels; i++)
	{
		if (pcmprop.nbits == 16)
		{
			rslt->overflow[i] = OverflowCheckStd(pcmprop.g + i * L, L);
		}
		else
		{
			rslt->overflow[i] = 0;
		}
		rslt->ch_peak[i] = VectorMax(pcmprop.g + i * L, L);
		IIRFilterCascade(pcmprop.g + i * L, L, IIR_HP200_COEF, IIR_HP200_SCALE);
		rslt->ch_energy[i] = 10.0f * log10(Var(pcmprop.g + i * L, L));
		if (mic & 0x0001)
		{
			if (mic_ch_cnt == 0)
			{
				mic_ch = i;
				max_mic_energy = rslt->ch_energy[i];
				min_mic_energy = rslt->ch_energy[i];
			}
			else
			{
				if (rslt->ch_energy[i] > max_mic_energy) max_mic_energy = rslt->ch_energy[i];
				if (rslt->ch_energy[i] < min_mic_energy) min_mic_energy = rslt->ch_energy[i];
			}
			mic_ch_cnt++;
		}

		if (ref & 0x0001)
		{
			if (ref_ch_cnt == 0)
			{
				ref_ch = i;
			}
			ref_ch_cnt++;
		}
		mic = mic >> 1;
		ref = ref >> 1;
	}

	if (aec_flag)
	{
		//Lag Estimate
		if (0 != LagEst(pcmprop.g + mic_ch * L, L, pcmprop.g + ref_ch * L, L, &lag)) return -1;
		rslt->aec_delay = lag;


		//AEC NC
		if (0 != WienerResid(pcmprop.g + ref_ch * L, pcmprop.g + mic_ch * L, out, L, FIR_WIENER_ORDER)) return -1;
		v1 = Var(out, L);
		v2 = Var(pcmprop.g + mic_ch * L, L);
		rslt->aec_nc = 10.0f * log10(MAX(v1, MIN_DIV0) / v2);

#ifdef AEC_DEBUG
		memmove(pcmprop.g + ref_ch * L, out, L * sizeof(double));
		MatrixTranspose(pcmprop.g, pcmprop.channels, L); //channel combine
		strcpy(pcmprop.name, "channel.pcm");
		PcmWrite(&pcmprop);
#endif
	}
	else
	{
		rslt->aec_delay = 0;
		rslt->aec_nc = 0;
	}
	

	free(out);
	PcmDestroy(&pcmprop);
	return 0;

}