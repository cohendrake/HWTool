#ifndef HWTEST_H_
#define HWTEST_H_

#define AEC_PCM_NUM 2 

#define PCM_MAX_CH 16

#define FIR_WIENER_ORDER 1024

//#define AEC_DEBUG

typedef struct aec_test {
	unsigned int channels;
	short mic;
	short ref;
	double ch_energy[PCM_MAX_CH];
	double ch_peak[PCM_MAX_CH];
	int overflow[PCM_MAX_CH];
	int aec_delay;
	double aec_nc;
}aec_test_t;

int __declspec(dllexport) TestProc(aec_test_t *rslt, char *filename, unsigned int fs, unsigned int channels, unsigned int nbits, \
	unsigned int hd_len, unsigned int data_size, short mic, short ref, int aec_flag);

#endif
