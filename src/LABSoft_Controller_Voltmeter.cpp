#include "LABSoft_Controller_Voltmeter.h"

LABSoft_Controller_Voltmeter:: 
LABSoft_Controller_Voltmeter (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI)
{
  m_LAB         = _LAB;
  m_LABSoft_GUI = _LABSoft_GUI;
}

void LABSoft_Controller_Voltmeter:: 
cb_run_stop  (Fl_Light_Button* w, 
              void*            data)
{
  if (w->value () == 0)
  {
    m_LAB->m_Voltmeter.stop ();
  }
  else 
  {
    m_LAB->m_Voltmeter.run ();

    // 
    m_LABSoft_GUI->oscilloscope_fl_light_button_run_stop->clear ();
  }
}

void LABSoft_Controller_Voltmeter:: 
display_update_cycle ()
{
  LAB& lab          = *m_LAB;
  LABSoft_GUI &gui  = *m_LABSoft_GUI;

  if (lab.m_Voltmeter.is_running ())
  {
    lab.m_Voltmeter.load_data_samples ();

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

    LabelValue lv0 (lab.m_Voltmeter.m_samples[0], LabelValue::TYPE::VOLTS);
    gui.voltmeter_fl_output_chan0_value->value (lv0.to_label_text ().c_str ());
    
    LabelValue lv1 (lab.m_Voltmeter.m_samples[1], LabelValue::TYPE::VOLTS);
    gui.voltmeter_fl_output_chan1_value->value (lv1.to_label_text ().c_str ());
  }
}


// EOF