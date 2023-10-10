#include "LAB_Oscilloscope.h"

#include <cmath>
#include <cstring>

#include "LAB.h"
#include "../Utility/LAB_Utility_Functions.h"

// delete soon
#include <iostream>
#include <bitset>

LAB_Oscilloscope:: 
LAB_Oscilloscope (LAB& _LAB)
  : LAB_Module (_LAB)
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
  rpi ().dma.stop  (LABC::DMA::CHAN::OSC_RX);
  rpi ().dma.stop  (LABC::DMA::CHAN::OSC_TX);
  rpi ().dma.stop  (LABC::DMA::CHAN::PWM_PACING);

  rpi ().dma.reset (LABC::DMA::CHAN::OSC_RX);
  rpi ().dma.reset (LABC::DMA::CHAN::OSC_TX);
  rpi ().dma.reset (LABC::DMA::CHAN::PWM_PACING);
}

void LAB_Oscilloscope:: 
init_spi ()
{
  rpi ().spi.clear_fifo ();
  rpi ().spi.reg        (AP::SPI::DC, (8 << 24) | (4 << 16) | (8 << 8) | 1);
  rpi ().spi.frequency  (LABC::SPI::FREQUENCY, LABC::LAB::GPU_CORE_CLOCK_FREQ);

  rpi ().gpio.set (LABC::PIN::OSC::ADC_CS,   AP::GPIO::FUNC::ALT0, AP::GPIO::PULL::OFF);
  rpi ().gpio.set (LABC::PIN::OSC::ADC_MISO, AP::GPIO::FUNC::ALT0, AP::GPIO::PULL::DOWN);
  rpi ().gpio.set (LABC::PIN::OSC::ADC_MOSI, AP::GPIO::FUNC::ALT0, AP::GPIO::PULL::OFF);
  rpi ().gpio.set (LABC::PIN::OSC::ADC_SCLK, AP::GPIO::FUNC::ALT0, AP::GPIO::PULL::OFF);
}

void LAB_Oscilloscope:: 
init_pwm ()
{
  rpi ().cm.pwm.frequency  (LABC::CLKMAN::FREQUENCY);

  rpi ().pwm.reg           (AP::PWM::DMAC, (1 << 31) | (8 << 8) | (1 << 0));
  rpi ().pwm.use_fifo      (LABC::PWM::DMA_PACING_CHAN, true);
  rpi ().pwm.algo          (LABC::PWM::DMA_PACING_CHAN, AP::PWM::ALGO::MARKSPACE);
  rpi ().pwm.frequency     (LABC::PWM::DMA_PACING_CHAN, LABD::OSC::SAMPLING_RATE);
  rpi ().pwm.duty_cycle    (LABC::PWM::DMA_PACING_CHAN, 50.0);

  // PWM pin (BCM pin 12) is in use by digital circuit checker
  // rpi ().gpio.set (LABC::PIN::PWM, AP::GPIO::FUNC::ALT0, AP::GPIO::PULL::DOWN);
}

void LAB_Oscilloscope::
init_gpio_pins ()
{
  // scaling
  for (int chan = 0; chan < m_parent_data.channel_data.size (); chan++)
  {
    int mux_a = static_cast<int>(m_parent_data.channel_data[chan].scaling);

    for (int a = 0; a < 2; a++)
    {
      rpi ().gpio.set (LABC::PIN::OSC::MUX[chan][a],
        AP::GPIO::FUNC::OUTPUT, AP::GPIO::PULL::DOWN, (mux_a >> a) & 0x1);
    }
  }

  // coupling
  for (int chan = 0; chan < m_parent_data.channel_data.size (); chan++)
  {
    bool enable = static_cast<int>(m_parent_data.channel_data[chan].coupling);

    rpi ().gpio.set (LABC::PIN::OSC::RELAY[chan], 
      AP::GPIO::FUNC::OUTPUT, AP::GPIO::PULL::DOWN, enable);
  }
}

void LAB_Oscilloscope::
init_dma ()
{
  config_dma_cb ();

  AikaPi::Uncached&          un = m_uncached_memory;
  LAB_DMA_Data_Oscilloscope& dd = *(static_cast<LAB_DMA_Data_Oscilloscope*>(un.virt ()));

  rpi ().dma.start (LABC::DMA::CHAN::OSC_TX,     un.bus (&dd.cbs[6]));
  rpi ().dma.start (LABC::DMA::CHAN::OSC_RX,     un.bus (&dd.cbs[0]));
  rpi ().dma.start (LABC::DMA::CHAN::PWM_PACING, un.bus (&dd.cbs[7]));
}

