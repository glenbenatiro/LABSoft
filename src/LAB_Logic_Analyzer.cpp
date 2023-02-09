#include "LAB_Logic_Analyzer.h"

LAB_Logic_Analyzer::
LAB_Logic_Analyzer (LAB_Core *_LAB_Core)
{
  m_LAB_Core = _LAB_Core;

  init_uncached_dma_data ();
}

void LAB_Logic_Analyzer:: 
init_uncached_dma_data ()
{
  m_LAB_Core->map_uncached_mem (&m_uncached_dma_data, LAB_LOGIC_ANALYZER_VC_MEM_SIZE);

  m_LAB_Core->dma_stop (LAB_LOGIC_ANALYZER_DMA_CHAN_GPIO_STORE);
  m_LAB_Core->dma_stop (LAB_OSCILLOSCOPE_DMA_CHAN_PWM_PACING);

  LAB_LOGIC_ANALYZER_DMA_DATA *dp = 
    static_cast<LAB_LOGIC_ANALYZER_DMA_DATA *>(m_uncached_dma_data.virt);
  
  MemoryMap *mp = &m_uncached_dma_data;
  
  LAB_LOGIC_ANALYZER_DMA_DATA dma_data = 
  {
    .cbs = 
    {
      // for dual buffer
      // CB0
      {
        LAB_LOGIC_ANALYZER_DMA_CB_TI_GPIO_STORE, 
        Utility::reg_bus_addr (&(m_LAB_Core->m_regs_gpio), GPIO_GPLEV0),
        Utility::mem_bus_addr (mp, dp->rxd0),
        (uint32_t)(LAB_LOGIB_ANALYZER_NUMBER_OF_SAMPLES * 4),
        0,
        Utility::mem_bus_addr (mp, cbs[1]),
        0
      },
      // CB1
      {
        LAB_LOGIC_ANALYZER_DMA_CB_TI_GPIO_STORE, 
        Utility::reg_bus_addr (&(m_LAB_Core->m_regs_gpio), GPIO_GPLEV0),
        Utility::mem_bus_addr (mp, dp->rxd1),
        (uint32_t)(LAB_LOGIB_ANALYZER_NUMBER_OF_SAMPLES * 4),
        0,
        Utility::mem_bus_addr (mp, cbs[2]),
        0
      },
      // CB2
      { 
        DMA_CB_TI_PWM, 
        MEM(mp, &dp->pwm_val),   
        REG(m_LAB_Core->m_regs_pwm, PWM_FIF1), 
        4, 
        0, 
        Utility::mem_bus_addr (mp, cbs[0]);
        0
      }, 


      // for single buffer
      // CB3
      {
        LAB_LOGIC_ANALYZER_DMA_CB_TI_GPIO_STORE, 
        Utility::reg_bus_addr (&(m_LAB_Core->m_regs_gpio), GPIO_GPLEV0),
        Utility::mem_bus_addr (mp, dp->rxd0),
        (uint32_t)(LAB_LOGIB_ANALYZER_NUMBER_OF_SAMPLES * 4),
        0,
        Utility::mem_bus_addr (mp, cbs[4]),
        0
      },
      // CB4
      { 
        DMA_CB_TI_PWM, 
        MEM(mp, &dp->pwm_val),   
        REG(m_LAB_Core->m_regs_pwm, PWM_FIF1), 
        4, 
        0, 
        Utility::mem_bus_addr (mp, cbs[3]);
        0
      }
    },

    .samp_size = 2, // in number of bytes
    .pwm_val   = m_pwm_range, 
    .usecs     = {0, 0},  
    .states    = {0, 0}, 
    .rxd0      = {0}, 
    .rxd1      = {0},
  };

  std::memcpy (static_cast<LAB_OSCILLOSCOPE_DMA_DATA *>(m_uncached_dma_data.virt), &dma_data, sizeof (dma_data));

  uint32_t m_pwm_range       = (LAB_PWM_FREQUENCY * 2) / LAB_LOGIC_ANALYZER_MAX_SAMPLING_RATE;
  m_LAB_Core->pwm_init (LAB_PWM_FREQUENCY, m_pwm_range, PWM_VALUE);   // Initialise PWM, with DMA
  *(Utility::get_reg32 (m_LAB_Core->m_regs_pwm, PWM_DMAC)) = PWM_DMAC_ENAB | PWM_ENAB;

  sampling_rate (LAB_OSCILLOSCOPE_SAMPLING_RATE);

  m_LAB_Core->dma_start (mp, LAB_LOGIC_ANALYZER_DMA_CHAN_GPIO_STORE, &dp->cbs[0], 0);  // Start SPI Tx DMA
  // m_LAB_Core->dma_start(mp, LAB_OSCILLOSCOPE_DMA_CHAN_PWM_PACING, &dp->cbs[7], 0);  // Start PWM DMA, for SPI trigger
}

void LAB_Logic_Analyzer:: 
master_run ()
{
  m_is_running = true;
}

void LAB_Logic_Analyzer:: 
master_stop ()
{
  m_is_running = false;
}
