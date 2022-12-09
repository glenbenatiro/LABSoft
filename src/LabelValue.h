#ifndef LabelValue_H
#define LabelValue_H

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

    static const char* parse_for_label (int value, char unit_prefix);
    
    int     exponent    ();
    int     exponent    (char m_unit_prefix);

    char    unit_prefix ();
    char    unit_prefix (char unit_prefix);
    char    unit_prefix (int exponent);

    float   coefficient ();
    float   short_value ();
    
    double  actual_value                ();
};

#endif