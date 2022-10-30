#include "LAB_Power_Supply.h"

#include <iostream>

LAB_Power_Supply:: 
LAB_Power_Supply ()
{

}

void LAB_Power_Supply:: 
update_power_channels ()
{
  if (!m_is_power_supply_enabled)
    {
      m_positive_12_volt_channel  = 0;
      m_negative_12_volt_channel  = 0;
      m_positive_5_volt_channel   = 0;
    }
  else 
  {
    if (m_is_positive_12_volts_enabled)
      m_positive_12_volt_channel = 1;
    else 
      m_positive_12_volt_channel = 0;

    if (m_is_negative_12_volts_enabled)
      m_negative_12_volt_channel = 1;
    else 
      m_negative_12_volt_channel = 0;

    if (m_is_positive_5_volts_enabled)
      m_positive_5_volt_channel = 1;
    else 
      m_positive_5_volt_channel = 0;
  }

  // DEBUG
  char s[50];
  sprintf (s, "DEBUG: channel values: %d, %d, %d\n", m_positive_12_volt_channel, 
    m_negative_12_volt_channel, m_positive_5_volt_channel);
  printf (s);
}