#include "LAB_Oscilloscope.h"

#include "LAB.h"

#include <cstring>

LAB_Oscilloscope::
LAB_Oscilloscope (LAB_Core *_LAB_Core, LAB *_LAB) 
: m_channel_signals (LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS, LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES, 0)
{
  m_LAB_Core  = _LAB_Core;
  m_LAB       = _LAB;

  // Initialize GPIO pins
  m_LAB_Core->gpio_set (LAB_OSCILLOSCOPE_SCALER_MUX_A0_PIN_CHANNEL_1, 
    AP_GPIO_FUNC_OUTPUT, AP_GPIO_PULL_DOWN, 0);
  m_LAB_Core->gpio_set (LAB_OSCILLOSCOPE_SCALER_MUX_A1_PIN_CHANNEL_1, 
    AP_GPIO_FUNC_OUTPUT, AP_GPIO_PULL_DOWN, 0);
  m_LAB_Core->gpio_set (LAB_OSCILLOSCOPE_SCALER_MUX_A0_PIN_CHANNEL_2, 
    AP_GPIO_FUNC_OUTPUT, AP_GPIO_PULL_DOWN, 0);
  m_LAB_Core->gpio_set (LAB_OSCILLOSCOPE_SCALER_MUX_A1_PIN_CHANNEL_2, 
    AP_GPIO_FUNC_OUTPUT, AP_GPIO_PULL_DOWN, 0);

  coupling (0, LABE_OSC_COUPLING_DC);
  coupling (1, LABE_OSC_COUPLING_DC);

  // PWM should be initialized already at this point in LAB
  m_LAB_Core->dma_reset (LAB_DMA_CHAN_OSCILLOSCOPE_SPI_LOAD);
  m_LAB_Core->dma_reset (LAB_DMA_CHAN_OSCILLOSCOPE_SPI_RX);
  m_LAB_Core->dma_reset (LAB_DMA_CHAN_OSCILLOSCOPE_SPI_TX);

  config_dma_control_blocks ();

  AP_MemoryMap              *ud = &(m_uncached_adc_dma_data);
  LAB_OSCILLOSCOPE_DMA_DATA *dd = static_cast<LAB_OSCILLOSCOPE_DMA_DATA *>(ud->virt);

  m_LAB_Core->dma_start (ud, LAB_DMA_CHAN_OSCILLOSCOPE_SPI_TX,   &(dd->cbs[6]), 0);  // Start SPI Tx DMA
  m_LAB_Core->dma_start (ud, LAB_DMA_CHAN_OSCILLOSCOPE_SPI_RX,   &(dd->cbs[0]), 0);  // Start SPI Rx DMA
  m_LAB_Core->dma_start (ud, LAB_DMA_CHAN_OSCILLOSCOPE_SPI_LOAD, &(dd->cbs[8]), 0); // Start PWM DMA, for SPI trigger
}

LAB_Oscilloscope::~LAB_Oscilloscope ()
{
  m_LAB_Core->unmap_periph_mem (&m_uncached_adc_dma_data);
}

void LAB_Oscilloscope:: 
run ()
{
  if (m_LAB->m_Voltmeter.is_running ())
  {
    m_LAB->m_Voltmeter.stop ();
  }

  run_master ();
}

