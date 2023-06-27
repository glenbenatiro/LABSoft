#ifndef LABCHECKER_CONTROLLER_H
#define LABCHECKER_CONTROLLER_H

#include "LABChecker_Main.h"
#include "LABChecker_GUI.h"

#include <FL/Fl_Choice.H>
#include <FL/Fl_Input.H>

class LABChecker_Controller 
{
  private:
    LABChecker_Main*  m_LABChecker_Main;
    LABChecker_GUI*   m_LABChecker_GUI;

  private:
    void init_gui ();
    void update_gui_digital_output_count (unsigned value);

    static void update_display (void* data);
    
  public:
    LABChecker_Controller (LABChecker_Main* main, LABChecker_GUI* gui);

    void cb_digital_input_bits    (Fl_Choice* w, void* data);
    void cb_digital_output_bits   (Fl_Choice* w, void* data);
    void cb_digital_output_count  (Fl_Input*  w, void* data);
};

#endif