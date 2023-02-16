#include "LabelValue.h"

#include <iostream>
#include <cstring>

LabelValue::
LabelValue (const char *label)
{
  std::string str (label);

  unsigned delim = str.find (" ");

  m_coefficient   = std::stod         (str.substr (0, delim));
  m_unit_prefix   = get_unit_prefix   (str.substr (delim + 1));
  m_unit          = get_unit          (str.substr (delim + 1)); 
  m_exponent      = get_exponent      (m_unit_prefix);
  m_actual_value  = get_actual_value  (m_coefficient, m_exponent);
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

  if (integral == 0.0 && fractional == 0.0)
  {
    *coefficient  = 0.0;
    *exponent     = 0;
  }
  else
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

    *coefficient = temp;
    *exponent    = a;
  } 
}

double LabelValue:: 
get_actual_value (double coefficient, 
                  int    exponent)
{
  return (coefficient * std::pow (10, -1 * exponent));
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

char LabelValue:: 
get_unit_prefix (std::string str)
{
  char prefix = str[0];

  switch (prefix)
  {
    case 'G':
    case 'M':
    case 'k':
    case 'm':
    case 'u':
    case 'n':
    case ' ':
      return prefix; 
      break;
    default:
      return ' ';
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

std::string LabelValue:: 
get_unit (std::string str)
{
  char c = str[0];

  switch (c)
  {
    case 'G':
    case 'M':
    case 'k':
    case 'm':
    case 'u':
    case 'n':
    case ' ':
      return str.substr (1);
      break;
    default:
      return str;
      break;
  }
}

std::string LabelValue:: 
get_unit (LE_UNIT le_unit)
{
  switch (le_unit)
  {
    case LE_UNIT_NONE:
      return " ";
      break;
    case LE_UNIT_VOLT:
      return "V";
      break; 
    case LE_UNIT_AMP:
      return "A";
      break; 
    case LE_UNIT_OHM:
      return "O";
      break;
    case LE_UNIT_HZ:
      return "Hz";
      break;
    case LE_UNIT_SEC:
      return "s";
    default:
      get_unit (LE_UNIT_NONE);
      break;
  }

  // just to shut the compiler up
  return (nullptr);
}

std::string LabelValue:: 
to_label_text ()
{
  char label[50];
  double integral, fractional;

  fractional = std::modf (m_coefficient, &integral);

  if (fractional = 0)
  {
    sprintf (label, "%3.0f %c%s", m_coefficient, m_unit_prefix, m_unit.c_str ());
  }
  else 
  {
    sprintf (label, "%3.2f %c%s", m_coefficient, m_unit_prefix, m_unit.c_str ());
  }

  return (std::string (label));
}

// EOF