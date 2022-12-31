#include "LABSoft_Controller_Multimeter.h"

#include <chrono>

LABSoft_Controller_Multimeter:: 
LABSoft_Controller_Multimeter (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI)
{
  m_LAB = _LAB;
  m_LABSoft_GUI = _LABSoft_GUI;
}

// 
void LABSoft_Controller_Multimeter:: 
update ()
{
  while (m_LAB->m_Multimeter->is_running ())
  {
    // update
    printf ("multimeter update\n");

    std::this_thread::sleep_for (std::chrono::duration <double, std::milli> (LABSOFT_CONTROLLER_MULTIMETER_UPDATE_RATE_MS));
  }
}

// callback functions
void LABSoft_Controller_Multimeter:: 
cb_unit      (Fl_Choice *w, void *data)
{

}

void LABSoft_Controller_Multimeter:: 
cb_mode      (Fl_Choice *w, void *data)
{

}

void LABSoft_Controller_Multimeter:: 
cb_run_stop  (Fl_Light_Button *w, void *data)
{
  if (w->value () == 1)
  {
    m_LAB->m_Multimeter->run ();

    m_update = std::thread (&LABSoft_Controller_Multimeter::update, this);
  }
  else 
  {
    m_LAB->m_Multimeter->stop ();

    m_update.join ();
  }
}

void LABSoft_Controller_Multimeter:: 
cb_generate_random_values (Fl_Light_Button *w, void *data)
{
  if (w->value () == 1)
  {
    m_LAB->m_Multimeter->run_random_value_generator ();
  }
  else 
  {
    m_LAB->m_Multimeter->stop_random_value_generator ();
  }
}
