#ifndef LAB_VOLTMETER_H
#define LAB_VOLTMETER_H

#include "LAB_Core.h"
#include "LAB_Meter.h"

class LAB_Voltmeter : public LAB_Meter 
{
  public:
    LAB_Voltmeter (LAB_Core *_LAB_Core);
};

#endif
