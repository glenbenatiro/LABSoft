#include "LAB_Oscilloscope.h"

#include <cstdio>
#include <cstring>
#include <bitset>
#include <iostream>

#include "Defaults.h"

LAB_Oscilloscope::
LAB_Oscilloscope (LAB_Core *_LAB_Core) 
  : m_LAB_Core (_LAB_Core),
    m_channel_signals (LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS, LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES, 0)
{
  m_LAB_Core->map_uncached_mem (&m_uncached_dma_data, VC_MEM_SIZE);

  m_LAB_Core->dma_stop(DMA_CHAN_A);
  m_LAB_Core->dma_stop(DMA_CHAN_B);
  m_LAB_Core->dma_stop(DMA_CHAN_C);

  ADC_DMA_DATA *dp  = static_cast<ADC_DMA_DATA *>(m_uncached_dma_data.virt);
  MemoryMap *mp     = &m_uncached_dma_data;

  ADC_DMA_DATA dma_data = {

    // Transfer Information
    // Source Address
    // Destination Address
    // Transfer Length
    // 2D Mode Stride
    // Next Control Block Address
    // Reserved - Set to 0
    
    .cbs = {

      // DMA Control Blocks
      // Page 40
      // Trans. Info., Src. Ad., Dest. Ad., Trans. Length, 2D Stride, Next CB, Reserved

      {DMA_CB_SPI_RX_TI, REG(m_LAB_Core->m_regs_usec, USEC_TIME), MEM(mp, &dp->usecs[0]),                         4, 0, CBS(1), 0}, // 0
      {DMA_CB_SPI_RX_TI, REG(m_LAB_Core->m_regs_spi,  SPI_FIFO),  MEM(mp, dp->rxd1), static_cast<uint32_t>(m_number_of_samples_per_channel*4), 0, CBS(2), 0}, // 1
      {DMA_CB_SPI_RX_TI, REG(m_LAB_Core->m_regs_spi,  SPI_CS),    MEM(mp, &dp->states[0]),                        4, 0, CBS(3), 0}, // 2
      {DMA_CB_SPI_RX_TI, REG(m_LAB_Core->m_regs_usec, USEC_TIME), MEM(mp, &dp->usecs[1]),                         4, 0, CBS(4), 0}, // 3
      {DMA_CB_SPI_RX_TI, REG(m_LAB_Core->m_regs_spi,  SPI_FIFO),  MEM(mp, dp->rxd2), static_cast<uint32_t>(m_number_of_samples_per_channel*4), 0, CBS(5), 0}, // 4
      {DMA_CB_SPI_RX_TI, REG(m_LAB_Core->m_regs_spi,  SPI_CS),    MEM(mp, &dp->states[1]),                        4, 0, CBS(0), 0}, // 5

      // {DMA_CB_SPI_RX_TI, Utility::reg_bus_addr (&(m_LAB_Core->m_regs_usec), USEC_CLO),  Utility::mem_bus_addr (mp, &dp->usecs[0]),  4,                                              0, CBS (1), 0}, // 0
      // {DMA_CB_SPI_RX_TI, Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi),  SPI_FIFO),  Utility::mem_bus_addr (mp, dp->rxd1),       (uint32_t)(m_number_of_samples_per_channel*4),  0, CBS (2), 0}, // 1
      // {DMA_CB_SPI_RX_TI, Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi),  SPI_CS),    Utility::mem_bus_addr (mp, &dp->states[0]), 4,                                              0, CBS (3), 0}, // 2
      // {DMA_CB_SPI_RX_TI, Utility::reg_bus_addr (&(m_LAB_Core->m_regs_usec), USEC_CLO),  Utility::mem_bus_addr (mp, &dp->usecs[1]),  4,                                              0, CBS (4), 0}, // 3
      // {DMA_CB_SPI_RX_TI, Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi),  SPI_FIFO),  Utility::mem_bus_addr (mp, dp->rxd2),       (uint32_t)(m_number_of_samples_per_channel*4),  0, CBS (5), 0}, // 4
      // {DMA_CB_SPI_RX_TI, Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi),  SPI_CS),    Utility::mem_bus_addr (mp, &dp->states[1]), 4,                                              0, CBS (0), 0}, // 5

      // Tx output: 2 data writes to SPI for chan 0 & 1, or both chan 0
      {DMA_CB_SPI_TX_TI,  MEM(mp, dp->txd),        REG(m_LAB_Core->m_regs_spi, SPI_FIFO), 8, 0, CBS(6), 0}, // 6

      // PWM ADC trigger: wait for PWM, set sample length, trigger SPI
      {DMA_CB_PWM_TI, MEM(mp, &dp->pwm_val),   REG(m_LAB_Core->m_pwm_regs, PWM_FIF1), 4, 0, CBS(8), 0}, // 7
      {DMA_CB_PWM_TI, MEM(mp, &dp->samp_size), REG(m_LAB_Core->m_regs_spi, SPI_DLEN), 4, 0, CBS(9), 0}, // 8
      {DMA_CB_PWM_TI, MEM(mp, &dp->adc_csd),   REG(m_LAB_Core->m_regs_spi, SPI_CS),   4, 0, CBS(7), 0}, // 9
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
 
  std::memcpy (static_cast<ADC_DMA_DATA *>(m_uncached_dma_data.virt), &dma_data, sizeof (dma_data));
  
  m_pwm_range       = (LAB_PWM_FREQUENCY * 2) / m_sample_rate;
  m_LAB_Core->pwm_init (LAB_PWM_FREQUENCY, m_pwm_range, PWM_VALUE);   // Initialise PWM, with DMA

  // *REG32(m_LAB_Core->m_pwm_regs, PWM_DMAC) = PWM_DMAC_ENAB | PWM_ENAB;
  //*(g_reg32 (m_LAB_Core->m_pwm_regs, PWM_DMAC)) = PWM_DMAC_ENAB | PWM_ENAB;

  *(Utility::get_reg32 (m_LAB_Core->m_pwm_regs, PWM_DMAC)) = PWM_DMAC_ENAB | PWM_ENAB;

  *REG32(m_LAB_Core->m_regs_spi, SPI_DC) = (8<<24) | (1<<16) | (8<<8) | 1;  // Set DMA priorities
  *REG32(m_LAB_Core->m_regs_spi, SPI_CS) = SPI_CS_CLEAR;                    // Clear SPI FIFOs

  m_LAB_Core->dma_start(mp, DMA_CHAN_C, &dp->cbs[6], 0);  // Start SPI Tx DMA
  m_LAB_Core->dma_start(mp, DMA_CHAN_B, &dp->cbs[0], 0);  // Start SPI Rx DMA
  m_LAB_Core->dma_start(mp, DMA_CHAN_A, &dp->cbs[7], 0);  // Start PWM DMA, for SPI trigger

  // m_LAB_Core->pwm_set_frequency (LAB_OSCILLOSCOPE_SAMPLE_RATE);

  printf ("oscilloscope ok!\n");
}

LAB_Oscilloscope::~LAB_Oscilloscope ()
{
  m_LAB_Core->unmap_periph_mem (&m_uncached_dma_data);
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


void LAB_Oscilloscope:: 
volts_per_division (unsigned channel, double value)
{
  m_channel_signals.m_chans[channel].volts_per_division (value);
}

void LAB_Oscilloscope:: 
vertical_offset (unsigned channel, double value)
{
  m_channel_signals.m_chans[channel].vertical_offset (value);
}

void LAB_Oscilloscope:: 
load_data_samples ()
{
  bool condition2 = true; // fast sampling rates

  uint32_t raw_data_buffer [LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES];
  ADC_DMA_DATA *dp = static_cast<ADC_DMA_DATA *>(m_uncached_dma_data.virt);

  for (int b = 0; b < 2; b++)
  {
    if (dp->states[b])
    {
      std::memcpy (raw_data_buffer, b ? (void *)(dp->rxd2) : (void *)(dp->rxd1), 
        m_number_of_samples_per_channel * 4);

      if (dp->states[b ^ 1])
      {
        dp->states[0] = dp->states[1] = 0;
        break;
      }

      dp->states[b] = 0;
    }
  }

  if (condition2)
  {
    // skip processing of data if all channels are disabled
    // HARD CODED FOR 2 CHANNELS LAB
    if (m_channel_signals.m_chans[0].is_enabled () || 
      m_channel_signals.m_chans[1].is_enabled ())
    {
      double data;
      uint32_t temp1, temp2;
      uint32_t shift_size = 32 / LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS;

      // go through all the samples in one go
      for (int samp = 0; samp < LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES; samp++)
      {
        // distribute the actual samples to each channel from a 32 bit sample
        for (int chan = 0; chan < LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS; chan++)
        {
          if (m_channel_signals.m_chans[chan].is_enabled ())
          {
            temp1 = (raw_data_buffer[samp]) >> (shift_size * chan);

            // rearrange bits to correct order
            // HARD CODED FOR 2 CHANNELS LAB
            temp2 = ((temp1 << 6) | (temp1 >> 10)) & 0x0FFF;

            // get MSB to determine sign
            bool sign = temp2 >> (LAB_OSCILLOSCOPE_ADC_RESOLUTION_BITS - 1);

            // mask temp2 to mask out MSB sign bit
            temp2 = temp2 & ((LAB_OSCILLOSCOPE_ADC_RESOLUTION_INT - 1) >> 1);

            if (sign)
            {
              data = static_cast<double>(temp2) * LAB_OSCILLOSCOPE_ADC_CONVERSION_CONSTANT;
            }
            else 
            {
              data = (static_cast<double>(temp2) * LAB_OSCILLOSCOPE_ADC_CONVERSION_CONSTANT) -  
                  LAB_OSCILLOSCOPE_ADC_REFERENCE_VOLTAGE;
            }

            m_channel_signals.m_chans[chan].osc.voltage_samples[samp] = data;
          }
        }
      }
    }
  }
}

// this changes PWM speed on board!! 
// verify no other are affected
void LAB_Oscilloscope:: 
sample_rate (int channel, double value)
{

  printf ("new sample rate: %f\n", value);
  m_LAB_Core->pwm_set_frequency (value);
}

// EOF