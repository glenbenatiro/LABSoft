#include "LABSoft_Controller.h"

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
  pre = std::chrono::steady_clock::now ();
  std::chrono::duration<double, std::micro> wait_dur = pre - post;
  
  
  // do stuff
  LAB *_LAB        = (static_cast<LAB_PACK *>(data))->_LAB;
  LABSoft_GUI *gui = (static_cast<LAB_PACK *>(data))->_LABSoft_GUI;

  if (_LAB->m_Oscilloscope.is_osc_frontend_running ())
  {
    _LAB->m_Oscilloscope.load_data_samples ();

    gui->oscilloscope_labsoft_oscilloscope_display_group_display->
    fill_pixel_points ();

    gui->oscilloscope_labsoft_oscilloscope_display_group_display->redraw ();
  }

  if (_LAB->m_Voltmeter.is_running ())
  {
    _LAB->m_Voltmeter.load_data_samples ();

    // Use this code soon if you plan to add 3 or more channels.
    // This allows us to update the Fl_Outputs in a loop.
    {
      /*
        Fl_Group* group       = gui->voltmeter_fl_group_values;
        int children          = group->children ();
        Fl_Widget* const* arr = group->array ();

        for (int a = 0; a < children; a++)
        {
          Fl_Output*  output  = static_cast<Fl_Output*>(arr[a]);
          int         chan    = (int)(output->user_data ());
        
          std::string value = std::to_string (_LAB->m_Voltmeter.m_samples[chan]);

          output->value (value.c_str ());
        }
      */ 
    }

    LabelValue lv0 (_LAB->m_Voltmeter.m_samples[0], LABELVALUE_TYPE::VOLTS);
    gui->voltmeter_fl_output_chan0_value->value (lv0.to_label_text ().c_str ());
    
    LabelValue lv1 (_LAB->m_Voltmeter.m_samples[1], LABELVALUE_TYPE::VOLTS);
    gui->voltmeter_fl_output_chan1_value->value (lv1.to_label_text ().c_str ());
  }

  if (_LAB->m_Logic_Analyzer.is_running ())
  {   
    _LAB->m_Logic_Analyzer.load_data_samples ();

    gui->logic_analyzer_labsoft_logic_analyzer_display_group_display->
      fill_pixel_points ();

    gui->logic_analyzer_labsoft_logic_analyzer_display_group_display->redraw ();
  }

  Fl::awake ();
  
  post = std::chrono::steady_clock::now ();
  // std::chrono::duration<double, std::micro> diff = post - pre;
  // std::cout << diff.count () 
  //           << "/" 
  //           << wait_dur.count () 
  //           << " us" 
  //           << std::endl;

  Fl::repeat_timeout (DISPLAY_UPDATE_RATE, update_display, data);  
}