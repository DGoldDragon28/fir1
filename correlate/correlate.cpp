#include "correlate.h"
#include <sys/stat.h>

/// Returns signal to noise ratio (SNR) from an input pearson's correlation coefficient
/// \param p pearson's correlation coefficient
/// \param as_db if true, SNR returned in decibels; otherwise returned as numeric ratio.
/// \return computed SNR
double wcorr_rho2snr(double p, bool as_db){
    double p_squared = p*p;
    double snr = p_squared / (1 - p_squared);
    if(as_db){
        snr = 10 * log(snr);
    }
    return snr;
}
/// calculates pearson's correlation coefficient between inputs x and y
/// \param x input audio signal x
/// \param y input audio signal y
/// \return computed pearson's correlation coefficient
double wcorr_rho(AudioFile<double> x, AudioFile<double> y){
    return wcorr_cov(x, y) / (wcorr_stdev(x) * wcorr_stdev(y));
}
/// computes the covariance between inputs x and y
/// \param x input audio signal x
/// \param y input audio signal y
/// \return computed covariance
double wcorr_cov(AudioFile<double> x, AudioFile<double> y){
    int n_channels = std::min(x.getNumChannels(), y.getNumChannels());
    int n_samples_per_channel = std::min(x.getNumSamplesPerChannel(), y.getNumSamplesPerChannel());
    double mean_x = wcorr_mean(x);
    double mean_y = wcorr_mean(y);
    double covariance = 0.0f;
    for(int channel_idx = 0; channel_idx < n_channels; channel_idx++) {
        for (int sample_idx = 0; sample_idx < n_samples_per_channel; sample_idx++) {
            covariance += (x.samples[channel_idx][sample_idx] - mean_x) * (y.samples[channel_idx][sample_idx] - mean_y);
        }
    }
    return covariance / (n_samples_per_channel * n_channels);
}
/// computes the standard deviation of an unput signal
/// \param signal input audio signal to compute standard deviation
/// \return standard deviation of input signal
double wcorr_stdev(AudioFile<double> signal){
    double N = signal.getNumChannels() * signal.getNumSamplesPerChannel();
    double total = 0.0f;
    double mean = wcorr_mean(signal);
    for(int channel_idx = 0; channel_idx < signal.getNumChannels(); channel_idx++){
        for(int sample_idx = 0; sample_idx < signal.getNumSamplesPerChannel(); sample_idx++){
            double diff = signal.samples[channel_idx][sample_idx] - mean;
            total += diff*diff;
        }
    }
    return sqrt(total/N);
}
/// computes the mean value of a given signal
/// \param signal input audio signal
/// \return mean value in the signal
double wcorr_mean(AudioFile<double> signal){
    //returns mean of a given signal
    double total = 0.0f;
    for(int channel_idx = 0; channel_idx < signal.getNumChannels(); channel_idx++) {
        for (int sample_idx = 0; sample_idx < signal.getNumSamplesPerChannel(); sample_idx++) {
            total += signal.samples[channel_idx][sample_idx];
        }
    }
    return total / (signal.getNumChannels() * signal.getNumSamplesPerChannel());
}


