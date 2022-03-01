// Usage Examples
//
// LMS demo
// removes 50Hz from an ECG with a powerline reference
// here generated with a simple sine. Should be ideally
// also measured with another ADC channel and fed into
// the filter.

// This is the only include you need
#include <Fir1.h>
#include "wavrw.h"

#define _USE_MATH_DEFINES
#include <stdio.h>
#include <math.h>

#define NTAPS 100
#define LEARNING_RATE 0.0005

int main (int,char**)
{
	// inits the filter
	Fir1 fir(NTAPS);
	fir.setLearningRate(LEARNING_RATE);

    FILE *finput = fopen("/home/ross/PycharmProjects/noise-guy/output/noisey/blues.00081-07042249.wav","rb");
    FILE *fnoise = fopen("/home/ross/PycharmProjects/noise-guy/noise-sources/fridge-hum/07042249.wav", "rb");
    FILE *foutput = fopen("filtered.wav", "wb");

    if(!finput || !fnoise || !foutput){
        printf("error");
    }


    wav_file wavFile;
    wavFile.file = finput;
    wav_file wavNoise;
    wavNoise.file = fnoise;
    wav16_read_head(&wavNoise);
    wav16_read_head(&wavFile);
    int srate = wavFile.hdr.w_srate;
    int nrchannels = wavFile.hdr.w_nchannels;
    wav_file outputWav;
    outputWav.file = foutput;
    wav16_init_head(&outputWav,srate, nrchannels);
    wav16_write_head(&outputWav);
    int channel = 0;
	for(int i=0;;i++)
	{
        int16_t buffer[2];
        if(wav16_read_sample(&wavFile, buffer)) break;
        float first_channel = (float)buffer[channel];

        if(wav16_read_sample(&wavNoise, buffer)) break;
        float noise_sample = (float)buffer[channel];

		float canceller = fir.filter(noise_sample);
		float output_signal = first_channel - canceller;

		fir.lms_update(output_signal);
        buffer[channel] = (int16_t)output_signal;
        if(wav16_write_sample(&outputWav, buffer)) break;
	}
	fclose(finput);
    fclose(fnoise);
	fclose(foutput);
	fprintf(stderr,"Written the filtered file\n");
}
