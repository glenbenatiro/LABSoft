#include "LAB_Function_Generator.h"

#include "LAB.h"

#include <iostream>

LAB_Function_Generator::
LAB_Function_Generator (LAB_Core *_LAB_Core) :
  m_LAB_Core (_LAB_Core),
  m_func_gen_ic (_LAB_Core)
{
  m_LAB_Core  = _LAB_Core;
}

int LAB_Function_Generator::
run_stop (unsigned  channel,
          bool      value)
{
  if (is_valid_channel (channel))
  {
    m_parent_data.channel_data[channel].is_enabled = value;

    value ? m_func_gen_ic.start () : m_func_gen_ic.stop ();

    return 1;
  }

  return -1;
}

int LAB_Function_Generator:: 
wave_type (unsigned     channel,
           LE_WAVE_TYPE _LE_WAVE_TYPE)
{
  if (is_valid_channel (channel))
  {
    m_parent_data.channel_data[channel].wave_type = _LE_WAVE_TYPE;

    m_func_gen_ic.wave_type (_LE_WAVE_TYPE);

    return 1;
  }

  return -1;
}

int LAB_Function_Generator:: 
frequency (unsigned channel,
           double   value)
{
  if (is_valid_channel (channel) && 
    is_valid_fg_param (value, LE_FG_PARAM_FREQUENCY))
  {
    m_parent_data.channel_data[channel].frequency = value;

    m_func_gen_ic.frequency (value);

    return 1;
  }

  return -1;
}

int LAB_Function_Generator:: 
period (unsigned channel,
        double   value)
{
  if (is_valid_channel (channel) && 
    is_valid_fg_param (value, LE_FG_PARAM_PERIOD))
  {
    m_parent_data.channel_data[channel].period = value;

    m_func_gen_ic.period (value);

    return 1;
  }

  return -1;
}

int LAB_Function_Generator:: 
amplitude (unsigned channel,
           double   value)
{
  if (is_valid_channel (channel) && 
    is_valid_fg_param (value, LE_FG_PARAM_AMPLITUDE))
  {
    m_parent_data.channel_data[channel].amplitude = value;

    set_hw_amplitude (channel, value);

    return 1;
  }

  return -1;
}

int LAB_Function_Generator:: 
vertical_offset (unsigned channel,
                 double   value)
{
  if (is_valid_channel (channel) && 
    is_valid_fg_param (value, LE_FG_PARAM_VERTICAL_OFFSET))
  {
    m_parent_data.channel_data[channel].vertical_offset = value;

    set_hw_vertical_offset (channel, value);

    return 1;
  }

  return -1;
}

int LAB_Function_Generator:: 
duty_cycle (unsigned channel,
            double   value)
{
  if (is_valid_channel (channel) && 
    is_valid_fg_param (value, LE_FG_PARAM_DUTY_CYCLE))
  {
    m_parent_data.channel_data[channel].duty_cycle = value;

    m_func_gen_ic.duty_cycle (value);

    return 1;
  }

  return -1;
}

int LAB_Function_Generator:: 
phase (unsigned channel,
            double   value)
{
  if (is_valid_channel (channel) && 
    is_valid_fg_param (value, LE_FG_PARAM_PHASE))
  {
    m_parent_data.channel_data[channel].phase = value;

    m_func_gen_ic.phase (value);

    return 1;
  }

  return -1;
}

double LAB_Function_Generator::
frequency (unsigned channel)
{
  return (m_parent_data.channel_data[channel].frequency);
}

int LAB_Function_Generator:: 
set_hw_amplitude (unsigned  channel, 
                  double    value)
{
  return 1;
}

int LAB_Function_Generator:: 
set_hw_vertical_offset (unsigned  channel, 
                        double    value)
{
  return 1;
}

bool LAB_Function_Generator:: 
is_valid_channel (unsigned channel)
{
  return (channel >= 0 && channel < LAB_FUNCTION_GENERATOR_NUMBER_OF_CHANNELS);
}

bool LAB_Function_Generator:: 
is_valid_fg_param (double value, LE_FG_PARAM _LE_FG_PARAM)
{
  switch (_LE_FG_PARAM)
  {
    case LE_FG_PARAM_FREQUENCY:
      return (value >= LAB_FUNCTION_GENERATOR_FREQUENCY_MIN &&
        value <= LAB_FUNCTION_GENERATOR_FREQUENCY_MAX);
      break;
    
    case LE_FG_PARAM_PERIOD:
      return (is_valid_fg_param (1.0 / value, LE_FG_PARAM_FREQUENCY));
      break;
    
    case LE_FG_PARAM_AMPLITUDE:
      return (value >= LAB_FUNCTION_GENERATOR_AMPLITUDE_MIN &&
        value <= LAB_FUNCTION_GENERATOR_AMPLITUDE_MAX);
      break;
    
    case LE_FG_PARAM_VERTICAL_OFFSET:
      return (value >= LAB_FUNCTION_GENERATOR_VERTICAL_OFFSET_MIN &&
        value <= LAB_FUNCTION_GENERATOR_VERTICAL_OFFSET_MAX);
      break;
    
    case LE_FG_PARAM_DUTY_CYCLE:
      return (value >= LAB_FUNCTION_GENERATOR_DUTY_CYCLE_MIN &&
        value <= LAB_FUNCTION_GENERATOR_DUTY_CYCLE_MAX);
      break;
  
    case LE_FG_PARAM_PHASE:
      return (value >= LAB_FUNCTION_GENERATOR_PHASE_MIN &&
        value <= LAB_FUNCTION_GENERATOR_PHASE_MAX);
      break;

    default:
      break;
  }

  // If nothing else
  return false;
}

// EOF