#ifndef LAB_FUNCTION_GENERATOR
#define LAB_FUNCTION_GENERATOR

#include "LAB_Core.h"
#include "Defaults.h"
#include "LAB_AD9833.h"

class LAB_Function_Generator 
{
  public:
    LAB_Core   *m_LAB_Core;
    LAB_AD9833  m_func_gen_ic;

  public:
    LAB_Parent_Data_Function_Generator m_parent_data;

    // --- Functions ---
    LAB_Function_Generator (LAB_Core *_LAB_Core);

    // 
    int run_stop (unsigned channel, bool value);
      
    // setters
    int wave_type       (unsigned channel, LE_WAVE_TYPE _WAVETYPE);
    int frequency       (unsigned channel, double frequency);
    int period          (unsigned channel, double period);
    int amplitude       (unsigned channel, double amplitude);
    int vertical_offset (unsigned channel, double offset);
    int duty_cycle      (unsigned channel, double duty_cycle);
    int phase           (unsigned channel, double phase);

    //
    int set_hw_amplitude        (unsigned channel, double value);
    int set_hw_vertical_offset  (unsigned channel, double value);
    
    //
    bool is_valid_channel   (unsigned channel);
    bool is_valid_fg_param  (double value, LE_FG_PARAM _LE_FG_PARAM);
};

#endif
