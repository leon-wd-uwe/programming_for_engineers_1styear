//
// main.c: Created by l2-wunderlichd on 22/04/2026.
//

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

    // Dynamic Memory (count how many data rows are in the file before allocating memory)
    // this means that we  open the file twice (once to count, once to read)
    int num_rows = count_csv_rows(argv[1]);
    if (num_rows <= 0) {
        printf("Error: File is empty or could not be read.\n");
        return 1;
    }
    //now malloc is sized to the actual file

    printf("Memory allocated successfully for %d samples.\n", num_rows);

    // We request space for as many "molds" (structs of WaveformSample)as we need for our data
    WaveformSample *data_log = (WaveformSample *)malloc(num_rows * sizeof(WaveformSample));
    //malloc(num_rows * sizeof(WaveformSample) is = amount of bytes of WaveformSample * 1000[num_rows] = amount of RAM needed

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



// --- START OF ANALYSIS ---

    // Array of names for the 3 phases
    const char *phase_labels[] = {"Phase A", "Phase B", "Phase C"};
    //cycling through the 3 phases and calculating with each one
    for (int phase = 0; phase < 3; phase++) {
        // 1. Calculate metrics for the current phase (A, B or C)
        double rms = calculate_rms(data_log, num_rows, phase);//calls the waveform.c function for calculating rms
        double peak_2_peak = calculate_peak_to_peak(data_log, num_rows, phase);//calls peak to peak calculation func.
        double dc_off = calculate_dc_offset(data_log, num_rows, phase);//calls DC offset calculation function
        int clips = count_clipping_samples(data_log, num_rows, phase);//calls the clipping sample counter func.

        // 2. Check compliance
        int is_compliant = check_tolerance_compliance(rms);//1 = compliant, 0 = non-compliant

        // We create a temporary pointer to hold our "decision word" (true or false)
        // 'const char*' means a string of text that won't change
        const char *status_text;
        if (is_compliant == 1) {
            // If 1 (True), we assign "PASS"
            status_text = "PASS";
        } else {
            // Otherwise, we assign "FAIL"
            status_text = "FAIL";
        }

        // 3. Print the "Live Report" to the screen (Live Demonstration)
        printf("\n--- Power Quality Report: %s ---\n", phase_labels[phase]); //report on each phase
        // We use %s to drop our chosen status_text into the sentence
        printf("RMS Voltage: %.2f V (%s)\n", rms, status_text);
        printf("Peak-to-Peak: %.2f V\n", peak_2_peak);
        printf("DC Offset: %.2f V\n", dc_off);
        printf("Clipping Detected: %d samples\n", clips);

        // 4. Save to the current phases results to the .txt file
        if (save_results("results.txt", phase_labels[phase], phase, rms, peak_2_peak, dc_off, clips, status_text) == 0) {
            printf("Results for %s saved to results.txt\n", phase_labels[phase]);
        }
    }

// --- END OF ANALYSIS ---


    // We release the memory now that we have used it for our usecase and leave it for other date to fill it later
    free(data_log);
    printf("Memory freed. Program exiting cleanly.\n");

    return 0;
}