void LAB_Oscilloscope:: 
init_state ()
{
  for (int a = 0; a < m_parent_data.channel_data.size (); a++)
  {
    scaling   (a, m_parent_data.channel_data[a].scaling);
    coupling  (a, m_parent_data.channel_data[a].coupling);
  }

  time_per_division (m_parent_data.time_per_division);
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
        rpi ().spi.bus   (AP::SPI::FIFO),
        m_uncached_memory.bus (&uncached_dma_data.rxd[0]),
        static_cast<uint32_t> (sizeof (uint32_t) * LABD::OSC::SAMPLES),
        0,
        m_uncached_memory.bus (&uncached_dma_data.cbs[1]),
        0
      },
      // 1
      {
        LABC::DMA::TI::OSC_RX | AP::DMA::TI_DATA::INTEN,
        rpi ().spi.bus   (AP::SPI::CS),
        m_uncached_memory.bus (&uncached_dma_data.status[0]),
        sizeof (uint32_t),
        0,
        m_uncached_memory.bus (&uncached_dma_data.cbs[2]),
        0
      },
      // 2
      {
        LABC::DMA::TI::OSC_RX,
        rpi ().spi.bus   (AP::SPI::FIFO),
        m_uncached_memory.bus (&uncached_dma_data.rxd[1]),
        static_cast<uint32_t> (sizeof (uint32_t) * LABD::OSC::SAMPLES),
        0,
        m_uncached_memory.bus (&uncached_dma_data.cbs[3]),
        0
      },
      // 3
      {
        LABC::DMA::TI::OSC_RX | AP::DMA::TI_DATA::INTEN,
        rpi ().spi.bus   (AP::SPI::CS),
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
        rpi ().spi.bus   (AP::SPI::FIFO),
        m_uncached_memory.bus (&uncached_dma_data.rxd[0]),
        static_cast<uint32_t> (sizeof (uint32_t) * LABD::OSC::SAMPLES),
        0,
        m_uncached_memory.bus (&uncached_dma_data.cbs[5]),
        0
      },
      // 5
      {
        LABC::DMA::TI::OSC_RX | AP::DMA::TI_DATA::INTEN,
        rpi ().spi.bus   (AP::SPI::CS),
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
        rpi ().spi.bus   (AP::SPI::FIFO),
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
        rpi ().pwm.bus   (AP::PWM::FIF1),
        sizeof (uint32_t),
        0,
        m_uncached_memory.bus (&uncached_dma_data.cbs[8]),
        0
      },
      // 8
      {
        LABC::DMA::TI::OSC_PWM_PACING,
        m_uncached_memory.bus (&uncached_dma_data.spi_cs_fifo_reset),
        rpi ().spi.bus   (AP::SPI::CS),
        sizeof (uint32_t),
        0,
        m_uncached_memory.bus (&uncached_dma_data.cbs[9]),
        0
      },
      // 9
      {
        LABC::DMA::TI::OSC_PWM_PACING,
        m_uncached_memory.bus (&uncached_dma_data.spi_samp_size),
        rpi ().spi.bus   (AP::SPI::DLEN),
        sizeof (uint32_t),
        0,
        m_uncached_memory.bus (&uncached_dma_data.cbs[10]),
        0
      },
      // 10
      {
        LABC::DMA::TI::OSC_PWM_PACING,
        m_uncached_memory.bus (&uncached_dma_data.spi_cs),
        rpi ().spi.bus   (AP::SPI::CS),
        sizeof (uint32_t),
        0,
        m_uncached_memory.bus (&uncached_dma_data.cbs[7]),
        0
      },     
    },   

    .spi_samp_size      = sizeof (uint32_t),    
    .spi_cs             = (1 <<  7) | // SPI CS TA
                          (1 << 11) | // SPI CS ADCS
                          (1 <<  8) | // SPI CS DMAEN
                          (1 <<  2) | // SPI CS CPHA = 1 (samp in on falling edge)
                          LABC::OSC::ADC_SPI_CHIP_ENABLE,
    .spi_cs_fifo_reset  = 0x00000030,
    .pwm_duty_cycle     = 0x0,
    .txd                = 0x0000'0000'ffff
  };

  std::memcpy (
    &uncached_dma_data, 
    &new_uncached_dma_data, 
    sizeof (new_uncached_dma_data)
  );
}


void LAB_Oscilloscope:: 
run ()
{
  if (lab ().m_Voltmeter.is_running ())
  {
    lab ().m_Voltmeter.stop ();
  }

  time_per_division (m_parent_data.time_per_division);
  master_run_stop   (true);

  m_parent_data.status = LABE::OSC::STATUS::AUTO;
}

void LAB_Oscilloscope:: 
stop ()
{
  master_run_stop (false);

  m_parent_data.status = LABE::OSC::STATUS::STOP;
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
    rpi ().pwm.start (LABC::PWM::DMA_PACING_CHAN);
    m_parent_data.is_backend_running = true;
  }
  else
  {
    rpi ().pwm.stop (LABC::PWM::DMA_PACING_CHAN);
    m_parent_data.is_backend_running = false;
  }
}

void LAB_Oscilloscope:: 
osc_frontend_run_stop (bool value)
{
  m_parent_data.is_frontend_running = value;
}

void LAB_Oscilloscope:: 
single ()
{
  if (!is_backend_running ())
  {
    osc_core_run_stop (true);
  }
  
  // reset DMA OSC RX chan interrupt flag
  rpi ().dma.clear_interrupt (LABC::DMA::CHAN::OSC_RX);

  m_parent_data.single = true;
}

void LAB_Oscilloscope:: 
status (LABE::OSC::STATUS _STATUS)
{
  m_parent_data.status = _STATUS;
}

bool LAB_Oscilloscope:: 
is_running ()
{
  return (m_parent_data.is_backend_running && m_parent_data.is_frontend_running);
}

void LAB_Oscilloscope::
channel_enable_disable (unsigned channel,
                        bool     enable)
{
  if (channel < m_parent_data.channel_data.size ())
  {
    m_parent_data.channel_data[channel].is_enabled = enable;
  }
}

void LAB_Oscilloscope:: 
voltage_per_division (unsigned  channel, 
                      double    value)
{
  if (LABF::is_within_range (value, LABC::OSC::MIN_VOLTAGE_PER_DIVISION, 
    LABC::OSC::MAX_VOLTAGE_PER_DIVISION))
  {
    m_parent_data.channel_data[channel].voltage_per_division = value;

    reset_dma_process ();
  }
}

void LAB_Oscilloscope:: 
vertical_offset (unsigned channel, 
                 double   value)
{
  m_parent_data.channel_data[channel].vertical_offset = value;
}

void LAB_Oscilloscope:: 
coupling (unsigned            channel,
          LABE::OSC::COUPLING coupling)
{
  if (channel < m_parent_data.channel_data.size ())
  {
    m_parent_data.channel_data[channel].coupling = coupling;

    rpi ().gpio.set (
      LABC::PIN::OSC::RELAY[channel],
      AP::GPIO::FUNC::OUTPUT, 
      AP::GPIO::PULL::OFF,
      (coupling == LABE::OSC::COUPLING::AC) ? 0 : 1
    );
  }
}

void LAB_Oscilloscope:: 
scaling (unsigned           channel, 
         LABE::OSC::SCALING scaling)
{
  int mux_a = static_cast<int>(scaling);

  for (int a = 0; a < 2; a++)
  {
    rpi ().gpio.write (LABC::PIN::OSC::MUX[channel][a], 
      (mux_a >> a) & 0x1);
  }

  m_parent_data.channel_data[channel].scaling = scaling;
}

