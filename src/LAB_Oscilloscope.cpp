#include "LAB_Oscilloscope.h"

#include <cstdio>
#include <cstring>
#include <bitset>
#include <iostream>
#include <algorithm>

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

    .cbs = {

      // DMA Control Blocks
      // Page 40
      // Trans. Info., Src. Ad., Dest. Ad., Trans. Length, 2D Stride, Next CB, Reserved

      
      {DMA_CB_TI_SPI_RX, Utility::reg_bus_addr (&(m_LAB_Core->m_regs_usec), USEC_CLO),  Utility::mem_bus_addr (mp, &dp->usecs[0]),  4,                                              0, CBS (1), 0}, // 0
      {DMA_CB_TI_SPI_RX, Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi),  SPI_FIFO),  Utility::mem_bus_addr (mp, dp->rxd1),       (uint32_t)(m_number_of_samples_per_channel*4),  0, CBS (2), 0}, // 1
      {DMA_CB_TI_SPI_RX, Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi),  SPI_CS),    Utility::mem_bus_addr (mp, &dp->states[0]), 4,                                              0, CBS (3), 0}, // 2
      {DMA_CB_TI_SPI_RX, Utility::reg_bus_addr (&(m_LAB_Core->m_regs_usec), USEC_CLO),  Utility::mem_bus_addr (mp, &dp->usecs[1]),  4,                                              0, CBS (4), 0}, // 3
      {DMA_CB_TI_SPI_RX, Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi),  SPI_FIFO),  Utility::mem_bus_addr (mp, dp->rxd2),       (uint32_t)(m_number_of_samples_per_channel*4),  0, CBS (5), 0}, // 4
      {DMA_CB_TI_SPI_RX, Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi),  SPI_CS),    Utility::mem_bus_addr (mp, &dp->states[1]), 4,                                              0, CBS (0), 0}, // 5

      // {DMA_CB_TI_SPI_RX, REG(m_LAB_Core->m_regs_usec, USEC_TIME), MEM(mp, &dp->usecs[0]),                         4, 0, CBS(1), 0}, // 0
      // {DMA_CB_TI_SPI_RX, REG(m_LAB_Core->m_regs_spi,  SPI_FIFO),  MEM(mp, dp->rxd1), static_cast<uint32_t>(m_number_of_samples_per_channel*4), 0, CBS(2), 0}, // 1
      // {DMA_CB_TI_SPI_RX, REG(m_LAB_Core->m_regs_spi,  SPI_CS),    MEM(mp, &dp->states[0]),                        4, 0, CBS(3), 0}, // 2
      // {DMA_CB_TI_SPI_RX, REG(m_LAB_Core->m_regs_usec, USEC_TIME), MEM(mp, &dp->usecs[1]),                         4, 0, CBS(4), 0}, // 3
      // {DMA_CB_TI_SPI_RX, REG(m_LAB_Core->m_regs_spi,  SPI_FIFO),  MEM(mp, dp->rxd2), static_cast<uint32_t>(m_number_of_samples_per_channel*4), 0, CBS(5), 0}, // 4
      // {DMA_CB_TI_SPI_RX, REG(m_LAB_Core->m_regs_spi,  SPI_CS),    MEM(mp, &dp->states[1]),                        4, 0, CBS(0), 0}, // 5

      // Tx output: 2 data writes to SPI for chan 0 & 1, or both chan 0
      {DMA_CB_TI_SPI_TX,  MEM(mp, dp->txd),        REG(m_LAB_Core->m_regs_spi, SPI_FIFO), 8, 0, CBS(6), 0}, // 6

      // PWM ADC trigger: wait for PWM, set sample length, trigger SPI
      {DMA_CB_TI_PWM, MEM(mp, &dp->pwm_val),   REG(m_LAB_Core->m_regs_pwm, PWM_FIF1), 4, 0, CBS(8), 0}, // 7
      {DMA_CB_TI_PWM, MEM(mp, &dp->samp_size), REG(m_LAB_Core->m_regs_spi, SPI_DLEN), 4, 0, CBS(9), 0}, // 8
      {DMA_CB_TI_PWM, MEM(mp, &dp->adc_csd),   REG(m_LAB_Core->m_regs_spi, SPI_CS),   4, 0, CBS(7), 0}, // 9
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
  
  // m_LAB_Core->pwm_init (LAB_PWM_FREQUENCY, m_pwm_range, PWM_VALUE);   // Initialise PWM, with DMA
  m_LAB_Core->pwm_init (LAB_PWM_FREQUENCY, m_pwm_range, PWM_VALUE);   // Initialise PWM, with DMA

  // *REG32(m_LAB_Core->m_regs_pwm, PWM_DMAC) = PWM_DMAC_ENAB | PWM_ENAB;
  //*(g_reg32 (m_LAB_Core->m_regs_pwm, PWM_DMAC)) = PWM_DMAC_ENAB | PWM_ENAB;

  *(Utility::get_reg32 (m_LAB_Core->m_regs_pwm, PWM_DMAC)) = PWM_DMAC_ENAB | PWM_ENAB;

  *REG32(m_LAB_Core->m_regs_spi, SPI_DC) = (8<<24) | (1<<16) | (8<<8) | 1;  // Set DMA priorities
  *REG32(m_LAB_Core->m_regs_spi, SPI_CS) = SPI_CS_CLEAR;                    // Clear SPI FIFOs

  

  // *(Utility::get_reg32 (m_LAB_Core->m_regs_pwm, PWM_DMAC)) = PWM_DMAC_ENAB | PWM_ENAB;


  // // Configure SPI DC Register
  // *(Utility::get_reg32 (m_LAB_Core->m_regs_spi, SPI_DC)) = 
  //   (8 << 24) | (1 << 16) | (8 << 8) | 1;

  // Clear SPI FIFO
  m_LAB_Core->spi_clear_fifo ();

  m_LAB_Core->dma_start(mp, DMA_CHAN_C, &dp->cbs[6], 0);  // Start SPI Tx DMA
  m_LAB_Core->dma_start(mp, DMA_CHAN_B, &dp->cbs[0], 0);  // Start SPI Rx DMA
  m_LAB_Core->dma_start(mp, DMA_CHAN_A, &dp->cbs[7], 0);  // Start PWM DMA, for SPI trigger

  // Start SPI TX DMA
  // m_LAB_Core->dma_start (DMA_CHAN_C, Utility::mem_bus_addr (mp, &(dp->cbs[6])));

  // // Start SPI RX DMA
  // m_LAB_Core->dma_start (DMA_CHAN_B, Utility::mem_bus_addr (mp, &(dp->cbs[0])));

  // // Start PWM DMA, for SPI trigger
  // m_LAB_Core->dma_start (DMA_CHAN_A, Utility::mem_bus_addr (mp, &(dp->cbs[7])));

  // m_LAB_Core->pwm_set_frequency (LAB_OSCILLOSCOPE_SAMPLE_RATE);

  // ---

  // {DMA_CB_TI_SPI_RX, REG(m_LAB_Core->m_regs_usec, USEC_TIME), MEM(mp, &dp->usecs[0]),                         4, 0, CBS(1), 0}, // 0
  // {DMA_CB_TI_SPI_RX, REG(m_LAB_Core->m_regs_spi,  SPI_FIFO),  MEM(mp, dp->rxd1), static_cast<uint32_t>(m_number_of_samples_per_channel*4), 0, CBS(2), 0}, // 1
  // {DMA_CB_TI_SPI_RX, REG(m_LAB_Core->m_regs_spi,  SPI_CS),    MEM(mp, &dp->states[0]),                        4, 0, CBS(3), 0}, // 2
  // {DMA_CB_TI_SPI_RX, REG(m_LAB_Core->m_regs_usec, USEC_TIME), MEM(mp, &dp->usecs[1]),                         4, 0, CBS(4), 0}, // 3
  // {DMA_CB_TI_SPI_RX, REG(m_LAB_Core->m_regs_spi,  SPI_FIFO),  MEM(mp, dp->rxd2), static_cast<uint32_t>(m_number_of_samples_per_channel*4), 0, CBS(5), 0}, // 4
  // {DMA_CB_TI_SPI_RX, REG(m_LAB_Core->m_regs_spi,  SPI_CS),    MEM(mp, &dp->states[1]),                        4, 0, CBS(0), 0}, // 5

  // *REG32(m_LAB_Core->m_regs_spi, SPI_DC) = (8<<24) | (1<<16) | (8<<8) | 1;  // Set DMA priorities
  // *REG32(m_LAB_Core->m_regs_spi, SPI_CS) = SPI_CS_CLEAR;                    // Clear SPI FIFOs

  // m_LAB_Core->dma_start(DMA_CHAN_C, &dp->cbs[6], 0);  // Start SPI Tx DMA
  // m_LAB_Core->dma_start(DMA_CHAN_B, &dp->cbs[0], 0);  // Start SPI Rx DMA
  // m_LAB_Core->dma_start(DMA_CHAN_A, &dp->cbs[7], 0);  // Start PWM DMA, for SPI trigger
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
  m_channel_signals.m_chans[channel].osc.volts_per_division = value;
}

