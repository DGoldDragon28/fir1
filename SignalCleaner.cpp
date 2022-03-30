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

double SignalCleaner::ComputeRMS(AudioFile<double> signal){
    double total_square = 0.0f;
    double n =  signal.getNumSamplesPerChannel() * signal.getNumChannels();
    for(int channel_idx = 0; channel_idx < signal.getNumChannels(); channel_idx++){
        for(int sample_idx = 0; sample_idx < signal.getNumSamplesPerChannel(); sample_idx++){
            total_square += std::pow(signal.samples[channel_idx][sample_idx], 2);
        }
    }
    return std::sqrt(total_square / n);
}

double SignalCleaner::ComputeSNR(AudioFile<double> signal, AudioFile<double> noise){
    double signal_rms = ComputeRMS(signal);
    double noise_rms = ComputeRMS(noise);
    std::cout << "  \n signal rms: " << signal_rms << "\n";
    std::cout << "  \n noise rms: " << noise_rms << "\n";
    return (signal_rms)/(signal_rms - noise_rms);

}
double SignalCleaner::SNRFromPearson(double p, bool as_db){
    double p_squared = pow(p, 2);
    double snr = p_squared / (1 - p_squared);
    if(as_db){
        snr = 10 * log(snr);
    }
    return snr;
}

double SignalCleaner::SNRPreFiltered(bool as_db) {
    double p = CalcPearsonCorrelationCoeff(clean, noisey_signal);
    return SNRFromPearson(p, as_db);
}
double SignalCleaner::SNRPostFiltered(bool as_db) {
   double p = CalcPearsonCorrelationCoeff(clean, output);
   return SNRFromPearson(p, as_db);
}

double SignalCleaner::CalcPearsonCorrelationCoeff(AudioFile<double> x, AudioFile<double> y){
    //calculates pearson correlation coeff between inputs x and y
    return CalcCovariance(x, y) / (CalcStdDev(x) * CalcStdDev(y));
}

double SignalCleaner::CalcCovariance(AudioFile<double> x, AudioFile<double> y){
    //presumes x and y are the same shape! IE they have the same nr channels
    int n_channels = std::min(x.getNumChannels(), y.getNumChannels());
    int n_samples_per_channel = std::min(x.getNumSamplesPerChannel(), y.getNumSamplesPerChannel());
    double mean_x = ComputeMean(x);
    double mean_y = ComputeMean(y);
    double covariance = 0.0f;
    for(int channel_idx = 0; channel_idx < n_channels; channel_idx++) {
        for (int sample_idx = 0; sample_idx < n_samples_per_channel; sample_idx++) {
            covariance += (x.samples[channel_idx][sample_idx] - mean_x) * (y.samples[channel_idx][sample_idx] - mean_y);
        }
    }
    return covariance / (n_samples_per_channel * n_channels);
}

double SignalCleaner::CalcStdDev(AudioFile<double> signal){
    double N = signal.getNumChannels() * signal.getNumSamplesPerChannel();
    double total = 0.0f;
    double mean = ComputeMean(signal);
    for(int channel_idx = 0; channel_idx < signal.getNumChannels(); channel_idx++){
        for(int sample_idx = 0; sample_idx < signal.getNumSamplesPerChannel(); sample_idx++){
            double diff = signal.samples[channel_idx][sample_idx] - mean;
            total += pow(diff, 2);
        }
    }
    return sqrt(total/N);
}

double SignalCleaner::ComputeMean(AudioFile<double> signal){
    //returns mean of a given signal
    double total = 0.0f;
    for(int channel_idx = 0; channel_idx < signal.getNumChannels(); channel_idx++) {
        for (int sample_idx = 0; sample_idx < signal.getNumSamplesPerChannel(); sample_idx++) {
            total += signal.samples[channel_idx][sample_idx];
        }
    }
    return total / (signal.getNumChannels() * signal.getNumSamplesPerChannel());
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