#include "LAB_Oscilloscope.h"

#include <cstring>
#include <iostream>

// remove soon
#include <bitset>

#include "LAB.h"

LAB_Oscilloscope::
LAB_Oscilloscope (LAB_Core  *_LAB_Core, 
                  LAB       *_LAB) 
{
  m_LAB_Core  = _LAB_Core;
  m_LAB       = _LAB;   

  init_spi        ();
  init_pwm        ();
  init_gpio_pins  ();
  init_dma        (); 
  init_state      ();
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

  *(Utility::reg_virt (m_LAB_Core->m_regs_spi, SPI_DC)) = 
    (8<<24) | (4 << 16) | (8<<8) | 1;

  
}

void LAB_Oscilloscope:: 
init_pwm ()
{
  m_LAB_Core->gpio_set      (LAB_RPI_PIN_PWM_CHAN_0, AP_GPIO_FUNC_ALT0, AP_GPIO_PULL_DOWN);
  m_LAB_Core->pwm_init      (LAB_OSCILLOSCOPE::SAMPLING_RATE);
  m_LAB_Core->pwm_algo      (LAB_PWM_DMA_PACING_PWM_CHAN, AP_PWM_ALGO_MARKSPACE);
  m_LAB_Core->pwm_use_fifo  (LAB_PWM_DMA_PACING_PWM_CHAN, 1);

  *(Utility::reg_virt (m_LAB_Core->m_regs_pwm, PWM_DMAC)) = 
    (1 << 31) | (8 << 8) | (1 << 0);
}

void LAB_Oscilloscope::
init_gpio_pins ()
{
  // scaling
  m_LAB_Core->gpio_set (LAB_PIN_OSCILLOSCOPE_MUX_SCALER_A0_CHANNEL_0, 
    AP_GPIO_FUNC_OUTPUT, AP_GPIO_PULL_DOWN, 1);
  m_LAB_Core->gpio_set (LAB_PIN_OSCILLOSCOPE_MUX_SCALER_A1_CHANNEL_0, 
    AP_GPIO_FUNC_OUTPUT, AP_GPIO_PULL_DOWN, 0);
  m_LAB_Core->gpio_set (LAB_PIN_OSCILLOSCOPE_MUX_SCALER_A0_CHANNEL_1, 
    AP_GPIO_FUNC_OUTPUT, AP_GPIO_PULL_DOWN, 1);
  m_LAB_Core->gpio_set (LAB_PIN_OSCILLOSCOPE_MUX_SCALER_A1_CHANNEL_1, 
    AP_GPIO_FUNC_OUTPUT, AP_GPIO_PULL_DOWN, 0);

  // coupling
  m_LAB_Core->gpio_set (LAB_PIN_OSCILLOSCOPE_COUPLING_SELECT_CHANNEL_0, 
    AP_GPIO_FUNC_OUTPUT, AP_GPIO_PULL_DOWN, 0);
  m_LAB_Core->gpio_set (LAB_PIN_OSCILLOSCOPE_COUPLING_SELECT_CHANNEL_1, 
    AP_GPIO_FUNC_OUTPUT, AP_GPIO_PULL_DOWN, 0);
}

void LAB_Oscilloscope::
init_dma ()
{
  // PWM should be initialized already at this point in LAB
  m_LAB_Core->dma_reset (LAB_DMA_CHAN_PWM_PACING);
  m_LAB_Core->dma_reset (LAB_DMA_CHAN_OSCILLOSCOPE_SPI_RX);
  m_LAB_Core->dma_reset (LAB_DMA_CHAN_OSCILLOSCOPE_SPI_TX);

  config_dma_cb ();

  AP_MemoryMap              *mp = &(m_uncached_dma_data_osc);
  LAB_DMA_Data_Oscilloscope *dp = static_cast<LAB_DMA_Data_Oscilloscope *>(mp->virt);

  m_LAB_Core->dma_start (mp, LAB_DMA_CHAN_OSCILLOSCOPE_SPI_TX, &(dp->cbs[6]), 0);  
  m_LAB_Core->dma_start (mp, LAB_DMA_CHAN_OSCILLOSCOPE_SPI_RX, &(dp->cbs[0]), 0);  
  m_LAB_Core->dma_start (mp, LAB_DMA_CHAN_PWM_PACING,          &(dp->cbs[7]), 0); 

  Utility::disp_reg_virt (m_LAB_Core->m_regs_dma, Utility::dma_chan_reg_offset (LAB_DMA_CHAN_OSCILLOSCOPE_SPI_TX, DMA_CONBLK_AD));
  Utility::disp_reg_virt (m_LAB_Core->m_regs_dma, Utility::dma_chan_reg_offset (LAB_DMA_CHAN_OSCILLOSCOPE_SPI_RX, DMA_CONBLK_AD));
  Utility::disp_reg_virt (m_LAB_Core->m_regs_dma, Utility::dma_chan_reg_offset (LAB_DMA_CHAN_PWM_PACING, DMA_CONBLK_AD));
}

