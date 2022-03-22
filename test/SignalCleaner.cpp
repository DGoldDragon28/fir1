//
// Created by ross on 08/03/2022.
//
#include "../SignalCleaner.h"

int main (int,char**) {
    SignalCleaner signalCleaner("/home/ross/PycharmProjects/noise-guy/output/noisy/blues.00081-07002027.wav",
                                "/home/ross/PycharmProjects/noise-guy/noise-sources/strip-lighting-hum/07002027.wav",
                                500, 0.0005);

    signalCleaner.SetClean("/home/ross/PycharmProjects/noise-guy/gtzan/blues/blues.00081.wav");
    std::cout << "clean mean: " << signalCleaner.ComputeMean(signalCleaner.clean) << "\n";
    std::cout << "std deviation clean signal: " << signalCleaner.CalcStdDev(signalCleaner.clean) << "\n";
    std::cout << "std deviation noisey signal: " << signalCleaner.CalcStdDev(signalCleaner.noisey_signal) << "\n";
    std::cout << "pre-filtered covariance: " << signalCleaner.CalcCovariance(signalCleaner.clean, signalCleaner.noisey_signal) << "\n";
    std::cout << "pre-filtered noisey mean: " << signalCleaner.ComputeMean(signalCleaner.noisey_signal) << "\n";

    std::cout << "pre-filtered pearsons: " << signalCleaner.CalcPearsonCorrelationCoeff(signalCleaner.clean, signalCleaner.noisey_signal) << "\n";
    std::cout << "pre-filtered snr: " << signalCleaner.SNRPreFiltered(false) << "\n";
    signalCleaner.Filter();
    std::cout << "post-filtered noisey mean: " << signalCleaner.ComputeMean(signalCleaner.output) << "\n";

    std::cout << "post-filtered snr: " << signalCleaner.SNRPostFiltered(false) << "\n";
    //signalCleaner.PrintSummaries();
    signalCleaner.SaveFiltered("./filtered.wav");

}