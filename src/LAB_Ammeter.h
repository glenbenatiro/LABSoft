#ifndef LAB_AMMETER_H
#define LAB_AMMETER_H

#include "LAB_Core.h"
#include "LAB_Meter.h"

class LAB_Ammeter : public LAB_Meter 
{
  public:
    LAB_Ammeter (LAB_Core *_LAB_Core);
};

#endif