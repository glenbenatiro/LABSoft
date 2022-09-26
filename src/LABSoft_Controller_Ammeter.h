#ifndef LABSOFT_CONTROLLER_AMMETER_H
#define LABSOFT_CONTROLLER_AMMETER_H

#include <FL/Fl_Output.H>

#include "LAB.h"
#include "LAB_Meter.h"
#include "LABSoft_GUI.h"
#include "LABSoft_Controller_Meter.h"

class LABSoft_Controller_Ammeter : public LABSoft_Controller_Meter
{
  public:
    // variables

    // functions
    LABSoft_Controller_Ammeter (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI, LAB_Meter *_LAB_Meter, Fl_Output *_Fl_Output);
};

#endif