void LAB_Oscilloscope:: 
vertical_offset (unsigned channel, double value)
{
  m_channel_signals.m_chans[channel].osc.vertical_offset = value;
}

void LAB_Oscilloscope:: 
load_data_samples ()
{
  // auto start = std::chrono::steady_clock::now ();

  uint32_t raw_data_buffer [LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES];
  ADC_DMA_DATA *dp = static_cast<ADC_DMA_DATA *>(m_uncached_dma_data.virt);

  if (m_channel_signals.m_chans[0].osc.osc_disp_mode == SCREEN)
  {
    if (dp->states[m_curr_screen_buffer])
    {
      // detect overrun
      if (dp->states[m_curr_screen_buffer ^ 1])
      {
        dp->states[0] = dp->states[1] = 0;
      }

      dp->states[m_curr_screen_buffer] = 0;
      m_curr_screen_buffer ^= 1;
    }
    
    std::memcpy (raw_data_buffer, m_curr_screen_buffer ? (void *)(dp->rxd2) : 
      (void *)(dp->rxd1), m_number_of_samples_per_channel * 4);
  }
  else if (m_channel_signals.m_chans[0].osc.osc_disp_mode == REPEATED)
  {
    for (int b = 0; b < 2; b++)
    {
      if (dp->states[b])
      {
        std::memcpy (raw_data_buffer, b ? (void *)(dp->rxd2) : (void *)(dp->rxd1), 
          m_number_of_samples_per_channel * 4);

        if (dp->states[b ^ 1])
        {
          dp->states[0] = dp->states[1] = 0;
          // overrun
          break;
        }

        dp->states[b] = 0;

        // if (usec_start == 0)
        // {
        //   usec_start = dp->usecs[b];
        // }
      }
    }
  }

  // skip processing of data if all channels are disabled
  // HARD CODED FOR 2 CHANNELS LAB
  if (m_channel_signals.m_chans[0].is_enabled () || 
    m_channel_signals.m_chans[1].is_enabled ())
  {
    double data;
    uint32_t shift_size = 32 / LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS;

    // go through all the samples in one go
    for (int samp = 0; samp < LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES; samp++)
    {
      // distribute the actual samples to each channel from a 32 bit sample
      for (int chan = 0; chan < LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS; chan++)
      {
        if (m_channel_signals.m_chans[chan].is_enabled ())
        {
          // START hard coded

          uint16_t temp1 = (raw_data_buffer[samp]) >> (16 * chan);
                    
          uint16_t temp2 = ((temp1 << 6) | (temp1 >> 10)) & 0x0FFF;  

          // END hard coded

          // if (samp == 0)
          // {
          //   std::cout << "temp1: " << 
          //   std::bitset<8> (temp1 >> 24) << " " <<
          //   std::bitset<8> (temp1 >> 16) << " " <<
          //   std::bitset<8> (temp1 >> 8) << " " << 
          //   std::bitset<8> (temp1) << "\n";
          // }      

          // if (samp == 0)
          // {
          //   std::cout << "temp2: " << 
          //   std::bitset<8> (temp2 >> 24) << " " <<
          //   std::bitset<8> (temp2 >> 16) << " " <<
          //   std::bitset<8> (temp2 >> 8) << " " << 
          //   std::bitset<8> (temp2) << "\n\n";
          // }      

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

          // if (samp == 10)
          // {
          //   printf ("data: %.9f\n", data);
          // }
        }
      }
    }
  }
  

  // auto end = std::chrono::steady_clock::now ();
  // std::chrono::duration<double, std::milli> elapsed = end - start;
  // std::cout << "Duration: " << elapsed.count () << " ms\n";
}

