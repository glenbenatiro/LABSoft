#include "LAB_Oscilloscope.h"

#include <cstring>
#include <iostream>

// remove soon
#include <chrono>

#include "LAB.h"

LAB_Oscilloscope:: 
LAB_Oscilloscope (LAB_Core* _LAB_Core, LAB* _LAB) 
  : m_LAB_Core (_LAB_Core), m_LAB (_LAB)
{
  init_spi        ();
  init_pwm        ();
  init_gpio_pins  ();
  init_dma        (); 
  init_state      ();
}

LAB_Oscilloscope::
~LAB_Oscilloscope ()
{
  m_LAB_Core->dma.stop  (LABC::DMA::CHAN::OSC_RX);
  m_LAB_Core->dma.stop  (LABC::DMA::CHAN::OSC_TX);
  m_LAB_Core->dma.stop  (LABC::DMA::CHAN::PWM_PACING);

  m_LAB_Core->dma.reset (LABC::DMA::CHAN::OSC_RX);
  m_LAB_Core->dma.reset (LABC::DMA::CHAN::OSC_TX);
  m_LAB_Core->dma.reset (LABC::DMA::CHAN::PWM_PACING);
}

void LAB_Oscilloscope:: 
init_spi ()
{
  m_LAB_Core->spi.clear_fifo  ();
  m_LAB_Core->spi.reg         (AP::SPI::DC, (8 << 24) | (4 << 16) | (8 << 8) | 1);
  m_LAB_Core->spi.frequency   (LABC::SPI::FREQUENCY);

  m_LAB_Core->gpio.set (AP::RPI::PIN::SPI::CE0,  AP::GPIO::FUNC::ALT0, AP::GPIO::PULL::OFF);
  m_LAB_Core->gpio.set (AP::RPI::PIN::SPI::CE1,  AP::GPIO::FUNC::ALT0, AP::GPIO::PULL::OFF);
  m_LAB_Core->gpio.set (AP::RPI::PIN::SPI::MISO, AP::GPIO::FUNC::ALT0, AP::GPIO::PULL::DOWN);
  m_LAB_Core->gpio.set (AP::RPI::PIN::SPI::MOSI, AP::GPIO::FUNC::ALT0, AP::GPIO::PULL::OFF);
  m_LAB_Core->gpio.set (AP::RPI::PIN::SPI::SCLK, AP::GPIO::FUNC::ALT0, AP::GPIO::PULL::OFF);
}

void LAB_Oscilloscope:: 
init_pwm ()
{
  m_LAB_Core->cm.pwm.frequency  (LABC::CLKMAN::FREQUENCY);

  m_LAB_Core->pwm.reg           (AP::PWM::DMAC, (1 << 31) | (8 << 8) | (1 << 0));
  m_LAB_Core->pwm.use_fifo      (LABC::PWM::DMA_PACING_CHAN, true);
  m_LAB_Core->pwm.algo          (LABC::PWM::DMA_PACING_CHAN, AP::PWM::ALGO::MARKSPACE);
  m_LAB_Core->pwm.frequency     (LABC::PWM::DMA_PACING_CHAN, LABC::OSC::SAMPLING_RATE);
  m_LAB_Core->pwm.duty_cycle    (LABC::PWM::DMA_PACING_CHAN, 50.0);

  m_LAB_Core->gpio.set          (LABC::PIN::PWM, AP::GPIO::FUNC::ALT0, AP::GPIO::PULL::DOWN);
}

