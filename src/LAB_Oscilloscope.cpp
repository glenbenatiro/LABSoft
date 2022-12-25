#include "LAB_Oscilloscope.h"

#include <cstdio>
#include <cstring>

#include "Defaults.h"

LAB_Oscilloscope::
LAB_Oscilloscope (LAB_Core *_LAB_Core) 
  : m_LAB_Core (_LAB_Core),
    m_channel_signals (LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS, 0, 0)
{
  m_LAB_Core->dma_stop(DMA_CHAN_A);
  m_LAB_Core->dma_stop(DMA_CHAN_B);
  m_LAB_Core->dma_stop(DMA_CHAN_C);

  m_pwm_range = (LAB_PWM_FREQUENCY * 2) / m_sample_rate;

  ADC_DMA_DATA *dp = static_cast<ADC_DMA_DATA*>(m_LAB_Core->m_vc_mem.virt);
  MemoryMap *mp = &(m_LAB_Core->m_vc_mem);

  ADC_DMA_DATA dma_data = {

    // Transfer Information
    // Source Address
    // Destination Address
    // Transfer Length
    // 2D Mode Stride
    // Next Control Block Address
    // Reserved - Set to 0
    
    .cbs = {
      // Rx input: read data from usec clock and SPI, into 2 ping-pong buffers
      

      // control block group to receive data from ADC (SPI_RX)



      {DMA_CB_SPI_RX_TI, REG(m_LAB_Core->m_usec_regs, USEC_TIME), MEM(mp, &dp->usecs[0]),                         4, 0, CBS(1), 0}, // 0
      {DMA_CB_SPI_RX_TI, REG(m_LAB_Core->m_spi_regs,  SPI_FIFO),  MEM(mp, dp->rxd1), static_cast<uint32_t>(m_number_of_samples_per_channel*4), 0, CBS(2), 0}, // 1
      {DMA_CB_SPI_RX_TI, REG(m_LAB_Core->m_spi_regs,  SPI_CS),    MEM(mp, &dp->states[0]),                        4, 0, CBS(3), 0}, // 2
      {DMA_CB_SPI_RX_TI, REG(m_LAB_Core->m_usec_regs, USEC_TIME), MEM(mp, &dp->usecs[1]),                         4, 0, CBS(4), 0}, // 3
      {DMA_CB_SPI_RX_TI, REG(m_LAB_Core->m_spi_regs,  SPI_FIFO),  MEM(mp, dp->rxd2), static_cast<uint32_t>(m_number_of_samples_per_channel*4), 0, CBS(5), 0}, // 4
      {DMA_CB_SPI_RX_TI, REG(m_LAB_Core->m_spi_regs,  SPI_CS),    MEM(mp, &dp->states[1]),                        4, 0, CBS(0), 0}, // 5

      // Tx output: 2 data writes to SPI for chan 0 & 1, or both chan 0
      {DMA_CB_SPI_TX_TI,  MEM(mp, dp->txd),        REG(m_LAB_Core->m_spi_regs, SPI_FIFO), 8, 0, CBS(6), 0}, // 6

      // PWM ADC trigger: wait for PWM, set sample length, trigger SPI
      {DMA_CB_PWM_TI, MEM(mp, &dp->pwm_val),   REG(m_LAB_Core->m_pwm_regs, PWM_FIF1), 4, 0, CBS(8), 0}, // 7
      {DMA_CB_PWM_TI, MEM(mp, &dp->samp_size), REG(m_LAB_Core->m_spi_regs, SPI_DLEN), 4, 0, CBS(9), 0}, // 8
      {DMA_CB_PWM_TI, MEM(mp, &dp->adc_csd),   REG(m_LAB_Core->m_spi_regs, SPI_CS),   4, 0, CBS(7), 0}, // 9
    },

    .samp_size = 2, // in number of bytes
    .pwm_val   = m_pwm_range, 
    .adc_csd   = SPI_CS_TA | SPI_CS_ADCS | SPI_CS_DMAEN | SPI_CS_CLEAR | LAB_OSCILLOSCOPE_ADC_CE,
    .txd       = {0xd0, static_cast<uint32_t>(m_number_of_channels > 1 ? 0xf0 : 0xd0)},
    .usecs     = {0, 0},  
    .states    = {0, 0}, 
    .rxd1      = {0}, 
    .rxd2      = {0},
  };
 
  //if (single)                                 // If single-shot, stop after first Rx block
    //dma_data.cbs[2].next_cb = 0;

  memcpy(dp, &dma_data, sizeof(dma_data));

  m_LAB_Core->pwm_init (LAB_PWM_FREQUENCY, m_pwm_range, PWM_VALUE);   // Initialise PWM, with DMA

  // *REG32(m_LAB_Core->m_pwm_regs, PWM_DMAC) = PWM_DMAC_ENAB | PWM_ENAB;
  *(g_reg32 (m_LAB_Core->m_pwm_regs, PWM_DMAC)) = PWM_DMAC_ENAB | PWM_ENAB;

  *REG32(m_LAB_Core->m_spi_regs, SPI_DC) = (8<<24) | (1<<16) | (8<<8) | 1;  // Set DMA priorities
  *REG32(m_LAB_Core->m_spi_regs, SPI_CS) = SPI_CS_CLEAR;                    // Clear SPI FIFOs

  m_LAB_Core->dma_start(mp, DMA_CHAN_C, &dp->cbs[6], 0);  // Start SPI Tx DMA
  m_LAB_Core->dma_start(mp, DMA_CHAN_B, &dp->cbs[0], 0);  // Start SPI Rx DMA
  m_LAB_Core->dma_start(mp, DMA_CHAN_A, &dp->cbs[7], 0);  // Start PWM DMA, for SPI trigger

  // m_LAB_Core->pwm_set_frequency (LAB_OSCILLOSCOPE_SAMPLE_RATE);

  printf ("oscilloscope ok!\n");
}

LAB_Oscilloscope::~LAB_Oscilloscope ()
{
  
}

void LAB_Oscilloscope::
run ()
{
  m_LAB_Core->pwm_start ();
  m_is_running = true;
}

void LAB_Oscilloscope::
stop ()
{
  m_LAB_Core->pwm_stop ();
  m_is_running = false;
}


// this changes PWM speed on board!! 
// verify no other are affected
void LAB_Oscilloscope:: 
sample_rate (double value)
{
  printf ("new sample rate: %f\n", value);
  m_LAB_Core->pwm_set_frequency (value);
}

// EOF