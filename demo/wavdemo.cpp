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
#include "../AudioFile/AudioFile.h"

#define NTAPS 100
#define LEARNING_RATE 0.0005

int main (int,char**)
{
	// inits the filter
	Fir1 fir(NTAPS);
	fir.setLearningRate(LEARNING_RATE);
    AudioFile<float> noiseyFile;
    noiseyFile.load("/home/ross/PycharmProjects/noise-guy/output/noisy/blues.00081-07042249.wav");
    int numSamples = noiseyFile.getNumSamplesPerChannel();
    noiseyFile.printSummary();
    AudioFile<float> noiseSource;
    noiseSource.load("/home/ross/PycharmProjects/noise-guy/noise-sources/fridge-hum/07042249.wav");
    noiseSource.printSummary();
    AudioFile<float> output;
    output.setAudioBufferSize (noiseyFile.getNumChannels(), numSamples);

    int channel = 0;
	for(int i=0; i < numSamples;i++)
    {
        float noisey_sample = noiseyFile.samples[channel][i];
        float noise_sample = noiseSource.samples[channel][i];
        float canceller = fir.filter(noise_sample);
        //printf("sample: %f \n noise: %f \n canceller: %f\n", noisey_sample, noise_sample, canceller );

        float output_signal = noisey_sample - canceller;
		fir.lms_update(output_signal);
        output.samples[channel][i] = output_signal;
	}
    output.save("filtered.wav");
	fprintf(stderr,"Written the filtered file\n");
}
