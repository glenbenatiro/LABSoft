#include "LABSoft_Controller_Power_Supply.h"

#include <cstring>

LABSoft_Controller_Power_Supply:: 
LABSoft_Controller_Power_Supply (LAB         *_LAB, 
                                 LABSoft_GUI *_LABSoft_GUI)
{
  m_LAB = _LAB;
  m_LABSoft_GUI = _LABSoft_GUI;
}

// callback functions
void LABSoft_Controller_Power_Supply:: 
cb_fl_light_button_run_stop_power_channel (Fl_Light_Button *w, 
                                           void            *data)
{
  bool *flag;
  const char *label = w->label ();
  char text[50];

  // get power channel 
  if (std::strcmp (label, "+12V") == 0)
    {
      flag = &(m_LAB->m_Power_Supply->m_is_positive_12_volts_enabled);
    }
  else if (std::strcmp (label, "-12V") == 0)
    {
      flag = &(m_LAB->m_Power_Supply->m_is_negative_12_volts_enabled);
    }
  else if (std::strcmp (label, "+5V") == 0)
    {
      flag = &(m_LAB->m_Power_Supply->m_is_positive_5_volts_enabled);
    }

  if (w->value () == 0)
    {
      *flag = false;
      sprintf (text, "DEBUG: %s volts flag disabled\n", label);
      printf ("%s", text);
    }
  else 
    {
      *flag = true;
      sprintf (text, "DEBUG: %s volts flag enabled\n", label);
      printf ("%s", text);
    }
  
  // call power supply backend
  m_LAB->m_Power_Supply->update_power_channels ();
}

void LABSoft_Controller_Power_Supply::
cb_fl_light_button_master_run_stop (Fl_Light_Button *w, 
                                    void            *data)
{
  if (w->value () == 0)
    {
      m_LAB->m_Power_Supply->m_is_power_supply_enabled = false;
      printf ("DEBUG: power supply disabled\n");
    }
  else 
    {
      m_LAB->m_Power_Supply->m_is_power_supply_enabled = true;
      printf ("DEBUG: power supply enabled\n");
    }
  
  // call power supply backend
  m_LAB->m_Power_Supply->update_power_channels ();
}

// EOF
