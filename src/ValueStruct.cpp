#include "ValueStruct.h"

#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cstdio>

ValueStruct::
ValueStruct (const char *label)
{
  m_short_value = atof (strtok (strdup (label), " "));
  m_unit_prefix = get_unit_prefix (strtok (NULL, "/")[0]);
  m_exponent    = get_unit_prefix_power_exponent ();
}   

char ValueStruct::
get_unit_prefix (char unit_prefix)
{
  switch (unit_prefix)
  {
    case 'G':
      return 'G';
      break;
    case 'M':
      return 'M';
      break;
    case 'k':
      return 'k';
      break;
    case 'm':
      return 'm';
      break;
    case 'u':
      return 'u';
      break;
    case 'n':
      return 'n';
      break;
    default:
      return ' ';
      break;
  }
}

int ValueStruct:: 
get_unit_prefix_power_exponent ()
{
  switch (m_unit_prefix)
  {
    case 'G':
      return 9;
    case 'M':
      return 6;
    case 'k':
      return 3;
    case 'm':
      return -3;
    case 'u':
      return -6;
    case 'n':
      return -9;
    default:
      return 0;
  }
}

double ValueStruct::
get_actual_value ()
{
  return (m_short_value * std::pow (10, m_exponent));
}


