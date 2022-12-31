#ifndef LABSOFT_CONTROLLER_MULTIMETER
#define LABSOFT_CONTROLLER_MULTIMETER

#include <FL/Fl_Choice.H>
#include <FL/Fl_Light_Button.H>

class LABSoft_Controller_Multimeter 
{
  private:
  
  public:
    LABSoft_Controller_Multimeter (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI);

    // callback functions
    void cb_unit      (Fl_Choice *w, void *data);
    void cb_mode      (Fl_Choice *w, void *data);
    void cb_run_stop  (Fl_Light_Button *w, void *data);
    void cb_generate_random_values (Fl_Light_Button *w, void *data);
};

#endif