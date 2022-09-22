#include "LABSoft_Controller_Power_Supply.h"

LABSoft_Controller_Power_Supply:: 
LABSoft_Controller_Power_Supply (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI)
{
  m_LAB = _LAB;
  m_LABSoft_GUI = _LABSoft_GUI;
}

void LABSoft_Controller_Power_Supply::
LABSoft_Controller_Power_Supply_cb_fl_light_button_positive_12_volts_run_stop (Fl_Light_Button *w, void *data)
{
  if (w->value () == 0)
    {
      m_LAB->m_LAB_Power_Supply->m_flag_is_positive_12_volts_running = false;
      printf ("DEBUG: positive 12 volts disabled\n");
    }
  else 
    {
      m_LAB->m_LAB_Power_Supply->m_flag_is_positive_12_volts_running = true;
      printf ("DEBUG: positive 12 volts enabled\n");
    }
}

void LABSoft_Controller_Power_Supply::
LABSoft_Controller_Power_Supply_cb_fl_light_button_negative_12_volts_run_stop (Fl_Light_Button *w, void *data)
{
  if (w->value () == 0)
    {
      m_LAB->m_LAB_Power_Supply->m_flag_is_negative_12_volts_running = false;
      printf ("DEBUG: negative 12 volts disabled\n");
    }
  else 
    {
      m_LAB->m_LAB_Power_Supply->m_flag_is_negative_12_volts_running = true;
      printf ("DEBUG: negative 12 volts enabled\n");
    }
}

void LABSoft_Controller_Power_Supply::
LABSoft_Controller_Power_Supply_cb_fl_light_button_positive_5_volts_run_stop (Fl_Light_Button *w, void *data)
{
  if (w->value () == 0)
    {
      m_LAB->m_LAB_Power_Supply->m_flag_is_positive_5_volts_running = false;
      printf ("DEBUG: positive 5 volts disabled\n");
    }
  else 
    {
      m_LAB->m_LAB_Power_Supply->m_flag_is_positive_5_volts_running = true;
      printf ("DEBUG: positive 5 volts enabled\n");
    }
}

void LABSoft_Controller_Power_Supply::
LABSoft_Controller_Power_Supply_cb_fl_light_button_master_enable_disable (Fl_Light_Button *w, void *data)
{
  if (w->value () == 0)
    {
      m_LAB->m_LAB_Power_Supply->m_flag_is_power_supply_enabled = false;
      printf ("DEBUG: power supply disabled\n");
    }
  else 
    {
      m_LAB->m_LAB_Power_Supply->m_flag_is_power_supply_enabled = true;
      printf ("DEBUG: power supply enabled\n");
    }
}
