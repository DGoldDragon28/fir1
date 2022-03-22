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
    AudioFile<float> noisey_signal;
    AudioFile<float> noise_source;
    AudioFile<float> output;
    AudioFile<float> clean;

    SignalCleaner(std::string noisey_signal_path, std::string noise_source_path, unsigned nr_taps, float learning_rate);

    bool SaveFiltered(std::string);

    void Filter();

    float SNRPreFiltered(bool as_db);

    float SNRPostFiltered(bool as_db);

    void PrintSummaries();

    float CalcPearsonCorrelationCoeff(AudioFile<float> x, AudioFile<float> y);

    void SetClean(std::string filename);

    float CalcCovariance(AudioFile<float> x, AudioFile<float> y);

    float CalcStdDev(AudioFile<float> signal);

    float ComputeMean(AudioFile<float> signal);

private:
    Fir1 internal_fir;


    bool FileExists(std::string filename);

    float ComputeRMS(AudioFile<float> signal);

    float ComputeSNR(AudioFile<float> signal, AudioFile<float> noise);
    float SNRFromPearson(float p, bool as_db);


};


#endif //FIR1_SIGNALCLEANER_H
