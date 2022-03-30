#include "correlate.h"
#include <cstdlib>

#define PRINT_USAGE(n) (std::cerr << "Usage: " << n << "[-l <lrate>] [-n <ntaps>] [-o <file>] <noise-source> [<input>]" << std::endl)

int main(int argc, char ** argv) {
    std::string paths[2];
    int i, pathcnt = 0;
    int ntaps = 500;
    double lrate = 0.0001;
    paths[1] = "/dev/stdin";
    std::string outpath;
    bool has_outpath = false;

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
	    case 'o':
                outpath = (argv[i][2] ? &argv[i][2] : argv[++i]);
		has_outpath = true;
		break;
            else:
                PRINT_USAGE(argv[0]);
                return EXIT_FAILURE;
        }
    }
    if(pathcnt < 1) {
        PRINT_USAGE(argv[0]);
        return EXIT_FAILURE;
    }

    SignalCleaner cleaner(paths[1], paths[0], ntaps, lrate);
    cleaner.Filter();
    signalCleaner.SaveFiltered(has_outpath ? outpath : "/dev/stdout");
}


