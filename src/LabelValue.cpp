#include "LabelValue.h"

#include <cstring>

LabelValue::
LabelValue (const char *label)
{
  m_coefficient   = std::atof (std::strtok (strdup (label), " "));

  char *unit_string = std::strtok (nullptr, "");

  if (unit_string != nullptr)
  {
    m_unit = (get_unit_prefix (unit_string, &m_unit_prefix)) ? unit_string[1] : 
      unit_string[0];
  }

  m_exponent = get_exponent (m_unit_prefix);

  m_actual_value  = get_actual_value (m_coefficient, m_exponent);
}   

LabelValue:: 
LabelValue (double value, LE_UNIT unit)
{
  m_actual_value  = value;

  exp_notation (value, &m_coefficient, &m_exponent);

  m_unit_prefix   = get_unit_prefix (m_exponent);
  m_unit          = get_unit (unit);
}

// NOTE: This function only checks until giga (exp +9) and nano (exp -9)
void LabelValue:: 
exp_notation (double  actual_value, 
              double *coefficient, 
              int    *exponent)
{
  int     a = 0;
  double  temp, intgr, integral, fractional = std::modf (actual_value, &integral);
  bool    flag = (integral == 0.0);

  if (!(integral == 0.0 && fractional == 0.0))
  {
    if (std::abs(integral) < 1000.0 && std::abs(integral) > 0.0)
    {
      *coefficient = actual_value;
      *exponent = a;

      return;
    }
    
    for (a = (flag ? 3 : -3); ; a += (flag ? 3 : -3))
    {
      temp       = actual_value * std::pow (10, a);
      fractional = std::modf (temp, &integral);
      intgr      = std::abs (integral);

      if (flag)
      {
        if (intgr > 0.0)
          break;
      }
      else 
      {
        if (intgr < 1000.0)
          break;
      }
    }
  }   
  
  *coefficient = temp;
  *exponent    = a;
}

double LabelValue:: 
get_actual_value (double coefficient, 
                  int    exponent)
{
  return (coefficient * std::pow (10, exponent));
}

int LabelValue:: 
get_exponent (char unit_prefix)
{
  switch (unit_prefix)
  {
    case 'G':
      return -9;
      break;
    case 'M':
      return -6;
      break;
    case 'k':
      return -3;
      break;
    case 'm':
      return 3;
      break;
    case 'u':
      return 6;
      break;
    case 'n':
      return 9;
      break;
    default:
      return 0;
      break;
  }
}

int LabelValue:: 
get_unit_prefix (char *unit_string, 
                 char *unit_prefix)
{
  char prefix = unit_string[0];

  switch (prefix)
  {
    case 'G':
    case 'M':
    case 'k':
    case 'm':
    case 'u':
    case 'n':
      *unit_prefix = prefix;
      return 1;
    default:
      *unit_prefix = ' ';
      return 0;
      break;
  }
}

char LabelValue::
get_unit_prefix (int exponent)
{
  switch (exponent)
  {
    case 11:
    case 10:
    case 9:
      return 'n';
      break;
    case 8:
    case 7:
    case 6:
      return 'u';
      break;
    case 5:
    case 4:
    case 3:
      return 'm';
      break;
    case -1:
    case -2:
    case -3:
      return ' ';
      break;
    case -4:
    case -5:
    case -6:
      return 'k';
      break;
    case -7:
    case -8:
    case -9:
      return 'M';
      break;
    case -10:
    case -11:
    case -12:
      return 'G';
      break;
    default:
      return ' ';
      break;
  }
}

char LabelValue:: 
get_unit (LE_UNIT unit)
{
  return (static_cast<char>(unit));
}

std::string LabelValue:: 
to_label_text ()
{
  char label[20];

  double integral, fractional = std::modf (m_coefficient, &integral);

  if (fractional == 0.0)
  {
    sprintf (label, "%3.0f %c%c", m_coefficient, m_unit_prefix, m_unit);
  }
  else 
  {
    sprintf (label, "%3.3f %c%c", m_coefficient, m_unit_prefix, m_unit);
  }

  //printf ("to_label_text: %s\n", label);

  return (std::string (label));
}

// EOF