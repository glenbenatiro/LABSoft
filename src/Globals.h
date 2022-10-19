#ifndef GLOBALS_H
#define GLOBALS_H
               
double globals_get_actual_value_from_label (const char *label);
int globals_get_unit_prefix_power_scaler (const char *label);
char globals_get_unit_prefix (int power_scaler);

#endif 