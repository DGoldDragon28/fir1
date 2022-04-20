#ifndef FIR1_SIGNALCLEANER_H
#define FIR1_SIGNALCLEANER_H
#include <Fir1.h>
#include "AudioFile/AudioFile.h"
#include <cmath>
#include <boost/circular_buffer.hpp>
#include <Iir.h>
#include <Fir1.h>
#include "dnf.h"

class SignalCleaner {
public:
    AudioFile<double> noisey_signal;
    AudioFile<double> noise_source;
    AudioFile<double> output;
    AudioFile<double> clean;

    SignalCleaner(std::string noisey_signal_path, std::string noise_source_path, unsigned nr_taps, double learning_rate);

    bool SaveFiltered(std::string);

    void FilterFir1();

    void FilterDnf();

    void PrintSummaries();
    
    void SetWeightsFile(std::string filename);
    
    void SetWeightDistanceFile(std::string filename);
    
    void SetRemoverFile(std::string filename);
    
    void ExportDNFWeights();
    
    void ExportDNFWeightDistance();
    
    void ExportDNFRemover();
    
    void CloseFiles();


private:
    Fir1* internal_fir = NULL;
    DNF* internal_dnf = NULL;
    std::fstream weights_file;
    std::fstream weight_distance_file;
    std::fstream remover_file;

    bool FileExists(std::string filename);
};


#endif //FIR1_SIGNALCLEANER_H
