#include "LabelValue.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cmath>

LabelValue::
LabelValue (const char *label)
{
  char *label_copy = strdup (label);

  m_coefficient = atof (strtok (label_copy, " "));

  char *temp = strtok (NULL, "/");
  m_unit_prefix = (temp == nullptr) ? ' ' : unit_prefix (*temp);

  m_exponent  = exponent (m_unit_prefix);

  free (label_copy);
}   

LabelValue:: 
LabelValue (double value)
{
  char value_char [20];

  sprintf (value_char, "%9.9e", value);

  m_coefficient   = atof (strtok (strdup (value_char), "e"));
  m_exponent      = atof (strtok (NULL, "/"));
  m_unit_prefix   = unit_prefix (m_exponent);

  // printf ("input: %f\ncoefficient: %f\nexponent: %d\nunit_prefix: %c\n\n", value, m_coefficient,
  //   m_exponent, m_unit_prefix);
}

// getters
int LabelValue:: 
exponent ()
{
  return m_exponent;
}

float LabelValue:: 
coefficient ()
{
  return m_coefficient;
}

float LabelValue:: 
short_value ()
{
  if (m_exponent < 0)
  {
    int exp = 3 - (std::abs (m_exponent) % 3);

    return (m_coefficient * pow (10, exp % 3));
  }
  else 
  {
    return (m_coefficient * pow (10, m_exponent % 3));
  }
}

char LabelValue:: 
unit_prefix ()
{
  return m_unit_prefix;
}

char LabelValue::
unit_prefix (char unit_prefix)
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

char LabelValue::
unit_prefix (int exponent)
{
  switch (exponent)
  {
    case 11:
    case 10:
    case 9:
      return 'G';
      break;
    case 8:
    case 7:
    case 6:
      return 'M';
      break;
    case 5:
    case 4:
    case 3:
      return 'k';
      break;
    case -1:
    case -2:
    case -3:
      return 'm';
      break;
    case -4:
    case -5:
    case -6:
      return 'u';
      break;
    case -7:
    case -8:
    case -9:
      return 'n';
      break;
    default:
      return ' ';
      break;
  }
}

int LabelValue:: 
exponent (char m_unit_prefix)
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

double LabelValue::
actual_value ()
{
  return (m_coefficient * std::pow (10, m_exponent));
}


std::string LabelValue:: 
label_text (double value, LABEL_TYPE _LABEL_TYPE)
{
  char label[20];
  LabelValue _LabelValue (value);

  bool int_check = (((int)(_LabelValue.short_value ())) == _LabelValue.short_value ()) ? 
    true : false;
    
  switch (_LABEL_TYPE)
  {
    case TIME_PER_DIVISION:
      if (int_check)
      {
        sprintf (label, "%.0f %cs", _LabelValue.short_value (), _LabelValue.unit_prefix ());
      }
      else 
      {
        sprintf (label, "%.2f %cs", _LabelValue.short_value (), _LabelValue.unit_prefix ());
      }
      break;
    default:
      label_text (value, TIME_PER_DIVISION);
      break;
  }

  std::string string_label (label);

  return (string_label);
}