LABE::OSC::SCALING LAB_Oscilloscope:: 
scaling (unsigned channel) const
{
  return (m_parent_data.channel_data[channel].scaling);
}

unsigned LAB_Oscilloscope:: 
channels () const
{
  return (m_parent_data.channel_data.size ());
}

LABE::OSC::COUPLING LAB_Oscilloscope:: 
coupling (unsigned channel)
{
  return (m_parent_data.channel_data[channel].coupling);
}

void LAB_Oscilloscope:: 
load_data_samples ()
{
  if (is_running ())
  {
    switch (m_parent_data.trigger_mode)
    {
      case (LABE::OSC::TRIG::MODE::NONE):
      {
        fill_raw_sample_buffer_from_dma_buffer  ();
        parse_raw_sample_buffer                 ();

        break;
      }

      case (LABE::OSC::TRIG::MODE::NORMAL):
      {
        if (m_parent_data.trigger_frame_ready)
        {
          parse_raw_sample_buffer ();
        }

        break;
      }

      case (LABE::OSC::TRIG::MODE::AUTO):
      {
        if (m_parent_data.auto_mode_frame_ready)
        {
          parse_raw_sample_buffer ();
        }

        break;
      }
    }  

    if (m_parent_data.single)
    {
      m_parent_data.single = false;

      stop ();
    }
  }
}

void LAB_Oscilloscope:: 
fill_raw_sample_buffer_from_dma_buffer ()
{
  LAB_DMA_Data_Oscilloscope& dma_data = *(
    static_cast<LAB_DMA_Data_Oscilloscope*>(m_uncached_memory.virt ())
  );

  switch (m_parent_data.mode)
  {
    case (LABE::OSC::MODE::REPEATED):
    {
      for (int buff = 0; buff < 2; buff++)
      {
        if (dma_data.status[buff])
        {
          std::memcpy (
            m_parent_data.raw_data_buffer.data (),
            const_cast<const void*>(static_cast<volatile void*>(dma_data.rxd[buff])),
            sizeof (uint32_t) * m_parent_data.samples
          );

          std::cout << std::bitset <32> (dma_data.rxd[buff][10]) << "\n";
        }

        // Check if the other buffer is also full.
        // If it is, then we have a buffer overflow (both buffers full).
        if (dma_data.status[buff ^ 1])
        {
          // std::cout << "OVERFLOW!" << "\n";
          m_parent_data.buffer_overflow_count++;

          dma_data.status[0] = dma_data.status[1] = 0;

          break;
        }
        else 
        {
          dma_data.status[buff] = 0;
        }
      }

      break;
    }

    case (LABE::OSC::MODE::SCREEN):
    {
      std::memcpy (
        m_parent_data.raw_data_buffer.data (),
        const_cast<const void*>(static_cast<volatile void*>(dma_data.rxd[0])),
        sizeof (uint32_t) * m_parent_data.samples
      );

      break;
    }
  }
}

void LAB_Oscilloscope:: 
parse_raw_sample_buffer ()
{
  LAB_Parent_Data_Oscilloscope& pdata = m_parent_data;

  // pdata.time_per_division_raw_buffer  = pdata.time_per_division;
  // pdata.samples_raw_buffer            = pdata.samples; 

  for (unsigned samp = 0; samp < pdata.samples; samp++)
  {
    for (unsigned chan = 0; chan < pdata.channel_data.size (); chan++)
    {
      pdata.channel_data[chan].samples[samp] = 
        conv_raw_chan_adc_bits_to_actual_value (pdata.raw_data_buffer[samp], chan);

      // // For debug
      // if (samp == 10 && chan == 0)
      // {
      //   //uint32_t raw_chan_bits = extract_raw_chan_adc_bits_from_raw_buff_samp (m_parent_data.raw_data_buffer[samp], chan);

      //   // std::cout << std::bitset <32> (raw_chan_bits);
      //   std::cout << std::bitset <32> (pdata.raw_data_buffer[samp]);
      //   //std::cout << " - " << (arrange_raw_chan_adc_bits (raw_chan_bits));
      //   //std::cout << " - " << conv_raw_chan_adc_bits_to_actual_value (m_parent_data.raw_data_buffer[samp], chan);
      //   std::cout << "\n";
      // }
    }
  }

  if (m_parent_data.trigger_frame_ready)
  {
    m_parent_data.trigger_frame_ready = false;
  }
}

constexpr double LAB_Oscilloscope:: 
conv_raw_chan_adc_bits_to_actual_value (uint32_t  raw_buff_samp,
                                        unsigned  channel)
{
  uint32_t  raw_chan_adc_bits     = extract_raw_chan_adc_bits_from_raw_buff_samp (raw_buff_samp, channel);
  uint32_t  arranged_bits         = arrange_raw_chan_adc_bits (raw_chan_adc_bits, channel);
  bool      arranged_bits_sign    = (arranged_bits >> (LABC::OSC::ADC_RESOLUTION_BITS - 1)) & 0x1;
  uint32_t  arranged_bits_abs_val = arranged_bits & ((LABC::OSC::ADC_RESOLUTION_INT - 1) >> 1);
  double    actual_value          = conv_arranged_raw_chan_adc_bits_to_actual_value (arranged_bits_abs_val, arranged_bits_sign); 

  return (actual_value);
}

constexpr uint32_t LAB_Oscilloscope::
extract_raw_chan_adc_bits_from_raw_buff_samp (uint32_t raw_buff_samp,
                                              unsigned channel)
{
  return ((raw_buff_samp >> (LABC::OSC::RAW_DATA_SHIFT_BIT_COUNT * channel)) &
      LABC::OSC::RAW_DATA_POST_SHIFT_MASK);
}

constexpr uint32_t LAB_Oscilloscope:: 
arrange_raw_chan_adc_bits (uint32_t raw_chan_bits, unsigned channel)
{
  // if you update this function, please also update 
  // reverse_arrange_raw_chan_adc_bits (), which is just 
  // the reverse of this function

  // uint32_t ret = ((raw_chan_bits & 0xF000) >> 12) | ((raw_chan_bits & 0x00FF) << 4);

  // if (channel == 0)
  // {
  //   std::cout << std::bitset <12> (ret) << "\n";
  // }

  return (((raw_chan_bits & 0xF000) >> 12) | ((raw_chan_bits & 0x00FF) << 4)); 
}

