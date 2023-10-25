#ifndef LABSOFT_GUI_LABEL
#define LABSOFT_GUI_LABEL

#include <unordered_map>
#include <string>

class LABSoft_GUI_Label
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
      NONE,
      ANY
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
    static std::unordered_map<UNIT, std::string>  m_unit_to_unit_string;
    static std::unordered_map<std::string, UNIT>  m_unit_string_to_unit;
    static std::unordered_map<std::string, UNIT>  m_unit_string_lowercase_to_unit;
    static std::unordered_map<std::string, int>   m_unit_prefix_to_exponent;
    static KeyLengths                             m_keylengths_unit_prefix_to_exponent;

    bool        m_is_valid        = false;
    double      m_reference       = 0.0;
    std::string m_cleaned_string  = "0";      
    UNIT        m_unit            = UNIT::NONE;
    double      m_actual_value    = 0.0;
    double      m_coefficient     = 0.0;
    int         m_exponent        = 0;
    std::string m_unit_prefix     = "";
    BASE        m_base            = BASE::TEN;

  private:
    template <typename Key, typename Value>
    static std::unordered_map<Value, Key> 
    invert_map (const std::unordered_map<Key, Value>& map);

    template <typename Value>
    static std::unordered_map<std::string, Value> 
    lowercase_map (const std::unordered_map<std::string, Value>& map);

    template <typename Value>
    static KeyLengths 
    find_map_keylengths (const std::unordered_map<std::string, Value>& map);

    static std::string to_lowercase (const std::string& input);

    std::string  remove_all_whitespaces             (std::string&& str);
    bool          parse_input                       (const std::string& str);
    bool          parse_input_string                (const std::string& str);
    void          find_unit_prefix_match            (const std::string& str);
    void          remove_from_string                (const std::string& to_remove, std::string& str);
    bool          parse_input_double                (double value);
    std::string   calc_unit_prefix                  (int exponent);
    double        calc_actual_value_using_reference (double value, double reference);
    void          calc_coefficient_and_exponent     (double value, double& coefficient, int& exponent);
    int           calc_exponent_from_unit_prefix    (const std::string& unit_prefix) const;
    int           correct_mod                       (int exponent, int modulo) const;

  public:
    LABSoft_GUI_Label (double value, UNIT unit = UNIT::NONE);
    LABSoft_GUI_Label (const char* input, UNIT none = UNIT::NONE);
    LABSoft_GUI_Label (const char* input, double reference, UNIT unit = UNIT::NONE);
    LABSoft_GUI_Label (const char* input);

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