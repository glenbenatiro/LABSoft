#include "LAB_Oscilloscope.h"

#include <cstring>
#include <bitset>

#include "LAB.h"

LAB_Oscilloscope::
LAB_Oscilloscope (LAB_Core  *_LAB_Core, 
                  LAB       *_LAB) 
{
  m_LAB_Core  = _LAB_Core;
  m_LAB       = _LAB;
  
  init_spi            ();
  init_pwm            ();
  init_osc_gpio_pins  ();
  init_osc_dma        ();
}

LAB_Oscilloscope::
~LAB_Oscilloscope ()
{
  m_LAB_Core->unmap_periph_mem (&m_uncached_dma_data_osc);
}

void LAB_Oscilloscope:: 
init_spi ()
{
  m_LAB_Core->spi_init (LAB_SPI_FREQUENCY);

  *(Utility::get_reg32 (&(m_LAB_Core->m_regs_spi), SPI_DC)) = 
    (8<<24) | (1<<16) | (8<<8) | 1;
}

void LAB_Oscilloscope:: 
init_pwm ()
{
  m_LAB_Core->gpio_set      (LAB_RPI_PIN_PWM_CHAN_0, AP_GPIO_FUNC_ALT0, AP_GPIO_PULL_DOWN);
  m_LAB_Core->pwm_init      (LAB_OSCILLOSCOPE_SAMPLING_RATE);
  m_LAB_Core->pwm_algo      (LAB_PWM_DMA_PACING_PWM_CHAN, AP_PWM_ALGO_MARKSPACE);
  m_LAB_Core->pwm_use_fifo  (LAB_PWM_DMA_PACING_PWM_CHAN, 1);

  *(Utility::get_reg32      (&(m_LAB_Core->m_regs_pwm), PWM_DMAC)) = 
    (1 << 31) | (8 << 8) | (1 << 0);
}

void LAB_Oscilloscope::
init_osc_gpio_pins ()
{
  m_LAB_Core->gpio_set (LAB_PIN_OSCILLOSCOPE_MUX_SCALER_A0_CHANNEL_0, 
    AP_GPIO_FUNC_OUTPUT, AP_GPIO_PULL_DOWN, 0);

  m_LAB_Core->gpio_set (LAB_PIN_OSCILLOSCOPE_MUX_SCALER_A1_CHANNEL_0, 
    AP_GPIO_FUNC_OUTPUT, AP_GPIO_PULL_DOWN, 0);

  m_LAB_Core->gpio_set (LAB_PIN_OSCILLOSCOPE_MUX_SCALER_A0_CHANNEL_1, 
    AP_GPIO_FUNC_OUTPUT, AP_GPIO_PULL_DOWN, 0);

  m_LAB_Core->gpio_set (LAB_PIN_OSCILLOSCOPE_MUX_SCALER_A1_CHANNEL_1, 
    AP_GPIO_FUNC_OUTPUT, AP_GPIO_PULL_DOWN, 0);

  coupling (0, LE_OSC_COUPLING_DC);
  coupling (1, LE_OSC_COUPLING_DC);
}

