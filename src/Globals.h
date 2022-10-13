#ifndef GLOBALS_H
#define GLOBALS_H

enum WaveType {SINE, 
               SQUARE, 
               TRIANGLE, 
               DC,
               SAWTOOTH,
               NOISE,
               OTHER};
               
double globals_get_actual_value (const char *label);
int globals_get_unit_prefix_power_scaler (const char *label);
char globals_get_unit_prefix (int power_scaler);

#endif 