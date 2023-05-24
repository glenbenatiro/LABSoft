#include "Defaults.h"

bool LABF::
compare_double (double a,
                double b,
                double epsilon)
{
  return ((std::abs (a - b)) < epsilon);
}

bool LABF:: 
is_within_range (double value,
                 double min,
                 double max)
{
  if ((value <= max && value >= min) || compare_double (value, min) || 
      compare_double (value, max))
  {
    return (true);
  }
  else 
  {
    return (false);
  }
}

int LABF::
normalize (int input, 
           int min_input, 
           int max_input, 
           int min_output, 
           int max_output)
{
  return (min_output + (((input - min_input) * (max_output - min_output)) / 
    (max_input - min_input)));
}
