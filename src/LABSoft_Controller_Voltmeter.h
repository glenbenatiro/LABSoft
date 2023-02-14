#ifndef LABSOFT_CONTROLLER_Voltmeter
#define LABSOFT_CONTROLLER_Voltmeter

#include <FL/Fl_Choice.H>
#include <FL/Fl_Light_Button.H>

#include "LAB.h"
#include "LABSoft_GUI.h"

class LABSoft_Controller_Voltmeter 
{
  private:
    LAB         *m_LAB;
    LABSoft_GUI *m_LABSoft_GUI;
    
  public:
    LABSoft_Controller_Voltmeter (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI);

    // callback functions
    void cb_unit      (Fl_Choice *w, void *data);
    void cb_run_stop  (Fl_Light_Button *w, void *data);
};

#endif