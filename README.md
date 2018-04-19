# FIR1

This is a library which implements an efficient FIR filter
in both floating point arithmetic and integer arithmetic.

The floating point class has also an adaptive filter
method for the LMS algorithm.


## Installation

### Linux / Unix / MACOSX

The build system is `cmake`. Install the library with
the standard sequence:
```
cmake .
make
sudo make install
sudo ldconfig
```

### Windows

Under windows only the static library is generated which
should be used for your code development.

```
cmake -G "Visual Studio 15 2017 Win64" .
```
and then start Visual C++ and compile it. Usually
you want to compile both the release and debug
libraries.



## How to use it

### Generating the FIR filter coefficients

You can set the coefficients either with a C floating point array or
with a text file containing the coefficients. The text file with the
coefficients can easily be generated by Python or OCTAVE/MATLAB:

1. In Python use for example the firwin command
```
# Sampling rate
fs = 1000
# bandstop between 45 and 55 Hz:
f1 = 45
f2 = 55
b = signal.firwin(999,[f1/fs*2,f2/fs*2])
np.savetxt("h.dat",b)
```
this text file can then be imported into the filter at
runtim or you convert it to a C floating point array and
point the filter to it.

For fixed point you need to scale up the coefficients,
for example by 16 bits: np.savetxt("h.dat",b*65536)

2. In octave/MATLAB
```
octave:1> h=fir1(100,0.1);
octave:2> save 'h.dat' h -ascii
```
which creates the coefficients of a lowpass filter with 100 taps
and normalised cutoff 0.1 to Nyquist.

Again, for fixed point "h" needs to be scaled.

### C++ coding
 
1. for floating point FIR filters
```
Fir1 fir("h.dat");
```
or import the coefficients as a floating point array (double).
```
Fir1 fir(coefficients,number_of_coefficients)
```
2. for integer FIR filters
```
Fir1fixed fir("h_fixed.dat",12);
```
where the coefficients have been scaled up by 2^12 and the
filter will scale them down by this amount (with the help of
a bitshift operation).

and then you can filter your realtime data with:

1. for double
```
double b = fir.filter(a);
```
2. for integer
```
int b = fir.filter(a);
```

## LMS algorithm

You can adjust the FIR coefficients with the help of an error
signal:
```
w_k(n+1) = w_k(n) + learning_rate * buffer_k(n) * error(n)
```
with the function `lms_update(error)`. Set the learning_rate
with `setLearningRate(learning_rate)`.

This error signal is usually calculated by subtracting the output
of the filter from an input or desired signal, for example
for artefact removal or system identification.


## Demos
Demo programs are in the "demo" directory which show how to use the
filters for both floating point and fixed point.
1. `firdemo` sends an impulse into the filter and you should see the impulse
response at its output.
2. `fixeddemo` filters an example ECG with 50Hz noise. The coefficients
are 12 bit and you can generate them either with OCTAVE/MATLAB or Python.
The scripts are also provided.
3. `lsmdemo` filters out 50Hz noise from an ECG with the help of
adaptive filtering by using the 50Hz powerline frequency as the input
to the filter. This can be replaced by any reference artefact signal
or signal which is correlated with the artefact.


## Credits

This library has been adapted form Graeme Hattan's original C code.

Enjoy!

Bernd Porr & Graeme Hattan