void LAB_Oscilloscope:: 
config_dma_control_blocks ()
{
  m_LAB_Core->map_uncached_mem (
    &m_uncached_dma_data_osc, 
    LAB_OSCILLOSCOPE_VC_MEM_SIZE
  );

  AP_MemoryMap *mp                   = &m_uncached_dma_data_osc;
  LAB_DMA_Data_Oscilloscope *dp      = static_cast<LAB_DMA_Data_Oscilloscope *>(mp->virt);
  LAB_DMA_Data_Oscilloscope dma_data = 
  {
    .cbs = 
    {
      // For double buffer SPI RX
      { // CB 0
        DMA_CB_TI_SPI_RX,
        Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi), SPI_FIFO),
        Utility::mem_bus_addr (mp, &dp->rxd[0]),
        (uint32_t)(4 * LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES),
        0,
        Utility::mem_bus_addr (mp, &dp->cbs[1]),
        0
      },
      { // CB 1
        DMA_CB_TI_SPI_RX, 
        Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi), SPI_CS),
        Utility::mem_bus_addr (mp, &dp->status[0]),
        4,
        0,
        Utility::mem_bus_addr (mp, &dp->cbs[2]),
        0
      }, 
      { // CB 2
        DMA_CB_TI_SPI_RX,
        Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi), SPI_FIFO),
        Utility::mem_bus_addr (mp, &dp->rxd[1]),
        (uint32_t)(4 * LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES),
        0,
        Utility::mem_bus_addr (mp, &dp->cbs[3]),
        0
      }, 
      { // CB 3
        DMA_CB_TI_SPI_RX, 
        Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi), SPI_CS),
        Utility::mem_bus_addr (mp, &dp->status[1]),
        4,
        0,
        Utility::mem_bus_addr (mp, &dp->cbs[0]),
        0
      }, 



      // For single buffer SPI RX
      { // CB 4
        DMA_CB_TI_SPI_RX,
        Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi), SPI_FIFO),
        Utility::mem_bus_addr (mp, &dp->rxd[0]),
        (uint32_t)(4 * LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES),
        0,
        Utility::mem_bus_addr (mp, &dp->cbs[5]),
        0
      },
      { // CB 5
        DMA_CB_TI_SPI_RX, 
        Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi), SPI_CS),
        Utility::mem_bus_addr (mp, &dp->status[0]),
        4,
        0,
        Utility::mem_bus_addr (mp, &dp->cbs[4]),
        0
      },  



      // For SPI TX
      { // CB 6
        DMA_CB_TI_SPI_TX,  
        Utility::mem_bus_addr (mp, &dp->txd),
        Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi), SPI_FIFO),
        //(uint32_t)((4 * LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES) + 4), 
        04,
        0, 
        Utility::mem_bus_addr (mp, &dp->cbs[6]), 
        0
      }, 



      // For PWM pacing
      { //CB 7
        DMA_CB_TI_PWM, 
        Utility::mem_bus_addr (mp, &dp->pwm_rng),   
        Utility::reg_bus_addr (&(m_LAB_Core->m_regs_pwm), PWM_FIF1), 
        4, 
        0, 
        Utility::mem_bus_addr (mp, &dp->cbs[8]), 
        0
      }, 
      { // CB 8
        DMA_CB_TI_PWM, 
        Utility::mem_bus_addr (mp, &dp->spi_cs_fifo_reset), 
        Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi), SPI_CS), 
        4, 
        0, 
        Utility::mem_bus_addr (mp, &dp->cbs[9]), 
        0
      },
      { // CB 9
        DMA_CB_TI_PWM, 
        Utility::mem_bus_addr (mp, &dp->spi_samp_size), 
        Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi), SPI_DLEN), 
        4, 
        0, 
        Utility::mem_bus_addr (mp, &dp->cbs[10]), 
        0
      },
      { // CB 10
        DMA_CB_TI_PWM, 
        Utility::mem_bus_addr (mp, &dp->spi_cs),   
        Utility::reg_bus_addr (&(m_LAB_Core->m_regs_spi), SPI_CS),   
        4, 
        0, 
        Utility::mem_bus_addr (mp, &dp->cbs[7]), 
        0
      },
    },

    .spi_samp_size      = 2,    // in bytes
    .spi_cs             = SPI_CS_TA | SPI_CS_ADCS | SPI_CS_DMAEN | LAB_OSCILLOSCOPE_ADC_CE,
    .spi_cs_fifo_reset  = 0x00000030,
    .pwm_rng            = 250,  // arbitrary value, will be changed in pwm_frequency ()
    .txd                = 0x0000ffff
  };

  // Fill in txd values with 0x0000ffff to toggle 
  // between channel 1 and 2 in hardware
  // std::fill_n (
  //   dma_data.txd, 
  //   sizeof (dma_data.txd) / sizeof (dma_data.txd[0]),
  //   0x0000ffff
  // );

  // The first value of txd is for SPI DMA operation.
  // Page 158 of BCM2385 datasheet
 // dma_data.txd[0] = 0x07d00080;

  //
  std::memcpy (dp, &dma_data, sizeof (dma_data));
}