constexpr uint32_t LAB_Oscilloscope:: 
reverse_arrange_raw_chan_adc_bits (uint32_t arranged_bits)
{
  return (((arranged_bits & 0x000F) << 12) | ((arranged_bits & 0x0FF0) >> 4));
}

constexpr double LAB_Oscilloscope::
conv_arranged_raw_chan_adc_bits_to_actual_value (uint32_t arranged_bits_abs_val, 
                                                 bool     arranged_bits_sign)
{
  if (arranged_bits_sign)
  {
    return (arranged_bits_abs_val * LABC::OSC::CONVERSION_CONSTANT);
  }
  else 
  {
    return ((arranged_bits_abs_val * LABC::OSC::CONVERSION_CONSTANT) - 
      LABC::OSC::CONVERSION_REFERENCE_VOLTAGE);
  }
}

constexpr uint32_t LAB_Oscilloscope:: 
conv_raw_buff_get_arranged_bits (uint32_t sample,
                                 unsigned channel)
{
  uint32_t raw_chan_bits = extract_raw_chan_adc_bits_from_raw_buff_samp (sample, channel);

  return (arrange_raw_chan_adc_bits (raw_chan_bits, channel));
}

void LAB_Oscilloscope:: 
reset_dma_process ()
{
  LAB_DMA_Data_Oscilloscope& dma_data = *(static_cast<LAB_DMA_Data_Oscilloscope*>
    (m_uncached_memory.virt ()));

  bool is_dma_running = rpi ().dma.is_running (LABC::DMA::CHAN::PWM_PACING);

  // 1. check if DMA is running. if it is, pause it
  if (is_dma_running)
  {
    rpi ().dma.pause (LABC::DMA::CHAN::PWM_PACING);
  }

  // 2. reset the DMA engine to the first control block, depending on the buffer
  switch (mode ())
  {
    case (LABE::OSC::MODE::REPEATED): // dual buffer
    {
      rpi ().dma.next_cb (LABC::DMA::CHAN::OSC_RX,
        m_uncached_memory.bus (&dma_data.cbs[0]));

      break;
    }

    case (LABE::OSC::MODE::SCREEN): // single buffer
    {
      rpi ().dma.next_cb (LABC::DMA::CHAN::OSC_RX,
        m_uncached_memory.bus (&dma_data.cbs[4]));
      
      break;
    }
  }

  // 3. abort the current control block
  rpi ().dma.abort (LABC::DMA::CHAN::OSC_RX);

  // 4. reset the DMA status flags
  dma_data.status[0] = dma_data.status[1] = 0;

  // 5. reset the 2D DMA OSC RX buffer array
  reset_uncached_rx_buffer ();

  // 6. run DMA again if it was running
  if (is_dma_running)
  {
    rpi ().dma.run (LABC::DMA::CHAN::PWM_PACING);
  }
}

