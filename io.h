//
// io.h Created by l2-wunderlichd on 22/04/2026.
//

#ifndef IO_H
#define IO_H
#include <stdio.h>
#include "waveform.h"

//the header file for io.c executes each one of its functions for the main to use
int load_csv(const char *filename, WaveformSample *data, int rows);
int count_csv_rows(const char *filename);
int save_results(const char *filename, const char *phase_label, int phase_num, double rms, double p2p, double dc, int clips, const char* status, double std_dev);

#endif