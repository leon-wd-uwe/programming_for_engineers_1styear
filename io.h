//
// Created by l2-wunderlichd on 22/04/2026.
//

#ifndef IO_H
#define IO_H
#include <stdio.h>
#include "waveform.h"

int load_csv(const char *filename, WaveformSample *data, int rows);
int save_results(const char *filename, double rms, double p2p, double dc, int clips, const char* status);

#endif