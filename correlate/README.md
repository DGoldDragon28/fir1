# Correlate

The `wave-correlate` tool provides facility to compute standard deviation, covariance, pearsons correlation coeffiecient and SNR for input wav files.

## Build

To try out `wav-correlate`, you can build the program with `cmake` from the root level `fir/` directory or from this directory as shown:

```asm
cmake .
make
```

## Usage

Example usage is shown:

```bash
Usage: wav-correlate <file1> [<file2>]

```

If no secondary file is specified, the program will read from `stdin`.

## Output

The program outputs metrics to `stdout`. Example program usage and output is shown:

```bash
$ correlate/wav-correlate wav-files/sinewave441Hz.wav wav-files/sinewave882Hz.wav

MEAN1  : -3.05176e-07
MEAN2  : 0
STDEV1 : 0.707091
STDEV2 : 0.707095
COV    : 2.62016e-22
RHO    : 5.24051e-22
SNR    : -980.009 dB

```