void LAB_Oscilloscope::
init_osc_dma ()
{
  // PWM should be initialized already at this point in LAB
  m_LAB_Core->dma_reset (LAB_DMA_CHAN_PWM_PACING);
  m_LAB_Core->dma_reset (LAB_DMA_CHAN_OSCILLOSCOPE_SPI_RX);
  m_LAB_Core->dma_reset (LAB_DMA_CHAN_OSCILLOSCOPE_SPI_TX);
  
  config_dma_control_blocks ();

  AP_MemoryMap              *mp = &(m_uncached_dma_data_osc);
  LAB_DMA_Data_Oscilloscope *dp = static_cast<LAB_DMA_Data_Oscilloscope *>(mp->virt);

  m_LAB_Core->dma_start (mp, LAB_DMA_CHAN_OSCILLOSCOPE_SPI_TX, &(dp->cbs[6]), 0);  
  m_LAB_Core->dma_start (mp, LAB_DMA_CHAN_OSCILLOSCOPE_SPI_RX, &(dp->cbs[0]), 0);  
  m_LAB_Core->dma_start (mp, LAB_DMA_CHAN_PWM_PACING,          &(dp->cbs[7]), 0); 
}

void LAB_Oscilloscope:: 
run ()
{
  if (m_LAB->m_Voltmeter.is_running ())
  {
    m_LAB->m_Voltmeter.stop ();
  }

  master_run_stop (true);
}

void LAB_Oscilloscope:: 
stop ()
{
  master_run_stop (false);
}

void LAB_Oscilloscope:: 
master_run_stop (bool value)
{
  osc_core_run_stop     (value);
  osc_frontend_run_stop (value);
}

void LAB_Oscilloscope:: 
osc_core_run_stop (bool value)
{
  if (value)
  {
    m_LAB_Core->pwm_start (LAB_PWM_DMA_PACING_PWM_CHAN);
    m_parent_data.is_osc_core_running = true;
  }
  else
  {
    m_LAB_Core->pwm_stop (LAB_PWM_DMA_PACING_PWM_CHAN);
    m_parent_data.is_osc_core_running = false;
  }
}

void LAB_Oscilloscope:: 
osc_frontend_run_stop (bool value)
{
  m_parent_data.is_osc_frontend_running = value;
}

bool LAB_Oscilloscope:: 
is_running ()
{
  return (m_parent_data.is_osc_core_running && 
    m_parent_data.is_osc_frontend_running);
}

void LAB_Oscilloscope::
channel_enable_disable (unsigned channel,
                        bool     value)
{
  m_parent_data.channel_data[channel].is_enabled = value;
}

void LAB_Oscilloscope:: 
voltage_per_division (unsigned  channel, 
                      double    value)
{
  m_parent_data.channel_data[channel].voltage_per_division = value;
}

void LAB_Oscilloscope:: 
vertical_offset (unsigned channel, 
                 double   value)
{
  m_parent_data.channel_data[channel].vertical_offset = value;
}

