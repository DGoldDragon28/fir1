#include "SignalCleaner.h"
#include <cstdlib>

//#include <fenv.h>
//feenableexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW);

#define PRINT_USAGE(n) (std::cerr << "Usage: " << n << "[-l <lrate>] [-n <ntaps>] [-o <file>] [-g <gain>] [-a <act>] [-d | -f] <noise-source> [<input>]" << std::endl)

int main(int argc, char ** argv) {
    std::string paths[2];
    int i, pathcnt = 0;
    int ntaps = 100;
    double lrate = 0.0001;
    paths[1] = "/dev/stdin";
    std::string outpath;
    bool has_outpath = false;
    bool use_dnf = false;
    double gain = 1.0f;
    Neuron::actMethod act = Neuron::Act_NONE;
    int a;
    
    for(i = 1; i < argc; i++) {
        if(argv[i][0] != '-') {
            if(pathcnt > 1) { PRINT_USAGE(argv[0]); return EXIT_FAILURE; }
            paths[pathcnt++] = argv[i];
        } else switch(argv[i][1]) {
            case '\0':
                if(pathcnt > 1) { PRINT_USAGE(argv[0]); return EXIT_FAILURE; }
                paths[pathcnt++] = "/dev/stdin";
                break;
            case 'l':
                lrate = std::stod(argv[i][2] ? &argv[i][2] : argv[++i]);
                break;
            case 'n':
                ntaps = std::stoi(argv[i][2] ? &argv[i][2] : argv[++i]);
                break;
            case 'g':
                gain = std::stod(argv[i][2] ? &argv[i][2] : argv[++i]);
                break;
            case 'a':
                a = std::stoi(argv[i][2] ? &argv[i][2] : argv[++i]);
                if(a < 4 && a >= 0) {
                    act = static_cast<Neuron::actMethod>(a);
                }
                else{ PRINT_USAGE(argv[0]); return EXIT_FAILURE; }
                break;
            case 'o':
                outpath = (argv[i][2] ? &argv[i][2] : argv[++i]);
                has_outpath = true;
                break;
            case 'd':
                use_dnf = true;
                break;
            case 'f':
                use_dnf = false;
                break;
                
            default:
                PRINT_USAGE(argv[0]);
                return EXIT_FAILURE;
        }
    }
    if(pathcnt < 1) {
        PRINT_USAGE(argv[0]);
        return EXIT_FAILURE;
    }
    
    SignalCleaner cleaner(paths[1], paths[0], ntaps, lrate, act);
    cleaner.SetGain(gain);
    if(use_dnf){
        cleaner.SetWeightDistanceFile("output_weight_distance.csv");
        cleaner.SetRemoverFile("output_remover.csv");
        cleaner.FilterDnf();
        cleaner.CloseFiles();
    }
    else{
        cleaner.FilterFir1();
    }
    cleaner.SaveFiltered(has_outpath ? outpath : "/dev/stdout");
}


