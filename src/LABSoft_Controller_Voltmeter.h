#ifndef LABSOFT_CONTROLLER_VOLTMETER_H
#define LABSOFT_CONTROLLER_VOLTMETER_H

#include <FL/Fl_Output.H>

#include "LAB.h"
#include "LAB_Meter.h"
#include "LABSoft_GUI.h"
#include "LABSoft_Controller_Meter.h"

class LABSoft_Controller_Voltmeter : public LABSoft_Controller_Meter
{
  public:
    LABSoft_Controller_Voltmeter (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI, LAB_Meter *_LAB_Meter, Fl_Output *_Fl_Output);
};

#endif