int LAB_Oscilloscope:: 
scaling (unsigned       channel, 
         LE_OSC_SCALING _LE_OSC_SCALING)
{
  unsigned a0, a1;

  if (channel == 0)
  {
    a0 = LAB_PIN_OSCILLOSCOPE_MUX_SCALER_A0_CHANNEL_0;
    a1 = LAB_PIN_OSCILLOSCOPE_MUX_SCALER_A1_CHANNEL_0;
  }
  else
  {
    a0 = LAB_PIN_OSCILLOSCOPE_MUX_SCALER_A0_CHANNEL_1;
    a1 = LAB_PIN_OSCILLOSCOPE_MUX_SCALER_A1_CHANNEL_1;
  }

  m_LAB_Core->gpio_write (a0, (_LE_OSC_SCALING == LE_OSC_SCALING_FOURTH || 
    _LE_OSC_SCALING == LE_OSC_SCALING_UNITY) ? 1 : 0);
  
  m_LAB_Core->gpio_write (a1, (_LE_OSC_SCALING == LE_OSC_SCALING_FOURTH || 
    _LE_OSC_SCALING == LE_OSC_SCALING_HALF) ? 1 : 0);

  m_parent_data.channel_data[channel].scaling = _LE_OSC_SCALING;

  return (_LE_OSC_SCALING);
}

int LAB_Oscilloscope:: 
coupling (unsigned        channel,
          LE_OSC_COUPLING _LE_OSC_COUPLING)
{
  unsigned pin = (channel == 0) ? LAB_PIN_OSCILLOSCOPE_COUPLING_SELECT_CHANNEL_1 :
    LAB_PIN_OSCILLOSCOPE_COUPLING_SELECT_CHANNEL_2;

  switch (_LE_OSC_COUPLING)
  {
    case LE_OSC_COUPLING_AC:
      m_LAB_Core->gpio_set   (pin, AP_GPIO_FUNC_OUTPUT, AP_GPIO_PULL_DOWN);
      m_LAB_Core->gpio_write (pin, 1);
      break;

    case LE_OSC_COUPLING_DC:
      m_LAB_Core->gpio_set   (pin, AP_GPIO_FUNC_OUTPUT, AP_GPIO_PULL_DOWN);
      m_LAB_Core->gpio_write (pin, 0);
      break;

    default: 
      coupling (channel, LE_OSC_COUPLING_DC);
      break;
  }

  return (_LE_OSC_COUPLING);
}

void LAB_Oscilloscope:: 
load_data_samples ()
{
  LAB_DMA_Data_Oscilloscope *dma_data = static_cast<LAB_DMA_Data_Oscilloscope *>
    (m_uncached_dma_data_osc.virt);
  

  if (m_parent_data.graph_disp_mode == LE_GRAPH_DISP_MODE_SCREEN)
  {
    // if (dma_data->status[m_curr_screen_buffer])
    // {
    //   dma_data->status[m_curr_screen_buffer] = 0;
    //   m_curr_screen_buffer ^= 1;
    // }

    std::memcpy (
      m_parent_data.raw_sample_buffer.data (), 
      // (void *)(dma_data->rxd[m_curr_screen_buffer]),
      (void *)(dma_data->rxd[0]),
      4 * LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES
    );
  }
  else if (m_parent_data.graph_disp_mode == LE_GRAPH_DISP_MODE_REPEATED)
  {
    for (int a = 0; a < 2; a++)
    {
      if (dma_data->status[a])
      {
        std::memcpy (
          m_parent_data.raw_sample_buffer.data (), 
          (void *)(dma_data->rxd[a]),
          4 * LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES
        );

        // std::cout << std::bitset <32> (dma_data->status[a]) << "\n";

        // for (int samp = 10; samp <= 20; samp++)
        // {
        //   std::cout << std::bitset <32> (dma_data->rxd[a][samp]) << "\n";
        // }

        // Check if the other buffer is also full. 
        // If it is, then we have a buffer overflow (both buffers full).
        if (dma_data->status[a ^ 1])
        {
          dma_data->status[0] = dma_data->status[1] = 0;

          break;
        }

        dma_data->status[a] = 0;
      }
    }
  } 

  parse_raw_sample_buffer ();
}

