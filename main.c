#include <stdio.h>
#include <stdlib.h>
#include "waveform.h" // Includes our struct "blueprint"

int main(int argc, char *argv[]) {
    //check if a filename was provided in the command line
    if (argc < 2) {/*if <2 then user forgot to indicate the desired file to open since they didn't
 *                  type at least 2 words (eg. ./PowerAnalyser log.csv) */
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

    //ACTUAL ACTION WILL HAPPEN HERE



    printf("Memory allocated successfully for %d samples.\n", num_rows);

    // We release the memory now that we have used it for our usecase and leave it for other date to fill it later
    free(data_log);
    printf("Memory freed. Program exiting cleanly.\n");

    return 0;
}
