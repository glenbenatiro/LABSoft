#include "LABSoft_Controller_Voltmeter.h"

LABSoft_Controller_Voltmeter:: 
LABSoft_Controller_Voltmeter (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI)
{
  m_LAB         = _LAB;
  m_LABSoft_GUI = _LABSoft_GUI;
}

// cbs
void LABSoft_Controller_Voltmeter:: 
LABSoft_Controller_Voltmeter_cb_fl_light_button_generate_random_values (Fl_Light_Button *w, 
                                                                    void            *data)
{
  if (w->value () == 0)
    {
      m_LAB->m_LAB_Voltmeter->m_flag_is_generate_random_values_running = false;

      thread1->join();

      w->label ("Run");
    }
  else 
    {
      m_LAB->m_LAB_Voltmeter->m_flag_is_generate_random_values_running = true;

      thread1 = new std::thread (&LAB_Meter::LAB_Meter_random_values_generator, m_LAB->m_LAB_Voltmeter);

      w->label ("Stop");
    }
}

void LABSoft_Controller_Voltmeter:: 
LABSoft_Controller_Voltmeter_cb_fl_light_button_start_stop (Fl_Light_Button *w,
                                                            void            *data)
{
  if (w->value () == 0)
    {
      m_LAB->m_LAB_Voltmeter->m_flag_is_meter_running = false;

      thread2->join ();

      w->label ("Run");
    }
  else 
    {
      m_LAB->m_LAB_Voltmeter->m_flag_is_meter_running = true;

      thread2 = new std::thread (&LABSoft_Controller_Voltmeter::LABSoft_Controller_Voltmeter_update_meter_value,
        this);

      w->label ("Stop");
    }
}

void LABSoft_Controller_Voltmeter::
LABSoft_Controller_Voltmeter_cb_fl_choice_unit (Fl_Choice *w, 
                                                void      *data)
{

}


void LABSoft_Controller_Voltmeter:: 
LABSoft_Controller_Voltmeter_update_meter_value ()
{
  float value, scaler;
  char s[10];

  while (m_LAB->m_LAB_Voltmeter->m_flag_is_meter_running)
    {
      // copy value
      scaler = (strcmp(m_LABSoft_GUI->voltmeter_fl_choice_unit->text(), "V") == 0 ? 1.0 : 1000.0);

      value = m_LAB->m_LAB_Voltmeter->m_value * scaler;

      if (scaler == 1000.0)
        {
          snprintf (s, 7, "%d", (int)value);
        }
      else
        {
          snprintf (s, 7, "%2.3f", value);
        }

      m_LABSoft_GUI->voltmeter_fl_output_value->value (s);

      // Fl::awake ();
    } 
}