#include "SignalCleaner.h"
#include "algorithm"
#include <sys/stat.h>

#define NLAYERS 6

SignalCleaner::SignalCleaner(std::string noisey_signal_path, std::string noise_source_path, unsigned nr_taps, double learning_rate, Neuron::actMethod actMeth){
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
    internal_fir = new Fir1(nr_taps);
    internal_fir->setLearningRate(learning_rate);
    internal_dnf = new DNF(NLAYERS, nr_taps, 1.0 * noisey_signal.getSampleRate(), actMeth);
    //set equal weight learning rate, bias rate zero
    internal_dnf->getNet().setLearningRate(learning_rate, 0);
}

void SignalCleaner::SetGain(double _gain){
    gain = _gain;
}

void SignalCleaner::FilterFir1() {
    int len_noise = noise_source.getNumSamplesPerChannel();
    int nr_noise_channels = noise_source.getNumChannels();
    for(int channel_idx = 0; channel_idx < output.getNumChannels(); channel_idx++){
        for(int sample_idx = 0; sample_idx < output.getNumSamplesPerChannel(); sample_idx++){
            double noisey_sample = noisey_signal.samples[channel_idx][sample_idx];
            double noise_sample = noise_source.samples[channel_idx%nr_noise_channels][sample_idx%len_noise];
            double canceller = internal_fir->filter(noise_sample);
            double output_signal = noisey_sample - canceller;
            internal_fir->lms_update(output_signal);
            output.samples[channel_idx][sample_idx] = output_signal;
        }
    }
}

void SignalCleaner::FilterDnf() {
    int len_noise = noise_source.getNumSamplesPerChannel();
    int nr_noise_channels = noise_source.getNumChannels();
    for(int channel_idx = 0; channel_idx < output.getNumChannels(); channel_idx++){
        for(int sample_idx = 0; sample_idx < output.getNumSamplesPerChannel(); sample_idx++){
            double noisey_sample = noisey_signal.samples[channel_idx][sample_idx];
            double noise_sample = noise_source.samples[channel_idx%nr_noise_channels][sample_idx%len_noise];
            double cancelled = internal_dnf->filter(noisey_sample, noise_sample);
            output.samples[channel_idx][sample_idx] = cancelled;
            if(weights_file.is_open()) ExportDNFWeights();
            if(weight_distance_file.is_open()) ExportDNFWeightDistance();
            if(remover_file.is_open()) ExportDNFRemover();
        }
    }
}

void SignalCleaner::ExportDNFWeights() {
    internal_dnf->getNet().snapWeights(weights_file);
}

void SignalCleaner::ExportDNFWeightDistance(){
    if(weight_distance_file) {
        weight_distance_file << internal_dnf->getNet().getWeightDistance();
        for (int i = 0; i < NLAYERS; i++) {
            double dist = internal_dnf->getNet().getLayerWeightDistance(i);
            weight_distance_file << "," << dist;
        }
        weight_distance_file << "\n";
    }
}

void SignalCleaner::ExportDNFRemover(){
    if(remover_file){
        remover_file << internal_dnf->getRemover() << "\n";
    }
}

void SignalCleaner::CloseFiles(){
    if(weights_file) weights_file.close();
    if(weight_distance_file) weight_distance_file.close();
    if(remover_file) remover_file.close();
}

void SignalCleaner::SetWeightsFile(std::string filename){
    weights_file.open(filename, std::fstream::out);
}

void SignalCleaner::SetWeightDistanceFile(std::string filename){
    weight_distance_file.open(filename, std::fstream::out);
}

void SignalCleaner::SetRemoverFile(std::string filename){
    remover_file.open(filename, std::fstream::out);
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
