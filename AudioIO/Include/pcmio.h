#ifndef PCMIO_H_
#define PCMIO_H_

typedef struct hdr {
	char riff[4];                          // String 'RIFF' without \0                
	unsigned int totsize;                  // Total file size - 8                     
	char wave[4];                          // String 'WAVE' without \0                
	char fmtstr[4];                        // String 'fmt_' without \0                
	unsigned int fmtsize;                  // Total size of format chunk (Usually 0x10)  from  compressionCode to nbitspersample         
	unsigned short compressionCode;        // Always 0x01                             
	unsigned short numchans;               // Number of channels                      
	unsigned int Fs;                       // Sample rate (in Hz)                     
	unsigned int nbytespersec;             // number of bytes/seconde                 
	unsigned short nbytespersample;        // number of bytes/sample                  
										   // (1=8 bit mono, 2=8 bit stereo or 16 bit mono, 4= 16 bit stereo)   
	unsigned short nbitspersample;         // number of bits/samples                  
	char data[4];                          // string 'data' without \0                
	unsigned int datsize;                  // number of data bytes (not samples)      
} hdr_t;

typedef struct pcmprop {
	char name[512];
	unsigned int fs;
	unsigned int channels;
	unsigned int nbits;
	unsigned int hd_len;
	unsigned int data_size;
	unsigned int sample_per_ch;
	unsigned int glen;
	double *g;
	short mic;
	short ref;
} pcmprop_t;

int PcmInit(pcmprop_t *pcmprop, const char *filename, unsigned int fs, unsigned int channels, unsigned int nbits, unsigned int hd_len, unsigned int data_size, short mic, short ref);

int PcmRead(pcmprop_t *pcmprop);

int PcmWrite(pcmprop_t *pcmprop);

void PcmDestroy(pcmprop_t *pcmprop);

#endif