int LAB_Oscilloscope:: 
parse_raw_sample_buffer ()
{
  double data;

  // Go through and distribute all the samples in one go
  for (int samp = 0; samp < (m_parent_data.raw_sample_buffer.size ()); samp++)
  {
    // From a 32-bit sample from the raw data buffer,  
    // slice it up and distribute actual samples to each channel
    for (int chan = 0; chan < (m_parent_data.channel_data.size ()); chan++)
    {
      if (m_parent_data.channel_data[chan].is_enabled)
      {
        // {
        //   std::cout << std::bitset <32> (m_parent_data.raw_sample_buffer[samp]) << "\n";
        // }
        
        uint32_t temp1 = ((m_parent_data.raw_sample_buffer[samp]) >>
          (LAB_OSCILLOSCOPE_RAW_DATA_SHIFT_BIT_COUNT * chan)) & 
            LAB_OSCILLOSCOPE_RAW_DATA_POST_SHIFT_MASK;
            
        // --- START HARD CODED ---

        // This formula is specific to the Texas Instruments ADS7883 ADC
        //uint32_t temp2 = ((temp1 << 6) | (temp1 >> 10)) & 0xFFF;

        // This formula is specific to the Microchip MCP33111 ADC
        uint32_t temp2 = ((temp1 & 0xff) << 4) | ((temp1 & 0xf000) >> 12);

        // --- END HARD CODED ---

        // Get MSB to determine sign
        bool sign = temp2 >> (LAB_OSCILLOSCOPE_ADC_RESOLUTION_BITS - 1);

        // Mask temp2 to mask out the MSB sign bit
        uint32_t temp3 = temp2 & ((LAB_OSCILLOSCOPE_ADC_RESOLUTION_INT - 1) >> 1);

        if (sign)
        {
          data = static_cast<double>(temp3) * LAB_OSCILLOSCOPE_ADC_CONVERSION_CONSTANT;
        }
        else 
        {
          data = (static_cast<double>(temp3) * LAB_OSCILLOSCOPE_ADC_CONVERSION_CONSTANT) -  
            LAB_OSCILLOSCOPE_ADC_REFERENCE_VOLTAGE;
        }
        
        m_parent_data.channel_data[chan].samples[samp] = data;
      }
    }
  }

  return 1;
}

bool LAB_Oscilloscope:: 
has_enabled_channel ()
{
  bool flag = false; 

  for (const auto &chan : m_parent_data.channel_data)
  {
    if (chan.is_enabled)
    {
      flag = true;
      break;
    }
  }

  return (flag);
}

void LAB_Oscilloscope:: 
sampling_rate (double value)
{
  double actual_freq = m_LAB_Core->pwm_frequency (
    LAB_PWM_DMA_PACING_PWM_CHAN, 
    value
  );
  
  m_LAB_Core->pwm_duty_cycle (LAB_PWM_DMA_PACING_PWM_CHAN, 50);

  // Change duty cycle by changing pwm_val in PWM DMA control block
  double dc_percentage  = (std::fmod (50, 100.0)) / (100.0);
  double fifo_data      = m_LAB_Core->m_pwm_range * dc_percentage;

  (static_cast<LAB_DMA_Data_Oscilloscope *>(m_uncached_dma_data_osc.virt))->
    pwm_rng = fifo_data;
}

