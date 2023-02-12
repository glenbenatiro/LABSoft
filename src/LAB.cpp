#include "LAB.h"

LAB::LAB ()
{
  m_Oscilloscope        = new LAB_Oscilloscope        (&(m_LAB_Core));
  m_Function_Generator  = new LAB_Function_Generator  (&(m_LAB_Core));
  m_Multimeter          = new LAB_Multimeter          (&(m_LAB_Core));
  m_Logic_Analyzer      = new LAB_Logic_Analyzer      (&(m_LAB_Core));

}

LAB::~LAB ()
{
  uint32_t m_pwm_range = (LAB_PWM_FREQUENCY * 2) / LAB_OSCILLOSCOPE_MAX_SAMPLING_RATE;

  m_LAB_Core.pwm_init (PWM_CHAN, LAB_PWM_FREQUENCY, m_pwm_range, PWM_VALUE);   // Initialise PWM, with DMA
}