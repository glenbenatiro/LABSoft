#include "LAB.h"

LAB::LAB ()
: m_Oscilloscope        (&(m_LAB_Core), this),
  m_Voltmeter           (&(m_LAB_Core), this),
  m_Function_Generator  (&(m_LAB_Core)),
  m_Logic_Analyzer      (&(m_LAB_Core), this),
  m_Circuit_Checker     (&(m_LAB_Core), this)
{
  // Initialize PWM
  double pwm_range = (LAB_PWM_FREQUENCY * 2.0) / (LAB_OSCILLOSCOPE_MAX_SAMPLING_RATE);

  m_LAB_Core.pwm_init (PWM_CHAN, LAB_PWM_FREQUENCY,
    static_cast<uint32_t>(pwm_range), static_cast<uint32_t>(PWM_VALUE));

  *(Utility::get_reg32 (m_LAB_Core.m_regs_pwm, PWM_DMAC)) = 
    PWM_DMAC_ENAB | PWM_ENAB;

  // Initialize SPI 
  m_LAB_Core.spi_init (LAB_SPI_FREQUENCY);

  *(Utility::get_reg32 (m_LAB_Core.m_regs_spi, SPI_DC)) = 
    (8<<24) | (1<<16) | (8<<8) | 1;

  // Compensating GPIO7 pin? Undergrads wrong pin allocation. Made this ground.
  // Temporary set this pin to output...?
  //m_LAB_Core.AP_gpio_set (7, AP_GPIO_FUNC_OUTPUT, AP_GPIO_PULL_DOWN);

  // m_LAB_Core.AP_gpio_set (12, AP_GPIO_FUNC_ALT0, AP_GPIO_PULL_DOWN);
}

LAB::~LAB ()
{
  
}