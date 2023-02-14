#ifndef LAB_VOLTMETER_H
#define LAB_VOLTMETER_H

#include "LAB_Core.h"

class LAB_Voltmeter
{
  private:
    LAB_Core *m_LAB_Core;

    bool m_is_running = false;

    double data_sample;

  public:
    LAB_Voltmeter (LAB_Core *_LAB_Core);

    void run ();
    void stop ();
    double get_data_sample ();

    bool is_running ()
    {
      return m_is_running;
    }
};

#endif
