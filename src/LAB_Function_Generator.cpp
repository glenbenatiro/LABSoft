#include "LAB_Function_Generator.h"

#include <iostream>

LAB_Function_Generator:: 
LAB_Function_Generator (LAB_Core* _LAB_Core)
  : m_LAB_Core       (_LAB_Core),
    m_digipot_amp    {LAB_MCP4XXX (MCP4XXX::PART_NUMBER::MCP4161, MCP4XXX::RESISTANCE_VERSION::_503),
                      LAB_MCP4XXX (MCP4XXX::PART_NUMBER::MCP4161, MCP4XXX::RESISTANCE_VERSION::_503)},
    m_digipot_offset {(LAB_MCP4XXX (MCP4XXX::PART_NUMBER::MCP4161, MCP4XXX::RESISTANCE_VERSION::_503))}
{
  m_digipot_amp[0].init (m_LAB_Core, 0);
  m_digipot_amp[1].init (m_LAB_Core, 1);
  m_digipot_offset[0].init (m_LAB_Core, 2);

  m_digipot_amp[0].resistance (0, 10'000.0);
}

void LAB_Function_Generator::
init_state ()
{
  
}

void LAB_Function_Generator:: 
run (unsigned channel)
{
  m_func_gen_ic[channel].run ();
}

void LAB_Function_Generator:: 
stop (unsigned channel)
{
  m_func_gen_ic[channel].stop ();
}

void LAB_Function_Generator:: 
wave_type (unsigned                  channel,
           LABC::FUNC_GEN::WAVE_TYPE wave_type)
{
  m_parent_data.channel_data[channel].wave_type = wave_type;
  AD9833::WAVE_TYPE ad9833_wave_type;
  
  switch (wave_type)
  {
    case (LABC::FUNC_GEN::WAVE_TYPE::SINE):
    {
      ad9833_wave_type = AD9833::WAVE_TYPE::SINE;
      break;
    }

    case (LABC::FUNC_GEN::WAVE_TYPE::TRIANGLE):
    {
      ad9833_wave_type = AD9833::WAVE_TYPE::TRIANGLE;
      break;
    }

    case (LABC::FUNC_GEN::WAVE_TYPE::SQUARE):
    {
      ad9833_wave_type = AD9833::WAVE_TYPE::SQUARE;
      break;
    }

    case (LABC::FUNC_GEN::WAVE_TYPE::DC):
    {
      ad9833_wave_type = AD9833::WAVE_TYPE::DC;
      break;
    }
  }

  m_func_gen_ic[channel].wave_type (ad9833_wave_type);
}

void LAB_Function_Generator:: 
amplitude (unsigned channel,
           double   value)
{
  m_parent_data.channel_data[channel].amplitude = value;

  set_hw_amplitude (channel, value);
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
phase (unsigned channel,
       double   value)
{
  m_parent_data.channel_data[channel].phase = value;

  m_func_gen_ic[channel].phase (value);
}

void LAB_Function_Generator:: 
vertical_offset (unsigned channel,
                 double   value)
{
  m_parent_data.channel_data[channel].vertical_offset = value;

  set_hw_vertical_offset (channel, value);
}

void LAB_Function_Generator:: 
set_hw_amplitude (unsigned channel,
                  double   value)
{
  // https://en.wikibooks.org/wiki/Electronics/Electronics_Formulas/Op_Amp_Configurations
  // Inverting amplifier. Vout = (-Rf/Rin) * Vin
  // Rf = digipot0 + digipot1
  // Rin = 1000 
}

void LAB_Function_Generator:: 
set_hw_vertical_offset (unsigned channel,
                        double   value)
{
  
}

LABC::FUNC_GEN::WAVE_TYPE LAB_Function_Generator::
wave_type (unsigned channel)
{
  return (m_parent_data.channel_data[channel].wave_type);
}

double LAB_Function_Generator::
amplitude (unsigned channel)
{
  return (m_parent_data.channel_data[channel].amplitude);
}

double LAB_Function_Generator::
frequency (unsigned channel)
{
  return (m_parent_data.channel_data[channel].frequency);
}

double LAB_Function_Generator::
period (unsigned channel)
{
  return (m_parent_data.channel_data[channel].period);
}

double LAB_Function_Generator::
phase (unsigned channel)
{
  return (m_parent_data.channel_data[channel].phase);
}

double LAB_Function_Generator::
vertical_offset (unsigned channel)
{
  return (m_parent_data.channel_data[channel].vertical_offset);
}


// EOF