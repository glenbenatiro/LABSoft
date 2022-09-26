#ifndef LAB_POWER_SUPPLY
#define LAB_POWER_SUPPLY

#include <iostream>

class LAB_Power_Supply
{
  public:
    int m_positive_12_volt_channel,
        m_negative_12_volt_channel,
        m_positive_5_volt_channel;

    bool m_flag_is_positive_12_volts_enabled = false,
         m_flag_is_negative_12_volts_enabled = false,
         m_flag_is_positive_5_volts_enabled  = false,
         m_flag_is_power_supply_enabled      = false;

    // functions
    void LAB_Power_Supply_update_power_channels ();
};



#endif