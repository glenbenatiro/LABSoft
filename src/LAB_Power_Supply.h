#ifndef LAB_POWER_SUPPLY
#define LAB_POWER_SUPPLY

class LAB_Power_Supply
{
  public:
    bool m_flag_is_positive_12_volts_running = false,
         m_flag_is_negative_12_volts_running = false,
         m_flag_is_positive_5_volts_running  = false,
         m_flag_is_power_supply_enabled      = false;
};

#endif