// this changes PWM speed on board!! 
// verify no other are affected
void LAB_Oscilloscope:: 
sampling_rate (int channel, double value)
{
  // static_cast<ADC_DMA_DATA *>(m_uncached_dma_data.virt)->pwm_val = 
  //   (LAB_PWM_FREQUENCY * 2) / value;

  m_LAB_Core->pwm_set_frequency (value);
}

void LAB_Oscilloscope:: 
time_per_division (unsigned channel, double value, unsigned osc_disp_num_cols)
{
  Channel_Signal_Oscilloscope *osc = &(m_channel_signals.m_chans[channel].osc);

  if (value >= LABSOFT_OSC_DISP_MIN_REPEATED_SAMP_PERIOD)
  {
    display_mode (channel, SCREEN);
    

    // std::fill(static_cast<ADC_DMA_DATA *>(m_uncached_dma_data.virt)->rxd1, 
    //   (static_cast<ADC_DMA_DATA *>(m_uncached_dma_data.virt)->rxd1) + LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES,
    //   0);

    // std::fill(static_cast<ADC_DMA_DATA *>(m_uncached_dma_data.virt)->rxd2, 
    //   (static_cast<ADC_DMA_DATA *>(m_uncached_dma_data.virt)->rxd2) + LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES,
    //   0);


  }
  else 
  {
    display_mode (channel, REPEATED);
  }

  double new_sampling_rate = (osc->samples) / (value * osc_disp_num_cols);

  osc->sampling_rate      = new_sampling_rate;
  osc->time_per_division  = value;

  sampling_rate (channel, new_sampling_rate);
}

// EOF