#ifndef LAB_UTILITY_FUNCTIONS_H
#define LAB_UTILITY_FUNCTIONS_H

#include <string>

namespace LABF
{
  bool is_equal         (double a, double b, double epsilon);
  bool is_within_range  (double value, double min, double max, double epsilon);



  double      normalize         (double input, double min_input, double max_input, double min_output, double max_output);
  std::string get_now_timestamp ();
};

#endif