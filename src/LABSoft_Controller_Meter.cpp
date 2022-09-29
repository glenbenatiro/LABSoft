#include "LABSoft_Controller_Meter.h"

LABSoft_Controller_Meter:: 
LABSoft_Controller_Meter (LAB         *_LAB, 
                          LABSoft_GUI *_LABSoft_GUI, 
                          LAB_Meter   *_LAB_Meter, 
                          Fl_Output   *_Fl_Output)
{ 
  m_LAB         = _LAB;
  m_LABSoft_GUI = _LABSoft_GUI;
  m_LAB_Meter   = _LAB_Meter;
  m_Fl_Output   = _Fl_Output;
}

void LABSoft_Controller_Meter:: 
LABSoft_Controller_Meter_update_meter_value ()
{
  char  s[20];
  float value  = m_LAB_Meter->m_value * m_LAB_Meter->m_unit_scaler,
        scaler = m_LAB_Meter->m_unit_scaler;

  while (m_LAB_Meter->m_is_meter_running)
    {
      sprintf (s, "%f", value);
      m_Fl_Output->value (s);
      m_Fl_Output->redraw ();

      // if (scaler == 0.001) // milli
      //   {
      //     snprintf (s, 7, "%d", (int)value);  // 1000 mV
      //   }
      // else if (scaler == 1.0) // base
      //   {
      //     snprintf (s, 7, "%2.3f", value);    // 10.000 V
      //   }
    
      // switch (scaler)
      //   {
      //     case 1000000000.0: // giga
      //         scaler = ;
      //         break;
      //     case 1000000.0: // mega
      //         scaler = ;
      //         break;
      //     case 1000.0: // kilo
      //         scaler = ;
      //         break;
      //     case 0.001: // milli
      //         scaler = 
      //         break;
      //     case 0.000001: // micro
      //         scaler = 
      //         break;
      //     case 0.000000001: // nano
      //         scaler = 
      //         break;
      //     default:
      //         scaler = 1.0;
      //         break;
      //   }

      // Fl::awake ();
    } 
}

// callback functions
void LABSoft_Controller_Meter::
LABSoft_Controller_Meter_cb_fl_choice_unit (Fl_Choice *w, 
                                            void      *data)
{
  printf ("unit: %f\n", atof (w->text ()));
  m_LAB_Meter->m_unit_scaler = atof (w->text ());
}

void LABSoft_Controller_Meter:: 
LABSoft_Controller_Meter_cb_fl_light_button_run_stop (Fl_Light_Button *w,
                                                      void            *data)
{
  if (w->value () == 0)
    {
      m_LAB_Meter->m_is_meter_running = false;

      thread2->join ();

      w->label ("Run");
    }
  else 
    {
      m_LAB_Meter->m_is_meter_running = true;

      thread2 = new std::thread (&LABSoft_Controller_Meter::LABSoft_Controller_Meter_update_meter_value,
        this);

      w->label ("Stop");
    }
}

void LABSoft_Controller_Meter:: 
LABSoft_Controller_Meter_cb_fl_light_button_generate_random_values (Fl_Light_Button *w, 
                                                                    void            *data)
{
  if (w->value () == 0)
    {
      m_LAB_Meter->m_is_generate_random_values_running = false;

      thread1->join();

      w->label ("Run");
    }
  else 
    {
      m_LAB_Meter->m_is_generate_random_values_running = true;

      thread1 = new std::thread (&LAB_Meter::LAB_Meter_random_values_generator, m_LAB_Meter);

      w->label ("Stop");
    }
}

// EOF