void LAB_Oscilloscope::
init_gpio_pins ()
{
  // scaling
  m_LAB_Core->gpio.set (LABC::PIN::OSC_MUX_SCALER_A0_CHAN_0, 
    AP::GPIO::FUNC::OUTPUT, AP::GPIO::PULL::DOWN, 1);

  m_LAB_Core->gpio.set (LABC::PIN::OSC_MUX_SCALER_A1_CHAN_0, 
    AP::GPIO::FUNC::OUTPUT, AP::GPIO::PULL::DOWN, 0);

  m_LAB_Core->gpio.set (LABC::PIN::OSC_MUX_SCALER_A0_CHAN_1, 
    AP::GPIO::FUNC::OUTPUT, AP::GPIO::PULL::DOWN, 1);

  m_LAB_Core->gpio.set (LABC::PIN::OSC_MUX_SCALER_A1_CHAN_1, 
    AP::GPIO::FUNC::OUTPUT, AP::GPIO::PULL::DOWN, 0);

  // coupling
  m_LAB_Core->gpio.set (LABC::PIN::OSC_COUPLING_SELECT_CHAN_0, 
    AP::GPIO::FUNC::OUTPUT, AP::GPIO::PULL::DOWN, 0);

  m_LAB_Core->gpio.set (LABC::PIN::OSC_COUPLING_SELECT_CHAN_1, 
    AP::GPIO::FUNC::OUTPUT, AP::GPIO::PULL::DOWN, 0);
}

