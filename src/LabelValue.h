#ifndef LabelValue_H
#define LabelValue_H

#include <string>

enum LABEL_TYPE
{
  VOLTS_PER_DIVISION = 0,
  TIME_PER_DIVISION  = 1
};

class LabelValue
{
  private:
    int     m_exponent;
    float   m_coefficient;
    
    char    m_unit_prefix;

    double  m_actual_value;

  public:
    LabelValue (const char *label);
    LabelValue (double value);
    
    int     exponent    ();
    int     exponent    (char m_unit_prefix);

    char    unit_prefix ();
    char    unit_prefix (char unit_prefix);
    char    unit_prefix (int exponent);

    float   coefficient ();
    float   short_value ();
    
    double  actual_value                ();

    static std::string label_text (double value, LABEL_TYPE _LABEL_TYPE);
};

#endif