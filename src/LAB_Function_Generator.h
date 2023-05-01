#ifndef LAB_FUNCTION_GENERATOR_H
#define LAB_FUNCTION_GENERATOR_H

#include "LAB_Core.h"
#include "LAB_AD9833.h"
#include "LAB_MCP4XXX.h"
#include "Defaults.h"

class LAB_Function_Generator 
{
  private: 
    LAB_Core*                           m_LAB_Core;
    std::array<LAB_AD9833, 1>           m_func_gen_ic;
    std::array<LAB_MCP4XXX, 3>          m_digipot;
    LAB_Parent_Data_Function_Generator  m_parent_data;

  public:
    LAB_Function_Generator      (LAB_Core* _LAB_Core);

    void init_state             ();
    void run_stop               (unsigned channel, bool         value);
    void wave_type              (unsigned channel, LE_WAVE_TYPE _WAVETYPE);
    void frequency              (unsigned channel, double       frequency);
    void period                 (unsigned channel, double       period);
    void amplitude              (unsigned channel, double       amplitude);
    void vertical_offset        (unsigned channel, double       offset);
    void duty_cycle             (unsigned channel, double       duty_cycle);
    void phase                  (unsigned channel, double       phase);
    void set_hw_amplitude       (unsigned channel, double       value);
    void set_hw_vertical_offset (unsigned channel, double       value);
};

#endif
