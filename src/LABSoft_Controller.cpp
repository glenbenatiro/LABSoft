#include "LABSoft_Controller.h"

#include <iostream>
#include <cstdio>

#include <FL/Fl.H>

// ----------
// Weird out of class declaration to initialize the value of static prev and curr
std::chrono::time_point<std::chrono::steady_clock> LABSoft_Controller::post = 
  std::chrono::steady_clock::now ();

std::chrono::time_point<std::chrono::steady_clock> LABSoft_Controller::pre =
  std::chrono::steady_clock::now ();
// ----------

LABSoft_Controller:: 
LABSoft_Controller (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI)
  : m_LAB                 (_LAB),
    m_LABSoft_GUI         (_LABSoft_GUI),
    m_Oscilloscope        (_LAB, _LABSoft_GUI),
    m_Voltmeter           (_LAB, _LABSoft_GUI),
    m_Function_Generator  (_LAB, _LABSoft_GUI),
    m_Logic_Analyzer      (_LAB, _LABSoft_GUI),
    m_Circuit_Checker     (_LAB, _LABSoft_GUI)   
{
  _LAB_PACK = {_LAB, _LABSoft_GUI};

  Fl::add_timeout (DISPLAY_UPDATE_RATE, update_display, (void*)(&_LAB_PACK));
}

void LABSoft_Controller::
update_display (void *data)
{
  // calculate wait duration
  //post = std::chrono::steady_clock::now ();
  //std::chrono::duration<double, std::milli> diff = post - pre;
  //printf ("Duration: %6.3f ms\n", diff.count ());

  // do stuff
  LAB *lab          = (static_cast<LAB_PACK *>(data))->_LAB;
  LABSoft_GUI *gui  = (static_cast<LAB_PACK *>(data))->_LABSoft_GUI;

  if (lab->m_Oscilloscope->is_master_running ())
  {
    //printf ("osc running\n");

    lab->m_Oscilloscope->load_data_samples ();

    gui->oscilloscope_labsoft_oscilloscope_display_group_display->
     load_channel_signals (&(lab->m_Oscilloscope->m_channel_signals));

    // draw signals
    gui->oscilloscope_labsoft_oscilloscope_display_group_display->display ()->redraw ();
    Fl::awake ();
  }

  if (lab->m_Voltmeter.is_running ())
  {
    double reading = lab->m_Voltmeter.get_data_sample ();

    char label[15];
    sprintf (label, "a");

    gui->voltmeter_fl_output_value->label (label);
  }
  
  // store time before timeout
  //pre = std::chrono::steady_clock::now ();

  // loop call timeout
  Fl::repeat_timeout (DISPLAY_UPDATE_RATE, update_display, data);  
}
  

// void LABSoft_Controller_Main:: 
// LABSoft_Controller_Main_cb_update_display (void *data)
// {
//   // sure??
//   LABSoft_GUI *gui = static_cast<LABSoft_GUI *>(data);
//   gui->main_fl_window->flush ();

//   printf ("hello p\n");

//   Fl::repeat_timeout (DISPLAY_UPDATE_RATE, LABSoft_Controller_Main_cb_update_display, (void *)gui);
// }