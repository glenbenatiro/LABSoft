#include "LabelValue.h"

#include <cmath>
#include <vector>
#include <iomanip>
#include <sstream>
#include <utility>
#include <iostream>
#include <algorithm>

// ----------
// Static Values

LabelValue::Min_Max_Map_Key_length LabelValue::m_min_max_unit_prefix_to_exponent  = 
  LabelValue::find_min_map_key_length (LabelValue::m_map_unit_prefix_to_exponent);

LabelValue::Min_Max_Map_Key_length LabelValue::m_min_max_unit_string_to_unit = 
  LabelValue::find_min_map_key_length (LabelValue::m_map_unit_string_to_unit);

unsigned LabelValue::m_largest_exponent = 
  LabelValue::calc_largest_exponent (LabelValue::m_map_unit_prefix_to_exponent);

std::map<LabelValue::UNIT, std::string> LabelValue::m_map_unit_to_unit_string = 
  LabelValue::reverse_map (m_map_unit_string_to_unit);

std::map<std::string, LabelValue::UNIT> LabelValue::m_map_unit_string_to_unit_lowercase = 
  LabelValue::map_str_keys_to_lower_case (m_)

// ----------

LabelValue:: 
LabelValue ()
{

}

LabelValue::
LabelValue (double            value, 
            LabelValue::UNIT  unit) : LabelValue ()
{
  m_unit                = unit;
  m_is_valid_label_text = parse_double (value);
}

LabelValue:: 
LabelValue (const char* label,
            double      reference,
            UNIT        unit) : LabelValue ()
{
  m_raw_string          = std::string (label);
  m_reference           = reference;
  m_unit                = unit;
  m_is_valid_label_text = parse_input_if_valid (m_raw_string);

  debug ();
}

LabelValue:: 
LabelValue (const char* label,
            UNIT        unit,
            double      reference)
{
  LabelValue (label, reference, unit);
}

inline int LabelValue:: 
correct_mod (int exponent,
             int modulo) const
{
  return (
    exponent < 0 ? ((exponent % modulo + modulo) % modulo) : (exponent % modulo)
  );
}

bool LabelValue:: 
parse_input_if_valid (std::string& str)
{
  if (str.find_first_not_of ("0123456789.-+") == std::string::npos)
  {
    try 
    {
      double value = std::stod (str);

      return (parse_double (value));
    }
    catch (const std::exception& e) 
    {
      return (false);
    }
  }
  else 
  {
    return (parse_string_if_valid (str));
  }
}

bool LabelValue:: 
parse_string_if_valid (std::string& str)
{
  // 1. Remove all whitespaces in the string
  removeWhitespaces (str);

  // 2. Find the position of the first non-digit, 
  //    non-dot, or non-sign character
  unsigned pos = str.find_first_not_of ("0123456789.+-");

  // 3. Assign the substring before pos as the coefficient
  try 
  {
    m_coefficient = std::stod (str.substr (0, pos));
  }
  catch (const std::exception& e)
  {
    return (false);
  }

  // 4. If what was left after getting the coefficient is an empty string,
  //    return true
  std::string post_coeff = str.substr (pos);

  if (post_coeff.empty ())
  {
    return (true);
  }

  // 5. Get unit string/s
  find_matches_reverse (post_coeff, m_map_unit_string_to_unit,
    m_min_max_unit_string_to_unit, m_unit_string_matches);

  // 6.
  std::string post_unit_string;

  if (m_unit_string_matches.empty ())
  {
    if (m_unit != LabelValue::UNIT::NONE)
    {
      return (false);
    }

    post_unit_string = post_coeff;
  }
  else 
  {

  }


    


  if (m_unit_string_matches.empty ())
  {
    if (m_unit != LabelValue::UNIT::NONE)
    {
      return (false);
    }
  }

  // 6. If there was no unit string found, this means the remaining string
  //    (post_coeff) might most likely be just the unit prefix
  std::string post_unit_string;

  if (m_unit_string_matches.empty ())
  {
    m_unit            = LabelValue::UNIT::NONE;
    post_unit_string  = post_coeff;
  }

  // 7. If there was a unit string found, "subtract" the longest value in the
  //    m_unit_matches vector with the post_coeff string to get the possible
  //    unit prefix string
  else 
  {
    unsigned us_pos   = post_coeff.find (m_unit_string_matches.back ());
    post_unit_string  = post_coeff.substr (0, us_pos);
  }

  // 8. 

  // 8. Get unit string

  m_exponent      = calc_sci_exponent (m_unit_prefix);
  m_actual_value  = calc_actual_value (m_coefficient, m_exponent);

  return (true);
}

