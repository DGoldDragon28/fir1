#include "correlate.h"
#include <cstdlib>
#include <cstdio>

int main(int argc, char ** argv) {
    AudioFile<double> f1, f2;
    std::string csv_filename;
    bool toCSV = false;
    char  *csv_label;
    switch(argc) {
        case 2:
            f1.load(argv[1]);
            f2.load("/dev/stdin");
            break;
        case 5:
            csv_label = argv[4];
        case 4:
            toCSV = true;
            csv_filename=argv[3];
        case 3:
            f1.load(argv[1]);
            f2.load(argv[2]);
            break;
        
        default:
            std::cerr << "Usage: " << argv[0] << " <file1> [<file2>]" << std::endl;
            return EXIT_FAILURE;
    }
    
    double mean_f1 = wcorr_mean(f1);
    double mean_f2 = wcorr_mean(f2);
    double stdev_f1 = wcorr_stdev(f1);
    double stdev_f2 = wcorr_stdev(f2);
    double cov = wcorr_cov(f1, f2);
    double rho = wcorr_rho(f1, f2);
    double snr = wcorr_rho2snr(wcorr_rho(f1, f2), true);
    
    if(toCSV){
        FILE* csv = fopen(csv_filename.c_str(), "a");
        if(csv){
            fprintf(csv,"%s,%f,%f,%f,%f,%f,%f,%f,\n", csv_label, mean_f1, mean_f2, stdev_f1, stdev_f2, cov, rho, snr);
        }
        else{
            std::cout << "can't open csv file";
        }
    }
    else{
        std::cout << "MEAN1  : " << mean_f1 <<  std::endl;
        std::cout << "MEAN2  : " << mean_f2 << std::endl;
        std::cout << "STDEV1 : " << stdev_f1 << std::endl;
        std::cout << "STDEV2 : " << stdev_f2 << std::endl;
        std::cout << "COV    : " << cov << std::endl;
        std::cout << "RHO    : " << rho << std::endl;
        std::cout << "SNR    : " << snr << " dB" << std::endl;
    }
    
}


