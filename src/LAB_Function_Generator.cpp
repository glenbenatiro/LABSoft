#include "LAB_Function_Generator.h"

LAB_Function_Generator::
LAB_Function_Generator () : LAB_Core ()
{
  m_frequency   = LAB_DEFAULTS_FUNCTION_GENERATOR_FREQUENCY; 
  m_period      = LAB_DEFAULTS_FUNCTION_GENERATOR_PERIOD; 
  m_amplitude   = LAB_DEFAULTS_FUNCTION_GENERATOR_AMPLITUDE; 
  //m_offset      = LAB_DEFAULTS_FUNCTION_GENERATOR_OFFSET;  
  m_duty_cycle  = LAB_DEFAULTS_FUNCTION_GENERATOR_DUTY_CYCLE; 
  m_phase       = LAB_DEFAULTS_FUNCTION_GENERATOR_PHASE; 
  m_wave_type   = LAB_DEFAULTS_FUNCTION_GENERATOR_WAVE_TYPE;
}

// EOFs