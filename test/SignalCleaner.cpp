//
// Created by ross on 08/03/2022.
//
#include "../SignalCleaner.h"

int main (int,char**) {
    SignalCleaner signalCleaner("/home/ross/PycharmProjects/noise-guy/output/noisy/blues.00081-07042249.wav",
                                "/home/ross/PycharmProjects/noise-guy/noise-sources/fridge-hum/07042249.wav",
                                100, 0.005);
    signalCleaner.Filter();
    signalCleaner.SaveFiltered("./filtered.wav");

}