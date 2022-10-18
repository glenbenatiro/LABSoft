#ifndef GLOBALS_H
#define GLOBALS_H

enum WaveType 
{ 
  SINE, 
  SQUARE, 
  TRIANGLE, 
  DC,
  SAWTOOTH,
  NOISE,
  OTHER
};

enum DisplayMode 
{
  REPEATED,
  SHIFT,
  SCREEN
};
               
double globals_get_actual_value_from_label (const char *label);
int globals_get_unit_prefix_power_scaler (const char *label);
char globals_get_unit_prefix (int power_scaler);

#endif 