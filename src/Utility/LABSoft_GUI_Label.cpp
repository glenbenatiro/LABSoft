#include "LABSoft_GUI_Label.h"

#include <cmath>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <algorithm>

// --- Initialize static member variables ---

std::unordered_map<LABSoft_GUI_Label::UNIT, std::string> LABSoft_GUI_Label::m_unit_to_unit_string = 
{
  {UNIT::VOLT               , "V"},
  {UNIT::VOLT_PER_DIVISION  , "V/div"},
  {UNIT::SECOND             , "s"},
  {UNIT::SECOND_PER_DIVISION, "s/div"},
  {UNIT::HERTZ              , "Hz"},
  {UNIT::DEGREE             , "deg"},
  {UNIT::NONE               , ""},
  {UNIT::ANY                , ""}
};

std::unordered_map<std::string, int> LABSoft_GUI_Label::m_unit_prefix_to_exponent = 
{
  // Base 10
  {"n", -9},
  {"u", -6},
  {"m", -3},
  {"",   0},
  {"k",  3},
  {"M",  6},
  {"G",  9},

  // Base 2
  {"Ki", 10},
  {"Mi", 20},
  {"Gi", 30},
};

std::unordered_map<std::string, LABSoft_GUI_Label::UNIT> LABSoft_GUI_Label::m_unit_string_to_unit = 
  invert_map (LABSoft_GUI_Label::m_unit_to_unit_string);

std::unordered_map<std::string, LABSoft_GUI_Label::UNIT> LABSoft_GUI_Label::m_unit_string_lowercase_to_unit =
  lowercase_map (m_unit_string_to_unit);

LABSoft_GUI_Label::KeyLengths LABSoft_GUI_Label::m_keylengths_unit_prefix_to_exponent = 
  find_map_keylengths (m_unit_prefix_to_exponent);

// ----------

LABSoft_GUI_Label:: 
LABSoft_GUI_Label (const std::string&             label,
                         double                   reference,
                         LABSoft_GUI_Label::UNIT  unit)
{
  m_raw_string      = label;
  m_cleaned_string  = remove_all_whitespacaes (m_raw_string);
  m_reference       = reference;
  m_unit            = unit;
  m_is_valid        = parse_input_string (m_cleaned_string);

  // debug ();
}

LABSoft_GUI_Label::
LABSoft_GUI_Label (double value)
{
  m_is_valid = parse_input_double (value);
}


bool LABSoft_GUI_Label:: 
parse_input_string (std::string_view str_view)
{
  try
  {
    m_coefficient = get_coefficient (str_view);
    m_unit_prefix = 
  }
  catch (const std::exception& e)
  {
    return (false);
  }
}

double LABSoft_GUI_Label:: 
get_coefficient ()

bool LABSoft_GUI_Label:: 
parse_input (const std::string& str)
{







  if (str.find_first_not_of ("0123456789.+-") == std::string::npos)
  {
    try
    {
      return (parse_input_double (std::stod (str)));
    }
    catch (const std::exception& e)
    {
      return (false);
    }
  }
  else 
  {
    return (parse_input_string (str));
  }
} 

bool LABSoft_GUI_Label:: 
parse_input_string (std::string_view str_view)
{
  try 
  {
    size_t pos      = str_view.find_first_not_of ("0123456789.+-");

    m_coefficient   = std::stod (str_view.substr (0, pos));
    m_unit_prefix   = find_unit_prefix_match (str.substr (pos));
    m_unit          = find_unit (str.substr (pos + m_unit_prefix.length ()));

    m_base          = calc_base (m_unit_prefix);
    m_exponent      = calc_exponent (m_unit_prefix);
    m_actual_value  = calc_actual_value (m_coefficient, m_base, m_exponent);

    return (false);
  }
  catch (const std::exception& e)
  {
    return (false);
  }


  remove_from_string (m_unit_prefix, post_coeff);

  post_coeff = to_lowercase (post_coeff);

  if (!post_coeff.empty ())
  {
    try
    {
      LABSoft_GUI_Label::UNIT unit = m_unit_string_lowercase_to_unit.at (post_coeff);

      if (unit != m_unit)
      {
        return (false);
      }
    }
    catch(const std::exception& e)
    {
      return (false);
    }
  }
}

bool LABSoft_GUI_Label:: 
parse_input_double (double value)
{
  m_actual_value = calc_actual_value_using_reference (value, m_reference);

  calc_coefficient_and_exponent (m_actual_value, m_coefficient, m_exponent);

  m_unit_prefix = calc_unit_prefix (m_exponent);

  return (true);
}

