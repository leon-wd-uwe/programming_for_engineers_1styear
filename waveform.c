//
// Created by l2-wunderlichd on 22/04/2026.
//

#include <math.h>
#include <stdio.h>
#include "waveform.h"

//calculating effective pressure (RMS)
double calculate_rms(WaveformSample *data, int rows) {
    double sum_sq = 0.0; //total number of samples: keeps track of total value of the data over time

    for (int i = 0; i < rows; i++) {
        //voltage measured at this instant for Phase A and square it to get meaning out of the values (effective voltage)
        double val = data[i].phase_A_voltage; //assigns the value of phase_A_voltage at the i-th
                                              // position of data to a variable called val
        sum_sq += (val * val); //sum_sq now adds the squared value of "val" to itself

        // process repeats until we've gone through all values of i, and therefore of the 1000 samples is data
    }

    //average the squares, then Root the average (to reset the squaring we've done of val
    return sqrt(sum_sq/rows); //dividing the total sum of values by number of rows (which is 1000)
    //returns this to the function called in main.c
}

//calculates the total wave peak-to-peak oscillation
double calculate_peak_to_peak(WaveformSample *data, int rows) {
    // Start by assuming the first sample is both the max and the min
    double max_v = data[0].phase_A_voltage;
    double min_v = data[0].phase_A_voltage;

    for (int i = 1; i < rows; i++) {//cycles through all the rows seeing if they are bigger than its previous max/min
        // If we see a value higher than our current max, update it
        if (data[i].phase_A_voltage > max_v) {
            max_v = data[i].phase_A_voltage;//new max
        }
        // If we see a value lower than our current min, update it
        if (data[i].phase_A_voltage < min_v) {
            min_v = data[i].phase_A_voltage;//new min
        }
    }

    // returning the total distance between the highest and lowest points (range between the two extrema)
    return (max_v - min_v);
    //this calculation shows whether our wave is healthy (doesn't go to far above or below our predicted voltage)
}

//compute the dc offset (average voltage to see if the AC signal is balanced)
double calculate_dc_offset(WaveformSample *data, int rows){
    double total_voltage = 0.0; //counter variable

    for (int i = 0; i < rows; i++){
        //add up all voltage samples
        total_voltage += data[i].phase_A_voltage;
    }

    //average of pushes and pulls of the AC signal (ideal is 0.0 since then it's balanced)
    return (total_voltage/rows);
}

//count how many times the signals "clip" the tolerance of the sensor (goes above or below [ACTUALLY IDK IF BELOW ALSO] its physical limits
// for sensing change in the wave, which are 325V)
int count_clipping_samples(WaveformSample *data, int rows){
    int clipping_count = 0;
    const double SENSOR_LIMIT = 324.9; // The physical limit of the sensors abilities

    for (int i = 0; i < rows; i++){
        //count times the signal goes over the limits of the sensor (flat-top reading on the wave)
        if (data[i].phase_A_voltage >= SENSOR_LIMIT){
            clipping_count++;
        }
    }

    return clipping_count;
}
