void LAB_Oscilloscope:: 
stop ()
{
  stop_master ();
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
  uint32_t raw_data_buffer [LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES];
  LAB_OSCILLOSCOPE_DMA_DATA *dp = static_cast<LAB_OSCILLOSCOPE_DMA_DATA *>(m_uncached_adc_dma_data.virt);

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
sampling_rate (double value)
{
  std::cout << "frequency to apply: " << value << "\n\n";
  
  // for some reason regarding DMA SPI DREQ, we multiply the actual pwm freq
  // by 4!

  double actual_freq = m_LAB_Core->pwm_frequency (LAB_PWM_DMA_PACING_PWM_CHAN, 
    value * 4.0);
  
  m_LAB_Core->pwm_duty_cycle (LAB_PWM_DMA_PACING_PWM_CHAN, 50);
}

double LAB_Oscilloscope:: 
time_per_division (double value, unsigned osc_disp_num_cols)
{
  Channel_Signal_Oscilloscope *osc = &(m_channel_signals.m_chans[0].osc);

  if (osc->time_per_division != value)
  {
    m_LAB_Core->dma_pause (LAB_DMA_CHAN_PWM_PACING);

    bool    flag = false;
    double  new_samples;
    double  new_sampling_rate;

    // 1. Calculate the new count of samples
    if (value >= LAB_OSCILLOSCOPE_MAX_TIME_PER_DIV_ZOOM)
    {
      new_samples = LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES;
    }
    else 
    {
      new_samples = LAB_OSCILLOSCOPE_MAX_SAMPLING_RATE * osc_disp_num_cols *
        value;
    }   

    // 2. Calculate the new sampling rate
    if (value <= LAB_OSCILLOSCOPE_MAX_TIME_PER_DIV_ZOOM)
    {
      new_sampling_rate = LAB_OSCILLOSCOPE_MAX_SAMPLING_RATE;
    }
    else 
    {
      new_sampling_rate = LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES / (value *
        osc_disp_num_cols);
    }

    // 3. Change the oscilloscope display mode if necessary
    if (value >= LAB_OSCILLOSCOPE_MIN_TIME_PER_DIV_OSC_DISP_MODE_SCREEN)
    {
      if (osc->osc_disp_mode != OSC_DISP_MODE_SCREEN)
      {
        osc->osc_disp_mode = OSC_DISP_MODE_SCREEN;
        buffer_switch (SINGLE_BUFFER);
      }
    }
    else
    {
      if (osc->osc_disp_mode != OSC_DISP_MODE_REPEATED)
      {
        osc->osc_disp_mode = OSC_DISP_MODE_REPEATED; 
        buffer_switch (DOUBLE_BUFFER);
      }
    }

    // 4. If new sample rate is different from current sample rate, change
    //    hardware sampling rate (PWM pacing)
    if (osc->sampling_rate != new_sampling_rate)
    {
      flag = true;
      sampling_rate (new_sampling_rate); 
    }

    // 5. Set the new time per division, sample count, and sampling rate values 
    //    to all channel data
    for (int a = 0; a < LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS; a++)
    {
      Channel_Signal_Oscilloscope *osc = &(m_channel_signals.m_chans[a].osc);

      osc->time_per_division  = value;
      osc->samples            = new_samples;

      if (flag)
      {
        osc->sampling_rate = new_sampling_rate;
      }
    }  

    m_LAB_Core->dma_play (LAB_DMA_CHAN_PWM_PACING);
  }

  return (osc->time_per_division);
}

void LAB_Oscilloscope:: 
update_dma_data (int osc_disp_mode)
{
  // m_LAB_Core->dma_pause (LAB_DMA_CHAN_PWM_PACING);
  
  // volatile uint32_t *reg = Utility::get_reg32 (m_LAB_Core->m_regs_dma,
  //   DMA_REG (LAB_DMA_CHAN_PWM_PACING, DMA_NEXTCONBK));

  // *reg = Utility::mem_bus_addr ()
  

  // Utility::reg_write (*reg, )

  // *REG32(m_regs_dma, DMA_REG(chan, DMA_CONBLK_AD)) = MEM_BUS_ADDR(mp, cbp);

  // m_LAB_Core->dma_play (LAB_DMA_CHAN_PWM_PACING);
}

void LAB_Oscilloscope:: 
buffer_switch (int buffer)
{
  bool flag = false;

  if (!(m_LAB_Core->is_dma_paused (LAB_DMA_CHAN_PWM_PACING)))
  {
    flag = true;
    m_LAB_Core->dma_pause (LAB_DMA_CHAN_PWM_PACING);
  }

  // load the next cb depending on buffer
  LAB_OSCILLOSCOPE_DMA_DATA *dp = static_cast<LAB_OSCILLOSCOPE_DMA_DATA *>
    (m_uncached_adc_dma_data.virt);
  
  volatile uint32_t *reg = Utility::get_reg32 (m_LAB_Core->m_regs_dma,
    DMA_REG (LAB_DMA_CHAN_OSCILLOSCOPE_SPI_RX, DMA_NEXTCONBK));

  if (buffer == SINGLE_BUFFER)
  {
    *reg = Utility::mem_bus_addr (&m_uncached_adc_dma_data, &(dp->cbs[4]));
  }
  else // (buffer == DOUBLE_BUFFER)
  {
    *reg = Utility::mem_bus_addr (&m_uncached_adc_dma_data, &(dp->cbs[0]));
  }

  // abort current DMA control block
  m_LAB_Core->dma_abort (LAB_DMA_CHAN_OSCILLOSCOPE_SPI_RX);

  // clear buffer states
  static_cast<LAB_OSCILLOSCOPE_DMA_DATA *>(m_uncached_adc_dma_data.virt)->
    states[0] = 0;
  static_cast<LAB_OSCILLOSCOPE_DMA_DATA *>(m_uncached_adc_dma_data.virt)->
    states[1] = 0;

  if (flag)
  {
    m_LAB_Core->dma_play (LAB_DMA_CHAN_PWM_PACING);
  }
}

// Enable all channels
void LAB_Oscilloscope:: 
channel_enable ()
{
  for (int a = 0; a < LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS; a++)
  {
    channel_enable (a);
  }
}

double LAB_Oscilloscope:: 
time_per_division ()
{
  return (m_channel_signals.m_chans[0].osc.time_per_division);
}

void LAB_Oscilloscope:: 
config_dma_control_blocks ()
{
  m_LAB_Core->map_uncached_mem (&m_uncached_adc_dma_data, 
    LAB_OSCILLOSCOPE_VC_MEM_SIZE);

  AP_MemoryMap *mp = &m_uncached_adc_dma_data;

  LAB_OSCILLOSCOPE_DMA_DATA *dp  = static_cast<LAB_OSCILLOSCOPE_DMA_DATA *>
    (m_uncached_adc_dma_data.virt);

  LAB_OSCILLOSCOPE_DMA_DATA dma_data = 
  {
    .cbs = 
    {
      // control blocks for SPI_RX dual buffer
      { // CB 0
        DMA_CB_TI_SPI_RX,
        REG (m_LAB_Core->m_regs_spi, SPI_FIFO),  
        MEM (mp, dp->rxd0),       
        (uint32_t)(LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES*4),  
        0, 
        MEM (mp, &dp->cbs[1]),
        0
      },
      { // CB 1
        DMA_CB_TI_SPI_RX, 
        REG (m_LAB_Core->m_regs_spi,  SPI_CS),
        MEM (mp, &dp->states[0]),
        4,
        0,
        MEM (mp, &dp->cbs[2]),
        0
      }, 
      { // CB 2
        DMA_CB_TI_SPI_RX, 
        REG (m_LAB_Core->m_regs_spi,  SPI_FIFO),
        MEM (mp, dp->rxd1),       
        (uint32_t)(LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES*4),  
        0, 
        MEM (mp, &dp->cbs[3]), 
        0
      }, 
      { // CB 3
        DMA_CB_TI_SPI_RX, 
        REG (m_LAB_Core->m_regs_spi,  SPI_CS),    
        MEM (mp, &dp->states[1]), 
        4,                                              
        0, 
        MEM (mp, &dp->cbs[0]), 
        0
      }, 


      // control blocks for SPI_RX single buffer
      { // CB 4
        DMA_CB_TI_SPI_RX, 
        REG (m_LAB_Core->m_regs_spi,  SPI_FIFO),
        MEM (mp, dp->rxd0),       
        (uint32_t)(m_number_of_samples_per_channel*4),  
        0, 
        MEM (mp, &dp->cbs[5]), 
        0
      },  
      { // CB 5
        DMA_CB_TI_SPI_RX, 
        REG (m_LAB_Core->m_regs_spi,  SPI_CS),    
        MEM (mp, &dp->states[1]), 
        4,                                              
        0, 
        MEM (mp, &dp->cbs[4]), 
        0
      },  


      // // Control Blocks for SPI TX
      // { // CB 6
      //   DMA_CB_TI_SPI_TX,  
      //   MEM(mp, dp->txd),        
      //   REG(m_LAB_Core->m_regs_spi, SPI_FIFO), 
      //   8, 
      //   0, 
      //   MEM (mp, &dp->cbs[6]), 
      //   0
      // }, 

      // Control Blocks for SPI TX
      { // CB 6
        DMA_CB_TI_SPI_TX,  
        MEM(mp, dp->txd),        
        REG(m_LAB_Core->m_regs_spi, SPI_FIFO), 
        8, 
        0, 
        MEM (mp, &dp->cbs[6]), 
        0
      }, 


      // PWM ADC trigger: wait for PWM, set sample length, trigger SPI
      { //CB 7
        DMA_CB_TI_PWM, 
        MEM(mp, &dp->pwm_val),   
        REG(m_LAB_Core->m_regs_pwm, PWM_FIF1), 
        4, 
        0, 
        MEM (mp, &dp->cbs[8]), 
        0
      }, 
      { // CB 8
        DMA_CB_TI_PWM, 
        MEM(mp, &dp->samp_size), 
        REG(m_LAB_Core->m_regs_spi, SPI_DLEN), 
        4, 
        0, 
        MEM (mp, &dp->cbs[9]), 
        0
      },
      { // CB 9
        DMA_CB_TI_PWM, 
        MEM(mp, &dp->adc_csd),   
        REG(m_LAB_Core->m_regs_spi, SPI_CS),   
        4, 
        0, 
        MEM (mp, &dp->cbs[8]), 
        0
      }, 
    },

    .samp_size = 2, // in number of bytes
    .pwm_val   = m_pwm_range, 
    .adc_csd   = SPI_CS_TA | SPI_CS_ADCS | SPI_CS_DMAEN | SPI_CS_CLEAR | LAB_OSCILLOSCOPE_ADC_CE,
    // .txd       = {0xd0, static_cast<uint32_t>(m_number_of_channels > 1 ? 0xf0 : 0xd0)},
    .txd       = {0xffff, 0x0000},
    .usecs     = {0, 0},  
    .states    = {0, 0}, 
    .rxd0      = {0}, 
    .rxd1      = {0},
  };
 
  std::memcpy (dp, &dma_data, sizeof (dma_data));
}

int LAB_Oscilloscope:: 
coupling (unsigned          channel,
          LABE_OSC_COUPLING _LABE_OSC_COUPLING)
{
  unsigned pin = (channel == 0) ? LAB_OSCILLOSCOPE_COUPLING_SELECT_PIN_CHANNEL_1 :
    LAB_OSCILLOSCOPE_COUPLING_SELECT_PIN_CHANNEL_2;

  switch (_LABE_OSC_COUPLING)
  {
    case LABE_OSC_COUPLING_AC:
      m_LAB_Core->gpio_set   (pin, AP_GPIO_FUNC_INPUT, AP_GPIO_PULL_DOWN);
      break;

    case LABE_OSC_COUPLING_DC:
      m_LAB_Core->gpio_set   (pin, AP_GPIO_FUNC_OUTPUT, AP_GPIO_PULL_UP);
      m_LAB_Core->AP_gpio_write (pin, 1);
      break;

    default: 
      coupling (channel, LABE_OSC_COUPLING_DC);
      break;
  }

  return (_LABE_OSC_COUPLING);
}

int LAB_Oscilloscope:: 
scaling (unsigned         channel, 
         LABE_OSC_SCALING _LABE_OSC_SCALING)
{
  unsigned a0, a1;

  if (channel == 0)
  {
    a0 = LAB_OSCILLOSCOPE_SCALER_MUX_A0_PIN_CHANNEL_1;
    a1 = LAB_OSCILLOSCOPE_SCALER_MUX_A1_PIN_CHANNEL_1;
  }
  else
  {
    a0 = LAB_OSCILLOSCOPE_SCALER_MUX_A0_PIN_CHANNEL_2;
    a1 = LAB_OSCILLOSCOPE_SCALER_MUX_A1_PIN_CHANNEL_2;
  }

  m_LAB_Core->AP_gpio_write (a0, (_LABE_OSC_SCALING == LABE_OSC_SCALING_EIGHTH || 
    _LABE_OSC_SCALING == LABE_OSC_SCALING_HALF) ? 1 : 0);
  
  m_LAB_Core->AP_gpio_write (a1, (_LABE_OSC_SCALING == LABE_OSC_SCALING_EIGHTH || 
    _LABE_OSC_SCALING == LABE_OSC_SCALING_QUARTER) ? 1 : 0);

  return (_LABE_OSC_SCALING);
}

// EOF