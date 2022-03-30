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

    double SNRPreFiltered(bool as_db);

    double SNRPostFiltered(bool as_db);

    void PrintSummaries();

    double CalcPearsonCorrelationCoeff(AudioFile<double> x, AudioFile<double> y);

    void SetClean(std::string filename);

    double CalcCovariance(AudioFile<double> x, AudioFile<double> y);

    double CalcStdDev(AudioFile<double> signal);

    double ComputeMean(AudioFile<double> signal);

private:
    Fir1 internal_fir;


    bool FileExists(std::string filename);

    double ComputeRMS(AudioFile<double> signal);

    double ComputeSNR(AudioFile<double> signal, AudioFile<double> noise);
    double SNRFromPearson(double p, bool as_db);


};


#endif //FIR1_SIGNALCLEANER_H
