#include "LABSoft_Controller.h"

#include <FL/Fl.H>

LABSoft_Controller:: 
LABSoft_Controller (LAB* _LAB, LABSoft_GUI* _LABSoft_GUI)
  : m_LAB                 (_LAB),
    m_LABSoft_GUI         (_LABSoft_GUI),
    m_Oscilloscope        (_LAB, _LABSoft_GUI),
    m_Voltmeter           (_LAB, _LABSoft_GUI),
    m_Function_Generator  (_LAB, _LABSoft_GUI),
    m_Logic_Analyzer      (_LAB, _LABSoft_GUI),
    m_Circuit_Checker     (_LAB, _LABSoft_GUI)   
{
  Fl::add_timeout (
    LABC::LABSOFT::DISPLAY_UPDATE_RATE,
    update_display,
    this
  );
}

void LABSoft_Controller::
update_display (void *data)
{
  LABSoft_Controller& controller = *((LABSoft_Controller*)(data));

  // pre = std::chrono::steady_clock::now ();
  // std::chrono::duration<double, std::micro> wait_dur = pre - post;

  controller.m_Oscilloscope.display_update_cycle    ();
  controller.m_Voltmeter.display_update_cycle       (); 
  controller.m_Logic_Analyzer.display_update_cycle  ();
  
  // post = std::chrono::steady_clock::now ();
  // // std::chrono::duration<double, std::micro> diff = post - pre;
  // // std::cout << diff.count () 
  // //           << "/" 
  // //           << wait_dur.count () 
  // //           << " us" 
  // //           << std::endl;

  Fl::repeat_timeout (LABC::LABSOFT::DISPLAY_UPDATE_RATE, update_display, data);  
}