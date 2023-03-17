#ifndef LABELVALUE_H
#define LABELVALUE_H

#include <string>

#include "Defaults.h"

class LabelValue
{
  private:
    double      m_actual_value  = 0.0;
    double      m_coefficient   = 0.0; 
    int         m_exponent      = 0;  
    char        m_unit_prefix   = ' ';
    std::string m_unit          = "V";

  public:
    LabelValue (const char *label);
    LabelValue (double value, LE_UNIT unit = LE_UNIT_NONE);

    void        exp_notation      (double actual_value, double *coefficient, int *exponent);
    double      get_actual_value  (double coefficient, int exponent);
    int         get_exponent      (char unit_prefix);
    char        get_unit_prefix   (std::string str);
    char        get_unit_prefix   (int exponent);
    std::string get_unit          (std::string str);
    std::string get_unit          (LE_UNIT le_unit);
    std::string to_label_text ();

    //

    double actual_value ()
    {
      return (m_actual_value);
    }

    double coefficient ()
    {
      return (m_coefficient);
    }

    int exponent ()
    {
      return (m_exponent);
    }

    char unit_prefix ()
    {
      return (m_unit_prefix);
    }   
};

#endif