void LAB_Oscilloscope:: 
reset_uncached_rx_buffer ()
{
  LAB_DMA_Data_Oscilloscope& dma_data = *(static_cast<LAB_DMA_Data_Oscilloscope*>
    (m_uncached_memory.virt ()));

  std::memset (
    const_cast<void*>(static_cast<volatile void*>(dma_data.rxd)),
    reverse_arrange_raw_chan_adc_bits (LABC::OSC::ADC_RESOLUTION_INT / 2),
    sizeof (dma_data.rxd)
  );
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
calc_time_per_division (unsigned  samples, 
                        double    sampling_rate)
{
  return (samples / (sampling_rate * LABC::OSC::DISPLAY_NUMBER_OF_COLUMNS));
}

double LAB_Oscilloscope:: 
calc_samples_displayed (double sampling_rate, 
                        double time_per_division)
{
  if (time_per_division < LABC::OSC::MIN_TIME_PER_DIVISION_NO_ZOOM)
  {
    return (sampling_rate * time_per_division * LABC::OSC::DISPLAY_NUMBER_OF_COLUMNS);
  }
  else 
  {
    return (LABC::OSC::MAX_SAMPLES);
  }
}

double LAB_Oscilloscope:: 
calc_sampling_rate (unsigned  samples,
                    double    time_per_division)
{
  double new_sampling_rate = samples / (time_per_division * 
    LABC::OSC::DISPLAY_NUMBER_OF_COLUMNS);
  
  return (new_sampling_rate > LABC::OSC::MAX_SAMPLING_RATE ? 
    LABC::OSC::MAX_SAMPLING_RATE : new_sampling_rate);
}

void LAB_Oscilloscope:: 
set_time_per_division (unsigned samples,
                       double   sampling_rate)
{
  set_time_per_division (calc_time_per_division (samples, sampling_rate));
}

void LAB_Oscilloscope:: 
set_time_per_division (double value)
{
  m_parent_data.time_per_division = value;

  set_mode (calc_mode (value));

  reset_dma_process ();
}

void LAB_Oscilloscope:: 
set_samples (unsigned value)
{
  m_parent_data.samples = value;

  LAB_DMA_Data_Oscilloscope& uncached_dma_data = 
    *(static_cast<LAB_DMA_Data_Oscilloscope*>(m_uncached_memory.virt ()));
  
  uncached_dma_data.cbs[0].txfr_len = static_cast<uint32_t>(sizeof (uint32_t) * value);
  uncached_dma_data.cbs[2].txfr_len = static_cast<uint32_t>(sizeof (uint32_t) * value);
  uncached_dma_data.cbs[4].txfr_len = static_cast<uint32_t>(sizeof (uint32_t) * value);
}

void LAB_Oscilloscope:: 
set_samples_displayed (unsigned value)
{
  m_parent_data.samples_displayed = value;

  m_parent_data.sample_start_index = ((LABC::OSC::NUMBER_OF_SAMPLES - 1) / 2.0) - 
    ((value - 1) / 2.0);
}

void LAB_Oscilloscope:: 
set_sampling_rate (double value)
{
  // 1. change the source frequency of the PWM peripheral
  rpi ().pwm.frequency (LABC::PWM::DMA_PACING_CHAN, value);

  // 2. set the duty cycle in the DMA data to 50%
  LAB_DMA_Data_Oscilloscope& dma_data = *(static_cast<LAB_DMA_Data_Oscilloscope*>
    (m_uncached_memory.virt ()));

  dma_data.pwm_duty_cycle = (rpi ().pwm.range (LABC::PWM::DMA_PACING_CHAN)) / 2.0;

  // 3. store the sampling rate
  m_parent_data.sampling_rate = value;
}

LABE::OSC::MODE LAB_Oscilloscope:: 
calc_mode (double time_per_division)
{
  return (time_per_division < LABC::OSC::MIN_TIME_PER_DIVISION_SCREEN ?
    LABE::OSC::MODE::REPEATED : m_parent_data.last_mode_before_repeated);
}

// --- Horizontal ---

void LAB_Oscilloscope:: 
horizontal_offset (double value)
{
  m_parent_data.horizontal_offset = value;
}

double LAB_Oscilloscope:: 
horizontal_offset () const
{
  return (m_parent_data.horizontal_offset);
}

void LAB_Oscilloscope:: 
time_per_division (double value)
{
  if (LABF::is_within_range (value, LABC::OSC::MIN_TIME_PER_DIVISION,
    LABC::OSC::MAX_TIME_PER_DIVISION))
  {
    double new_sampling_rate      = 0.0;
    double new_samples_displayed  = 0.0;

    // 1. 
    new_sampling_rate     = calc_sampling_rate (m_parent_data.samples, value);

    // 2.
    new_samples_displayed = calc_samples_displayed (new_sampling_rate, value);

    // 3. 
    set_sampling_rate     (new_sampling_rate);
    set_samples_displayed (new_samples_displayed);
    set_time_per_division (value);
  }
}

void LAB_Oscilloscope:: 
samples (unsigned value)
{
  if (!(LABF::is_within_range (value, LABC::OSC::MIN_SAMPLES, 
    LABC::OSC::MAX_SAMPLES_RECORDING)))
  {
    return;
  }

  if ((m_parent_data.mode != LABE::OSC::MODE::RECORD) && 
    (value > LABC::OSC::MAX_SAMPLES))
  {
    return;
  }

  // 

  set_samples           (value);
  set_time_per_division (value, m_parent_data.sampling_rate);
}

void LAB_Oscilloscope:: 
sampling_rate (double value)
{
  if (LABF::is_within_range (value, LABC::OSC::MIN_SAMPLING_RATE,
    LABC::OSC::MAX_SAMPLING_RATE))
  {
    set_time_per_division (m_parent_data.samples, value);
    set_sampling_rate     (value);
  }
}

// --- Trigger ---

void LAB_Oscilloscope:: 
parse_trigger_mode ()
{
  switch (m_parent_data.trigger_mode)
  {
    case (LABE::OSC::TRIG::MODE::NONE):
    {
      if (m_thread_find_trigger.joinable ())
      {
        m_parent_data.find_trigger = false;
        m_thread_find_trigger.join ();
      }

      break;
    }

    case (LABE::OSC::TRIG::MODE::AUTO):
    {
      if (!m_thread_find_trigger.joinable ())
      {
        m_parent_data.find_trigger = true;
        m_thread_find_trigger = std::thread (&LAB_Oscilloscope::find_trigger_point_loop, this);
      }

      break;
    }

    case (LABE::OSC::TRIG::MODE::NORMAL):
    {
      if (!m_thread_find_trigger.joinable ())
      {
        m_parent_data.find_trigger = true;
        m_thread_find_trigger = std::thread (&LAB_Oscilloscope::find_trigger_point_loop, this);
      }

      break;
    }
  }
}

void LAB_Oscilloscope:: 
find_trigger_point_loop ()
{
  LAB_DMA_Data_Oscilloscope& dma_data = *(static_cast<LAB_DMA_Data_Oscilloscope*>
    (m_uncached_memory.virt ()));

  // ----------
  
  // Cached variables

  // Regs
  volatile uint32_t* int_status     = rpi ().dma.MemoryMap::reg (AP::DMA::INT_STATUS);
  volatile uint32_t* curr_conblk_ad = rpi ().dma.reg (LABC::DMA::CHAN::OSC_RX, AP::DMA::CONBLK_AD);

  // Vars
  uint32_t  buff0_cbs_addr[2] = {m_uncached_memory.bus (&dma_data.cbs[0]),
                                 m_uncached_memory.bus (&dma_data.cbs[1])};
  uint32_t  buff1_cbs_addr[2] = {m_uncached_memory.bus (&dma_data.cbs[2]),
                                 m_uncached_memory.bus (&dma_data.cbs[3])};
  
  // ----------

  // sync_find_trigger_point_loop ();

  status (LABE::OSC::STATUS::CONFIG);

  switch (m_parent_data.mode)
  {
    case (LABE::OSC::MODE::REPEATED):
    {
      // 1. Pause osc RX DMA channel
      rpi ().dma.pause (LABC::DMA::CHAN::OSC_RX);

      // 2. Set the contents of its next control block register
      //    to point to the first control block
      rpi ().dma.reg (LABC::DMA::CHAN::OSC_RX, AP::DMA::NEXTCONBK,
        m_uncached_memory.bus (&dma_data.cbs[0]));
      
      // 3. Abort current control block
      rpi ().dma.reg_wbits (LABC::DMA::CHAN::OSC_RX, AP::DMA::CS, 1, 30);

      std::this_thread::sleep_for (std::chrono::microseconds (5));

      // 4. Unpause oscilloscope RX DMA channel
      rpi ().dma.run (LABC::DMA::CHAN::OSC_RX);  

      // 5. Wait until first buffer is filled
      while (*curr_conblk_ad == buff0_cbs_addr[0] || *curr_conblk_ad == buff0_cbs_addr[1]);

      // 6. Wait until second buffer is filled
      while (*curr_conblk_ad == buff1_cbs_addr[0] || *curr_conblk_ad == buff1_cbs_addr[1]); 

      // 7. Reset osc RX DMA interrupt
      rpi ().dma.MemoryMap::reg_wbits (AP::DMA::INT_STATUS, 0, LABC::DMA::CHAN::OSC_RX);

      break;
    }

    case (LABE::OSC::MODE::SCREEN):
    {
      break;
    }
  }
  
  // ----------

  status (LABE::OSC::STATUS::ARMED);
  
  while (m_parent_data.find_trigger)
  {
    if (!m_parent_data.trigger_found)
    {
      // 1. Check the oscilloscope RX DMA channel interrupt if it is asserted.
      //    If it is, this means that a buffer was just fully written to.
      if (rpi ().dma.interrupt (LABC::DMA::CHAN::OSC_RX))
      {
        // 2. Store the current control block running in the oscilloscope RX DMA 
        //    engine. This is to know what buffer (0 or 1) was just filled.
        uint32_t curr_conblk_ad = *(rpi ().dma.reg (LABC::DMA::CHAN::OSC_RX, AP::DMA::CONBLK_AD));

        // 3. Reset the interrupt
        rpi ().dma.clear_interrupt (LABC::DMA::CHAN::OSC_RX);

        // 4. Store a copy of the uncached receive buffers
        std::memcpy (
          m_parent_data.trig_buffs.pre_trigger.data (),
          const_cast<const void*>(static_cast<const volatile void*>(dma_data.rxd)),
          sizeof (uint32_t) * LABC::OSC::NUMBER_OF_CHANNELS * LABC::OSC::NUMBER_OF_SAMPLES
        );

        // 5. Identify the buffer that was filled 
        if (curr_conblk_ad == buff0_cbs_addr[0] || curr_conblk_ad == buff0_cbs_addr[1])
        {
          m_parent_data.trigger_buffer_index = 1;
        }
        else if (curr_conblk_ad == buff1_cbs_addr[0] || curr_conblk_ad == buff1_cbs_addr[1])
        {
          m_parent_data.trigger_buffer_index = 0;
        }

        // 6. Search for trigger
        if (find_trigger_point ())
        {
          create_trigger_frame ();
        }
      }
    }
  }
}

inline bool LAB_Oscilloscope:: 
find_trigger_point ()
{
  switch (m_parent_data.trig_type)
  {
    case (LABE::OSC::TRIG::TYPE::LEVEL):
    {
      for (int a = 0; a < m_parent_data.trig_buffs.pre_trigger[m_parent_data.
        trigger_buffer_index].size (); a += m_parent_data.find_trig_sample_skip)
      {
        uint32_t samp  = conv_raw_buff_get_arranged_bits (
          m_parent_data.trig_buffs.pre_trigger[m_parent_data.trigger_buffer_index][a], 
          m_parent_data.trigger_source
        );

        if (samp >= m_parent_data.trigger_level_raw_bits)
        {
          m_parent_data.trig_index = a;
          m_parent_data.trigger_found = true;

          return (true);
        }
      }

      break;
    }

    case (LABE::OSC::TRIG::TYPE::EDGE):
    {
      uint32_t prev = conv_raw_buff_get_arranged_bits (
        m_parent_data.trig_buffs.pre_trigger[m_parent_data.trigger_buffer_index][0],
        m_parent_data.trigger_source
      );

      switch (m_parent_data.trig_condition)
      {
        case (LABE::OSC::TRIG::CND::RISING):
        {
          for (int a = 1; a < m_parent_data.trig_buffs.pre_trigger[m_parent_data.
            trigger_buffer_index].size (); a += m_parent_data.find_trig_sample_skip)
          {
            uint32_t samp = conv_raw_buff_get_arranged_bits (
              m_parent_data.trig_buffs.pre_trigger[m_parent_data.trigger_buffer_index][a],
              m_parent_data.trigger_source
            );

            if ((samp >= m_parent_data.trigger_level_raw_bits) && (prev < m_parent_data.trigger_level_raw_bits))
            {
              m_parent_data.trig_index = a;
              m_parent_data.trigger_found = true;

              return (true);
            }
            else 
            {
              prev = samp;
            }
          }

          break;
        }

        case (LABE::OSC::TRIG::CND::FALLING):
        {
          for (int a = 1; a < m_parent_data.trig_buffs.pre_trigger[m_parent_data.
            trigger_buffer_index].size (); a += m_parent_data.find_trig_sample_skip)
          {
            uint32_t samp = conv_raw_buff_get_arranged_bits (
              m_parent_data.trig_buffs.pre_trigger[m_parent_data.trigger_buffer_index][a],
              m_parent_data.trigger_source
            );

            if ((samp <= m_parent_data.trigger_level_raw_bits) && (prev > m_parent_data.trigger_level_raw_bits))
            {
              m_parent_data.trig_index = a;
              m_parent_data.trigger_found = true;

              return (true);
            }
            else 
            {
              prev = samp;
            }
          }

          break;
        }

        case (LABE::OSC::TRIG::CND::EITHER):
        {
          for (int a = 1; a < m_parent_data.trig_buffs.pre_trigger[m_parent_data.
            trigger_buffer_index].size (); a += 4)
          {
            uint32_t samp = conv_raw_buff_get_arranged_bits (
              m_parent_data.trig_buffs.pre_trigger[m_parent_data.trigger_buffer_index][a],
              m_parent_data.trigger_source
            );

            if (((samp <= m_parent_data.trigger_level_raw_bits) && (prev > m_parent_data.trigger_level_raw_bits)) ||
                ((samp >= m_parent_data.trigger_level_raw_bits) && (prev < m_parent_data.trigger_level_raw_bits)))
            {
              m_parent_data.trig_index = a;
              m_parent_data.trigger_found = true;

              return (true);
            }
            else 
            {
              prev = samp;
            }
          }

          break;
        }
      }

      break;
    }
  }

  return (false);
}

inline void LAB_Oscilloscope:: 
create_trigger_frame ()
{
  static constexpr unsigned samp_half         = LABC::OSC::NUMBER_OF_SAMPLES / 2;
  static constexpr unsigned samp_half_index   = samp_half - 1;
  static LAB_DMA_Data_Oscilloscope& dma_data  = *(static_cast<LAB_DMA_Data_Oscilloscope*>
                                                  (m_uncached_memory.virt ()));

  if (m_parent_data.trig_index >= samp_half_index)
  {
    unsigned  copy_count_0  = samp_half,
              copy_count_1  = LABC::OSC::NUMBER_OF_SAMPLES - 1 - m_parent_data.trig_index,
              copy_count_2  = samp_half - copy_count_1;
    
    std::memcpy (
      m_parent_data.raw_data_buffer.data (),
      m_parent_data.trig_buffs.pre_trigger[m_parent_data.trigger_buffer_index].data () 
        + (m_parent_data.trig_index - samp_half_index),
      sizeof (uint32_t) * copy_count_0
    );

    std::memcpy (
      m_parent_data.raw_data_buffer.data () + (copy_count_0),
      m_parent_data.trig_buffs.pre_trigger[m_parent_data.trigger_buffer_index].data () 
        + (m_parent_data.trig_index + 1),
      sizeof (uint32_t) * copy_count_1
    );

    while (!((*(rpi ().dma.MemoryMap::reg (AP::DMA::INT_STATUS)) >> LABC::DMA::CHAN::OSC_RX) & 0x1));

    std::memcpy (
      m_parent_data.raw_data_buffer.data () + (copy_count_0 + copy_count_1),
      const_cast<const void*>(static_cast<const volatile void*>(dma_data.rxd[m_parent_data.trigger_buffer_index ^ 1])),
      sizeof (uint32_t) * copy_count_2
    );

    rpi ().dma.MemoryMap::reg_wbits (AP::DMA::INT_STATUS, 0, LABC::DMA::CHAN::OSC_RX);
  }
  else if (m_parent_data.trig_index < samp_half_index)
  {
    unsigned  copy_count_2 = samp_half,
              copy_count_1 = m_parent_data.trig_index + 1,
              copy_count_0 = samp_half - copy_count_1;
                  
    std::memcpy (
      m_parent_data.raw_data_buffer.data (),
      m_parent_data.trig_buffs.pre_trigger[m_parent_data.trigger_buffer_index ^ 1].data () 
        + (LABC::OSC::NUMBER_OF_SAMPLES - copy_count_0),
      sizeof (uint32_t) * copy_count_0
    );

    std::memcpy (
      m_parent_data.raw_data_buffer.data () + (copy_count_0),
      m_parent_data.trig_buffs.pre_trigger[m_parent_data.trigger_buffer_index].data (),
      sizeof (uint32_t) * copy_count_1
    );

    std::memcpy (
      m_parent_data.raw_data_buffer.data () + (copy_count_0 + copy_count_1),
      m_parent_data.trig_buffs.pre_trigger[m_parent_data.trigger_buffer_index].data () + 
        (m_parent_data.trig_index + 1),
      sizeof (uint32_t) * copy_count_2
    );
  }

  m_parent_data.trigger_frame_ready = true;
}

void LAB_Oscilloscope:: 
find_trigger_timeout_timer ()
{
  // uint32_t start    = rpi ().st.low ();
  // uint32_t timeout  = start + (LABC::OSC::) 
  // while (m_parent_data.trigger_mode == LABE::OSC::TRIG::MODE::AUTO)
  // {
  //   {
  //     uint32_t start    = rpi ().st.low ();
  //     uint32_t timeout  = 0;

  //     while (!m_parent_data.trigger_found)
  //     {
  //       if (rpi ().st.low () >= timeout)
  //       {
  //         m_parent_data.find_trig_timeout = true;
  //       }

  //       while (!m_parent_data.trigger_found);

  //       m_parent_data.trigger_found = false;
  //       m_parent_data.find_trig_timeout = false;
  //     }
  //   }
  // }
}

uint32_t LAB_Oscilloscope:: 
calc_trigger_level_raw_bits (double trigger_level)
{
  if (LABF::is_within_range (
    trigger_level, 
    LABC::OSC::MIN_OSC_HARDWARE_TRIGGER_LEVEL,
    LABC::OSC::MAX_OSC_HARDWARE_TRIGGER_LEVEL
  ))
  {
    double adc_trigger_level = LABF::normalize (
      trigger_level,
      LABC::OSC::MIN_OSC_HARDWARE_TRIGGER_LEVEL,
      LABC::OSC::MAX_OSC_HARDWARE_TRIGGER_LEVEL,
      0,
      LABC::OSC::ADC_RESOLUTION_INT - 1
    );

    return (reverse_arrange_raw_chan_adc_bits (std::round (adc_trigger_level)));
  }
  else 
  {
    return (0xFFFFFFFF);
  }
}

void LAB_Oscilloscope:: 
config_dma_cb_record ()
{
  // LAB_DMA_Data_Oscilloscope dma_data = 
  // {
  //   .cbs = 
  //   {
  //     LABC::DMA::TI::OSC_RX,
  //     rpi ().spi.bus (AP::SPI::FIFO),

  //     static_cast<uint32_t> (sizeof (uint32_t) * m_parent_data.samples),
  //     0,
  //     0,
  //     0
  //   }
  // }
}

void LAB_Oscilloscope:: 
trigger_mode (LABE::OSC::TRIG::MODE value)
{
  m_parent_data.trigger_mode = value;

  parse_trigger_mode ();
}

LABE::OSC::TRIG::MODE LAB_Oscilloscope:: 
trigger_mode () const
{
  return (m_parent_data.trigger_mode);
}

void LAB_Oscilloscope:: 
trigger_source (unsigned chan)
{
  if (chan >= 0 && chan <= LABC::OSC::NUMBER_OF_CHANNELS)
  {
    m_parent_data.trigger_source = chan;
  }
}

double LAB_Oscilloscope:: 
trigger_source () const
{
  return (m_parent_data.trigger_source);
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
  if (LABF::is_within_range (value, LABC::OSC::MIN_TRIGGER_LEVEL,
    LABC::OSC::MAX_TRIGGER_LEVEL))
  {
    m_parent_data.trigger_level           = value;
    m_parent_data.trigger_level_raw_bits  = calc_trigger_level_raw_bits (value);
  }
}

double LAB_Oscilloscope::    
trigger_level () const
{
  return (m_parent_data.trigger_level);
}

bool LAB_Oscilloscope::            
trigger_found () const
{
  return (m_parent_data.trigger_found);
}

void LAB_Oscilloscope::                 
trigger_serviced ()
{
  m_parent_data.trigger_found = false;
}

void LAB_Oscilloscope::
record_start ()
{
  // record_init ();

  // // x. Stop PWM
  // stop ();

  // // x. Pause OSC RX DMA engine
  // rpi ().dma.pause (LABC::DMA::CHAN::OSC_RX);

  // // x. 
  // rpi ().dma.next_cb (LABC::DMA::CHAN::OSC_RX, c)



  // // x. Start PWM
  // run ();

  // // x. Copy raw samples from DMA buffer
  // std::memcpy (
  //   m_parent_data.recording_raw_sample_buffer.data (),
  //   const_cast<const void*>(static_cast<volatile void*>(&(dma_data.rxd))),
  //   sizeof (uint32_t) * m_parent_data.samples
  // );

  // // x.
}

void LAB_Oscilloscope:: 
record_init ()
{
  // x. 
  m_parent_data.recording_raw_sample_buffer.reserve (LABC::OSC::MAX_SAMPLES_RECORDING);
}

// --- Mode ---

void LAB_Oscilloscope::   
mode (LABE::OSC::MODE mode)
{
  switch (mode)
  {
    case (LABE::OSC::MODE::REPEATED):
    {
      break;
    }

    case (LABE::OSC::MODE::SCREEN):
    {
      if (time_per_division () < LABC::OSC::MIN_TIME_PER_DIVISION_SCREEN)
      {
        time_per_division (LABC::OSC::MIN_TIME_PER_DIVISION_SCREEN);
      }

      break;
    }

    case (LABE::OSC::MODE::RECORD):
    {
      stop ();
      break;
    }
  }

  set_mode (mode);
}

LABE::OSC::MODE LAB_Oscilloscope:: 
mode ()
{
  return (m_parent_data.mode);
}

void LAB_Oscilloscope:: 
update_dma_data (int display_mode)
{
  // rpi ().dma_pause (LABC::DMA::CHANPWM_PACING);
  
  // volatile uint32_t *reg = Utility::reg (rpi ().m_regs_dma,
  //   DMA_REG (LABC::DMA::CHANPWM_PACING, DMA_NEXTCONBK));

  // *reg = Utility::bus ()
  

  // Utility::write_reg_virt (*reg, )

  // *REG32(m_regs_dma, DMA_REG(chan, DMA_CONBLK_AD)) = MEM_BUS_ADDR(mp, cbp);

  // rpi ().dma_play (LABC::DMA::CHANPWM_PACING);
}

void LAB_Oscilloscope:: 
set_mode (LABE::OSC::MODE mode)
{
  if (m_parent_data.mode != mode)
  {
    if (time_per_division () >= LABC::OSC::MIN_TIME_PER_DIVISION_SCREEN)
    {
      m_parent_data.last_mode_before_repeated = mode;
    }

    switch (mode)
    {
      case (LABE::OSC::MODE::REPEATED):
      {
        dma_buffer_count  (LABE::OSC::BUFFER_COUNT::DOUBLE);
        break;
      }

      case (LABE::OSC::MODE::SCREEN):
      {
        dma_buffer_count  (LABE::OSC::BUFFER_COUNT::SINGLE);
        break;
      }
    }

    m_parent_data.mode = mode; 
  }
}

void LAB_Oscilloscope:: 
dma_buffer_count (LABE::OSC::BUFFER_COUNT buffer_count)
{
  bool is_running = false; 

  LAB_DMA_Data_Oscilloscope& dma_data = *(static_cast<LAB_DMA_Data_Oscilloscope*>
    (m_uncached_memory.virt ()));
  
  // 1. Pause PWM pacing if running
  if (rpi ().dma.is_running (LABC::DMA::CHAN::PWM_PACING))
  {
    is_running = true;
    rpi ().dma.pause (LABC::DMA::CHAN::PWM_PACING);
  }

  // 2. Assign next control block depending on buffer
  if (buffer_count == LABE::OSC::BUFFER_COUNT::SINGLE)
  { 
    rpi ().dma.next_cb (LABC::DMA::CHAN::OSC_RX, m_uncached_memory.bus (&dma_data.cbs[4]));
  }
  else if (buffer_count == LABE::OSC::BUFFER_COUNT::DOUBLE)
  {
    rpi ().dma.next_cb (LABC::DMA::CHAN::OSC_RX, m_uncached_memory.bus (&dma_data.cbs[0]));
  }

  // 3. Abort the current control block 
  rpi ().dma.abort (LABC::DMA::CHAN::OSC_RX);

  // 4. Clean buffer status
  dma_data.status[0] = dma_data.status[1] = 0;

  // 5. Run DMA channel if it was running
  if (is_running)
  {
    rpi ().dma.run (LABC::DMA::CHAN::PWM_PACING);
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

bool LAB_Oscilloscope:: 
is_channel_enabled (unsigned channel)
{
  return (m_parent_data.channel_data[channel].is_enabled);
}

// Getters
bool LAB_Oscilloscope:: 
is_frontend_running ()
{
  return (m_parent_data.is_frontend_running);
}

bool LAB_Oscilloscope:: 
is_backend_running ()
{
  return (m_parent_data.is_backend_running);
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
time_per_division () const
{
  return (m_parent_data.time_per_division);
}

unsigned LAB_Oscilloscope:: 
samples () const
{
  return (m_parent_data.samples);
}

double LAB_Oscilloscope:: 
sampling_rate () const
{
  return (m_parent_data.sampling_rate);
}

const LAB_Parent_Data_Oscilloscope& LAB_Oscilloscope:: 
parent_data ()
{
  return (m_parent_data);
}

const std::array<double, LABC::OSC::NUMBER_OF_SAMPLES> LAB_Oscilloscope:: 
channel_samples (unsigned channel)
{
  return (m_parent_data.channel_data[channel].samples);
}

// EOF