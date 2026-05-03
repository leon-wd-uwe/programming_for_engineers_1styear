# Power Quality Waveform Analyser
UGMFGT-15-1 Programming for Engineers | Leon Wunderlich Derfield | 2025-26

## What this program does
Reads a 3-phase power quality CSV log file, and then computes RMS voltage, peak-to-peak
amplitude, DC offset, and clipping detection for each phase, checks
tolerance compliance, and writes a results report to results.txt.

## How to compile and run — for CLion
1. Open CLion and open the project folder (the folder containing CMakeLists.txt). Make sure it's not linked to OneDrive.
2. Wait for CMake to configure (the CMake panel at the bottom should show [Finished])
3. Go to Run → Edit Configurations
4. Set Program Arguments to: ../power_quality_log.csv
5. Click the green Run button

## How to compile and run — command line (gcc) (non-CLion use)
Navigate to the project folder in a terminal, then run:

    gcc -std=c99 -o PowerAnalyser main.c waveform.c io.c -lm

This tells gcc to compile all three .c files into a program called PowerAnalyser
using the C99 standard. The -lm flag links the maths library needed for sqrt().
Then run it with:

    ./PowerAnalyser power_quality_log.csv

On Windows:

     PowerAnalyser.exe power_quality_log.csv

## Expected output
The program prints a power quality report for each of the three phases to the
console and writes the same report to results.txt in the working directory.

## GitHub repository
    https://github.com/leon-wd-uwe/programming_for_engineers_1styear