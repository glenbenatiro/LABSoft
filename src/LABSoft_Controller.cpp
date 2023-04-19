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
    LABSOFT_GENERAL::DISPLAY_UPDATE_RATE,
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
  controller.m_Logic_Analyzer.display_update_cycle  ();

  // if (_LAB->m_Voltmeter.is_running ())
  // {
  //   _LAB->m_Voltmeter.load_data_samples ();

  //   // Use this code soon if you plan to add 3 or more channels.
  //   // This allows us to update the Fl_Outputs in a loop.
  //   {
  //     /*
  //       Fl_Group* group       = gui->voltmeter_fl_group_values;
  //       int children          = group->children ();
  //       Fl_Widget* const* arr = group->array ();

  //       for (int a = 0; a < children; a++)
  //       {
  //         Fl_Output*  output  = static_cast<Fl_Output*>(arr[a]);
  //         int         chan    = (int)(output->user_data ());
        
  //         std::string value = std::to_string (_LAB->m_Voltmeter.m_samples[chan]);

  //         output->value (value.c_str ());
  //       }
  //     */ 
  //   }

  //   LabelValue lv0 (_LAB->m_Voltmeter.m_samples[0], LABELVALUE_TYPE::VOLTS);
  //   gui->voltmeter_fl_output_chan0_value->value (lv0.to_label_text ().c_str ());
    
  //   LabelValue lv1 (_LAB->m_Voltmeter.m_samples[1], LABELVALUE_TYPE::VOLTS);
  //   gui->voltmeter_fl_output_chan1_value->value (lv1.to_label_text ().c_str ());
  // }
  
  // post = std::chrono::steady_clock::now ();
  // // std::chrono::duration<double, std::micro> diff = post - pre;
  // // std::cout << diff.count () 
  // //           << "/" 
  // //           << wait_dur.count () 
  // //           << " us" 
  // //           << std::endl;

  Fl::repeat_timeout (LABSOFT_GENERAL::DISPLAY_UPDATE_RATE, update_display, data);  
}