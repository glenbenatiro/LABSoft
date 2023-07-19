#ifndef LAB_UTILITY_FUNCTIONS_H
#define LAB_UTILITY_FUNCTIONS_H

namespace LABF
{
  bool is_equal   (double a, double b, double epsilon = 1e-9);
  bool is_within_range  (double value, double min, double max);
  int  normalize        (int input, int min_input, int max_input, int min_output, int max_output);
};

#endif