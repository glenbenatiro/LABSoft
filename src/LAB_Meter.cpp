#include "LAB_Meter.h"

LAB_Meter:: 
LAB_Meter () : LAB_Core ()
{
  // initialize defaults
  m_value       = LAB_DEFAULTS_METER_VALUE;
  m_unit_scaler = LAB_DEFAULTS_METER_UNIT_SCALER;
}

void LAB_Meter:: 
LAB_Meter_update_value (Fl_Output *w)
{
  while (m_flag_is_meter_running)
    {
      char s[5]; // store up to max 9.999
      snprintf (s, 6, "%f", m_value);

      w->value (s);
    }
}

// callback functions
void LAB_Meter:: 
LAB_Meter_cb_fl_light_button_run_stop (Fl_Light_Button *w, 
                                       void            *data)
{
  if (w->value () == 0)
    {
      m_flag_is_meter_running = false;

      w->label ("Run");
    }
  else 
    {
      m_flag_is_meter_running = true;

      std::thread thread1;

      w->label ("Stop");
    }
}

void LAB_Meter:: 
LAB_Meter_cb_fl_choice_unit (Fl_Choice *w,
                             void      *data)
{
 

}

void LAB_Meter:: 
LAB_Meter_DEBUG_cb_meter_fl_light_button_generate_random_values (Fl_Light_Button *w,
                                                                 void            *data)
{
  m_flag_is_generate_random_values_running = (w->value () == 0) ? false : true;
}

void LAB_Meter:: 
LAB_Meter_random_values_generator ()
{
  while (m_flag_is_generate_random_values_running)
    {
      m_value = ((rand () % 501) + 3000) / 1000.0;
    }
}