void LAB_Oscilloscope::
init_dma ()
{
  config_dma_cb ();

  AikaPi::Uncached&          mp = m_uncached_memory;
  LAB_DMA_Data_Oscilloscope& dp = *(static_cast<LAB_DMA_Data_Oscilloscope*>(mp.virt ()));

  m_LAB_Core->dma.start (LABC::DMA::CHAN::OSC_TX,     mp.bus (&dp.cbs[6]));
  m_LAB_Core->dma.start (LABC::DMA::CHAN::OSC_RX,     mp.bus (&dp.cbs[0]));
  m_LAB_Core->dma.start (LABC::DMA::CHAN::PWM_PACING, mp.bus (&dp.cbs[7]));
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
  m_uncached_memory.map_uncached_mem (LABC::OSC::VC_MEM_SIZE);

  LAB_DMA_Data_Oscilloscope& uncached_dma_data = 
    *(static_cast<LAB_DMA_Data_Oscilloscope*>(m_uncached_memory.virt ()));

  LAB_DMA_Data_Oscilloscope new_uncached_dma_data = 
  {
    .cbs = 
    {
      // SPI RX - Double Buffer
      // 0
      {
        LABC::DMA::TI::OSC_RX,
        m_LAB_Core->spi.bus   (SPI_FIFO),
        m_uncached_memory.bus (&uncached_dma_data.rxd[0]),
        static_cast<uint32_t> (sizeof (uint32_t) * LABC::OSC::NUMBER_OF_SAMPLES),
        0,
        m_uncached_memory.bus (&uncached_dma_data.cbs[1]),
        0
      },
      // 1
      {
        LABC::DMA::TI::OSC_RX | AP::DMA::TI_DATA::INTEN,
        m_LAB_Core->spi.bus   (SPI_CS),
        m_uncached_memory.bus (&uncached_dma_data.status[0]),
        sizeof (uint32_t),
        0,
        m_uncached_memory.bus (&uncached_dma_data.cbs[2]),
        0
      },
      // 2
      {
        LABC::DMA::TI::OSC_RX,
        m_LAB_Core->spi.bus   (SPI_FIFO),
        m_uncached_memory.bus (&uncached_dma_data.rxd[1]),
        static_cast<uint32_t> (sizeof (uint32_t) * LABC::OSC::NUMBER_OF_SAMPLES),
        0,
        m_uncached_memory.bus (&uncached_dma_data.cbs[3]),
        0
      },
      // 3
      {
        LABC::DMA::TI::OSC_RX | AP::DMA::TI_DATA::INTEN,
        m_LAB_Core->spi.bus   (SPI_CS),
        m_uncached_memory.bus (&uncached_dma_data.status[1]),
        sizeof (uint32_t),
        0,
        m_uncached_memory.bus (&uncached_dma_data.cbs[0]),
        0
      },

      // SPI RX - Single Buffer
      // 4
      {
        LABC::DMA::TI::OSC_RX,
        m_LAB_Core->spi.bus   (SPI_FIFO),
        m_uncached_memory.bus (&uncached_dma_data.rxd[0]),
        static_cast<uint32_t> (sizeof (uint32_t) * LABC::OSC::NUMBER_OF_SAMPLES),
        0,
        m_uncached_memory.bus (&uncached_dma_data.cbs[5]),
        0
      },
      // 5
      {
        LABC::DMA::TI::OSC_RX | AP::DMA::TI_DATA::INTEN,
        m_LAB_Core->spi.bus   (SPI_CS),
        m_uncached_memory.bus (&uncached_dma_data.status[0]),
        sizeof (uint32_t),
        0,
        m_uncached_memory.bus (&uncached_dma_data.cbs[4]),
        0
      },

      // SPI TX
      // 6
      {
        LABC::DMA::TI::OSC_TX,
        m_uncached_memory.bus (&uncached_dma_data.txd),
        m_LAB_Core->spi.bus   (SPI_FIFO),
        sizeof (uint32_t),
        0,
        m_uncached_memory.bus (&uncached_dma_data.cbs[6]),
        0
      },

      // PWM Pacing
      // 7
      {
        LABC::DMA::TI::OSC_PWM_PACING,
        m_uncached_memory.bus (&uncached_dma_data.pwm_duty_cycle),
        m_LAB_Core->pwm.bus   (PWM_FIF1),
        sizeof (uint32_t),
        0,
        m_uncached_memory.bus (&uncached_dma_data.cbs[8]),
        0
      },
      // 8
      {
        LABC::DMA::TI::OSC_PWM_PACING,
        m_uncached_memory.bus (&uncached_dma_data.spi_cs_fifo_reset),
        m_LAB_Core->spi.bus   (SPI_CS),
        sizeof (uint32_t),
        0,
        m_uncached_memory.bus (&uncached_dma_data.cbs[9]),
        0
      },
      // 9
      {
        LABC::DMA::TI::OSC_PWM_PACING,
        m_uncached_memory.bus (&uncached_dma_data.spi_samp_size),
        m_LAB_Core->spi.bus   (SPI_DLEN),
        sizeof (uint32_t),
        0,
        m_uncached_memory.bus (&uncached_dma_data.cbs[10]),
        0
      },
      // 10
      {
        LABC::DMA::TI::OSC_PWM_PACING,
        m_uncached_memory.bus (&uncached_dma_data.spi_cs),
        m_LAB_Core->spi.bus   (SPI_CS),
        sizeof (uint32_t),
        0,
        m_uncached_memory.bus (&uncached_dma_data.cbs[7]),
        0
      },     
    },   

    .spi_samp_size      = sizeof (uint32_t),    
    .spi_cs             = SPI_CS_TA | SPI_CS_ADCS | SPI_CS_DMAEN | LABC::OSC::ADC_SPI0_CHIP_ENABLE,
    .spi_cs_fifo_reset  = 0x00000030,
    .pwm_duty_cycle     = 0x0,
    .txd                = 0x0000ffff
  };

  std::memcpy (
    &uncached_dma_data, 
    &new_uncached_dma_data, 
    sizeof (new_uncached_dma_data)
  );
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
    m_LAB_Core->pwm.start (LABC::PWM::DMA_PACING_CHAN);
    m_parent_data.is_osc_core_running = true;
  }
  else
  {
    m_LAB_Core->pwm.stop (LABC::PWM::DMA_PACING_CHAN);
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
scaling (unsigned           channel, 
         LABE::OSC::SCALING scaling)
{
  m_parent_data.channel_data[channel].scaling = scaling;

  unsigned a0, a1;

  if (channel == 0)
  {
    a0 = LABC::PIN::OSC_MUX_SCALER_A0_CHAN_0;
    a1 = LABC::PIN::OSC_MUX_SCALER_A1_CHAN_0;
  }
  else
  {
    a0 = LABC::PIN::OSC_MUX_SCALER_A0_CHAN_1;
    a1 = LABC::PIN::OSC_MUX_SCALER_A1_CHAN_1;
  }

  m_LAB_Core->gpio.write (a0, (scaling == LABE::OSC::SCALING::FOURTH || 
    scaling == LABE::OSC::SCALING::UNITY) ? 1 : 0);
  
  m_LAB_Core->gpio.write (a1, (scaling == LABE::OSC::SCALING::FOURTH || 
    scaling == LABE::OSC::SCALING::HALF) ? 1 : 0);
}

void LAB_Oscilloscope:: 
coupling (unsigned            channel,
          LABE::OSC::COUPLING coupling)
{
  m_parent_data.channel_data[channel].coupling = coupling;

  unsigned pin = (channel == 0) ? LABC::PIN::OSC_COUPLING_SELECT_CHAN_0 :
    LABC::PIN::OSC_COUPLING_SELECT_CHAN_1;

  switch (m_parent_data.channel_data[channel].coupling)
  {
    case LABE::OSC::COUPLING::AC:
    {
      m_LAB_Core->gpio.set (pin, AP::GPIO::FUNC::INPUT, AP::GPIO::PULL::OFF);
      break;
    }

    case LABE::OSC::COUPLING::DC:
    {
      m_LAB_Core->gpio.set (pin, AP::GPIO::FUNC::OUTPUT, AP::GPIO::PULL::DOWN, 0);
      break;
    }

    default:
    {
      break;
    }
  }
}

void LAB_Oscilloscope:: 
load_data_samples ()
{
  fill_raw_sample_buffer  ();
  parse_raw_sample_buffer ();
}

void LAB_Oscilloscope:: 
fill_raw_sample_buffer ()
{
  LAB_DMA_Data_Oscilloscope& dma_data = *(static_cast<LAB_DMA_Data_Oscilloscope*>
    (m_uncached_memory.virt ()));

  if (disp_mode () == LE::DISPLAY_MODE::SCREEN)
  {
    std::memcpy (
      m_parent_data.raw_sample_buffer.data (),
      const_cast<const void*>(static_cast<volatile void*>(dma_data.rxd[0])),
      sizeof (uint32_t) * LABC::OSC::NUMBER_OF_SAMPLES
    );
  }
  else if (disp_mode () == LE::DISPLAY_MODE::REPEATED)
  {
    for (int buff = 0; buff < 2; buff++)
    {
      if (dma_data.status[buff])
      {
        std::memcpy (
          m_parent_data.raw_sample_buffer.data (),
          const_cast<const void*>(static_cast<volatile void*>(dma_data.rxd[1])),
          sizeof (uint32_t) * LABC::OSC::NUMBER_OF_SAMPLES
        );
      }

      // Check if the other buffer is also full.
      // If it is, then we have a buffer overflow (both buffers full).
      if (dma_data.status[buff ^ 1])
      {
        dma_data.status[0] = dma_data.status[1] = 0;

        break;
      }

      dma_data.status[buff] = 0;
    }
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
      
      m_parent_data.channel_data[chan].samples[samp] = actual_value;
    }
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
constexpr double LAB_Oscilloscope:: 
conv_raw_samp_buff_samp (uint32_t sample,
                         unsigned channel)
{
  uint32_t  raw_channel_bits  = extract_chan_raw_samp_buff_samp (sample, channel);
  uint32_t  arranged_bits     = arrange_raw_chan_bits           (raw_channel_bits);
  bool      sign              = arranged_bits >> (LABC::OSC::ADC_RESOLUTION_BITS - 1);
  uint32_t  abs_arranged_bits = arranged_bits & ((LABC::OSC::ADC_RESOLUTION_INT - 1) >> 1);
  double    actual_value      = arranged_bits_to_actual_value   (abs_arranged_bits, sign);

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
constexpr uint32_t LAB_Oscilloscope::
extract_chan_raw_samp_buff_samp (uint32_t sample,
                                 unsigned channel)
{
  return (
    (sample >> (LABC::OSC::RAW_DATA_SHIFT_BIT_COUNT * channel)) &
      LABC::OSC::RAW_DATA_POST_SHIFT_MASK
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
constexpr uint32_t LAB_Oscilloscope:: 
arrange_raw_chan_bits (uint32_t sample)
{
  return (((sample & 0x7F) << 5) | ((sample & 0xF800) >> 11));
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
constexpr double LAB_Oscilloscope::
arranged_bits_to_actual_value (uint32_t abs_arranged_bits, 
                               bool     sign)
{
  if (sign)
  {
    return (static_cast<double>(abs_arranged_bits) *
      LABC::OSC::CONVERSION_CONSTANT);
  }
  else 
  {
    return ((static_cast<double>(abs_arranged_bits) * 
      LABC::OSC::CONVERSION_CONSTANT) - 
      LABC::OSC::CONVERSION_REFERENCE_VOLTAGE
    );
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
  if (time_per_div >= LABC::OSC::MIN_TIME_PER_DIV_NO_ZOOM)
  {
    return (LABC::OSC::NUMBER_OF_SAMPLES);
  }
  else 
  {
    return (LABC::OSC::MAX_SAMPLING_RATE * osc_disp_num_cols *
      time_per_div);
  }
}

double LAB_Oscilloscope:: 
calc_samp_rate (double time_per_div, unsigned osc_disp_num_cols)
{
  if (time_per_div <= LABC::OSC::MIN_TIME_PER_DIV_NO_ZOOM)
  {
    return (LABC::OSC::MAX_SAMPLING_RATE);
  }
  else 
  {
    return (
      LABC::OSC::NUMBER_OF_SAMPLES / (time_per_div * osc_disp_num_cols)
    );
  }
}

LE::DISPLAY_MODE LAB_Oscilloscope:: 
calc_disp_mode (double time_per_div)
{
  if (time_per_div >= LABC::OSC::MIN_TIME_PER_DIV_DISP_SCREEN)
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
  double tpd = LABC::OSC::NUMBER_OF_SAMPLES / (value * osc_disp_num_cols);  
  
  time_per_division (tpd, osc_disp_num_cols);
}

void LAB_Oscilloscope:: 
set_hw_sampling_rate (double value)
{
  m_LAB_Core->pwm.frequency (LABC::PWM::DMA_PACING_CHAN, value);

  // Set duty cycle to 50%
  uint32_t fifo_data = m_LAB_Core->pwm.range (LABC::PWM::DMA_PACING_CHAN) / 2.0;

  static_cast<LAB_DMA_Data_Oscilloscope*>(m_uncached_memory.virt ())->
    pwm_duty_cycle = fifo_data;
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
parse_trigger (LABE::OSC::TRIG::MODE value)
{
  switch (value)
  {
    case LABE::OSC::TRIG::MODE::NONE:
    {
      m_parent_data.find_trigger = false;
      m_trigger_thread.join ();
      break;
    }

    case LABE::OSC::TRIG::MODE::NORMAL:
    {
      m_parent_data.find_trigger = true;
      m_trigger_thread = std::thread (&LAB_Oscilloscope::search_trigger_point, this);
      
      break;
    }

    case LABE::OSC::TRIG::MODE::AUTO:
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
  volatile uint32_t* int_reg          = m_LAB_Core->dma.Peripheral::reg (AP::DMA::INT_STATUS);
  LAB_DMA_Data_Oscilloscope& dma_data = *(static_cast<LAB_DMA_Data_Oscilloscope*>
    (m_uncached_memory.virt ()));

  const uint32_t buff0_bus_addr = m_uncached_memory.bus (&dma_data.cbs[0]);
  const uint32_t buff1_bus_addr = m_uncached_memory.bus (&dma_data.cbs[2]);

  bool    filled_buffer = 0;
  bool    trigger_found = false;
  double  samp          = 0.0;
  double  before        = 0.0;

  std::cout << std::hex;

  while (m_parent_data.find_trigger)
  {
    m_LAB_Core->dma.disp_reg (LABC::DMA::CHAN::OSC_RX, AP::DMA::CONBLK_AD);
    
    
    // x. Check if the store SPI CS control block has finished (interrupt asserted)
    //if (((*int_reg >> LABC::DMA::CHAN::OSC_RX) & 0x1) && !trigger_found)
    //{
      // x. Store the current control block address to know what buffer # was just filled
      // uint32_t curr_conblk_ad = *(m_LAB_Core->dma.reg (LABC::DMA::CHAN::OSC_RX, AP::DMA::CONBLK_AD));

      // // x. Reset interrupt flag
      // m_LAB_Core->dma.Peripheral::reg_wbits (AP::DMA::INT_STATUS, 0, LABC::DMA::CHAN::OSC_RX);

      // // x. Store a copy of both volatile buffers
      // // std::memcpy (
      // //   m_parent_data.trig_buffers.pre_trigger.data (),
      // //   const_cast<const void*>(static_cast<const volatile void*>(dma_data.rxd)),
      // //   sizeof (uint32_t) * LABC::OSC::NUMBER_OF_CHANNELS * LABC::OSC::NUMBER_OF_SAMPLES
      // // );

      // if ((*int_reg >> LABC::DMA::CHAN::OSC_RX) & 0x1)
      // {
      //   std::cout << "trigger too slow!" << std::endl;
      //   m_LAB_Core->dma.Peripheral::reg_wbits (AP::DMA::INT_STATUS, 0, LABC::DMA::CHAN::OSC_RX);
      // }

      // // x. Identify the buffer that was filled
      // if (curr_conblk_ad == buff0_bus_addr)
      // {
      //   filled_buffer = 1;
      // }
      // else if (curr_conblk_ad == buff1_bus_addr)
      // {
      //   filled_buffer = 0;
      // }

      // // x. Search if trigger is present
      // switch (m_parent_data.trig_type)
      // {
      //   case (LABC::OSC::TRIG::TYPE::EDGE):
      //   {
      //     // x. Init start condition
      //     before = conv_raw_samp_buff_samp (
      //       m_parent_data.trig_buffers.pre_trigger[filled_buffer][0],
      //       m_parent_data.trig_source
      //     );

      //     switch (m_parent_data.trig_condition)
      //     {
      //       case (LABC::OSC::TRIG::CND::RISING):
      //       {
      //         for (int a = 1; a < LABC::OSC::NUMBER_OF_SAMPLES; a++)
      //         {
      //           double samp = conv_raw_samp_buff_samp (
      //             m_parent_data.trig_buffers.pre_trigger[filled_buffer][a],
      //             m_parent_data.trig_source
      //           );

      //           if ((samp >= m_parent_data.trig_level) && (before < m_parent_data.trig_level))
      //           {
      //             m_parent_data.trig_index  = a;
      //             m_parent_data.trig_buffer = filled_buffer;
      //             trigger_found             = true;
      //             break;
      //           }

      //           before = samp;
      //         }

      //         break;
      //       }

      //       case (LABC::OSC::TRIG::CND::FALLING):
      //       {
      //         for (int a = 1; a < LABC::OSC::NUMBER_OF_SAMPLES; a++)
      //         {
      //           double samp = conv_raw_samp_buff_samp (
      //             m_parent_data.trig_buffers.pre_trigger[filled_buffer][a],
      //             m_parent_data.trig_source
      //           );

      //           if ((samp <= m_parent_data.trig_level) && (before > m_parent_data.trig_level))
      //           {
      //             m_parent_data.trig_index  = a;
      //             m_parent_data.trig_buffer = filled_buffer;
      //             trigger_found             = true;
      //             break;
      //           }

      //           before = samp;
      //         }

      //         break;
      //       }

      //       case (LABC::OSC::TRIG::CND::EITHER):
      //       {
      //         for (int a = 1; a < LABC::OSC::NUMBER_OF_SAMPLES; a++)
      //         {
      //           double samp = conv_raw_samp_buff_samp (
      //             m_parent_data.trig_buffers.pre_trigger[filled_buffer][a],
      //             m_parent_data.trig_source
      //           );

      //           if (((samp <= m_parent_data.trig_level) && (before > m_parent_data.trig_level)) || 
      //           ((samp >= m_parent_data.trig_level) && (before < m_parent_data.trig_level)))
      //           {
      //             m_parent_data.trig_index  = a;
      //             m_parent_data.trig_buffer = filled_buffer;
      //             trigger_found             = true;
      //             break;
      //           }

      //           before = samp;
      //         }

      //         break;
      //       }
      //     }

      //     break;
      //   }
      // }

      // // x. Check if trigger was found
      // if (trigger_found)
      // {
      //   // service trigger here

      //   // create_trigger_block ();

      //   trigger_found = false;
      //}

      // x. Check if the DMA interrupt flag is asserted. If it is, that means 
      //    we ran out of time; execution time >= 10ms. Throw.
      // if ((*int_reg >> LABC::DMA::CHAN::OSC_RX) & 0x1)
      // {
      //   // throw (std::runtime_error ("Oscilloscope trigger too slow! >= 10ms"));
      //   printf ("Oscilloscope trigger too slow! >= 10ms\n");
      // }

      /**
       *  END TRIGGER CYCLE. 
       * 
       *  TIME STOP. 
       */
    
  }
}

void LAB_Oscilloscope:: 
create_trigger_block ()
{
  static constexpr unsigned samp_half       = LABC::OSC::NUMBER_OF_SAMPLES / 2.0;
  static constexpr unsigned samp_half_index = samp_half - 1;
  static constexpr unsigned copy_size       = sizeof (uint32_t) * samp_half;

  // if (m_parent_data.trig_index >= samp_half_index)
  // {
  //   // copy pre trigger
  //   std::memcpy (
  //     m_parent_data.trig_buffers.assembled_block.data (),
  //     m_parent_data.trig_buffers.pre_trigger[m_parent_data.trig_buffer].data () + 
  //       (m_parent_data.trig_index - samp_half),
  //     sizeof (uint32_t) * samp_half
  //   );

  //   // copy post trigger



  //   unsigned pbuff0_length = (LABC::OSC::NUMBER_OF_SAMPLES - 1) - 
  //     m_parent_data.trig_index;

  //   // copy post buffer (first buffer)
  //   std::memcpy (
  //     m_parent_data.trig_buffers.assembled_block.data () + (samp_half + 1),
  //     m_parent_data.trig_buffers.pre_trigger[m_parent_data.trig_buffer].data () + 
  //       (m_parent_data.trig_index + 1),
  //     pbuff0_length      
  //   );

  //   unsigned pbuff1_length = (LABC::OSC::NUMBER_OF_SAMPLES - 1) - 
  //     pbuff0_length;

  //   // copy post buffer (second buffer)
  //   std::memcpy (
  //     m_parent_data.trig_buffers.assembled_block.data () + (samp_half + 1 + pbuff0_length),
  //     m_parent_data.trig_buffers.pre_trigger[(m_parent_data.trig_buffer) ^ 1].data () + 
  //       (m_parent_data.trig_index + 1),
  //     pbuff1_length      
  //   );
  // }
  // else 
  // {

  // }




  
  // if (m_parent_data.trig_index >= samp_half_index)
  // {
  //   // create pre trigger buffer
  //   std::memcpy (
  //     m_parent_data.trig_buffers.assembled_block.data (),
  //     m_parent_data.trig_buffers.pre_trigger[m_parent_data.trig_buffer].data () + 
  //       (m_parent_data.trig_index - samp_half_index),
  //     copy_size
  //   );

  //   // create post trigger buffer
  //   std::memcpy (
  //     m_parent_data.trig_buffers.assembled_block.data () + (copy_size),
  //     m_parent_data.trig_buffers.pre_trigger[m_parent_data.trig_buffer].data () + 
  //       (m_parent_data.trig_index + 1),
  //     copy_size
  //   );

  //   // std::memcpy (
  //   //   m_parent_data.raw_sample_buffer.data () + (copy_size),
  //   //   m_parent_data.trig_buffer.post_
  //   //   copy_size
  //   // );
  // }
  // else // (trix_index < samp_half_index)
  // {

  // }
}

void LAB_Oscilloscope:: 
trigger_mode (LABE::OSC::TRIG::MODE value)
{
  m_parent_data.trig_mode = value;

  parse_trigger (m_parent_data.trig_mode);
}

LABE::OSC::TRIG::MODE LAB_Oscilloscope:: 
trigger_mode () const
{
  return (m_parent_data.trig_mode);
}

void LAB_Oscilloscope:: 
trigger_source (unsigned chan)
{
  if (chan >= 0 && chan <= LABC::OSC::NUMBER_OF_CHANNELS)
  {
    m_parent_data.trig_source = chan;
  }
}

double LAB_Oscilloscope:: 
trigger_source () const
{
  return (m_parent_data.trig_source);
}

void LAB_Oscilloscope::
trigger_type (LABE::OSC::TRIG::TYPE value)
{
  m_parent_data.trig_type = value;
}

LABE::OSC::TRIG::TYPE LAB_Oscilloscope:: 
trigger_type () const
{
  return (m_parent_data.trig_type);
}

void LAB_Oscilloscope::
trigger_condition (LABE::OSC::TRIG::CND value)
{
  m_parent_data.trig_condition = value;
}

LABE::OSC::TRIG::CND LAB_Oscilloscope::
trigger_condition () const
{
  return (m_parent_data.trig_condition);
}

void LAB_Oscilloscope:: 
trigger_level (double value)
{
  if (value >= LABC::OSC::MIN_TRIGGER_LEVEL && 
    value <= LABC::OSC::MAX_TRIGGER_LEVEL)
  {
    m_parent_data.trig_level = value;
  }
}

double LAB_Oscilloscope::    
trigger_level () const
{
  return (m_parent_data.trig_level);
}

// Display
void LAB_Oscilloscope:: 
display_mode_frontend (LE::DISPLAY_MODE _DISPLAY_MODE)
{
  switch (_DISPLAY_MODE)
  {
    case LE::DISPLAY_MODE::SCREEN:
    {
      time_per_division (LABC::OSC::MIN_TIME_PER_DIV_DISP_SCREEN);
      break;
    }

    case LE::DISPLAY_MODE::REPEATED:
    {
      time_per_division (LABC::OSC::MIN_TIME_PER_DIV_NO_ZOOM);
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
      switch_dma_buffer (LABC::DMA::BUFFER_COUNT::DOUBLE);
      break;
    }

    case LE::DISPLAY_MODE::SCREEN:
    { 
      switch_dma_buffer (LABC::DMA::BUFFER_COUNT::SINGLE);
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
  // m_LAB_Core->dma_pause (LABC::DMA::CHANPWM_PACING);
  
  // volatile uint32_t *reg = Utility::reg (m_LAB_Core->m_regs_dma,
  //   DMA_REG (LABC::DMA::CHANPWM_PACING, DMA_NEXTCONBK));

  // *reg = Utility::bus ()
  

  // Utility::write_reg_virt (*reg, )

  // *REG32(m_regs_dma, DMA_REG(chan, DMA_CONBLK_AD)) = MEM_BUS_ADDR(mp, cbp);

  // m_LAB_Core->dma_play (LABC::DMA::CHANPWM_PACING);
}

void LAB_Oscilloscope:: 
switch_dma_buffer (LABC::DMA::BUFFER_COUNT buff_count)
{
  bool flag = false; 

  LAB_DMA_Data_Oscilloscope& dma_data = *(static_cast<LAB_DMA_Data_Oscilloscope*>
    (m_uncached_memory.virt ()));
  
  // 1. Pause PWM pacing if running
  if (m_LAB_Core->dma.is_running (LABC::DMA::CHAN::PWM_PACING))
  {
    flag = true;
    m_LAB_Core->dma.pause (LABC::DMA::CHAN::PWM_PACING);
  }

  // 2. Assign next control block depending on buffer
  if (buff_count == LABC::DMA::BUFFER_COUNT::SINGLE)
  { 
    m_LAB_Core->dma.next_cb (LABC::DMA::CHAN::OSC_RX, m_uncached_memory.bus (&dma_data.cbs[4]));
  }
  else if (buff_count == LABC::DMA::BUFFER_COUNT::DOUBLE)
  {
    m_LAB_Core->dma.next_cb (LABC::DMA::CHAN::OSC_RX, m_uncached_memory.bus (&dma_data.cbs[0]));
  }

  // 3. Abort the current control block 
  m_LAB_Core->dma.abort (LABC::DMA::CHAN::OSC_RX);

  // 4. Clean buffer status
  dma_data.status[0] = dma_data.status[1] = 0;

  // 5. Run DMA channel if it was running
  if (flag)
  {
    m_LAB_Core->dma.run (LABC::DMA::CHAN::PWM_PACING);
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