double LAB_Oscilloscope:: 
time_per_division (double value, unsigned osc_disp_num_cols)
{
  double  new_samp_count;
  double  new_samp_rate;

  // 1. Calculate the new count of samples
  if (value >= LAB_OSCILLOSCOPE_MIN_TIME_PER_DIV_NO_ZOOM)
  {
    new_samp_count = LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES;
  }
  else 
  {
    new_samp_count = LAB_OSCILLOSCOPE_MAX_SAMPLING_RATE * osc_disp_num_cols *
      value;
  }   

  // 2. Calculate the new sampling rate
  if (value <= LAB_OSCILLOSCOPE_MIN_TIME_PER_DIV_NO_ZOOM)
  {
    new_samp_rate = LAB_OSCILLOSCOPE_MAX_SAMPLING_RATE;
  }
  else 
  {
    new_samp_rate = LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES / (value * 
      osc_disp_num_cols);
  }

  // 3. Change the oscilloscope display mode if necessary
  if (value >= LAB_OSCILLOSCOPE_MIN_TIME_PER_DIV_GRAPH_DISP_MODE_SCREEN)
  {
    if (m_parent_data.graph_disp_mode != LE_GRAPH_DISP_MODE_SCREEN)
    {
      m_parent_data.graph_disp_mode = LE_GRAPH_DISP_MODE_SCREEN;
      switch_dma_buffer (LE_SPI_DMA_BUFFER_COUNT_SINGLE);
    }
  }
  else
  {
    if (m_parent_data.graph_disp_mode != LE_GRAPH_DISP_MODE_REPEATED)
    {
      m_parent_data.graph_disp_mode = LE_GRAPH_DISP_MODE_REPEATED; 
      switch_dma_buffer (LE_SPI_DMA_BUFFER_COUNT_DOUBLE);
    }
  }

  // 4. Set the new time per division, sample count, sampling rate values 
  //    to all channel data
  m_parent_data.time_per_division = value;
  m_parent_data.w_samp_count      = new_samp_count;

  // 5. Set new sampling rate
  sampling_rate (new_samp_rate); 

  return (m_parent_data.time_per_division);
}

double LAB_Oscilloscope:: 
horizontal_offset (double value)
{
  m_parent_data.horizontal_offset = value;

  return (value);
}

void LAB_Oscilloscope:: 
update_dma_data (int graph_disp_mode)
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
switch_dma_buffer (int buffer)
{
  bool flag = false;

  if (!(m_LAB_Core->is_dma_paused (LAB_DMA_CHAN_PWM_PACING)))
  {
    flag = true;
    m_LAB_Core->dma_pause (LAB_DMA_CHAN_PWM_PACING);
  }

  // load the next cb depending on buffer
  LAB_DMA_Data_Oscilloscope *dp = static_cast<LAB_DMA_Data_Oscilloscope *>
    (m_uncached_dma_data_osc.virt);
  
  volatile uint32_t *reg = Utility::get_reg32 (m_LAB_Core->m_regs_dma,
    DMA_REG (LAB_DMA_CHAN_OSCILLOSCOPE_SPI_RX, DMA_NEXTCONBK));

  if (buffer == LE_SPI_DMA_BUFFER_COUNT_SINGLE)
  {
    *reg = Utility::mem_bus_addr (&m_uncached_dma_data_osc, &(dp->cbs[4]));
  }
  else // (buffer == LE_SPI_DMA_BUFFER_COUNT_DOUBLE)
  {
    *reg = Utility::mem_bus_addr (&m_uncached_dma_data_osc, &(dp->cbs[0]));
  }

  // abort current DMA control block
  m_LAB_Core->dma_abort (LAB_DMA_CHAN_OSCILLOSCOPE_SPI_RX);

  // clear buffer status
  static_cast<LAB_DMA_Data_Oscilloscope *>(m_uncached_dma_data_osc.virt)->
    status[0] = 0;
  static_cast<LAB_DMA_Data_Oscilloscope *>(m_uncached_dma_data_osc.virt)->
    status[1] = 0;

  if (flag)
  {
    m_LAB_Core->dma_play (LAB_DMA_CHAN_PWM_PACING);
  }
}

int LAB_Oscilloscope:: 
update_state ()
{
  // // Time per division
  // time_per_division (m_parent_data.time_per_division,
  //   LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_COLUMNS);

  // Init scaling
  for (int a = 0; a < m_parent_data.channel_data.size (); a++)
  {
    scaling (a, m_parent_data.channel_data[a].scaling);
  }

  return 1;
}

// EOF