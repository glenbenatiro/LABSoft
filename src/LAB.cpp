#include "LAB.h"

LAB::LAB ()
: m_Oscilloscope        (&(m_LAB_Core), this),
  m_Voltmeter           (&(m_LAB_Core), this),
  m_Function_Generator  (&(m_LAB_Core)),
  m_Logic_Analyzer      (&(m_LAB_Core)),
  m_Circuit_Checker     (&(m_LAB_Core), this)
{
  // initialize SPI 
  m_LAB_Core.spi_init (LAB_SPI_FREQUENCY);

  // initialize PWM 
  //   uint32_t m_pwm_range = (LAB_PWM_FREQUENCY * 2) / LAB_OSCILLOSCOPE_MAX_SAMPLING_RATE;
  uint32_t m_pwm_range = (LAB_PWM_FREQUENCY) / LAB_OSCILLOSCOPE_MAX_SAMPLING_RATE;
  m_LAB_Core.pwm_init (PWM_CHAN, LAB_PWM_FREQUENCY, m_pwm_range, PWM_VALUE);   // Initialise PWM, with DMA
}

LAB::~LAB ()
{
  
}