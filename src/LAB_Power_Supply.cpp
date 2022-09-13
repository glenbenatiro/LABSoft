#include "LAB_Power_Supply.h"

void LAB_Power_Supply::
LAB_Power_Supply_cb_power_supply_fl_light_button_positive_12_volts (Fl_Light_Button *w,
                                                                    void            *data)
{
  if (w->value () == 0)
    {
      printf ("DEBUG: positive 12 volts enabled\n");
    }
  else 
    {
      printf ("DEBUG: positive 12 volts disabled\n");
    }
}

void LAB_Power_Supply::
LAB_Power_Supply_cb_power_supply_fl_light_button_negative_12_volts (Fl_Light_Button *w,
                                                                    void            *data)
{
  if (w->value () == 0)
    {
      printf ("DEBUG: negative 12 volts enabled\n");
    }
  else 
    {
      printf ("DEBUG: negative 12 volts disabled\n");
    }
}

void LAB_Power_Supply::
LAB_Power_Supply_cb_power_supply_fl_light_button_positive_5_volts (Fl_Light_Button *w,
                                                                   void            *data)
{
  if (w->value () == 0)
    {
      printf ("DEBUG: positive 5 volts enabled\n");
    }
  else 
    {
      printf ("DEBUG: positive 5 volts disabled\n");
    }
}

void LAB_Power_Supply::
LAB_Power_Supply_cb_power_supply_fl_light_button_master_enable_disable (Fl_Light_Button *w,
                                                                        void            *data)
{
  if (w->value () == 0)
    {
      m_flag_master_enable = false;
    }
  else
    {
      m_flag_master_enable = true;
    }
}