bool LabelValue:: 
parse_double (double value)
{
  m_actual_value = calc_actual_value_using_reference (value, m_reference);

  calc_sci_coefficient_and_exponent (m_actual_value, m_coefficient, m_exponent);

  m_unit_prefix = calc_unit_prefix (m_exponent);

  return (true);
}

double LabelValue:: 
calc_actual_value_using_reference (double value, 
                                   double reference)
{
  if (reference == 0.0)
  {
    return (value);
  }
  else 
  {
    double  temp_coeff  = 0.0;
    int     temp_exp    = 0;
    
    // 1. Get the coefficient and exponent of the 
    //    reference value's scientific notation
    calc_sci_coefficient_and_exponent (reference, temp_coeff, temp_exp);

    // 2. Using the exponent, get the unit prefix
    std::string temp_unit_prefix = calc_unit_prefix (temp_exp);

    // 3. Using the unit prefix, get the exponent
    int pow_exp = calc_sci_exponent (temp_unit_prefix);

    return (value * std::pow (10, pow_exp));
  }
}

void LabelValue::
calc_sci_coefficient_and_exponent (double   value,
                                   double&  coefficient,
                                   int&     exponent)
{
  // 1. Convert the value to a string representation of its scientific notation
  std::stringstream ss;
  ss << std::scientific << std::setprecision (m_largest_exponent) << value;
  std::string str = ss.str ();

  // 2. Extract the coefficient and exponent from the string representation
  unsigned e_pos  = str.find_first_of ("Ee");
  coefficient     = std::stod (str.substr (0, e_pos));
  exponent        = std::stoi (str.substr (e_pos + 1));
}

std::string LabelValue:: 
calc_unit_prefix (int exponent) const
{
  switch (exponent)
  {
    case -9:
    case -8:
    case -7:
      return ("n");

    case -6:
    case -5:
    case -4:
      return ("u");

    case -3:
    case -2:
    case -1:
      return ("m");
    
    case 0:
    case 1:
    case 2:
      return ("");
    
    case 3:
    case 4:
    case 5:
      return ("k");

    case 6:
    case 7:
    case 8:
      return ("M");
    
    case 9:
    case 10:
    case 11:
      return ("G");
  }
}

int LabelValue:: 
calc_sci_exponent (std::string& unit_prefix) const
{
  return (m_map_unit_prefix_to_exponent.at (unit_prefix));
}

void LabelValue:: 
debug ()
{
  std::cout << "----------\n";
  std::cout << "actual value    : " << m_actual_value                 << "\n";
  std::cout << "coefficient     : " << m_coefficient                  << "\n";
  std::cout << "exponent        : " << m_exponent                     << "\n";
  std::cout << "unit prefix     : " << m_unit_prefix                  << "\n";
  std::cout << "label type      : " << static_cast<int>(m_label_type) << "\n";
  std::cout << "is valid        : " << m_is_valid_label_text          << "\n";
  std::cout << "reference value : " << m_reference              << "\n";
  std::cout << "----------\n";
}

void LabelValue:: 
removeWhitespaces (std::string& str)
{
  str.erase (std::remove_if (str.begin (), str.end (), ::isspace), str.end ());
}

