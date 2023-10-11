#ifndef LAB_VOLTMETER_H
#define LAB_VOLTMETER_H

#include "LAB_Module.h"

class LAB_Voltmeter : public LAB_Module
{
  private:
    bool m_is_running = false;

  public:
    LAB_Voltmeter (LAB& _LAB);

    bool    is_running    ();
    void    run           ();
    void    stop          ();
    double  read_voltage  (unsigned chan);
};

#endif
