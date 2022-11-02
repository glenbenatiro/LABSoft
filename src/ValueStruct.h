#ifndef VALUESTRUCT_H
#define VALUESTRUCT_H

class ValueStruct
{
  public:
    char m_unit_prefix;
    int m_exponent;
    int m_short_value;

            ValueStruct                     (const char *label);
    char    get_unit_prefix                 (char unit_prefix);
    int     get_unit_prefix_power_exponent  ();
    double  get_actual_value                ();
    
    static const char* parse_for_label (int value, char unit_prefix);
};

#endif