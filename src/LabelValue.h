#ifndef LABELVALUE_H
#define LABELVALUE_H

#include <string>
#include <map>

enum class LABELVALUE_TYPE
{
  NONE,
  VOLTS_PER_DIVISION,
  VOLTS,
  SECONDS,
  HERTZ
};

class LabelValue
{
  private:
    double            m_actual_value        = 0.0;
    double            m_coefficient         = 0.0;
    int               m_exponent            = 0;
    std::string       m_unit_prefix         = " ";
    LABELVALUE_TYPE   m_label_type          = LABELVALUE_TYPE::NONE;
    bool              m_is_valid_label_text = false;
    double            m_reference_value     = 0.0;

    std::map<int, std::string> m_exp_to_si_prefix = 
    {
        {-9, "n"}, 
        {-8, "n"},
        {-7, "n"},
        {-6, "u"},
        {-5, "u"},
        {-4, "u"},
        {-3, "m"},
        {-2, "m"},
        {-1, "m"},
        { 0, " "}, 
        { 1, " "},
        { 2, " "}, 
        { 3, "k"},
        { 4, "k"},
        { 5, "k"},   
        { 6, "M"},  
        { 7, "M"},
        { 8, "M"},
        { 9, "G"},
        {10, "G"},
        {11, "G"},
    };

    std::map<std::string, int> m_si_prefix_to_exp = 
    {
        {"n", -9},  // nano
        {"u", -6},  // micro
        {"m", -3},  // milli
        {" ", 0},   // (none)
        {"k", 3},   // kilo
        {"M", 6},   // mega
        {"G", 9}    // giga
    };

    std::map<LABELVALUE_TYPE, std::string> labelvalue_for_string_format =
    {
      {LABELVALUE_TYPE::NONE,                ""},
      {LABELVALUE_TYPE::VOLTS_PER_DIVISION,  "V/div"},
      {LABELVALUE_TYPE::VOLTS,               "V"},
      {LABELVALUE_TYPE::SECONDS,             "s"},
      {LABELVALUE_TYPE::HERTZ,               "Hz"},
    };

    bool        parse_widget_input_if_valid       (const std::string& str);
    bool        parse_double_if_valid             (double value);
    bool        parse_string_if_valid             (const std::string& str);
    double      calc_actual_value_using_reference (double actual_value, double reference);
    double      calc_actual_value                 (double coefficient, int exponent);
    int         calc_sci_exponent                 (std::string unit_prefix);
    void        calc_sci_coefficient_and_exponent (double value, double& coefficient, int& exponent);
    std::string calc_unit_prefix                  (int exponent);

    // inline getters
    int correct_mod (int exponent, int modulo)
    {
      int ret = exponent < 0 ? ((exponent % modulo + modulo) % modulo) :
        (exponent % modulo);

      return (ret);
    }
  
  public: 
    LabelValue ();
    
    LabelValue (
      double          value, 
      LABELVALUE_TYPE parse_input_as = LABELVALUE_TYPE::NONE
    );

    LabelValue (
      const char      *label, 
      double           reference,
      LABELVALUE_TYPE  parse_input_as = LABELVALUE_TYPE::NONE
    );

    LabelValue (
      const char      *label,
      LABELVALUE_TYPE  parse_input_as = LABELVALUE_TYPE::NONE
    );

    std::string to_label_text ();
    std::string to_label_text (LABELVALUE_TYPE _LABELVALUE_TYPE);
    
    // Getters
    double      actual_value        ();
    double      coefficient         ();
    int         exponent            ();
    char        unit_prefix         ();
    std::string label_for           ();
    bool        is_valid_label_text ();
    std::string short_value         ();

    // Setters
    void        label_for           (LABELVALUE_TYPE parse_output_as);
};

#endif