#include "Defaults.h"

namespace LAB_DEFAULTS
{
  bool compareDouble (double a,
                      double b,
                      double epsilon)
  {
    return ((std::abs (a - b)) < epsilon);
  }

  bool isWithinRange (double value,
                      double min,
                      double max)
  {
    if ((value >= min && value <= max) || compareDouble (value, min) || 
      compareDouble (value, max))
    {
      return (true);
    }
    
    return (false);
  }
}