void LAB_Oscilloscope:: 
init_state ()
{
  // per channel
  for (int a = 0; a < m_parent_data.channel_data.size (); a++)
  {
    scaling   (a, m_parent_data.channel_data[a].scaling);
    coupling  (a, m_parent_data.channel_data[a].coupling);
  }

  // as entire oscilloscope
  time_per_division (m_parent_data.time_per_division, 
    LABSOFT_OSCILLOSCOPE_DISPLAY::NUMBER_OF_COLUMNS);
}

void LAB_Oscilloscope:: 
config_dma_cb ()
{
  m_LAB_Core->map_uncached_mem (
    &m_uncached_dma_data_osc, 
    LAB_OSCILLOSCOPE::VC_MEM_SIZE
  );

  AP_MemoryMap& mp                   = m_uncached_dma_data_osc;
  LAB_DMA_Data_Oscilloscope& dp      = *(static_cast<LAB_DMA_Data_Oscilloscope*>(mp.virt));
  LAB_DMA_Data_Oscilloscope dma_data = 
  {
    .cbs = 
    {
      // For double buffer SPI RX
      { // CB 0
        LAB_DMA_TI_OSC_RX,
        Utility::reg_bus          (m_LAB_Core->m_regs_spi, SPI_FIFO),
        Utility::uncached_reg_bus (mp, &dp.rxd[0]),
        (uint32_t)(4 * LAB_OSCILLOSCOPE::NUMBER_OF_SAMPLES),
        0,
        Utility::uncached_reg_bus (mp, &dp.cbs[1]),
        0
      },
      { // CB 1
        LAB_DMA_TI_OSC_RX, 
        Utility::reg_bus          (m_LAB_Core->m_regs_spi, SPI_CS),
        Utility::uncached_reg_bus (mp, &dp.status[0]),
        4,
        0,
        Utility::uncached_reg_bus (mp, &dp.cbs[2]),
        0
      }, 
      { // CB 2
        LAB_DMA_TI_OSC_RX,
        Utility::reg_bus          (m_LAB_Core->m_regs_spi, SPI_FIFO),
        Utility::uncached_reg_bus (mp, &dp.rxd[1]),
        (uint32_t)(4 * LAB_OSCILLOSCOPE::NUMBER_OF_SAMPLES),
        0,
        Utility::uncached_reg_bus (mp, &dp.cbs[3]),
        0
      }, 
      { // CB 3
        LAB_DMA_TI_OSC_RX, 
        Utility::reg_bus          (m_LAB_Core->m_regs_spi, SPI_CS),
        Utility::uncached_reg_bus (mp, &dp.status[1]),
        4,
        0,
        Utility::uncached_reg_bus (mp, &dp.cbs[0]),
        0
      }, 



      // For single buffer SPI RX
      { // CB 4
        LAB_DMA_TI_OSC_RX,
        Utility::reg_bus          (m_LAB_Core->m_regs_spi, SPI_FIFO),
        Utility::uncached_reg_bus (mp, &dp.rxd[0]),
        (uint32_t)(4 * LAB_OSCILLOSCOPE::NUMBER_OF_SAMPLES),
        0,
        Utility::uncached_reg_bus (mp, &dp.cbs[5]),
        0
      },
      { // CB 5
        LAB_DMA_TI_OSC_RX, 
        Utility::reg_bus          (m_LAB_Core->m_regs_spi, SPI_CS),
        Utility::uncached_reg_bus (mp, &dp.status[0]),
        4,
        0,
        Utility::uncached_reg_bus (mp, &dp.cbs[4]),
        0
      },  



      // For SPI TX
      { // CB 6
        LAB_DMA_TI_OSC_TX,  
        Utility::uncached_reg_bus (mp, &dp.txd),
        Utility::reg_bus          (m_LAB_Core->m_regs_spi, SPI_FIFO),
        //(uint32_t)((4 * LAB_OSCILLOSCOPE::NUMBER_OF_SAMPLES) + 4), 
        4,
        0, 
        Utility::uncached_reg_bus (mp, &dp.cbs[6]), 
        0
      }, 



      // For PWM pacing
      { //CB 7
        LAB_DMA_TI_OSC_PWM_PACING, 
        Utility::uncached_reg_bus (mp, &dp.pwm_rng),   
        Utility::reg_bus (m_LAB_Core->m_regs_pwm, PWM_FIF1), 
        4, 
        0, 
        Utility::uncached_reg_bus (mp, &dp.cbs[8]), 
        0
      }, 
      { // CB 8
        LAB_DMA_TI_OSC_PWM_PACING, 
        Utility::uncached_reg_bus (mp, &dp.spi_cs_fifo_reset), 
        Utility::reg_bus (m_LAB_Core->m_regs_spi, SPI_CS), 
        4, 
        0, 
        Utility::uncached_reg_bus (mp, &dp.cbs[9]), 
        0
      },
      { // CB 9
        LAB_DMA_TI_OSC_PWM_PACING, 
        Utility::uncached_reg_bus (mp, &dp.spi_samp_size), 
        Utility::reg_bus (m_LAB_Core->m_regs_spi, SPI_DLEN), 
        4, 
        0, 
        Utility::uncached_reg_bus (mp, &dp.cbs[10]), 
        0
      },
      { // CB 10
        LAB_DMA_TI_OSC_PWM_PACING, 
        Utility::uncached_reg_bus (mp, &dp.spi_cs),   
        Utility::reg_bus (m_LAB_Core->m_regs_spi, SPI_CS),   
        4, 
        0, 
        Utility::uncached_reg_bus (mp, &dp.cbs[7]), 
        0
      },
    },

    .spi_samp_size      = 4,    // in bytes
    .spi_cs             = SPI_CS_TA | SPI_CS_ADCS | SPI_CS_DMAEN | LAB_OSCILLOSCOPE::ADC_SPI0_CHIP_ENABLE,
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

  std::memcpy (&dp, &dma_data, sizeof (dma_data));
}

LE::DISPLAY_MODE LAB_Oscilloscope:: 
disp_mode ()
{
  return (m_parent_data.disp_mode);
}

void LAB_Oscilloscope:: 
run ()
{
  if (m_LAB->m_Voltmeter.is_running ())
  {
    m_LAB->m_Voltmeter.stop ();
  }

  time_per_division (m_parent_data.time_per_division);
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

void LAB_Oscilloscope:: 
scaling (unsigned       channel, 
         LE_OSC_SCALING _LE_OSC_SCALING)
{
  m_parent_data.channel_data[channel].scaling = _LE_OSC_SCALING;

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

  m_LAB_Core->gpio_write (a0, (_LE_OSC_SCALING == LE_OSC_SCALING::FOURTH || 
    _LE_OSC_SCALING == LE_OSC_SCALING::UNITY) ? 1 : 0);
  
  m_LAB_Core->gpio_write (a1, (_LE_OSC_SCALING == LE_OSC_SCALING::FOURTH || 
    _LE_OSC_SCALING == LE_OSC_SCALING::HALF) ? 1 : 0);
}

void LAB_Oscilloscope:: 
coupling (unsigned        channel,
          LE_OSC_COUPLING _LE_OSC_COUPLING)
{
  m_parent_data.channel_data[channel].coupling = _LE_OSC_COUPLING;

  unsigned pin = (channel == 0) ? LAB_PIN_OSCILLOSCOPE_COUPLING_SELECT_CHANNEL_0 :
    LAB_PIN_OSCILLOSCOPE_COUPLING_SELECT_CHANNEL_1;

  switch (m_parent_data.channel_data[channel].coupling)
  {
    case LE_OSC_COUPLING::AC:
    {
      m_LAB_Core->gpio_set (pin, AP_GPIO_FUNC_INPUT, AP_GPIO_PULL_OFF);
      break;
    }

    case LE_OSC_COUPLING::DC:
    {
      m_LAB_Core->gpio_set (pin, AP_GPIO_FUNC_OUTPUT, AP_GPIO_PULL_DOWN, 0);
      break;
    }

    default:
    {
      break;
    }
  }
}

void LAB_Oscilloscope:: 
fill_raw_sample_buffer ()
{
  LAB_DMA_Data_Oscilloscope* dma_data = static_cast<LAB_DMA_Data_Oscilloscope*>
    (m_uncached_dma_data_osc.virt);
    
  if (disp_mode () == LE::DISPLAY_MODE::SCREEN)
  {
    std::memcpy (
      m_parent_data.raw_sample_buffer.data (),
      (void*)(dma_data->rxd[0]),
      4 *LAB_OSCILLOSCOPE::NUMBER_OF_SAMPLES
    );
  }
  else if (disp_mode () == LE::DISPLAY_MODE::REPEATED)
  {
    for (int a = 0; a < 2; a++)
    {
      if (dma_data->status[a])
      {
        std::memcpy (
          m_parent_data.raw_sample_buffer.data (),
          (void*)(dma_data->rxd[a]),
          4 * LAB_OSCILLOSCOPE::NUMBER_OF_SAMPLES
        );
      }

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

void LAB_Oscilloscope:: 
load_data_samples ()
{
  if (m_parent_data.trig_mode == LE_OSC_TRIG_MODE::NONE && 
    m_parent_data.trig_flag_no_trig_found_yet)
  {
    fill_raw_sample_buffer  ();
    parse_raw_sample_buffer ();
  }
  else 
  {

  }
}

/**
 * @brief Given a uint32_t sample from the raw sample buffer, convert it to the
 *        selected channel's actual value.
 * 
 * @param sample  uint32_t sample from the raw sample buffer
 * @param channel channel of interest
 * @return voltage reading
 */
double LAB_Oscilloscope:: 
conv_raw_samp_buff_samp (uint32_t sample,
                         unsigned channel)
{
  uint32_t  raw_channel_bits  = extract_chan_raw_samp_buff_samp (sample, channel);
  uint32_t  arranged_bits     = arrange_raw_chan_bits (raw_channel_bits);
  bool      sign              = arranged_bits >> (LAB_OSCILLOSCOPE::ADC_RESOLUTION_BITS - 1);
  uint32_t  abs_arranged_bits = arranged_bits & ((LAB_OSCILLOSCOPE::ADC_RESOLUTION_INT - 1) >> 1);
  double    actual_value      = arranged_bits_to_actual_value (abs_arranged_bits, sign);

  return (actual_value);
}

/**
 * @brief Given a uint32_t sample from the raw sample buffer, extract the raw
 *        bits of the given channel.
 * 
 * @param sample uint32_t sample from the raw sample buffer
 * @param channel channel of interest
 * @return raw bits of the given channel
 */
uint32_t LAB_Oscilloscope::
extract_chan_raw_samp_buff_samp (uint32_t  sample,
                                 unsigned channel)
{
  return (
    (sample >> (LAB_OSCILLOSCOPE::RAW_DATA_SHIFT_BIT_COUNT * channel)) &
      LAB_OSCILLOSCOPE::RAW_DATA_POST_SHIFT_MASK
  );
}

/**
 * @brief Given a uint32_t containing the raw data bits of a channel extracted
 *        from the raw sample buffer, arrange it, taking into account the use
 *        of the MCP33111 ADC at 10 MHz SPI frequency.
 * 
 * @param sample raw channel bits
 * @return arranged raw channel bits (for MCP33111 ADC)
 */
uint32_t LAB_Oscilloscope:: 
arrange_raw_chan_bits (uint32_t sample)
{
  return (
    ((sample & 0x7f) << 5) | ((sample & 0xf800) >> 11)
  );
}

/**
 * @brief Given a proper, arranged raw channel bits from the raw sample buffer,
 *        convert it to the actual voltage value. Note that the arranged bits
 *        is minus one bit as the MSB was used as a sign bit.
 * 
 * @param abs_arranged_bits arranged bits, minus the MSB
 * @param sign sign of the arranged channel bits, taken from MSB
 * @return converted voltage value
 */
double LAB_Oscilloscope::
arranged_bits_to_actual_value (uint32_t abs_arranged_bits, bool sign)
{
  if (sign)
  {
    return (static_cast<double>(abs_arranged_bits) *
      LAB_OSCILLOSCOPE::CONVERSION_CONSTANT);
  }
  else 
  {
    return ((static_cast<double>(abs_arranged_bits) * 
      LAB_OSCILLOSCOPE::CONVERSION_CONSTANT) - 
      LAB_OSCILLOSCOPE::CONVERSION_REFERENCE_VOLTAGE
    );
  }
}

void LAB_Oscilloscope:: 
parse_raw_sample_buffer ()
{
  for (int samp = 0; samp < (m_parent_data.raw_sample_buffer.size ()); samp++)
  {
    for (int chan = 0; chan < (m_parent_data.channel_data.size ()); chan++)
    {
      double actual_value = conv_raw_samp_buff_samp (
        m_parent_data.raw_sample_buffer[samp], chan);
      
      // if (samp == 0)
      //   std::cout << actual_value << std::endl;

      m_parent_data.channel_data[chan].samples[samp] = actual_value;
    }
  }
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

double LAB_Oscilloscope:: 
calc_samp_count (double time_per_div, unsigned osc_disp_num_cols)
{
  if (time_per_div >= LAB_OSCILLOSCOPE::MIN_TIME_PER_DIV_NO_ZOOM)
  {
    return (LAB_OSCILLOSCOPE::NUMBER_OF_SAMPLES);
  }
  else 
  {
    return (LAB_OSCILLOSCOPE::MAX_SAMPLING_RATE * osc_disp_num_cols *
      time_per_div);
  }
}

double LAB_Oscilloscope:: 
calc_samp_rate (double time_per_div, unsigned osc_disp_num_cols)
{
  if (time_per_div <= LAB_OSCILLOSCOPE::MIN_TIME_PER_DIV_NO_ZOOM)
  {
    return (LAB_OSCILLOSCOPE::MAX_SAMPLING_RATE);
  }
  else 
  {
    return (
      LAB_OSCILLOSCOPE::NUMBER_OF_SAMPLES / (time_per_div * osc_disp_num_cols)
    );
  }
}

LE::DISPLAY_MODE LAB_Oscilloscope:: 
calc_disp_mode (double time_per_div)
{
  if (time_per_div >= LAB_OSCILLOSCOPE::MIN_TIME_PER_DIV_DISP_SCREEN)
  {
    return (LE::DISPLAY_MODE::SCREEN);
  }
  else 
  {
    return (LE::DISPLAY_MODE::REPEATED);
  }
}

void LAB_Oscilloscope:: 
time_per_division (double value, unsigned osc_disp_num_cols)
{
  double            new_samp_count  = calc_samp_count (value, osc_disp_num_cols);
  double            new_samp_rate   = calc_samp_rate  (value, osc_disp_num_cols);
  LE::DISPLAY_MODE  new_disp_mode   = calc_disp_mode  (value);

  m_parent_data.time_per_division = value;
  m_parent_data.w_samp_count      = new_samp_count;
  m_parent_data.sampling_rate     = new_samp_rate;

  display_mode          (new_disp_mode);
  set_hw_sampling_rate  (m_parent_data.sampling_rate);
}


void LAB_Oscilloscope:: 
sampling_rate (double value, unsigned osc_disp_num_cols)
{
  double tpd = LAB_OSCILLOSCOPE::NUMBER_OF_SAMPLES / (value * osc_disp_num_cols);  
  
  time_per_division (tpd, osc_disp_num_cols);
}

void LAB_Oscilloscope:: 
set_hw_sampling_rate (double value)
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

void LAB_Oscilloscope:: 
horizontal_offset (double value)
{
  m_parent_data.horizontal_offset = value;
}

double LAB_Oscilloscope:: 
horizontal_offset ()
{
  return (m_parent_data.horizontal_offset);
}

// Trigger
void LAB_Oscilloscope:: 
parse_trigger (LE_OSC_TRIG_MODE _LE_OSC_TRIG_MODE)
{
  switch (_LE_OSC_TRIG_MODE)
  {
    case LE_OSC_TRIG_MODE::NONE:
    {
      m_trigger_thread.join ();
      break;
    }

    case LE_OSC_TRIG_MODE::NORMAL:
    {
      m_trigger_thread = std::thread (&LAB_Oscilloscope::search_trigger_point, this);
      
      break;
    }

    case LE_OSC_TRIG_MODE::AUTO:
    {
      break;
    }

    default:
    {
      break;
    }
  }
}

void LAB_Oscilloscope:: 
search_trigger_point ()
{
  uint32_t offset = Utility::dma_chan_reg_offset 
      (LAB_DMA_CHAN_OSCILLOSCOPE_SPI_RX, DMA_DEST_AD);

  while (m_parent_data.trig_mode != LE_OSC_TRIG_MODE::NONE)
  {
    static uint32_t x;

    uint32_t lmao = *(Utility::reg_virt (m_LAB_Core->m_regs_spi, SPI_FIFO));

    if (x != lmao)
    {
      x = lmao;

      std::cout << std::hex << x << std::endl;
    }
  }
}

bool LAB_Oscilloscope:: 
is_trigger_point (unsigned buff,
                  unsigned samp)
{
  LAB_DMA_Data_Oscilloscope& dma_data = *(static_cast<LAB_DMA_Data_Oscilloscope*>
    (m_uncached_dma_data_osc.virt));

  switch (m_parent_data.trig_type)
  {
    case (LE_OSC_TRIG_TYPE::EDGE):
    {
      switch (m_parent_data.trig_condition)
      {
        case (LE_OSC_TRIG_CND::RISING):
        {
          if (dma_data.rxd[buff][samp] > m_parent_data.trig_level)
          {
            return (true);
          }
          else 
          {
            return (false);
          }

          break;
        }

        case (LE_OSC_TRIG_CND::FALLING):
        {
          if (dma_data.rxd[buff][samp] < m_parent_data.trig_level)
          {
            return (true);
          }
          else 
          {
            return (false);
          }

          break;
        }

        default:
        {
          break;
        }
      }

      break;
    }

    default:
    {
      return (false);
      break;
    }
  }

  return (false);
}

void LAB_Oscilloscope::
service_trigger (unsigned buff,
                 unsigned samp)
{
  std::cout << "Triggered!" << std::endl;
  std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(1))
;}

void LAB_Oscilloscope:: 
trigger_mode (LE_OSC_TRIG_MODE _LE_OSC_TRIG_MODE)
{
  m_parent_data.trig_mode = _LE_OSC_TRIG_MODE;

  parse_trigger (m_parent_data.trig_mode);
}

LE_OSC_TRIG_MODE LAB_Oscilloscope:: 
trigger_mode () 
{
  return (m_parent_data.trig_mode);
}

void LAB_Oscilloscope:: 
trigger_level (double value)
{
  if (value >= LAB_OSCILLOSCOPE::MIN_TRIGGER_LEVEL && 
    value <= LAB_OSCILLOSCOPE::MAX_TRIGGER_LEVEL)
  {
    m_parent_data.trig_level = value;
  }
}

double LAB_Oscilloscope::    
trigger_level ()
{
  return (m_parent_data.trig_level);
}

void LAB_Oscilloscope:: 
trigger_source  (unsigned chan)
{
  if (chan >= 0 && chan <= LAB_OSCILLOSCOPE::NUMBER_OF_CHANNELS)
  {
    m_parent_data.trig_source = chan;
  }
}

double LAB_Oscilloscope:: 
trigger_source ()
{
  return (m_parent_data.trig_source);
}

// Display
void LAB_Oscilloscope:: 
display_mode_frontend (LE::DISPLAY_MODE _DISPLAY_MODE)
{
  switch (_DISPLAY_MODE)
  {
    case LE::DISPLAY_MODE::SCREEN:
    {
      time_per_division (LAB_OSCILLOSCOPE::MIN_TIME_PER_DIV_DISP_SCREEN);
      break;
    }

    case LE::DISPLAY_MODE::REPEATED:
    {
      time_per_division (LAB_OSCILLOSCOPE::MIN_TIME_PER_DIV_NO_ZOOM);
      break;
    }

    default:
    {
      break;
    }
  }
}

LE::DISPLAY_MODE LAB_Oscilloscope::
display_mode ()
{
  return (m_parent_data.disp_mode);
}

void LAB_Oscilloscope::   
display_mode (LE::DISPLAY_MODE _DISPLAY_MODE)
{
  switch (_DISPLAY_MODE)
  {
    case LE::DISPLAY_MODE::REPEATED:
    {
      switch_dma_buffer (LE_SPI_DMA_NUMBER_OF_BUFFERS_DOUBLE);
      break;
    }

    case LE::DISPLAY_MODE::SCREEN:
    { 
      switch_dma_buffer (LE_SPI_DMA_NUMBER_OF_BUFFERS_SINGLE);
      break;
    }

    default:
    {
      break;
    }
  }

  m_parent_data.disp_mode = _DISPLAY_MODE;
}

void LAB_Oscilloscope:: 
update_dma_data (int disp_mode)
{
  // m_LAB_Core->dma_pause (LAB_DMA_CHAN_PWM_PACING);
  
  // volatile uint32_t *reg = Utility::reg_virt (m_LAB_Core->m_regs_dma,
  //   DMA_REG (LAB_DMA_CHAN_PWM_PACING, DMA_NEXTCONBK));

  // *reg = Utility::uncached_reg_bus ()
  

  // Utility::write_reg_virt (*reg, )

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

  // 1. Load the next control block, depending on the buffer 
  LAB_DMA_Data_Oscilloscope& dma_data = *(static_cast<LAB_DMA_Data_Oscilloscope*>
    (m_uncached_dma_data_osc.virt));

  volatile uint32_t& reg = *(Utility::reg_virt (m_LAB_Core->m_regs_dma,
    Utility::dma_chan_reg_offset (LAB_DMA_CHAN_OSCILLOSCOPE_SPI_RX, DMA_NEXTCONBK)));

  if (buffer == LE_SPI_DMA_NUMBER_OF_BUFFERS_SINGLE)
  {
    reg = Utility::uncached_reg_bus (m_uncached_dma_data_osc, &dma_data.cbs[4]);
  }
  else // (buffer == LE_SPI_DMA_NUMBER_OF_BUFFERS_DOUBLE)
  {
    reg = Utility::uncached_reg_bus (m_uncached_dma_data_osc, &dma_data.cbs[0]);
  }  

  // 2. Abort current DMA control block
  m_LAB_Core->dma_abort (LAB_DMA_CHAN_OSCILLOSCOPE_SPI_RX);

  // 3. Clear buffer status
  dma_data.status[0] = dma_data.status[1] = 0;

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
  //   LABSOFT_OSCILLOSCOPE_DISPLAY::NUMBER_OF_COLUMNS);

  // Init scaling
  for (int a = 0; a < m_parent_data.channel_data.size (); a++)
  {
    scaling (a, m_parent_data.channel_data[a].scaling);
  }

  return 1;
}

// Getters
bool LAB_Oscilloscope:: 
is_osc_frontend_running ()
{
  return (m_parent_data.is_osc_frontend_running);
}

bool LAB_Oscilloscope:: 
is_osc_core_running ()
{
  return (m_parent_data.is_osc_core_running);
}

double LAB_Oscilloscope::  
voltage_per_division (unsigned channel)
{
  return (m_parent_data.channel_data[channel].voltage_per_division);
}

double LAB_Oscilloscope:: 
vertical_offset (unsigned channel)
{
  return (m_parent_data.channel_data[channel].vertical_offset);
}

double LAB_Oscilloscope:: 
time_per_division ()
{
  return (m_parent_data.time_per_division);
}

double LAB_Oscilloscope:: 
sampling_rate ()
{
  return (m_parent_data.sampling_rate);
}

// EOF