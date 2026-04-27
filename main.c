#include <stdio.h>
#include <stdlib.h>
#include "waveform.h" // Includes our struct "blueprint"
#include "io.h"

int main(int argc, char *argv[]) {
    //check if a filename was provided in the command line
    if (argc < 2) {/*if <2 then user forgot to indicate the desired file to open since they didn't
 *                  type at least 2 words (e.g. ./PowerAnalyser log.csv) */
        printf("Usage: %s <filename.csv>\n", argv[0]);
        return 1;
    }

    // set up the Dynamic Memory
    int num_rows = 1000; //number of rows needed

    // We request space for 1,000 "molds" (structs of WaveformSample) of our data
    WaveformSample *data_log = (WaveformSample *)malloc(num_rows * sizeof(WaveformSample));
    //malloc(num_rows * sizeof(WaveformSample) is: amount of bytes of WaveformSample * 1000[num_rows] = amount of RAM needed

    // Safety Check to see if the space was available. if not, print error
    if (data_log == NULL) {// if computer is out of RAM then it tells you "Error, no more space"
        printf("Error: Memory allocation failed!\n");
        return 1;
    }

// Physical data ingestion: pouring the CSV into our memory molds of the struct
    if (load_csv(argv[1], data_log, num_rows) != 0) {
        printf("Critical Error: Could not load data from %s\n", argv[1]);
        free(data_log); // Clean up memory before exiting
        return 1;
    }
    //STILL HAVENT PUT IN THE CSV FILE
    //REMEMBER TO UPLOAD

    // --- START OF ANALYSIS ---

    // 1. Calculate metrics for Phase A
    double rms_a = calculate_rms(data_log, num_rows);//calls the waveform.c function for calculating rms
    double peak_p_a = calculate_peak_to_peak(data_log, num_rows);//calls peak to peak calculation func.
    double dc_off_a = calculate_dc_offset(data_log, num_rows);//calls DC offset calculation function
    int clips_a = count_clipping_samples(data_log, num_rows);//calls the clipping sample counter func.

    // 2. Check compliance
    int is_compliant = check_tolerance_compliance(rms_a);//1 = compliant, 0 = non-compliant

    // We create a temporary pointer to hold our "Decision Word"
    // 'const char*' means a string of text that won't change
    const char* status_text;

    if (is_compliant == 1) {
        // If 1 (True), we assign "PASS"
        status_text = "PASS";
    } else {
        // Otherwise, we assign "FAIL"
        status_text = "FAIL";
    }

    // 3. Print the "Live Report" to the screen (Live Demonstration)
    printf("\n--- Power Quality Report: Phase A ---\n");
    // We use %s to drop our chosen status_text into the sentence
    printf("RMS Voltage: %.2f V (%s)\n", rms_a, status_text);
    printf("Peak-to-Peak: %.2f V\n", peak_p_a);
    printf("DC Offset: %.2f V\n", dc_off_a);
    printf("Clipping Detected: %d samples\n", clips_a);

    // --- END OF ANALYSIS ---


    printf("Memory allocated successfully for %d samples.\n", num_rows);

    // 4. Save to .txt file
    if (save_results("results.txt", rms_a, peak_p_a, dc_off_a, clips_a, status_text) == 0) {
        printf("\nResults successfully saved to results.txt\n");
    }

    // We release the memory now that we have used it for our usecase and leave it for other date to fill it later
    free(data_log);
    printf("Memory freed. Program exiting cleanly.\n");

    return 0;
}
