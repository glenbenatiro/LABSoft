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