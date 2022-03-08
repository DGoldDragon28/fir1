//
// Created by ross on 08/03/2022.
//

#include "SignalCleaner.h"
#include <sys/stat.h>


SignalCleaner::SignalCleaner(std::string noisey_signal_path, std::string noise_source_path, unsigned nr_taps, float learning_rate) : internal_fir(nr_taps){
    if(!FileExists(noisey_signal_path)){
        throw std::invalid_argument("Noisey signal file does not exist.");
    }
    else{
        noisey_signal.load(noisey_signal_path);
        output.setSampleRate(noisey_signal.getSampleRate());
        output.setAudioBufferSize(noisey_signal.getNumChannels(), noisey_signal.getNumSamplesPerChannel());
    }
    if(!FileExists(noise_source_path)){
        throw std::invalid_argument("Noise source file does not exist.");
    }
    else{
        noise_source.load(noise_source_path);
    }
    internal_fir.setLearningRate((double)learning_rate);
}
void SignalCleaner::Filter() {
    int len_noise = noise_source.getNumSamplesPerChannel();
    int nr_noise_channels = noise_source.getNumChannels();
    int I = noisey_signal.getNumSamplesPerChannel();
    printf("%d \n", I);
    for(int channel_idx = 0; channel_idx < output.getNumChannels(); channel_idx++){
        for(int sample_idx = 0; sample_idx < output.getNumSamplesPerChannel(); sample_idx++){
            float noisey_sample = noisey_signal.samples[channel_idx][sample_idx];
            float noise_sample = noise_source.samples[channel_idx%nr_noise_channels][sample_idx%len_noise];
            double canceller = internal_fir.filter(noise_sample);
            double output_signal = noisey_sample - canceller;
            printf("s: %f, n: %f, o: %f \n",noisey_sample, noise_sample, output_signal);
            internal_fir.lms_update(output_signal);
            output.samples[channel_idx][sample_idx] = output_signal;
        }
    }
}

bool SignalCleaner::SaveFiltered(std::string filename){
    return output.save(filename, AudioFileFormat::Wave);
}

bool SignalCleaner::FileExists(std::string filename) {
    struct stat buffer;
    return (stat (filename.c_str(), &buffer) == 0);
}