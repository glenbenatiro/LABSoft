#ifndef LABSOFT_CONTROLLER_Voltmeter
#define LABSOFT_CONTROLLER_Voltmeter

#include <thread>

#include <FL/Fl_Choice.H>
#include <FL/Fl_Light_Button.H>

#include "LAB.h"
#include "LABSoft_GUI.h"

//

constexpr double LABSOFT_CONTROLLER_Voltmeter_UPDATE_RATE = 1; // Hz
constexpr double LABSOFT_CONTROLLER_Voltmeter_UPDATE_RATE_MS = (1.0 / LABSOFT_CONTROLLER_Voltmeter_UPDATE_RATE) * 1000;

//

class LABSoft_Controller_Voltmeter 
{
  private:
    LAB *m_LAB;
    LABSoft_GUI *m_LABSoft_GUI;
    std::thread m_update, m_random_value_generator;
    
  public:
    LABSoft_Controller_Voltmeter (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI);

    // 
    void update ();

    // callback functions
    void cb_unit      (Fl_Choice *w, void *data);
    void cb_mode      (Fl_Choice *w, void *data);
    void cb_run_stop  (Fl_Light_Button *w, void *data);
    void cb_generate_random_values (Fl_Light_Button *w, void *data);
};

#endif