template <typename Key, typename Value>
std::unordered_map<Value, Key> LABSoft_GUI_Label::
invert_map (const std::unordered_map<Key, Value>& map)
{
  std::unordered_map<Value, Key> new_map;

  for (const std::pair<Key, Value> entry : map)
  {
    new_map[entry.second] = entry.first;
  }

  return (new_map);
}

template <typename Value>
std::unordered_map<std::string, Value> LABSoft_GUI_Label::
lowercase_map (const std::unordered_map<std::string, Value>& map)
{
  std::unordered_map<std::string, Value> new_map;

  for (const std::pair<std::string, Value> entry : map)
  {
    new_map[to_lowercase (entry.first)] = entry.second;
  }

  return (new_map);
}

template <typename Value>
LABSoft_GUI_Label::KeyLengths LABSoft_GUI_Label:: 
find_map_keylengths (const std::unordered_map<std::string, Value>& map)
{
  LABSoft_GUI_Label::KeyLengths keylengths;

  for (const std::pair<std::string, Value>& pair : map)
  {
    const std::string& key = pair.first;
    unsigned length = key.length ();

    if (length < keylengths.shortest)
    {
      keylengths.shortest = length;
    }

    if (length > keylengths.longest)
    {
      keylengths.longest = length;
    }
  }

  return (keylengths);
}

std::string LABSoft_GUI_Label:: 
to_lowercase (const std::string& input)
{
  std::string lowercase_str = input;

  std::transform (
    lowercase_str.begin (),
    lowercase_str.end   (),
    lowercase_str.begin (),
    ::tolower
  );

  return (lowercase_str);
}

std::string LABSoft_GUI_Label::
remove_all_whitespaces (const std::string& str)
{
  std::string cln_str = str;

  cln_str.erase (std::remove_if (cln_str.begin (), cln_str.end (), ::isspace), str.end ());

  return (cln_str);
}

std::string LABSoft_GUI_Label:: 
find_unit_prefix_match (const std::string& str) const
{
  KeyLengths& kl          = m_keylengths_unit_prefix_to_exponent;
  std::string unit_prefix = "";

  // 1.
  for (int a = 0; (a < kl.longest) && (a < str.length ()); a++)
  {
    std::string substr = str.substr (0, a + 1);

    std::unordered_map<std::string, int>::iterator it =
      m_unit_prefix_to_exponent.find (substr);
    
    if (it != m_unit_prefix_to_exponent.end ())
    {
      unit_prefix = substr;
    }
  }

  return (unit_prefix);
}

LABSoft_GUI_Label::UNIT LABSoft_GUI_Label:: 
find_unit (const std::string& str) const
{
  std::string lower_str = to_lowercase (str);

  return (m_unit_string_lowercase_to_unit.at (lower_str));
}

LABSoft_GUI_Label::BASE LABSoft_GUI_Label:: 
calc_base (const std::string& unit_prefix) const
{
  return (unit_prefix.length () > 1 ?
    LABSoft_GUI_Label::BASE::TWO :
    LABSoft_GUI_Label::BASE::TEN
  );
}

int LABSoft_GUI_Label:: 
calc_exponent (const std::string& unit_prefix) const 
{
  return (m_unit_prefix_to_exponent.at (unit_prefix));
}

double LABSoft_GUI_Label:: 
calc_actual_value (double coefficient,
                   int    base,
                   double exponent)
{
  return (coefficient * std::pow (base, exponent));
}

void LABSoft_GUI_Label:: 
remove_from_string (const std::string& to_remove, std::string& str)
{
  unsigned pos = str.find (to_remove);

  if (pos != std::string::npos)
  {
    str.erase (pos, to_remove.length ());
  }
}

double LABSoft_GUI_Label:: 
calc_actual_value_using_reference (double value,
                                   double reference)
{
  if (reference == 0.0)
  {
    return (value);
  }
  else 
  {
    double  ref_coeff  = 0.0;
    int     ref_exp    = 0;

    // 1. Get the coefficient and exponent of the 
    //    reference value's scientific notation
    calc_coefficient_and_exponent (reference, ref_coeff, ref_exp);

    // 2. Using the reference's exponent, calculate the unit prefix
    std::string ref_unit_prefix = calc_unit_prefix (ref_exp);

    // 3. Using the reference's unit prefix, get the exponent
    int ref_pow_exp = calc_exponent_from_unit_prefix (ref_unit_prefix);

    double act_val = value * std::pow (static_cast<int>(m_base), ref_pow_exp);

    return (act_val);    
  }
}

