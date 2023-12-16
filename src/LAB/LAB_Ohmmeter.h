#ifndef LAB_OHMMETER_H
#define LAB_OHMMETER_H

#include "LAB_Module.h"

class LAB_Ohmmeter : public LAB_Module 
{
  private:
    bool m_is_running = false;

  public:
    LAB_Ohmmeter (LAB& LAB);

    bool is_running () const;
};

#endif