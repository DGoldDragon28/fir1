# Signal Cleaner

The `signal-cleaner` tool may be used to run the lms fir algorithm on a noisey input file and produce a clean output file. The noise source is also required. All inputs are files with `.wav` extenstion.

## Build

To try out `signal-cleaner`, you can build the program with `cmake` from the root level `fir/` directory or from this directory as shown:

```asm
cmake .
make
```

## Usage 

A usage example is provided below: 

```bash
Usage: ./signal-cleaner[-l <lrate>] [-n <ntaps>] [-o <file>] <noise-source> [<input>]
```

As well as providing input and noise sources, the `ntaps` and `lrate` for the filter may be configured. 

If no output file is specified, the program will output to `stdout`.

If no input file is specified, the program will input from `stdin`.

## Output

The output file is the filtered result.

