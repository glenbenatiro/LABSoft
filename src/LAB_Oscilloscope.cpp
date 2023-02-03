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

  m_LAB_Core->dma_stop(DMA_CHAN_PWM_PACING);
  m_LAB_Core->dma_stop(DMA_CHAN_SPI_RX);
  m_LAB_Core->dma_stop(DMA_CHAN_SPI_TX);

  ADC_DMA_DATA *dp  = static_cast<ADC_DMA_DATA *>(m_uncached_dma_data.virt);
  MemoryMap *mp     = &m_uncached_dma_data;

  // Construct dual buffer (ping-pong buffer) ADC_DMA_DATA
  m_dma_data_dual_buffer = 
  {
    .cbs = 
    {
      // Control blocks for SPI RX
      { // CB 0
        DMA_CB_TI_SPI_RX, 
        Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi),  SPI_FIFO),  
        Utility::mem_bus_addr (mp, dp->rxd0),       
        (uint32_t)(m_number_of_samples_per_channel*4),  
        0, 
        CBS (1),
        0
      },
      { // CB 1
        DMA_CB_TI_SPI_RX, 
        Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi),  SPI_CS),
        Utility::mem_bus_addr (mp, &dp->states[0]),
        4,
        0,
        CBS (2),
        0
      }, 
      { // CB 2
        DMA_CB_TI_SPI_RX, 
        Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi),  SPI_FIFO),
        Utility::mem_bus_addr (mp, dp->rxd1),       
        (uint32_t)(m_number_of_samples_per_channel*4),  
        0, 
        CBS (3), 
        0
      }, 
      { // CB 3
        DMA_CB_TI_SPI_RX, 
        Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi),  SPI_CS),    
        Utility::mem_bus_addr (mp, &dp->states[1]), 
        4,                                              
        0, 
        CBS (0), 
        0
      },  

      // Control Blocks for SPI TX
      { // CB 4
        DMA_CB_TI_SPI_TX,  
        MEM(mp, dp->txd),        
        REG(m_LAB_Core->m_regs_spi, SPI_FIFO), 
        8, 
        0, 
        CBS(4), 
        0
      }, 

      // PWM ADC trigger: wait for PWM, set sample length, trigger SPI
      { //CB 5
        DMA_CB_TI_PWM, 
        MEM(mp, &dp->pwm_val),   
        REG(m_LAB_Core->m_regs_pwm, PWM_FIF1), 
        4, 
        0, 
        CBS(6), 
        0
      }, 
      { // CB 6
        DMA_CB_TI_PWM, 
        MEM(mp, &dp->samp_size), 
        REG(m_LAB_Core->m_regs_spi, SPI_DLEN), 
        4, 
        0, 
        CBS(7), 
        0
      },
      { // CB 7
        DMA_CB_TI_PWM, 
        MEM(mp, &dp->adc_csd),   
        REG(m_LAB_Core->m_regs_spi, SPI_CS),   
        4, 
        0, 
        CBS(5), 
        0
      }, 
    },

    .samp_size = 2, // in number of bytes
    .pwm_val   = m_pwm_range, 
    .adc_csd   = SPI_CS_TA | SPI_CS_ADCS | SPI_CS_DMAEN | SPI_CS_CLEAR | LAB_OSCILLOSCOPE_ADC_CE,
    .txd       = {0xd0, static_cast<uint32_t>(m_number_of_channels > 1 ? 0xf0 : 0xd0)},
    .usecs     = {0, 0},  
    .states    = {0, 0}, 
    .rxd0      = {0}, 
    .rxd1      = {0},
  };

  // Construct single buffer ADC_DMA_DATA
  m_dma_data_single_buffer = 
  {
    .cbs = 
    {
      // Control blocks for SPI RX
      { // CB 0
        DMA_CB_TI_SPI_RX, 
        Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi),  SPI_FIFO),  
        Utility::mem_bus_addr (mp, dp->rxd0),       
        (uint32_t)(m_number_of_samples_per_channel*4),  
        0, 
        CBS (1),
        0
      },
      { // CB 1
        DMA_CB_TI_SPI_RX, 
        Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi),  SPI_CS),
        Utility::mem_bus_addr (mp, &dp->states[0]),
        4,
        0,
        CBS (0),
        0
      }, 
      { // CB 2
        DMA_CB_TI_SPI_RX, 
        Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi),  SPI_FIFO),
        Utility::mem_bus_addr (mp, dp->rxd1),       
        (uint32_t)(m_number_of_samples_per_channel*4),  
        0, 
        CBS (3), 
        0
      }, 
      { // CB 3
        DMA_CB_TI_SPI_RX, 
        Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi),  SPI_CS),    
        Utility::mem_bus_addr (mp, &dp->states[1]), 
        4,                                              
        0, 
        CBS (2), 
        0
      },  

      // Control Blocks for SPI TX
      { // CB 4
        DMA_CB_TI_SPI_TX,  
        MEM(mp, dp->txd),        
        REG(m_LAB_Core->m_regs_spi, SPI_FIFO), 
        8, 
        0, 
        CBS(4), 
        0
      }, 

      // PWM ADC trigger: wait for PWM, set sample length, trigger SPI
      { //CB 5
        DMA_CB_TI_PWM, 
        MEM(mp, &dp->pwm_val),   
        REG(m_LAB_Core->m_regs_pwm, PWM_FIF1), 
        4, 
        0, 
        CBS(6), 
        0
      }, 
      { // CB 6
        DMA_CB_TI_PWM, 
        MEM(mp, &dp->samp_size), 
        REG(m_LAB_Core->m_regs_spi, SPI_DLEN), 
        4, 
        0, 
        CBS(7), 
        0
      },
      { // CB 7
        DMA_CB_TI_PWM, 
        MEM(mp, &dp->adc_csd),   
        REG(m_LAB_Core->m_regs_spi, SPI_CS),   
        4, 
        0, 
        CBS(5), 
        0
      }, 
    },

    .samp_size = 2, // in number of bytes
    .pwm_val   = m_pwm_range, 
    .adc_csd   = SPI_CS_TA | SPI_CS_ADCS | SPI_CS_DMAEN | SPI_CS_CLEAR | LAB_OSCILLOSCOPE_ADC_CE,
    .txd       = {0xd0, static_cast<uint32_t>(m_number_of_channels > 1 ? 0xf0 : 0xd0)},
    .usecs     = {0, 0},  
    .states    = {0, 0}, 
    .rxd0      = {0}, 
    .rxd1      = {0},
  };
 
  std::memcpy (static_cast<ADC_DMA_DATA *>(m_uncached_dma_data.virt), &m_dma_data_dual_buffer, sizeof (m_dma_data_dual_buffer));
  m_pwm_range       = (LAB_PWM_FREQUENCY * 2) / LAB_OSCILLOSCOPE_MAX_SAMPLING_RATE;
  m_LAB_Core->pwm_init (LAB_PWM_FREQUENCY, m_pwm_range, PWM_VALUE);   // Initialise PWM, with DMA
  *(Utility::get_reg32 (m_LAB_Core->m_regs_pwm, PWM_DMAC)) = PWM_DMAC_ENAB | PWM_ENAB;
  *REG32(m_LAB_Core->m_regs_spi, SPI_DC) = (8<<24) | (1<<16) | (8<<8) | 1;  // Set DMA priorities
  *REG32(m_LAB_Core->m_regs_spi, SPI_CS) = SPI_CS_CLEAR;                    // Clear SPI FIFOs

  sampling_rate (LAB_OSCILLOSCOPE_MAX_SAMPLING_RATE);

  // Clear SPI FIFO
  m_LAB_Core->spi_clear_fifo ();

  m_LAB_Core->dma_start(mp, DMA_CHAN_SPI_TX, &dp->cbs[4], 0);  // Start SPI Tx DMA
  m_LAB_Core->dma_start(mp, DMA_CHAN_SPI_RX, &dp->cbs[0], 0);  // Start SPI Rx DMA
  m_LAB_Core->dma_start(mp, DMA_CHAN_PWM_PACING, &dp->cbs[5], 0);  // Start PWM DMA, for SPI trigger

  // m_LAB_Core->dma_start(DMA_CHAN_SPI_TX, &dp->cbs[6], 0);  // Start SPI Tx DMA
  // m_LAB_Core->dma_start(DMA_CHAN_SPI_RX, &dp->cbs[0], 0);  // Start SPI Rx DMA
  // m_LAB_Core->dma_start(DMA_CHAN_PWM_PACING, &dp->cbs[7], 0);  // Start PWM DMA, for SPI trigger
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

  if (m_channel_signals.m_chans[0].osc.osc_disp_mode == OSC_DISP_MODE_SCREEN)
  {
    if (dp->states[m_curr_screen_buffer])
    {
      dp->states[m_curr_screen_buffer] = 0;
      m_curr_screen_buffer ^= 1;
    }

    std::memcpy (
      raw_data_buffer, 
      m_curr_screen_buffer ? (void *)(dp->rxd1) : (void *)(dp->rxd0),
      LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES * 4
    );
  }
  else if (m_channel_signals.m_chans[0].osc.osc_disp_mode == OSC_DISP_MODE_REPEATED)
  {
    for (int b = 0; b < 2; b++)
    {
      if (dp->states[b])
      {
        std::memcpy (raw_data_buffer, b ? (void *)(dp->rxd1) : (void *)(dp->rxd0), 
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
sampling_rate (double value)
{
  for (int a = 0; a < LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS; a++)
  {
    sampling_rate (a, value);
  }
}

double LAB_Oscilloscope:: 
time_per_division (unsigned channel, double value, unsigned osc_disp_num_cols)
{
  Channel_Signal_Oscilloscope *osc = &(m_channel_signals.m_chans[channel].osc);

  if (osc->time_per_division != value)
  {
    osc->time_per_division  = value;

    bool flag = true;
    double new_sampling_rate;

    if (value >= LAB_OSCILLOSCOPE_MIN_TIME_PER_DIV_SCREEN_MODE)
    {
      if (osc->osc_disp_mode != OSC_DISP_MODE_SCREEN)
      {
        osc->osc_disp_mode = OSC_DISP_MODE_SCREEN;
        update_dma_data (OSC_DISP_MODE_SCREEN);
      }
      
      static_cast<ADC_DMA_DATA *>(m_uncached_dma_data.virt)->states[0] = 0;
      static_cast<ADC_DMA_DATA *>(m_uncached_dma_data.virt)->states[1] = 0;
    }
    else 
    {
      if (osc->osc_disp_mode != OSC_DISP_MODE_REPEATED)
      {
        osc->osc_disp_mode = OSC_DISP_MODE_REPEATED;
        update_dma_data (OSC_DISP_MODE_SCREEN);
      }

      if (value < LAB_OSCILLOSCOPE_MAX_TIME_PER_DIV_ZOOM)
      {
        if (osc->sampling_rate == LAB_OSCILLOSCOPE_MAX_SAMPLING_RATE)
        {
          return (osc->time_per_division);
        }
        else 
        {
          flag = false;
        }
      }
    }

    new_sampling_rate = flag ? ((osc->samples) / (value * osc_disp_num_cols)) :
      LAB_OSCILLOSCOPE_MAX_SAMPLING_RATE;

    sampling_rate (channel, new_sampling_rate); 

    osc->sampling_rate      = new_sampling_rate;
  }

  return (osc->time_per_division);
}

void LAB_Oscilloscope:: 
update_dma_data (int osc_disp_mode)
{
  m_LAB_Core->dma_pause (DMA_CHAN_PWM_PACING);
  
  volatile uint32_t *reg = Utility::get_reg32 (m_LAB_Core->m_regs_dma,
    DMA_REG (DMA_CHAN_PWM_PACING, DMA_NEXTCONBK));

  *reg = Utility::mem_bus_addr ()
  

  Utility::reg_write (*reg, )

  *REG32(m_regs_dma, DMA_REG(chan, DMA_CONBLK_AD)) = MEM_BUS_ADDR(mp, cbp);

  m_LAB_Core->dma_play (DMA_CHAN_PWM_PACING);
}

// EOF