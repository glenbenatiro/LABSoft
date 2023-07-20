#ifndef LABSOFT_CONTROLLER_VOLTMETER_H
#define LABSOFT_CONTROLLER_VOLTMETER_H

#include <FL/Fl_Light_Button.H>

#include "../LAB/LAB.h"
#include "../LABSoft_GUI/LABSoft_GUI.h"

// forward declare LABSoft_Controller, #include in .cpp file
class LABSoft_Controller;

class LABSoft_Controller_Voltmeter 
{
  private:
    LAB*                m_LAB;
    LABSoft_GUI*        m_LABSoft_GUI;
    LABSoft_Controller* m_LABSoft_Controller;

  private:
    void init_gui_values ();
    
  public:
    LABSoft_Controller_Voltmeter (LAB*                _LAB, 
                                  LABSoft_GUI*        _LABSoft_GUI,
                                  LABSoft_Controller* _LABSoft_Controller);

    void cb_run_stop          (Fl_Light_Button *w, void *data);
    void display_update_cycle ();
    void update_gui_main      (bool value);
};

#endif