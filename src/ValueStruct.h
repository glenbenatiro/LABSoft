#ifndef VALUESTRUCT_H
#define VALUESTRUCT_H

class ValueStruct
{
  private:
    int     m_exponent;
    float   m_coefficient;
    
    char    m_unit_prefix;

    double  m_actual_value;

  public:
    ValueStruct (const char *label);
    ValueStruct (double value);

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