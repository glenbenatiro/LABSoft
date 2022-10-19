#include "LAB_Oscilloscope.h"

#include <cstring>

#include "Defaults.h"

LAB_Oscilloscope::
LAB_Oscilloscope (LAB_Core *_LAB_Core) 
  : m_LAB_Core (_LAB_Core),
    m_channel_signals (LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS, 0, 0)
{
  // refresh from last. this is a RESET DMA command
  m_LAB_Core->LAB_Core_dma_stop(DMA_CHAN_A);
  m_LAB_Core->LAB_Core_dma_stop(DMA_CHAN_B);
  m_LAB_Core->LAB_Core_dma_stop(DMA_CHAN_C);

  m_pwm_range = (PWM_FREQ * 2) / m_sample_rate;
  printf ("m_pwm_range: %d\n", m_pwm_range);


  MEM_MAP *mp = &(m_LAB_Core->m_vc_mem);
  ADC_DMA_DATA *dp = static_cast<ADC_DMA_DATA*>(mp->virt);

  ADC_DMA_DATA dma_data = {
    .cbs = {
      // Rx input: read data from usec clock and SPI, into 2 ping-pong buffers
      {SPI_RX_TI, REG(m_LAB_Core->m_usec_regs, USEC_TIME), MEM(mp, &dp->usecs[0]),                         4, 0, CBS(1), 0}, // 0
      {SPI_RX_TI, REG(m_LAB_Core->m_spi_regs,  SPI_FIFO),  MEM(mp, dp->rxd1), static_cast<uint32_t>(m_sample_count*4), 0, CBS(2), 0}, // 1
      {SPI_RX_TI, REG(m_LAB_Core->m_spi_regs,  SPI_CS),    MEM(mp, &dp->states[0]),                        4, 0, CBS(3), 0}, // 2
      {SPI_RX_TI, REG(m_LAB_Core->m_usec_regs, USEC_TIME), MEM(mp, &dp->usecs[1]),                         4, 0, CBS(4), 0}, // 3
      {SPI_RX_TI, REG(m_LAB_Core->m_spi_regs,  SPI_FIFO),  MEM(mp, dp->rxd2), static_cast<uint32_t>(m_sample_count*4), 0, CBS(5), 0}, // 4
      {SPI_RX_TI, REG(m_LAB_Core->m_spi_regs,  SPI_CS),    MEM(mp, &dp->states[1]),                        4, 0, CBS(0), 0}, // 5

      // Tx output: 2 data writes to SPI for chan 0 & 1, or both chan 0
      {SPI_TX_TI, MEM(mp, dp->txd),          REG(m_LAB_Core->m_spi_regs, SPI_FIFO), 8, 0, CBS(6), 0}, // 6

      // PWM ADC trigger: wait for PWM, set sample length, trigger SPI
      {PWM_TI,    MEM(mp, &dp->pwm_val),     REG(m_LAB_Core->m_pwm_regs, PWM_FIF1), 4, 0, CBS(8), 0}, // 7
      {PWM_TI,    MEM(mp, &dp->samp_size),   REG(m_LAB_Core->m_spi_regs, SPI_DLEN), 4, 0, CBS(9), 0}, // 8
      {PWM_TI,    MEM(mp, &dp->adc_csd),     REG(m_LAB_Core->m_spi_regs, SPI_CS),   4, 0, CBS(7), 0}, // 9
    },
    .samp_size = 2, 
    .pwm_val   = m_pwm_range, 
    .adc_csd   = SPI_TFR_ACT | SPI_AUTO_CS | SPI_DMA_EN | SPI_FIFO_CLR | ADC_CE_NUM,
    .txd       = {0xd0, static_cast<uint32_t>(m_number_of_channels > 1 ? 0xf0 : 0xd0)},
    .usecs     = {0, 0},  
    .states    = {0, 0}, 
    .rxd1      = {0}, 
    .rxd2      = {0},
  };
  
  printf ("oscilloscope 2!\n");

  //if (single)                                 // If single-shot, stop after first Rx block
    //dma_data.cbs[2].next_cb = 0;

  printf ("size of dma data: %d, size samps mp: %\n", sizeof (dma_data), mp->size);
  printf ("add of virt: %p\n", dp);

  memcpy (dp, &dma_data, sizeof(dma_data));    // Copy DMA data into uncached memory

  printf ("oscilloscope 2!\n");

  m_LAB_Core->LAB_Core_pwm_init (PWM_FREQ, m_pwm_range, PWM_VALUE);   // Initialise PWM, with DMA

  *REG32(m_LAB_Core->m_pwm_regs, PWM_DMAC) = PWM_DMAC_ENAB | PWM_ENAB;
  *REG32(m_LAB_Core->m_spi_regs, SPI_DC) = (8<<24) | (1<<16) | (8<<8) | 1;  // Set DMA priorities
  *REG32(m_LAB_Core->m_spi_regs, SPI_CS) = SPI_FIFO_CLR;                    // Clear SPI FIFOs

  m_LAB_Core->LAB_Core_dma_start(mp, DMA_CHAN_C, &dp->cbs[6], 0);  // Start SPI Tx DMA
  m_LAB_Core->LAB_Core_dma_start(mp, DMA_CHAN_B, &dp->cbs[0], 0);  // Start SPI Rx DMA
  m_LAB_Core->LAB_Core_dma_start(mp, DMA_CHAN_A, &dp->cbs[7], 0);  // Start PWM DMA, for SPI trigger

  printf ("oscilloscope ok!\n");
}

LAB_Oscilloscope::~LAB_Oscilloscope ()
{
  
}

int 
LAB_Oscilloscope::stream_values ()

{
  uint32_t  i, 
            n, 
            usec, 
            slen = 0;

  //ADC_DMA_DATA *dp = static_cast<ADC_DMA_DATA*>(m_LAB_Core->m_vc_mem.virt);
  ADC_DMA_DATA *dp = (ADC_DMA_DATA*)(m_LAB_Core->m_vc_mem.virt);
  
  for (n = 0; n < 2 && slen == 0; n++)
    {
      if (dp->states[n])
      {
          //m_samp_total += m_sample_count;
          // memcpy (m_rx_buff, n ? (void *)dp->rxd2 : (void *)dp->rxd1, m_sample_count * 4);
          // usec = dp->usecs[n];

          // if (dp->states[n^1])
          //   {
          //     dp->states[0] = dp->states[1] = 0;
          //     m_overrun_total++;
          //     break;
          //   }
            
          // dp->states[n] = 0;

          // if (m_usec_start == 0)
          //   m_usec_start = usec;

    
          // for (int i=0; i<m_sample_count*4; i++)
          //   printf("%02X ", *(((uint8_t *)m_rx_buff)+i));
              
          // printf("\n");
          
        }
    }

  return 0;
}

void LAB_Oscilloscope::
run ()
{
  m_LAB_Core->LAB_Core_pwm_start ();
}

void LAB_Oscilloscope::
stop ()
{
  m_LAB_Core->LAB_Core_pwm_stop ();
}

// EOF