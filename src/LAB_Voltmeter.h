#ifndef LAB_VOLTMETER_H
#define LAB_VOLTMETER_H

#include "LAB_Core.h"

// forward declare LAB, #include "LAB.h" in Voltmeter cpp file
class LAB;

class LAB_Voltmeter
{
  private:
    LAB *m_LAB;
    LAB_Core *m_LAB_Core;

    bool m_is_running   = false;
    double data_sample  = 0.0;

  public:
    LAB_Voltmeter (LAB_Core *_LAB_Core, LAB *_LAB);

    void run ();
    void stop ();
    double get_data_sample ();

    bool is_running ()
    {
      return m_is_running;
    }
};

#endif
