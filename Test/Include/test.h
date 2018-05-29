#ifndef TEST_H_
#define TEST_H_

#define AEC_PCM_NUM 2 

#define PCM_MAX_CH 16

#define FIR_WIENER_ORDER 1024

#define AEC_DEBUG

typedef struct aec_test {
	unsigned channels;
	short mic;
	short ref;
	double ch_energy[PCM_MAX_CH];
	double ch_peak[PCM_MAX_CH];
	int overflow[PCM_MAX_CH];
	int aec_delay;
	double aec_nc;
}aec_test_t;

int TestProc(pcmprop_t *pcmprop, aec_test_t *rslt, int aec_flag);

int TestInit(pcmprop_t pcmprop[AEC_PCM_NUM], char *filename[AEC_PCM_NUM], unsigned int fs[AEC_PCM_NUM], \
	unsigned int channels[AEC_PCM_NUM], unsigned int nbits[AEC_PCM_NUM], \
	unsigned int hd_len[AEC_PCM_NUM], unsigned int data_size[AEC_PCM_NUM], short mic[AEC_PCM_NUM], short ref[AEC_PCM_NUM]);

void TestDestroy(pcmprop_t *pcmprop);

#endif
