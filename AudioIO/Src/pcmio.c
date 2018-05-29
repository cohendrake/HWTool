#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "pcmio.h"

int PcmRead(pcmprop_t *pcmprop)
{
	FILE *fd_in = NULL;
	int max_len = 0;
	void *x= NULL;
	int i;
	if (NULL == (fd_in = fopen(pcmprop->name, "rb")))
	{
		return -1;
	}

	max_len = pcmprop->glen;
	fseek(fd_in, pcmprop->hd_len, SEEK_SET);

	if (pcmprop->nbits == 16)
	{
		if (NULL == (x = (short*)malloc(max_len * sizeof(short))))
		{
			fclose(fd_in);
			return -1;
		}
		if (max_len > (int)fread((short *)x, sizeof(short), max_len, fd_in))
		{
			free(x);
			fclose(fd_in);
			return -1;
		}
		for (i = 0; i < max_len; i++)
		{
			pcmprop->g[i] = (double)(*((short*)x + i)) / INT16_MAX;
		}
	}
	else if (pcmprop->nbits == 32)
	{
		if (NULL == (x = (float*)malloc(max_len * sizeof(float))))
		{
			fclose(fd_in);
			return -1;
		}
		if (max_len > (int)fread((float *)x, sizeof(float), max_len, fd_in))
		{
			free(x);
			fclose(fd_in);
			return -1;
		}
		for (i = 0; i < max_len; i++)
		{
			pcmprop->g[i] = (double)(*((float*)x + i));
		}
	}
	else
	{
		fclose(fd_in);
		return -1;
	}

	free(x);
	fclose(fd_in);
	return 0;
}

int PcmWrite(pcmprop_t *pcmprop)
{
	FILE *fd_out = NULL;
	int max_len = 0;
	void *x = NULL;
	int i;
	if (NULL == (fd_out = fopen(pcmprop->name, "wb")))
	{
		return -1;
	}

	max_len = pcmprop->glen;
	fseek(fd_out, 0, SEEK_SET);

	if (pcmprop->nbits == 16)
	{
		if (NULL == (x = (short*)malloc(max_len * sizeof(short))))
		{
			fclose(fd_out);
			return -1;
		}
		for (i = 0; i < max_len; i++)
		{
			*((short*)x + i) = (short)(pcmprop->g[i] * INT16_MAX);//may add overflow check
		}
		if (max_len > (int)fwrite((short *)x, sizeof(short), max_len, fd_out))
		{
			free(x);
			fclose(fd_out);
			return -1;
		}

	}
	else if (pcmprop->nbits == 32)
	{
		if (NULL == (x = (float*)malloc(max_len * sizeof(float))))
		{
			fclose(fd_out);
			return -1;
		}
		for (i = 0; i < max_len; i++)
		{
			*((float*)x + i) = (float)(pcmprop->g[i]);
		}
		if (max_len > (int)fwrite((float *)x, sizeof(float), max_len, fd_out))
		{
			free(x);
			fclose(fd_out);
			return -1;
		}

	}
	else
	{
		fclose(fd_out);
		return -1;
	}

	free(x);
	fclose(fd_out);
	return 0;
}

int PcmInit(pcmprop_t *pcmprop, const char *filename, unsigned int fs, unsigned int channels, unsigned int nbits, unsigned int hd_len, unsigned int data_size, short mic, short ref)
{
	strcpy(pcmprop->name, filename);
	pcmprop->channels = channels;
	pcmprop->fs = fs;
	pcmprop->nbits = nbits;
	pcmprop->hd_len = hd_len;
	pcmprop->data_size = data_size;
	pcmprop->mic = mic;
	pcmprop->ref = ref;
	if (nbits == 16)
	{
		pcmprop->glen = (data_size - hd_len) / sizeof(short);
	}
	else if (nbits = 32)
	{
		pcmprop->glen = (data_size - hd_len) / sizeof(float);
	}
	else
	{
		return -1;
	}

	pcmprop->sample_per_ch = pcmprop->glen / pcmprop->channels;

	if (NULL == (pcmprop->g = (double *)malloc(pcmprop->glen * sizeof(double))))
	{
		return -1;
	}
	return 0;
}

void PcmDestroy(pcmprop_t *pcmprop)
{
	if (pcmprop->g)
	{
		free(pcmprop->g);
	}
}
