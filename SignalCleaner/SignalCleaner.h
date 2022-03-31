//
// Created by ross on 08/03/2022.
//

#ifndef FIR1_SIGNALCLEANER_H
#define FIR1_SIGNALCLEANER_H
#include <Fir1.h>
#include "AudioFile/AudioFile.h"
#include <cmath>


class SignalCleaner {
public:
    AudioFile<double> noisey_signal;
    AudioFile<double> noise_source;
    AudioFile<double> output;
    AudioFile<double> clean;

    SignalCleaner(std::string noisey_signal_path, std::string noise_source_path, unsigned nr_taps, double learning_rate);

    bool SaveFiltered(std::string);

    void Filter();

    void PrintSummaries();


private:
    Fir1 internal_fir;

    bool FileExists(std::string filename);
};


#endif //FIR1_SIGNALCLEANER_H
