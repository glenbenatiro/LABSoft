#include "LAB.h"

#include <cstring>

LAB::LAB ()
: m_Oscilloscope        (&(m_LAB_Core), this),
  m_Voltmeter           (&(m_LAB_Core), this),
  m_Function_Generator  (&(m_LAB_Core)),
  m_Logic_Analyzer      (&(m_LAB_Core), this),
  m_Circuit_Checker     (&(m_LAB_Core), this)
{
  setup_pwm ();

  setup_spi (); 
}

LAB::~LAB ()
{
  m_LAB_Core.unmap_periph_mem (& (m_uncached_pwm_dma_data));
}

void LAB:: 
setup_pwm ()
{
  // Initialize PWM (for oscilloscope (and in the meantime, for logic analzyer))
  m_LAB_Core.pwm_init     (LAB_OSCILLOSCOPE_SAMPLING_RATE);
  m_LAB_Core.pwm_algo     (LAB_PWM_DMA_PACING_PWM_CHAN, AP_PWM_ALGO_MARKSPACE);
  m_LAB_Core.pwm_use_fifo (LAB_PWM_DMA_PACING_PWM_CHAN, 1);

  // Init GPIO pin for PWM
  m_LAB_Core.gpio_set (LAB_RPI_PIN_PWM_CHAN_0, AP_GPIO_FUNC_ALT0, AP_GPIO_PULL_DOWN);

  // Setup PWM DMA control blocks
  config_pwm_dma_control_blocks ();

  // Enable DMA for PWM and DREQ threshold
  *(Utility::get_reg32 (m_LAB_Core.m_regs_pwm, PWM_DMAC)) = (1 << 31) | (1 << 0);

  // Reset DMA chan
  m_LAB_Core.dma_reset (LAB_DMA_CHAN_PWM_PACING);

  // Load up DMA control block for PWM pacing and start DMA
  m_LAB_Core.dma_start (&m_uncached_pwm_dma_data, LAB_DMA_CHAN_PWM_PACING, 
    &((static_cast<LAB_PWM_PACING_DMA_DATA *>(m_uncached_pwm_dma_data.virt))->
      cbs[0]), 0); 
}

void LAB:: 
config_pwm_dma_control_blocks ()
{
  // Map uncached memory for PWM DMA data
  // (lol just estimated the memory size XD)
  m_LAB_Core.map_uncached_mem (& (m_uncached_pwm_dma_data), 0x1000 * 10);

  // Setup PWM control blocks
  AP_MemoryMap *mp = &m_uncached_pwm_dma_data;

  LAB_PWM_PACING_DMA_DATA *dp = static_cast<LAB_PWM_PACING_DMA_DATA *>
    (m_uncached_pwm_dma_data.virt);
  
  LAB_PWM_PACING_DMA_DATA adc_dma_data = 
  {
    .cbs = 
    {
      // { //CB 0
      //   DMA_CB_TI_PWM, 
      //   MEM(mp, &dp->pwm_val),   
      //   REG(AP.m_regs_pwm, PWM_FIF1), 
      //   4, 
      //   0, 
      //   Utility::mem_bus_addr (mp, &dp->cbs[0]), 
      //   0
      // }, 

      { //CB 0
        DMA_CB_TI_PWM, 
        MEM (mp, &dp->pwm_val),   
        REG (m_LAB_Core.m_regs_pwm, PWM_FIF1), 
        4, 
        0, 
        Utility::mem_bus_addr (mp, &dp->cbs[8]), 
        0
      }, 
    },

    .pwm_val = 250,
  };
  
  // Copy control blocks to uncached mem
  std::memcpy (dp, &adc_dma_data, sizeof (adc_dma_data));
}

void LAB:: 
setup_spi ()
{
  m_LAB_Core.spi_init (LAB_SPI_FREQUENCY);

  *(Utility::get_reg32 (m_LAB_Core.m_regs_spi, SPI_DC)) = 
    (8<<24) | (1<<16) | (8<<8) | 1;
}