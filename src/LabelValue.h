#ifndef LABELVALUE_H
#define LABELVALUE_H

#include <map>
#include <string>
#include <vector>

class LabelValue
{
  public:
    enum class UNIT
    {
      VOLT,
      VOLT_PER_DIVISION,
      SECOND,
      SECOND_PER_DIVISION,
      HERTZ,
      DEGREE,
      NONE
    };

    enum class BASE
    {
      TWO = 2,
      TEN = 10
    };
  
  private:
    struct KeyLengths
    {
      unsigned shortest = 0;
      unsigned longest = 0;
    };

  private:
    static std::map<UNIT, std::string>  m_unit_to_unit_string;
    static std::map<std::string, UNIT>  m_unit_string_to_unit;
    static std::map<std::string, UNIT>  m_unit_string_lowercase_to_unit;
    static std::map<std::string, int>   m_unit_prefix_to_exponent;
    static KeyLengths                   m_keylengths_unit_prefix_to_exponent;

    bool        m_is_valid      = false;
    double      m_reference     = 0.0;
    std::string m_raw_string    = "0";      
    UNIT        m_unit          = UNIT::NONE;
    double      m_actual_value  = 0.0;
    double      m_coefficient   = 0.0;
    int         m_exponent      = 0;
    std::string m_unit_prefix   = "";
    BASE        m_base          = BASE::TEN;

  private:
    template <typename Key, typename Value>
    static std::map<Value, Key> 
    invert_map    (const std::map<Key, Value>& map);

    template <typename Value>
    static std::map<std::string, Value> 
    lowercase_map (const std::map<std::string, Value>& map);

    template <typename Value>
    static KeyLengths 
    find_map_keylengths (const std::map<std::string, Value>& map);

    static std::string        to_lowercase            (const std::string& input);
    static std::vector<UNIT>  find_per_division_units (const std::map<UNIT, std::string>& map);

    void        remove_whitespaces                (std::string& str);
    bool        parse_input                       (const std::string& str);
    bool        parse_input_string                (const std::string& str);
    void        find_unit_prefix_match            (const std::string& str);
    void        remove_from_string                (const std::string& to_remove, std::string& str);
    bool        parse_input_double                (double value);
    std::string calc_unit_prefix                  (int exponent);
    double      calc_actual_value_using_reference (double value, double reference);
    void        calc_coefficient_and_exponent     (double value, double& coefficient, int& exponent);
    int         calc_exponent_from_unit_prefix    (const std::string& unit_prefix) const;
    int         correct_mod                       (int exponent, int modulo) const;

  public:
    LabelValue (double value, UNIT unit = UNIT::NONE);
    LabelValue (const char* input, UNIT none = UNIT::NONE);
    LabelValue (const char* input, double reference, UNIT unit = UNIT::NONE);

    // Setter
    void unit (UNIT unit);

    // Getter
    bool        is_valid      () const;
    double      actual_value  () const;
    std::string unit_prefix   () const;
    std::string unit_string   () const;
    std::string to_label_text (unsigned precision = 3) const;
    std::string to_label_text (UNIT unit, unsigned precision = 3);

    //
    void        debug         () const;
    static UNIT get_unit_from_unit_string (const char* string);
    
};

#endif