#include "LAB_Globals.h"

float LAB_Globals_label_to_scaled_value (const char *label)
{
  float scaler = 1.0;
  float value = atof (strtok (strdup (label),  " "));
  char unit = strtok (NULL, "/")[0];

  switch (unit)
    {
      case 'G':
          scaler = 1000000000.0;
          break;
      case 'M':
          scaler = 1000000.0;
          break;
      case 'k':
          scaler = 1000.0;
          break;
      case 'm':
          scaler = 0.001;
          break;
      case 'u':
          scaler = 0.000001;
          break;
      case 'n':
          scaler = 0.000000001;
          break;
      default:
          scaler = 1.0;
          break;
    }
  
  return value * scaler;
}

float LAB_Globals_label_to_unit_scaler (const char *label)
{
  float scaler = 1.0;
  float value = atof (strtok (strdup (label),  " "));
  char unit = strtok (NULL, "/")[0];

  switch (unit)
    {
      case 'G':
          scaler = 1000000000.0;
          break;
      case 'M':
          scaler = 1000000.0;
          break;
      case 'k':
          scaler = 1000.0;
          break;
      case 'm':
          scaler = 0.001;
          break;
      case 'u':
          scaler = 0.000001;
          break;
      case 'n':
          scaler = 0.000000001;
          break;
      default:
          scaler = 1.0;
          break;
    }
  
  // return value * scaler;
  return scaler;
}