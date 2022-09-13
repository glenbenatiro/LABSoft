#ifndef LAB_OHMMETER_H
#define LAB_OHMMETER_H

#include "LAB_Core.h"
#include "LAB_Meter.h"

class LAB_Ohmmeter : public LAB_Meter 
{
  public:
    LAB_Ohmmeter (LAB_Core *_LAB_Core);
};

#endif