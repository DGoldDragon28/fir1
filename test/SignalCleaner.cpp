//
// Created by ross on 08/03/2022.
//
#include "../SignalCleaner.h"

int main (int,char**) {
//    SignalCleaner signalCleaner("/home/ross/PycharmProjects/noise-guy/output/noisy/disco.00060-sine50Hz30s.wav",//"/home/ross/PycharmProjects/noise-guy/output/noisy/blues.00081-07002027.wav",
//                                "/home/ross/PycharmProjects/noise-guy/noise-sources/50hz-sine/sine50Hz30s.wav",
//                                500, 0.0001);
//    signalCleaner.PrintSummaries();
//    signalCleaner.SetClean("/home/ross/PycharmProjects/noise-guy/output/upsampled/disco.00060.wav");
//    std::cout << "clean mean: " << signalCleaner.ComputeMean(signalCleaner.clean) << "\n";
//    std::cout << "std deviation clean signal: " << signalCleaner.CalcStdDev(signalCleaner.clean) << "\n";
//    std::cout << "std deviation noisey signal: " << signalCleaner.CalcStdDev(signalCleaner.noisey_signal) << "\n";
//    std::cout << "pre-filtered covariance: " << signalCleaner.CalcCovariance(signalCleaner.clean, signalCleaner.noisey_signal) << "\n";
//    std::cout << "pre-filtered noisey mean: " << signalCleaner.ComputeMean(signalCleaner.noisey_signal) << "\n";
//
//    std::cout << "pre-filtered pearsons: " << signalCleaner.CalcPearsonCorrelationCoeff(signalCleaner.clean, signalCleaner.noisey_signal) << "\n";
//    std::cout << "pre-filtered snr: " << signalCleaner.SNRPreFiltered(true) << "\n";
//    signalCleaner.Filter();
//    std::cout << "post-filtered covariance: " << signalCleaner.CalcCovariance(signalCleaner.clean, signalCleaner.output) << "\n";
//    std::cout << "std deviation output signal: " << signalCleaner.CalcStdDev(signalCleaner.output) << "\n";
//    std::cout << "post-filtered noisey mean: " << signalCleaner.ComputeMean(signalCleaner.output) << "\n";
//    std::cout << "post-filtered pearsons: " << signalCleaner.CalcPearsonCorrelationCoeff(signalCleaner.clean, signalCleaner.output) << "\n";
//    std::cout << "post-filtered snr: " << signalCleaner.SNRPostFiltered(true) << "\n";
//    signalCleaner.SaveFiltered("./filtered.wav");
}