std::string LabelValue:: 
strToLowerCase (std::string& str)
{
  std::string str_lower = str;

  std::transform (str_lower.begin (), str_lower.end (), str_lower.begin (),
    [](unsigned char c) {return std::tolower (c);});
  
  return (std::string);
}

template <typename KeyType, typename ValueType>
std::map<KeyType, ValueType> LabelValue::
reverse_map (const std::map<KeyType, ValueType>& map)
{
  std::map<ValueType, KeyType> reversed_map;

  for (const auto& pair : map)
  {
    reversed_map[pair.second] = pair.first;
  }

  return (reversed_map);
}

template <typename ValueType>
LabelValue::Min_Max_Map_Key_length LabelValue:: 
find_min_map_key_length (const std::map<std::string, ValueType>& map)
{
  unsigned min = 0;
  unsigned max = 0;

  for (auto it = map.begin (); it != map.end (); ++it)
  {
    const std::string& key = it->first;

    if (it == map.begin ())
    {
      min = key.length ();
      max = key.length ();
    }

    min = (key.length () < min ) ? key.length () : min;
    max = (key.length () > max ) ? key.length () : max;
  }

  return (LabelValue::Min_Max_Map_Key_length {min, max});
}

template <typename ValueType>
unsigned LabelValue:: 
check_for_match (const  std::string&                      str, 
                        std::map<std::string, ValueType>& map,
                        unsigned                          max_str_length)
{
  for (int i = 1; i <= (max_str_length + 1); i++)
  {
    std::string substr = str.substr (0, i);

    typename std::map<std::string, ValueType>::iterator it = map.find (substr);

    if (it != map.end ())
    {
      return (i);
    }
  }

  return (-1);
}

template <typename KeyType>
unsigned LabelValue:: 
calc_largest_exponent (const std::map<KeyType, int>& map)
{
  int x = 0;

  for (const auto& pair : map)
  {
    x = (std::abs(pair.second) > x) ? std::abs(pair.second) : x;
  }

  return (x);
}

template <typename ValueType>
void LabelValue:: 
find_matches_reverse (const std::string&                      str,
                            std::map<std::string, ValueType>& map,
                            Min_Max_Map_Key_length            key_length,
                            std::vector<std::string>&         unit_vector)
{
  std::string str_lower = strToLowerCase (str);

  for (int i = key_length.min; i <= str.length () && i <= key_length.max; i++)
  {
    std::string substr = str_lower.substr (str.length () - i, str.length () + 1);

    typename std::map<std::string, ValueType>::iterator it = map.find (substr);

    if (it != map.end ())
    {
      unit_vector.emplace_back (substr);
    }
  }
}

std::string LabelValue:: 
to_label_text (unsigned precision)
{
  std::stringstream ss;

  ss  <<  (m_coefficient * std::pow (10, correct_mod (m_exponent, 3)))
      <<  (m_exponent == 0 ? "" : " ") 
      <<  m_unit_prefix 
      <<  labelvalue_for_string_format[m_label_type];
  
  return (ss.str ());
}

std::string LabelValue:: 
to_label_text (LabelValue::UNIT parse_input_as, unsigned precision)
{
  m_label_type = parse_input_as;

  return (to_label_text ());
}

void LabelValue::
label_for (LabelValue::UNIT parse_output_as)
{
  m_label_type = parse_output_as;
}

int LabelValue:: 
exponent ()
{
  return (m_exponent);
}

bool LabelValue::
is_valid_label_text ()
{
  return (m_is_valid_label_text);
}

LabelValue::UNIT LabelValue:: 
unit ()
{
  return (m_unit);
}

double LabelValue:: 
coefficient ()
{
  return (m_coefficient);
}

double LabelValue:: 
actual_value ()
{
  return (m_actual_value);
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

std::string LabelValue:: 
unit_prefix ()
{
  return ();
}

// EOF