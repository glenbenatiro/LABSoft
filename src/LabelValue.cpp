#include "LabelValue.h"

#include <iomanip>
#include <sstream>
#include <cmath>

#include <iostream>

LabelValue::
LabelValue (double          value, 
            LABELVALUE_TYPE parse_input_as)
{
  m_label_type          = parse_input_as;
  m_is_valid_label_text = parse_double_if_valid (value);
}

LabelValue::
LabelValue (const char      *label, 
            double           reference,
            LABELVALUE_TYPE  parse_input_as)
{
  std::string str (label);

  m_label_type          = parse_input_as;  
  m_reference_value     = reference;
  m_is_valid_label_text = parse_widget_input_if_valid (str);
}

LabelValue::
LabelValue (const char      *label,
            LABELVALUE_TYPE  parse_input_as)
{
  LabelValue (label, 0.0, parse_input_as);
}

//

bool LabelValue:: 
parse_widget_input_if_valid (const std::string& str)
{
  // 1. Check if the string is empty
  if (str.empty ())
    return (false);

  // 2. Check if the first digit of the string is not a number,
  //    not a dot, or not a dash (for negative)
  if (! (std::isdigit (str[0]) || str[0] == '.' || str[0] == '-'))
    return (false);

  // 3. If no non-digit or non-dot is found, the string may most likely 
  //    be a value. Else, further parse the string.
  if (str.find_first_not_of ("0123456789.-") == std::string::npos)
  {
    return (parse_double_if_valid (std::stod (str)));
  }
  else 
  {
    return (parse_string_if_valid (str));
  }
}

bool LabelValue:: 
parse_double_if_valid (double value)
{
  // do checking of double if valid here.
  // if not, proceed straight away to parsing

  // scale actual value using m_reference
  m_actual_value = calc_actual_value_using_reference (value, m_reference_value);
  calc_sci_coefficient_and_exponent (m_actual_value, m_coefficient, m_exponent);
  m_unit_prefix = calc_unit_prefix (m_exponent);

  return (true);
}

bool LabelValue:: 
parse_string_if_valid (const std::string& str)
{
  // 1. Find the position of the first non-digit or non-dot character
  unsigned nd_pos = str.find_first_not_of ("0123456789.-");

  // 2. Assign the substring before the position as the coefficient
  m_coefficient = std::stod (str.substr (0, nd_pos));

  // 3. Remove all possible whitespaces between the coefficient and the unit
  std::string post_coeff  = str.substr (nd_pos);
  nd_pos                  = post_coeff.find_first_not_of (" \t\r\n");
  post_coeff              = post_coeff.substr (nd_pos);

  // 4.1 If the remaining string is equivalent to the labelvalue_for_string_format,
  //     this means that exponent is 0 = base value.
  if (post_coeff == labelvalue_for_string_format[m_label_type])
  {
    m_unit_prefix = " ";
  }
  else 
  {
    std::string poss_unit   = post_coeff.substr (0, 1);
    std::string poss_format = post_coeff.substr (1);

    auto it = m_si_prefix_to_exp.find (poss_unit);

    if (it != m_si_prefix_to_exp.end () && 
      poss_format == labelvalue_for_string_format[m_label_type])
    {
      m_unit_prefix = poss_unit;
    }
    else 
    {
      return (false);
    }
  }

  m_exponent      = calc_sci_exponent (m_unit_prefix);
  m_actual_value  = calc_actual_value (m_coefficient, m_exponent);

  return (true);
}

double LabelValue:: 
calc_actual_value_using_reference (double actual_value, 
                                   double reference)
{
  if (reference == 0.0)
  {
    return (actual_value);
  }
  else 
  {
    int     temp_exp;
    double  temp_coeff;
  
    // Get the unit prefix of the reference value
    calc_sci_coefficient_and_exponent (
      reference, 
      temp_coeff, 
      temp_exp
    );

    // Using the unit prefix, get the unit prefix integer
    int pow_exp = calc_sci_exponent (calc_unit_prefix (temp_exp));

    return (actual_value * std::pow (10, pow_exp));
  }
}

double LabelValue:: 
calc_actual_value (double coefficient, 
                   int    exponent)
{
  return (coefficient * std::pow (10, exponent));
}

int LabelValue:: 
calc_sci_exponent (std::string unit_prefix)
{
  // TO-DO: do bounds checking

  return (m_si_prefix_to_exp[unit_prefix]);
}

void LabelValue::
calc_sci_coefficient_and_exponent (double   value,
                                   double&  coefficient,
                                   int&     exponent)
{
  // Convert value to a string representation of its scientific notation
  std::stringstream ss;
  ss << std::scientific << std::setprecision (12) << value;
  std::string str = ss.str ();

  // Extract coefficient and exponent from the string
  unsigned e_pos  = str.find_first_of ("eE");
  coefficient     = std::stod (str.substr(0, e_pos));
  exponent        = std::stoi (str.substr (e_pos + 1));
}

std::string LabelValue:: 
calc_unit_prefix (int exponent)
{
  // TO-DO: do bounds checking

  return (m_exp_to_si_prefix[exponent]);
}

std::string LabelValue:: 
to_label_text ()
{
  std::stringstream ss;

  ss  <<  (m_coefficient * std::pow (10, correct_mod (m_exponent, 3)))
      <<  (m_exponent == 0 ? "" : " ") 
      <<  m_unit_prefix 
      <<  labelvalue_for_string_format[m_label_type];
  
  return (ss.str ());
}

std::string LabelValue:: 
to_label_text (LABELVALUE_TYPE parse_input_as)
{
  m_label_type = parse_input_as;

  return (to_label_text ());
}

// Getters

double LabelValue:: 
actual_value ()
{
  return (m_actual_value);
}

double LabelValue:: 
coefficient ()
{
  return (m_coefficient);
}

int LabelValue:: 
exponent ()
{
  return (m_exponent);
}

char LabelValue:: 
unit_prefix ()
{
  return (m_unit_prefix[0]);
}

std::string LabelValue::
label_for ()
{
  return (labelvalue_for_string_format[m_label_type]);
}

bool LabelValue::
is_valid_label_text ()
{
  return (m_is_valid_label_text);
}

std::string LabelValue::
short_value ()
{
  std::stringstream ss;

  double val = m_coefficient * std::pow (10, correct_mod (m_exponent, 3));

  if (val == 0)
  {
    std::abs (val);
  }

  // Huh. Amazing. When adding a space after the val, 
  // the string output is clean. No uneeded decimal point.
  // No trailing zeros. Amazing.
  ss << val << " ";
  
  return (ss.str ());
}

// Setters
void LabelValue::
label_for (LABELVALUE_TYPE parse_output_as)
{
  m_label_type = parse_output_as;
}

// EOF