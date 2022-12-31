#ifndef LABSOFT_CONTROLLER_OHMMETER
#define LABSOFT_CONTROLLER_OHMMETER

#include <thread>

#include <FL/Fl_Choice.H>
#include <FL/Fl_Light_Button.H>

#include "LAB.h"
#include "LABSoft_GUI.h"


class LABSoft_Controller_Ohmmeter 
{
  private:
    LAB *m_LAB;
    LABSoft_GUI *m_LABSoft_GUI;

    std::thread *m_thread_run_stop,
                *m_thread_random_value_generator;

  public:
    // functions
          LABSoft_Controller_Ohmmeter (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI);
    void  update_loop ();

    // callback functions
    void  cb_unit                    (Fl_Choice *w, void *data);
    void  cb_run_stop                (Fl_Light_Button *w, void *data);
    void  cb_generate_random_values  (Fl_Light_Button *w, void *data);
};

#endif