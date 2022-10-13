
#ifndef LAB_METER
#define LAB_METER

#include "LAB_Core.h"

class LAB_Meter : public LAB_Core
{
  public:
    bool  m_is_running = false,
          m_is_random_value_generator_running = false;

    float m_value;

  
    // functions  
          LAB_Meter ();
    void  random_value_generator ();

    // getters
    bool  is_running ();
    bool  is_random_value_generator_running ();
    float value ();

    // setters
    void  is_running (bool value);
    void  is_random_value_generator_running (bool value);
    void  value (float value);
};

#endif
