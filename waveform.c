//
// Created by l2-wunderlichd on 22/04/2026.
//

#include <math.h>
#include <stdio.h>
#include "waveform.h"

//calculating effective voltage (with the RMS)
double calculate_rms(WaveformSample *data, int rows, int phase_num) {
    double sum_sq = 0.0; //total number of samples: keeps track of total value of the data over time

    for (int i = 0; i < rows; i++) {
        //voltage measured at this instant for Phase A and square it to get meaning out of the values (effective voltage)
        double val; //assigns the value of phase_A_voltage at the i-th
                                              // position of data to a variable called val
        if (phase_num == 0) { val = data[i].phase_A_voltage;}
            //assigns the value of phase_A_voltage at the i-th position of data, to a variable called val
        else if (phase_num == 1){ val = data[i].phase_B_voltage; }
            //does the same for B and C
        else { val = data[i].phase_C_voltage;}

        sum_sq += (val * val); //sum_sq now adds the squared value of "val" to itself (for rms calculation)

        // process repeats until we've gone through all values of i, and therefore of the 1000 samples is data
    }

    //average the squares, then Root the average (to reset the squaring we've done of val
    return sqrt(sum_sq / (double)rows); //dividing the total sum of values by number of rows (which is 1000)
    //returns this to the function called in main.c
}



//calculates the total wave PEAK-TO-PEAK OSCILLATION
double calculate_peak_to_peak(WaveformSample *data, int rows, int phase) {
    double max_v; double min_v;

    // Initialising max/min with the first sample of the CHOSEN phase
    // 0 = Phase A, 1 = Phase B, and 2 = Phase C.
    if (phase == 0)      { max_v = data[0].phase_A_voltage; min_v = data[0].phase_A_voltage; }
    else if (phase == 1) { max_v = data[0].phase_B_voltage; min_v = data[0].phase_B_voltage; }
    else                 { max_v = data[0].phase_C_voltage; min_v = data[0].phase_C_voltage; }

    for (int i = 1; i < rows; i++) {//cycles through all the rows seeing if they are bigger than its previous max/min
        double current_val;

        //check to see what phase we're in and set that phase to current_val
        if (phase == 0){current_val = data[i].phase_A_voltage;}
        else if (phase == 1){current_val = data[i].phase_B_voltage;}
        else{current_val = data[i].phase_C_voltage;}

        // If we see a value higher or lower than our current max or min, update it
        if (current_val > max_v){max_v = current_val;}
        if (current_val < min_v){min_v = current_val;}
    }

    // returning the total distance between the highest and lowest points (range between the two extrema)
    return (max_v - min_v);
    //this calculation shows whether our wave is healthy (doesn't go to far above or below our predicted voltage)
}



//compute the DC OFFSET (average voltage to see if the AC signal is balanced)
double calculate_dc_offset(WaveformSample *data, int rows, int phase){
    double total_voltage = 0.0; //counter variable

    for (int i = 0; i < rows; i++){
        //add up all voltage samples to the correct phase (0 = Phase A, 1 = Phase B, and 2 = Phase C)
        if (phase == 0){total_voltage += data[i].phase_A_voltage;}
        else if (phase == 1){total_voltage += data[i].phase_B_voltage;}
        else{total_voltage += data[i].phase_C_voltage;}
    }

    //average of pushes and pulls of the AC signal (ideal is 0.0 since then it's balanced)
    return (total_voltage/rows);
}


//count how many times the signals "clip" the tolerance of the sensor (goes above or below
// its physical limits for sensing change in the wave, which are 325V)
int count_clipping_samples(WaveformSample *data, int rows, int phase){
    int clipping_count = 0;
    const double SENSOR_LIMIT = 324.9; //the physical limit of the sensors abilities

    for (int i = 0; i < rows; i++){
        //count times the signal goes over the limits of the sensor (flat-top reading on the wave)
        WaveformSample *current = &data[i];//assigns the address of current to the data array at the i-th pont
        double val;

        // Select the value once
        if (phase == 0)      val = current->phase_A_voltage;
        //-> gets the data from the address of phase_A_voltage and assigns that value to current
        else if (phase == 1) val = current->phase_B_voltage;
        else                 val = current->phase_C_voltage;

        // Check the limit once
        if (val >= SENSOR_LIMIT) {
            clipping_count++;
        }
    }

    return clipping_count;
}


//check if RMS is within 10% of 230V
//returns 1 if compliant, 0 if out of tolerance
int check_tolerance_compliance(double rms_value) {
    const double NOMINAL_V = 230.0;
    const double TOLERANCE_PERCENT = 0.10;

    //Calculate the upper and lower boundaries of the RMS voltage to +-10%
    double lower_limit = NOMINAL_V * (1.0 - TOLERANCE_PERCENT);//lower 90% boundary
    double upper_limit = NOMINAL_V * (1.0 + TOLERANCE_PERCENT);//upper 110% boundary

    //exclusive AND logic
    if (rms_value >= lower_limit && rms_value <= upper_limit) {
        return 1; // Compliant
    } else {
        return 0; // Non-compliant
    }
}

