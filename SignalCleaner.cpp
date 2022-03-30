#include "SignalCleaner.h"
#include "algorithm"
#include <sys/stat.h>


SignalCleaner::SignalCleaner(std::string noisey_signal_path, std::string noise_source_path, unsigned nr_taps, double learning_rate) : internal_fir(nr_taps){
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
    internal_fir.setLearningRate(learning_rate);
}

void SignalCleaner::Filter() {
    int len_noise = noise_source.getNumSamplesPerChannel();
    int nr_noise_channels = noise_source.getNumChannels();
    for(int channel_idx = 0; channel_idx < output.getNumChannels(); channel_idx++){
        for(int sample_idx = 0; sample_idx < output.getNumSamplesPerChannel(); sample_idx++){
            double noisey_sample = noisey_signal.samples[channel_idx][sample_idx];
            double noise_sample = noise_source.samples[channel_idx%nr_noise_channels][sample_idx%len_noise];
            double canceller = internal_fir.filter(noise_sample);
            double output_signal = noisey_sample - canceller;
            internal_fir.lms_update(output_signal);
            output.samples[channel_idx][sample_idx] = output_signal;
        }
    }
}

void SignalCleaner::SetClean(std::string filename) {
    if(!FileExists(filename)){
        throw std::invalid_argument("Clean signal file does not exist.");
    }
    else{
        clean.load(filename);
    }
//    for(int channel_idx = 0; channel_idx < clean.getNumChannels(); channel_idx++) {
//        for (int sample_idx = 0; sample_idx < clean.getNumSamplesPerChannel(); sample_idx++) {
//            clean.samples[channel_idx][sample_idx] = 0.5 * clean.samples[channel_idx][sample_idx];
//        }
//    }
    clean.printSummary();
}


bool SignalCleaner::SaveFiltered(std::string filename){
    return output.save(filename, AudioFileFormat::Wave);
}

bool SignalCleaner::FileExists(std::string filename) {
    struct stat buffer;
    return (stat (filename.c_str(), &buffer) == 0);
}

void SignalCleaner::PrintSummaries() {
    std::cout << "noisey signal summary: \n";
    noisey_signal.printSummary();

    std::cout << "noise source summary: \n";
    noise_source.printSummary();

    std::cout << "output signal summary: \n";
    output.printSummary();
}