void LABSoft_GUI_Label:: 
calc_coefficient_and_exponent (double   value, 
                               double&  coefficient, 
                               int&     exponent)
{
  // 1. Convert the value to a string representation of its scientific notation
  std::stringstream ss;
  ss << std::scientific << std::setprecision (3) << value;
  std::string str = ss.str ();

  // 2. Extract the coefficient and exponent from the string representation
  unsigned e_pos  = str.find_first_of ("Ee");
  coefficient     = std::stod (str.substr (0, e_pos));
  exponent        = std::stoi (str.substr (e_pos + 1));
}

std::string LABSoft_GUI_Label:: 
calc_unit_prefix (int exponent)
{
  std::string unit_prefix;

  switch (exponent)
  {
    case -9:
    case -8:
    case -7:
      unit_prefix = "n";
      break;

    case -6:
    case -5:
    case -4:
      unit_prefix = "u";
      break;

    case -3:
    case -2:
    case -1:
      unit_prefix = "m";
      break;
    
    case 0:
    case 1:
    case 2:
      unit_prefix = "";
      break;  

    case 3:
    case 4:
    case 5:
      unit_prefix = "k";
      break;

    case 6:
    case 7:
    case 8:
      unit_prefix = "M";
      break;
    
    case 9:
    case 10:
    case 11:
      unit_prefix = "G";
      break;
  }

  return (unit_prefix);
}

int LABSoft_GUI_Label:: 
calc_exponent_from_unit_prefix (const std::string& unit_prefix) const
{
  return (m_unit_prefix_to_exponent.at (unit_prefix));
}

int LABSoft_GUI_Label::
correct_mod (int exponent, 
             int modulo) const
{
  return (exponent < 0 ? ((exponent % modulo + modulo) % modulo) : (exponent %modulo));
}

// Setter
void LABSoft_GUI_Label:: 
unit (LABSoft_GUI_Label::UNIT unit)
{
  m_unit = unit;
}

// Getter
bool LABSoft_GUI_Label::
is_valid () const
{
  return (m_is_valid);
}

double LABSoft_GUI_Label:: 
actual_value () const
{
  return (m_actual_value);
}

std::string LABSoft_GUI_Label:: 
unit_prefix () const
{
  return (m_unit_prefix);
}

std::string LABSoft_GUI_Label:: 
unit_string () const
{
  return (m_unit_to_unit_string[m_unit]);
}

std::string LABSoft_GUI_Label::
to_label_text (unsigned precision) const
{
  std::stringstream ss;

  ss  <<  std::setprecision (precision) 
      << (m_coefficient * std::pow (10, correct_mod (m_exponent, 3)))
      <<  " "
      <<  m_unit_prefix
      <<  m_unit_to_unit_string[m_unit];

  return (ss.str ());
}

std::string LABSoft_GUI_Label::
to_label_text (UNIT     unit, 
               unsigned precision)
{
  m_unit = unit;

  return (to_label_text (precision));
}

void LABSoft_GUI_Label:: 
debug () const
{
  std::cout << "----------\n";
  std::cout << "raw string      : " << m_cleaned_string             << "\n";
  std::cout << "is valid        : " << m_is_valid               << "\n";
  std::cout << "actual value    : " << m_actual_value           << "\n";
  std::cout << "coefficient     : " << m_coefficient            << "\n";
  std::cout << "exponent        : " << m_exponent               << "\n";
  std::cout << "unit prefix     : " << m_unit_prefix            << "\n";
  std::cout << "label type      : " << static_cast<int>(m_unit) << "\n";
  std::cout << "reference value : " << m_reference              << "\n";
  std::cout << "----------\n\n";
}

LABSoft_GUI_Label::UNIT LABSoft_GUI_Label:: 
get_unit_from_unit_string (const char* string)
{
  std::string str = to_lowercase (std::string (string));

  std::unordered_map<std::string, LABSoft_GUI_Label::UNIT>::iterator it = 
    m_unit_string_lowercase_to_unit.find (str);

  if (it != m_unit_string_lowercase_to_unit.end ())
  {
    return (it->second);
  }
  else 
  {
    throw (std::invalid_argument ("Invalid unit."));
  }
}

// EOF