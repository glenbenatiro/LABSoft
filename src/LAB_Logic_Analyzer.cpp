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
  



  //   m_LAB_Core->map_uncached_mem (&m_uncached_adc_dma_data, VC_MEM_SIZE);

  // m_LAB_Core->dma_stop(LAB_OSCILLOSCOPE_DMA_CHAN_PWM_PACING);
  // m_LAB_Core->dma_stop(LAB_OSCILLOSCOPE_DMA_CHAN_SPI_RX);
  // m_LAB_Core->dma_stop(LAB_OSCILLOSCOPE_DMA_CHAN_SPI_TX);

  // LAB_OSCILLOSCOPE_DMA_DATA *dp  = static_cast<LAB_OSCILLOSCOPE_DMA_DATA *>(m_uncached_adc_dma_data.virt);
  // MemoryMap *mp     = &m_uncached_adc_dma_data;

  // LAB_OSCILLOSCOPE_DMA_DATA adc_dma_data = 
  // {
  //   .cbs = 
  //   {
  //     // control blocks for SPI_RX dual buffer
  //     { // CB 0
  //       DMA_CB_TI_SPI_RX,
  //       Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi),  SPI_FIFO),  
  //       Utility::mem_bus_addr (mp, dp->rxd0),       
  //       (uint32_t)(m_number_of_samples_per_channel*4),  
  //       0, 
  //       CBS (1),
  //       0
  //     },
  //     { // CB 1
  //       DMA_CB_TI_SPI_RX, 
  //       Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi),  SPI_CS),
  //       Utility::mem_bus_addr (mp, &dp->states[0]),
  //       4,
  //       0,
  //       CBS (2),
  //       0
  //     }, 
  //     { // CB 2
  //       DMA_CB_TI_SPI_RX, 
  //       Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi),  SPI_FIFO),
  //       Utility::mem_bus_addr (mp, dp->rxd1),       
  //       (uint32_t)(m_number_of_samples_per_channel*4),  
  //       0, 
  //       CBS (3), 
  //       0
  //     }, 
  //     { // CB 3
  //       DMA_CB_TI_SPI_RX, 
  //       Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi),  SPI_CS),    
  //       Utility::mem_bus_addr (mp, &dp->states[1]), 
  //       4,                                              
  //       0, 
  //       CBS (0), 
  //       0
  //     }, 


  //     // control blocks for SPI_RX single buffer
  //     { // CB 4
  //       DMA_CB_TI_SPI_RX, 
  //       Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi),  SPI_FIFO),
  //       Utility::mem_bus_addr (mp, dp->rxd0),       
  //       (uint32_t)(m_number_of_samples_per_channel*4),  
  //       0, 
  //       CBS (5), 
  //       0
  //     },  
  //     { // CB 5
  //       DMA_CB_TI_SPI_RX, 
  //       Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi),  SPI_CS),    
  //       Utility::mem_bus_addr (mp, &dp->states[1]), 
  //       4,                                              
  //       0, 
  //       CBS (4), 
  //       0
  //     },  


  //     // Control Blocks for SPI TX
  //     { // CB 6
  //       DMA_CB_TI_SPI_TX,  
  //       MEM(mp, dp->txd),        
  //       REG(m_LAB_Core->m_regs_spi, SPI_FIFO), 
  //       8, 
  //       0, 
  //       CBS(6), 
  //       0
  //     }, 


  //     // PWM ADC trigger: wait for PWM, set sample length, trigger SPI
  //     { //CB 7
  //       DMA_CB_TI_PWM, 
  //       MEM(mp, &dp->pwm_val),   
  //       REG(m_LAB_Core->m_regs_pwm, PWM_FIF1), 
  //       4, 
  //       0, 
  //       CBS(8), 
  //       0
  //     }, 
  //     { // CB 8
  //       DMA_CB_TI_PWM, 
  //       MEM(mp, &dp->samp_size), 
  //       REG(m_LAB_Core->m_regs_spi, SPI_DLEN), 
  //       4, 
  //       0, 
  //       CBS(9), 
  //       0
  //     },
  //     { // CB 9
  //       DMA_CB_TI_PWM, 
  //       MEM(mp, &dp->adc_csd),   
  //       REG(m_LAB_Core->m_regs_spi, SPI_CS),   
  //       4, 
  //       0, 
  //       CBS(7), 
  //       0
  //     }, 
  //   },

  //   .samp_size = 2, // in number of bytes
  //   .pwm_val   = m_pwm_range, 
  //   .adc_csd   = SPI_CS_TA | SPI_CS_ADCS | SPI_CS_DMAEN | SPI_CS_CLEAR | LAB_OSCILLOSCOPE_ADC_CE,
  //   .txd       = {0xd0, static_cast<uint32_t>(m_number_of_channels > 1 ? 0xf0 : 0xd0)},
  //   .usecs     = {0, 0},  
  //   .states    = {0, 0}, 
  //   .rxd0      = {0}, 
  //   .rxd1      = {0},
  // };

 
  // std::memcpy (static_cast<LAB_OSCILLOSCOPE_DMA_DATA *>(m_uncached_adc_dma_data.virt), &adc_dma_data, sizeof (adc_dma_data));

  // m_pwm_range       = (LAB_PWM_FREQUENCY * 2) / LAB_OSCILLOSCOPE_MAX_SAMPLING_RATE;
  // m_LAB_Core->pwm_init (LAB_PWM_FREQUENCY, m_pwm_range, PWM_VALUE);   // Initialise PWM, with DMA
  // *(Utility::get_reg32 (m_LAB_Core->m_regs_pwm, PWM_DMAC)) = PWM_DMAC_ENAB | PWM_ENAB;
  // *REG32(m_LAB_Core->m_regs_spi, SPI_DC) = (8<<24) | (1<<16) | (8<<8) | 1;  // Set DMA priorities
  // *REG32(m_LAB_Core->m_regs_spi, SPI_CS) = SPI_CS_CLEAR;                    // Clear SPI FIFOs

  // sampling_rate (LAB_OSCILLOSCOPE_SAMPLING_RATE);

  // // Clear SPI FIFO
  // m_LAB_Core->spi_clear_fifo ();

  // m_LAB_Core->dma_start(mp, LAB_OSCILLOSCOPE_DMA_CHAN_SPI_TX, &dp->cbs[6], 0);  // Start SPI Tx DMA
  // m_LAB_Core->dma_start(mp, LAB_OSCILLOSCOPE_DMA_CHAN_SPI_RX, &dp->cbs[0], 0);  // Start SPI Rx DMA
  // m_LAB_Core->dma_start(mp, LAB_OSCILLOSCOPE_DMA_CHAN_PWM_PACING, &dp->cbs[7], 0);  // Start PWM DMA, for SPI trigger

  // // m_LAB_Core->dma_start(LAB_OSCILLOSCOPE_DMA_CHAN_SPI_TX, &dp->cbs[6], 0);  // Start SPI Tx DMA
  // // m_LAB_Core->dma_start(LAB_OSCILLOSCOPE_DMA_CHAN_SPI_RX, &dp->cbs[0], 0);  // Start SPI Rx DMA
  // // m_LAB_Core->dma_start(LAB_OSCILLOSCOPE_DMA_CHAN_PWM_PACING, &dp->cbs[7], 0);  // Start PWM DMA, for SPI trigger
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
