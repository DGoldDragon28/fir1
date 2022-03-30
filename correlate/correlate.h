//
// Created by ross on 08/03/2022.
//

#ifndef CORRELATE_H
#define CORRELATE_H
#include "AudioFile/AudioFile.h"
#include <cmath>



double wcorr_rho(AudioFile<double> x, AudioFile<double> y);

double wcorr_cov(AudioFile<double> x, AudioFile<double> y);

double wcorr_stdev(AudioFile<double> signal);

double wcorr_mean(AudioFile<double> signal);

double wcorr_rho2snr(double p, bool as_db);


#endif

