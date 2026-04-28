//
// io.c: Created by l2-wunderlichd on 22/04/2026.
//

#include <stdlib.h>
#include <stdio.h>
#include "io.h"
#include "waveform.h"

int load_csv(const char *filename, WaveformSample *data, int rows) {
    // opens the pipeline between the program and the hard drive
    FILE *file = fopen(filename, "r"); //File pointer

    // safety check: If the file doesn't exist, handle the error
    if (file == NULL) {//if the file is NULL then it doesn't exist
        printf("Error: Could not open file %s\n", filename);
        return 1;
    }

    // skip the header row (the row of words/labels)
    char buffer[1024]; //temporary storage space for the labels
    fgets(buffer, sizeof(buffer), file); /*takes out all the words form the struct into a temporary
 *                                         buffer so that the loop can start at the numbers*/

    // sorting loop to fill our 1,000 data 'molds' determined by WaveformSample
    for (int i = 0; i < rows; i++) {
        // We use pointer arithmetic to move to the next 'mold' in memory with the variable i
        // &data[i] is the address of the i-th storage compartment within the struct
        int read_count = fscanf(file,
                                "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
                //(above) 8 repeated long float (%lf) or double format for the 8 variables specifying the format to store in
                /*all the different fields in the struct are determined by the struct definition in
                 * waveform.h and we cycle through the blocks of available memory with the variable i
                 * filling them with the values we read*/
                                &data[i].timestamp, //pointer of where this specific information should be stored
                                &data[i].phase_A_voltage,
                                &data[i].phase_B_voltage,
                                &data[i].phase_C_voltage,
                                &data[i].line_current,
                                &data[i].frequency,
                                &data[i].power_factor,
                                &data[i].thd_percent);

        // Check if the line was read correctly (it should find 8 numbers since we have 8 labels (struct definitions))
        if (read_count != 8) {
            printf("Error reading data at row %d\n", i);
            fclose(file);
            return 1;
        }
    }

    // closing the pipeline
    fclose(file);
    return 0; //as long as it returns 0 at the end we're all good
}



int count_csv_rows(const char *filename) {
    FILE *file = fopen(filename, "r");//"r" means reads the inputted file

    if (file == NULL) {
        printf("Error: Could not open file %s\n", filename);
        return -1; // signals failure to the caller
    }

    // Skip the header row ( we don't want to count the label line as data)
    char buffer[1024];
    fgets(buffer, sizeof(buffer), file);

    int row_count = 0;
    // Read lines until we hit the end of the file, counting each one
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        row_count++;
    }

    fclose(file);
    return row_count;
}



    //exporting the results to a text file
int save_results(const char *filename, const char *phase_label, int phase_num, double rms, double p2p, double dc, int clips, const char* status)  {
    const char *mode;
    if (phase_num == 0){// only enters this if we're back at phase A
        mode = "w"; //write to clear previous power quality reports
    }
    else {
        mode = "a"; //append the next phase to the previous in the .txt file
    }

    //opens the output pipeline
    FILE *out = fopen(filename, mode);//mode variable allows the txt. file to only include all 3 phases

    if (out == NULL) {
        printf("Error: Could not create results file!\n");
        return 1;
    }

    //write the Report in a .txt file (Physical storage on disk)
    fprintf(out, "\n--- Power Quality Report: %s ---\n", phase_label);
    fprintf(out, "RMS Voltage: %.2f V (%s)\n", rms, status);
    fprintf(out, "Peak-to-Peak: %.2f V\n", p2p);
    fprintf(out, "DC Offset: %.2f V\n", dc);
    fprintf(out, "Clipping Count: %d\n", clips);

    //finalise the function
    fclose(out);
    return 0;
}


