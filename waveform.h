//
// Created by l2-wunderlichd on 22/04/2026.
//
// waveform.h

//In yellow are Preprocessor Directives (#ifndef, #define, and #endif)
//These do not run when your program is executing but are handled by the C Preprocessor before compiling
#ifndef WAVEFORM_H /*#ifndef = if not defined; checking if WAVEFORM_H has already been defined,
 * skipping the file if it already has, and lets the computer read it if not*/
#define WAVEFORM_H //If the identifier is not found, this line defines it

typedef struct {
    double timestamp;
    double phase_A_voltage;
    double phase_B_voltage;
    double phase_C_voltage;
    double line_current;
    double frequency;
    double power_factor;
    double thd_percent;
} WaveformSample; //Naming this struct WaveformSample

#endif //This terminates the conditional block started by #ifndef