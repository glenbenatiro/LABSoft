#ifndef LAB_VOLTMETER_H
#define LAB_VOLTMETER_H

#include "LAB_Core.h"

class LAB_Voltmeter
{
  private:
    LAB_Core *m_LAB_Core;

  public:
    LAB_Voltmeter (LAB_Core *_LAB_Core);
};

#endif
