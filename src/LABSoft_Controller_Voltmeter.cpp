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
       printf ("hello!\n");

      m_LAB->m_LAB_Voltmeter->m_flag_is_generate_random_values_running = false;

      thread1->join();

      w->label ("Run");
    }
  else 
    {
      m_LAB->m_LAB_Voltmeter->m_flag_is_generate_random_values_running = true;

      thread1 = new std::thread (&LAB_Voltmeter::LAB_Meter_random_values_generator, m_LAB->m_LAB_Voltmeter);

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

      thread2 = new std::thread (&LABSoft_Controller_Voltmeter::LABSoft_Controller_Voltmeter_update_voltmeter_value,
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
LABSoft_Controller_Voltmeter_update_voltmeter_value ()
{
  float value;
  char s[6];

  while (m_LAB->m_LAB_Voltmeter->m_flag_is_meter_running)
    {
      value = m_LAB->m_LAB_Voltmeter->m_value;
      snprintf (s, 6, "%f", value);

      m_LABSoft_GUI->voltmeter_fl_output_value->value (s);

      // Fl::awake ();
    } 
}