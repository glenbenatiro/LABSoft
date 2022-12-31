#include "LABSoft_Controller_Voltmeter.h"

#include <cstdio>
#include <chrono>
#include <cmath>

#include <FL/Fl.H>


#include "Defaults.h"

LABSoft_Controller_Voltmeter:: 
LABSoft_Controller_Voltmeter (LAB         *_LAB, 
                              LABSoft_GUI *_LABSoft_GUI)
{
  m_LAB = _LAB;
  m_LABSoft_GUI = _LABSoft_GUI;
}

void LABSoft_Controller_Voltmeter:: 
update_loop ()
{
  while (m_LAB->m_Voltmeter.m_is_running)
    {
      // assemble text
      char display[50];
      // int unit_power = aux_unit_label_to_unit_power
      //   (m_LABSoft_GUI->voltmeter_fl_choice_unit->text ());

      int unit_power = 0;
      double scaled_value = (m_LAB->m_Voltmeter.m_value) * (pow (10, (-3 * unit_power)));

      if (unit_power >= 0)
        sprintf (display, "%4.3lf", scaled_value);
      else 
        sprintf (display, "%4.0lf", scaled_value);

      // update gui
      Fl::lock ();
      m_LABSoft_GUI->voltmeter_fl_output_value->value (display);
      Fl::unlock ();

      // importante ni ang sleep para dili mu lag!
      std::this_thread::sleep_for(std::chrono::milliseconds(PRE_FL_AWAKE_SLEEP_MS));
      
      // call main() to update display from this thread
      Fl::awake ();
    }
}

// callback functions
void LABSoft_Controller_Voltmeter:: 
cb_unit (Fl_Choice *w,
                   void      *data)
{
  // nothing to do here
  // unit will be read from widget during fl_output update
}

void LABSoft_Controller_Voltmeter:: 
cb_run_stop (Fl_Light_Button *w,
             void            *data)
{
  if (w->value () == 1)
    {
      // backend
      m_LAB->m_Voltmeter.m_is_running = true;

      // controller
      m_thread_run_stop = new std::thread 
        (&LABSoft_Controller_Voltmeter::update_loop, this);
    }
  else 
    {
      // backend 
      m_LAB->m_Voltmeter.m_is_running = false;

      // reset values
      m_LABSoft_GUI->voltmeter_fl_output_value->value ("0.000");

      // m_thread_run_stop->join ();
    }
}

void LABSoft_Controller_Voltmeter:: 
cb_generate_random_values (Fl_Light_Button *w,
                                           void            *data)
{
  if (w->value () == 1)
    {
      m_LAB->m_Voltmeter.m_is_random_value_generator_running = true;

      // backend random value generator
      m_thread_random_value_generator = new std::thread 
        (&LAB_Voltmeter::random_value_generator, m_LAB->m_Voltmeter);
    } 
  else 
    {
      m_LAB->m_Voltmeter.m_is_random_value_generator_running = false;
      
      // m_thread_random_value_generator->join ();
    }
}
