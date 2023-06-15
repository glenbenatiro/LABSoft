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
      NONE,
      DIV,
      VOLTS,
      VOLTS_PER_DIVISION,
      SECONDS,
      SECONDS_PER_DIVISION,
      HERTZ,
      DEGREES
    };
  
  private:
    static std::map<std::string, int>   m_map_unit_prefix_to_exponent;
    static std::map<std::string, UNIT>  m_map_unit_string_to_unit;
    static std::map<std::string, UNIT>  m_map_unit_string_to_unit_lowercase;
    static std::map<UNIT, std::string>  m_map_unit_to_unit_string;

    struct Min_Max_Map_Key_length
    {
      unsigned shortest = 0;
      unsigned longest = 0;
    };
    
  private:
    static Min_Max_Map_Key_length m_min_max_unit_prefix_to_exponent;
    static Min_Max_Map_Key_length m_min_max_unit_string_to_unit;
    static unsigned               m_largest_exponent;  
    std::vector<std::string>      m_unit_string_matches;    

    int         m_exponent            = 0;
    bool        m_is_valid_label_text = false;
    UNIT        m_unit                = UNIT::NONE;
    double      m_coefficient         = 0.0;
    double      m_reference           = 0.0;
    double      m_actual_value        = 0.0;
    std::string m_raw_string          = "0";
    std::string m_unit_prefix         = "";

  private:
    int         correct_mod                       (int exponent, int modulo) const;
    bool        parse_input_if_valid              (std::string& str);
    bool        parse_string_if_valid             (std::string& str);
    bool        parse_double                      (double value);
    double      calc_actual_value_using_reference (double value, double reference);
    void        calc_sci_coefficient_and_exponent (double value, double& coefficient, int& exponent);
    std::string calc_unit_prefix                  (int exponent) const;
    int         calc_sci_exponent                 (std::string& unit_prefix) const;
    void        debug                             ();
    void        removeWhitespaces                 (std::string& str);

    static std::string  str_to_lower_case           (std::string& str);

    template <typename ValueType>
    static void map_str_keys_to_lower_case  
    (std::map<std::string, ValueType>& src_map, std::map<std::string, ValueType>& dest_map);

    template <typename KeyType, typename ValueType>
    static std::map<KeyType, ValueType> reverse_map 
    (const std::map<KeyType, ValueType>& map);
     
    

    template <typename ValueType>
    unsigned check_for_match 
    (const std::string& str, std::map<std::string, ValueType>& map, unsigned max_str_length);
    
    template <typename KeyType>
    static unsigned calc_largest_exponent 
    (const std::map<KeyType, int>& map);

    template <typename ValueType>
    void find_matches_reverse 
    (const std::string& str, std::map<std::string, ValueType>& map, 
    Min_Max_Map_Key_length key_length, std::vector<std::string>& unit_vector);

    // Run once functions (for caching)
    template <typename ValueType>
    static Min_Max_Map_Key_length find_min_map_key_length 
    (const std::map<std::string, ValueType>& map);
    
  public: 
    LabelValue ();
    LabelValue (double value, UNIT unit = UNIT::NONE);
    LabelValue (const char* label, double reference = 0.0, UNIT unit = UNIT::NONE);
    LabelValue (const char* label, UNIT unit = UNIT::NONE, double reference = 0.0);

    std::string to_label_text       (unsigned precision = 3);
    std::string to_label_text       (UNIT unit, unsigned precision = 3);

    void        label_for           (UNIT parse_output_as);

    int         exponent            ();
    bool        is_valid_label_text ();
    UNIT        unit                ();
    double      coefficient         ();
    double      reference_value     ();
    double      actual_value        ();
    std::string raw_string          ();
    std::string unit_string         ();
    std::string unit_prefix         ();    
};

#endif