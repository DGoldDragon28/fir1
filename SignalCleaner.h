//
// Created by ross on 08/03/2022.
//

#ifndef FIR1_SIGNALCLEANER_H
#define FIR1_SIGNALCLEANER_H
#include <Fir1.h>
#include "AudioFile/AudioFile.h"

#define N 100

class SignalCleaner {
public:
    SignalCleaner(std::string noisey_signal_path, std::string noise_source_path, unsigned nr_taps, float learning_rate);
    bool SaveFiltered(std::string);
    void Filter();
private:
    Fir1 internal_fir;
    AudioFile<float> noisey_signal;
    AudioFile<float> noise_source;
    AudioFile<float> output;
    bool FileExists(std::string filename);
};


#endif //FIR1_SIGNALCLEANER_H
