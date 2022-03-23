//
// Created by ross on 08/03/2022.
//

#include "SignalCleaner.h"
#include "algorithm"
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
    for(int channel_idx = 0; channel_idx < output.getNumChannels(); channel_idx++){
        for(int sample_idx = 0; sample_idx < output.getNumSamplesPerChannel(); sample_idx++){
            float noisey_sample = noisey_signal.samples[channel_idx][sample_idx];
            float noise_sample = noise_source.samples[channel_idx%nr_noise_channels][sample_idx%len_noise];
            double canceller = internal_fir.filter(noise_sample);
            double output_signal = noisey_sample - canceller;
            internal_fir.lms_update(output_signal);
            output.samples[channel_idx][sample_idx] = output_signal;
        }
    }
}

float SignalCleaner::ComputeRMS(AudioFile<float> signal){
    float total_square = 0.0f;
    float n = (float) signal.getNumSamplesPerChannel() * (float) signal.getNumChannels();
    for(int channel_idx = 0; channel_idx < signal.getNumChannels(); channel_idx++){
        for(int sample_idx = 0; sample_idx < signal.getNumSamplesPerChannel(); sample_idx++){
            total_square += std::pow(signal.samples[channel_idx][sample_idx], 2);
        }
    }
    return std::sqrt(total_square / n);
}

float SignalCleaner::ComputeSNR(AudioFile<float> signal, AudioFile<float> noise){
    float signal_rms = ComputeRMS(signal);
    float noise_rms = ComputeRMS(noise);
    std::cout << "  \n signal rms: " << signal_rms << "\n";
    std::cout << "  \n noise rms: " << noise_rms << "\n";
    return (signal_rms)/(signal_rms - noise_rms);

}
float SignalCleaner::SNRFromPearson(float p, bool as_db){
    float p_squared = pow(p, 2);
    float snr = p_squared / (1 - p_squared);
    if(as_db){
        snr = 10 * log(snr);
    }
    return snr;
}

float SignalCleaner::SNRPreFiltered(bool as_db) {
    float p = CalcPearsonCorrelationCoeff(clean, noisey_signal);
    return SNRFromPearson(p, as_db);
}
float SignalCleaner::SNRPostFiltered(bool as_db) {
   float p = CalcPearsonCorrelationCoeff(clean, output);
   return SNRFromPearson(p, as_db);
}

float SignalCleaner::CalcPearsonCorrelationCoeff(AudioFile<float> x, AudioFile<float> y){
    //calculates pearson correlation coeff between inputs x and y
    return CalcCovariance(x, y) / (CalcStdDev(x) * CalcStdDev(y));
}

float SignalCleaner::CalcCovariance(AudioFile<float> x, AudioFile<float> y){
    //presumes x and y are the same shape! IE they have the same nr channels
    int n_channels = std::min(x.getNumChannels(), y.getNumChannels());
    int n_samples_per_channel = std::min(x.getNumSamplesPerChannel(), y.getNumSamplesPerChannel());
    float mean_x = ComputeMean(x);
    float mean_y = ComputeMean(y);
    float covariance = 0.0f;
    for(int channel_idx = 0; channel_idx < n_channels; channel_idx++) {
        for (int sample_idx = 0; sample_idx < n_samples_per_channel; sample_idx++) {
            covariance += (x.samples[channel_idx][sample_idx] - mean_x) * (y.samples[channel_idx][sample_idx] - mean_y);
        }
    }
    return covariance;// / (float)(n_samples_per_channel * n_channels);
}

float SignalCleaner::CalcStdDev(AudioFile<float> signal){
    float N = (float)(signal.getNumChannels() * signal.getNumSamplesPerChannel());
    float total = 0.0f;
    float mean = ComputeMean(signal);
    for(int channel_idx = 0; channel_idx < signal.getNumChannels(); channel_idx++){
        for(int sample_idx = 0; sample_idx < signal.getNumSamplesPerChannel(); sample_idx++){
            float diff = signal.samples[channel_idx][sample_idx] - mean;
            total += pow(diff, 2);
        }
    }
    return sqrt(total);///N);
}

float SignalCleaner::ComputeMean(AudioFile<float> signal){
    //returns mean of a given signal
    float total = 0.0f;
    for(int channel_idx = 0; channel_idx < signal.getNumChannels(); channel_idx++) {
        for (int sample_idx = 0; sample_idx < signal.getNumSamplesPerChannel(); sample_idx++) {
            total += signal.samples[channel_idx][sample_idx];
        }
    }
    return total / (float)(signal.getNumChannels() * signal.getNumSamplesPerChannel());
}

void SignalCleaner::SetClean(std::string filename) {
    if(!FileExists(filename)){
        throw std::invalid_argument("Clean signal file does not exist.");
    }
    else{
        clean.load(filename);
    }
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