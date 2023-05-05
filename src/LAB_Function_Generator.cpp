#include "LAB_Function_Generator.h"

#include <iostream>

LAB_Function_Generator:: 
LAB_Function_Generator (LAB_Core* _LAB_Core)
  : m_LAB_Core    (_LAB_Core),
    m_func_gen_ic {LAB_AD9833 (_LAB_Core)},
    m_digipot {LAB_MCP4XXX (MCP4XXX::PART_NUMBER::MCP4161, MCP4XXX::RESISTANCE_VERSION::_503),
               LAB_MCP4XXX (MCP4XXX::PART_NUMBER::MCP4161, MCP4XXX::RESISTANCE_VERSION::_503),
               LAB_MCP4XXX (MCP4XXX::PART_NUMBER::MCP4161, MCP4XXX::RESISTANCE_VERSION::_103)}
{
  m_digipot[0].init (m_LAB_Core, 0);
  m_digipot[1].init (m_LAB_Core, 1);
  m_digipot[2].init (m_LAB_Core, 2);

  m_digipot[0].resistance (0, 10'000.0);
}

void LAB_Function_Generator::
init_state ()
{
  
}

void LAB_Function_Generator:: 
run_stop (unsigned channel,
          bool value)
{
  m_parent_data.channel_data[channel].is_enabled = value; 

  value ? m_func_gen_ic[channel].start () : m_func_gen_ic[channel].stop ();
}

void LAB_Function_Generator:: 
wave_type (unsigned channel,
           LE_WAVE_TYPE _LE_WAVE_TYPE)
{
  m_parent_data.channel_data[channel].wave_type = _LE_WAVE_TYPE;

  m_func_gen_ic[channel].wave_type (_LE_WAVE_TYPE);
}

void LAB_Function_Generator:: 
frequency (unsigned channel,
           double   value)
{
  m_parent_data.channel_data[channel].frequency = value;

  m_func_gen_ic[channel].frequency (value);
}

void LAB_Function_Generator:: 
period (unsigned channel,
        double   value)
{
  m_parent_data.channel_data[channel].period = value;

  m_func_gen_ic[channel].period (value);
}

void LAB_Function_Generator:: 
amplitude (unsigned channel,
           double   value)
{
  m_parent_data.channel_data[channel].amplitude = value;

  //set_hw_amplitude (value);
}

void LAB_Function_Generator:: 
vertical_offset (unsigned channel,
                 double   value)
{
  m_parent_data.channel_data[channel].vertical_offset = value;

  //set_hw_vertical_offset (value);
}

void LAB_Function_Generator:: 
duty_cycle (unsigned channel,
            double   value)
{
  m_parent_data.channel_data[channel].duty_cycle = value;

  m_func_gen_ic[channel].duty_cycle (value);
}

void LAB_Function_Generator:: 
phase (unsigned channel,
       double   value)
{
  m_parent_data.channel_data[channel].phase = value;

  m_func_gen_ic[channel].phase (value);
}

void LAB_Function_Generator:: 
set_hw_amplitude (unsigned channel,
                  double   value)
{
  
}

void LAB_Function_Generator:: 
set_hw_vertical_offset (unsigned channel,
                        double   value)
{
  
}

// EOF