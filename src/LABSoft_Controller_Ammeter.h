#ifndef LABSOFT_CONTROLLER_AMMETER
#define LABSOFT_CONTROLLER_AMMETER

#include <thread>

#include <FL/Fl_Choice.H>
#include <FL/Fl_Light_Button.H>

#include "LAB.h"
#include "LABSoft_GUI.h"
#include "Auxiliary.h"

class LABSoft_Controller_Ammeter 
{
  private:
    LAB *m_LAB;
    LABSoft_GUI *m_LABSoft_GUI;

    std::thread *m_thread_run_stop,
                *m_thread_random_value_generator;

  public:
    // functions
          LABSoft_Controller_Ammeter (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI);
    void  update_loop ();

    // callback functions
    void  cb_unit                    (Fl_Choice *w, void *data);
    void  cb_run_stop                (Fl_Light_Button *w, void *data);
    void  cb_generate_random_values  (Fl_Light_Button *w, void *data);
};

#endif