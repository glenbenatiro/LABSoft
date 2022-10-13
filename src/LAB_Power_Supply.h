#ifndef LAB_POWER_SUPPLY
#define LAB_POWER_SUPPLY

#include "LAB_Core.h"

class LAB_Power_Supply : public LAB_Core
{
  public:
    int m_positive_12_volt_channel,
        m_negative_12_volt_channel,
        m_positive_5_volt_channel;

    bool m_is_positive_12_volts_enabled = false,
         m_is_negative_12_volts_enabled = false,
         m_is_positive_5_volts_enabled  = false,
         m_is_power_supply_enabled      = false;

    // functions
    LAB_Power_Supply ();
    
    void update_power_channels ();
};

#endif

// EOF