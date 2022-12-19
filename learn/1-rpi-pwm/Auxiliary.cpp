#include "Utility.h"

#include <cstdlib>
#include <cmath>

double aux_unit_label_to_unit_scaler (const char *label)
{
  int a;
  char prefix = label[0];
 
  switch (prefix)
    {
      case ('G'):
        a = 3;
        break;
      case ('M'):
        a = 2;
        break;
      case ('k'):
        a = 1;
        break;
      case ('m'):
        a = -1;
        break;
      case ('u'):
        a = -2;
        break;
      case ('n'):
        a = -3;
        break;
      default:
        a = 0;
        break;
    }
  
  return pow (10, (3 * (a * -1)));
}

double aux_unit_label_to_unit_power (const char *label)
{
  int a;
  char prefix = label[0];
 
  switch (prefix)
    {
      case ('G'):
        a = 3;
        break;
      case ('M'):
        a = 2;
        break;
      case ('k'):
        a = 1;
        break;
      case ('m'):
        a = -1;
        break;
      case ('u'):
        a = -2;
        break;
      case ('n'):
        a = -3;
        break;
      default:
        a = 0;
        break;
    }
  
  return a;
}
