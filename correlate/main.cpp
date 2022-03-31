#include "correlate.h"
#include <cstdlib>

int main(int argc, char ** argv) {
    AudioFile<double> f1, f2;

    switch(argc) {
        case 2:
            f1.load(argv[1]);
            f2.load("/dev/stdin");
            break;
        case 3:
            f1.load(argv[1]);
            f2.load(argv[2]);
            break;
        default:
            std::cerr << "Usage: " << argv[0] << " <file1> [<file2>]" << std::endl;
            return EXIT_FAILURE;
    }

    std::cout << "MEAN1  : " << wcorr_mean(f1) << std::endl;    
    std::cout << "MEAN2  : " << wcorr_mean(f2) << std::endl;    
    std::cout << "STDEV1 : " << wcorr_stdev(f1) << std::endl;    
    std::cout << "STDEV2 : " << wcorr_stdev(f2) << std::endl;    
    std::cout << "COV    : " << wcorr_cov(f1,f2) << std::endl;    
    std::cout << "RHO    : " << wcorr_rho(f1,f2) << std::endl;    
    std::cout << "SNR    : " << wcorr_rho2snr(wcorr_rho(f1,f2), true) << " dB" << std::endl;    
}


