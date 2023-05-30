#include "LAB_Logic_Analyzer.h"

#include "LAB.h"

#include <cstring>
#include <bitset>

// remove soon
#include <iostream>
// #include <bitset>

LAB_Logic_Analyzer::
LAB_Logic_Analyzer (LAB_Core* _LAB_Core, LAB* _LAB)
  : m_LAB_Core (_LAB_Core), m_LAB (_LAB)
{
  // init_pwm (); // or maybe init_pcm () ???
  init_gpio_pins  ();
  init_dma        ();
}

LAB_Logic_Analyzer::
~LAB_Logic_Analyzer ()
{

}

void LAB_Logic_Analyzer:: 
init_gpio_pins ()
{
  for (int chan = 0; chan < m_parent_data.channel_data.size (); chan++)
  {
    m_LAB_Core->gpio.set (
      LABC::PIN::LOGIC_ANALYZER[chan],
      AP::GPIO::FUNC::INPUT,
      AP::GPIO::PULL::DOWN
    );
  }
}

void LAB_Logic_Analyzer:: 
init_dma ()
{
  config_dma_cb ();

  AikaPi::Uncached&            un = m_uncached_memory;
  LAB_DMA_Data_Logic_Analyzer& dd = *(static_cast<LAB_DMA_Data_Logic_Analyzer*>(un.virt ()));

  m_LAB_Core->dma.start (LABC::DMA::CHAN::LOGAN_GPIO_STORE, un.bus (&dd.cbs[0]));
}

void LAB_Logic_Analyzer::
config_dma_cb ()
{
  m_uncached_memory.map_uncached_mem (LAB_LOGIC_ANALYZER::VC_MEM_SIZE);

  LAB_DMA_Data_Logic_Analyzer& uncached_dma_data =
    *(static_cast<LAB_DMA_Data_Logic_Analyzer*>(m_uncached_memory.virt ()));

  LAB_DMA_Data_Logic_Analyzer new_uncached_dma_data  = 
  {
    .cbs = 
    { 
      // Double buffer
      // 0
      {
        LABC::DMA::TI::LOGAN_STORE,
        m_LAB_Core->gpio.bus  (AP::GPIO::GPLEV0),
        m_uncached_memory.bus (&uncached_dma_data.rxd[0]),
        static_cast<uint32_t> (sizeof (uint32_t) * LAB_LOGIC_ANALYZER::NUMBER_OF_SAMPLES),
        0,
        m_uncached_memory.bus (&uncached_dma_data.cbs[1]),
        0
      },
      // 1
      {
        LABC::DMA::TI::LOGAN_STORE | AP::DMA::TI_DATA::INTEN,
        m_uncached_memory.bus (&uncached_dma_data.buffer_ok_flag),
        m_uncached_memory.bus (&uncached_dma_data.status[0]),
        sizeof (uint32_t),
        0,
        m_uncached_memory.bus (&uncached_dma_data.cbs[2]),
        0
      },
      // 2
      {
        LABC::DMA::TI::LOGAN_STORE,
        m_LAB_Core->gpio.bus  (AP::GPIO::GPLEV0),
        m_uncached_memory.bus (&uncached_dma_data.rxd[1]),
        static_cast<uint32_t> (sizeof (uint32_t) * LAB_LOGIC_ANALYZER::NUMBER_OF_SAMPLES),
        0,
        m_uncached_memory.bus (&uncached_dma_data.cbs[3]),
        0
      },
      // 3
      {
        LABC::DMA::TI::LOGAN_STORE | AP::DMA::TI_DATA::INTEN,
        m_uncached_memory.bus (&uncached_dma_data.buffer_ok_flag),
        m_uncached_memory.bus (&uncached_dma_data.status[1]),
        sizeof (uint32_t),
        0,
        m_uncached_memory.bus (&uncached_dma_data.cbs[0]),
        0
      },

      // Single buffer
      // 4
      {
        LABC::DMA::TI::LOGAN_STORE,
        m_LAB_Core->gpio.bus  (AP::GPIO::GPLEV0),
        m_uncached_memory.bus (&uncached_dma_data.rxd[0]),
        static_cast<uint32_t> (sizeof (uint32_t) * LAB_LOGIC_ANALYZER::NUMBER_OF_SAMPLES),
        0,
        m_uncached_memory.bus (&uncached_dma_data.cbs[5]),
        0
      },
      // 5
      {
        LABC::DMA::TI::LOGAN_STORE | AP::DMA::TI_DATA::INTEN,
        m_uncached_memory.bus (&uncached_dma_data.buffer_ok_flag),
        m_uncached_memory.bus (&uncached_dma_data.status[0]),
        sizeof (uint32_t),
        0,
        m_uncached_memory.bus (&uncached_dma_data.cbs[4]),
        0
      },
    },
  };

  std::memcpy (
    &uncached_dma_data, 
    &new_uncached_dma_data, 
    sizeof (new_uncached_dma_data)
  );
}

void LAB_Logic_Analyzer:: 
run ()
{
  // in the meantime, use PWM triggering
  //m_LAB_Core->pwm_start (LAB_PWM_DMA_PACING_PWM_CHAN);
  m_parent_data.is_enabled = true;
}

void LAB_Logic_Analyzer:: 
stop ()
{
  //m_LAB_Core->pwm_stop (LAB_PWM_DMA_PACING_PWM_CHAN);
  m_parent_data.is_enabled = false;
}

void LAB_Logic_Analyzer:: 
sampling_rate (double value)
{
  // static_cast<LAB_DMA_Data_Oscilloscope *>(m_uncached_dma_data_logan.virt)->pwm_val = 
  //   (LAB_PWM_FREQUENCY * 2) / value;

  // m_LAB_Core->pwm.frequency (value, LAB_PWM_DUTY_CYCLE);
}

LABE::DISPLAY::MODE LAB_Logic_Analyzer:: 
display_mode ()
{
  return (m_parent_data.display_mode);
}

void LAB_Logic_Analyzer:: 
load_data_samples ()
{
  fill_raw_sample_buffer  ();
  parse_raw_sample_buffer ();
}

void LAB_Logic_Analyzer:: 
fill_raw_sample_buffer ()
{
  LAB_DMA_Data_Logic_Analyzer& dma_data = *(static_cast<LAB_DMA_Data_Logic_Analyzer*>
    (m_uncached_memory.virt ()));
  
  if (display_mode () == LABE::DISPLAY::MODE::SCREEN)
  {
    std::memcpy (
      m_parent_data.raw_sample_buffer.data (),
      const_cast<const void*>(static_cast<volatile void*>(dma_data.rxd[0])),
      sizeof (uint32_t) * LABC::LOGAN::NUMBER_OF_SAMPLES
    );
  }
  else if (display_mode () == LABE::DISPLAY::MODE::REPEATED)
  {
    for (int buff = 0; buff < 2; buff++)
    {
      if (dma_data.status[buff])
      {
        std::memcpy (
          m_parent_data.raw_sample_buffer.data (),
          const_cast<const void*>(static_cast<volatile void*>(dma_data.rxd[1])),
          sizeof (uint32_t) * LABC::LOGAN::NUMBER_OF_SAMPLES
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

void LAB_Logic_Analyzer::
parse_raw_sample_buffer()
{
  for (int samp = 0; samp < (m_parent_data.raw_sample_buffer.size ()); samp++)
  {
    for (int chan = 0; chan < (m_parent_data.channel_data.size ()); chan++)
    {
      m_parent_data.channel_data[chan].samples[samp] = 
        (m_parent_data.raw_sample_buffer[samp] >> LABC::PIN::LOGIC_ANALYZER[chan])
        & 0x1;
    }
  }
}

double LAB_Logic_Analyzer:: 
time_per_division (double value, unsigned disp_num_cols)
{
  double            new_samp_count  = calc_samp_count (value, disp_num_cols);
  double            new_samp_rate   = calc_samp_rate  (value, disp_num_cols);
  LABE::DISPLAY::MODE  new_display_mode   = calc_display_mode  (value);

  m_parent_data.time_per_division = value;
  m_parent_data.w_samp_count      = new_samp_count;
  m_parent_data.sampling_rate     = new_samp_rate;

  //display_mode          (new_display_mode);
  //set_hw_sampling_rate  (m_parent_data.sampling_rate);

  return 0;
}

double LAB_Logic_Analyzer:: 
calc_samp_count (double time_per_div, unsigned osc_disp_num_cols)
{
  if (time_per_div >= LAB_LOGIC_ANALYZER::MIN_TIME_PER_DIV_NO_ZOOM)
  {
    return (LAB_LOGIC_ANALYZER::NUMBER_OF_SAMPLES);
  }
  else 
  {
    return (LAB_LOGIC_ANALYZER::MAX_SAMPLING_RATE * osc_disp_num_cols *
      time_per_div);
  }
}

double LAB_Logic_Analyzer:: 
calc_samp_rate (double time_per_div, unsigned osc_disp_num_cols)
{
  if (time_per_div <= LAB_LOGIC_ANALYZER::MIN_TIME_PER_DIV_NO_ZOOM)
  {
    return (LAB_LOGIC_ANALYZER::MAX_SAMPLING_RATE);
  }
  else 
  {
    return (
      LAB_LOGIC_ANALYZER::NUMBER_OF_SAMPLES / (time_per_div * osc_disp_num_cols)
    );
  }
}

LABE::DISPLAY::MODE LAB_Logic_Analyzer:: 
calc_display_mode (double time_per_div)
{
  if (time_per_div >= LAB_LOGIC_ANALYZER::MIN_TIME_PER_DIV_DISP_SCREEN)
  {
    return (LABE::DISPLAY::MODE::SCREEN);
  }
  else 
  {
    return (LABE::DISPLAY::MODE::REPEATED);
  }
}

double LAB_Logic_Analyzer:: 
horizontal_offset (double value)
{
  m_parent_data.horizontal_offset = value;

  return (value);
}

void LAB_Logic_Analyzer:: 
switch_dma_buffer (LABE::DMA::BUFFER_COUNT buff_count)
{
  bool flag = false; 

  LAB_DMA_Data_Logic_Analyzer& dma_data = *(static_cast<LAB_DMA_Data_Logic_Analyzer*>
    (m_uncached_memory.virt ()));
  
  // 1. Pause PWM pacing DMA chan if running
  if (m_LAB_Core->dma.is_running (LABC::DMA::CHAN::PWM_PACING))
  {
    flag = true;
    m_LAB_Core->dma.pause (LABC::DMA::CHAN::PWM_PACING);
  }

  // 2. Assign next control block depending on buffer
  if (buff_count == LABE::DMA::BUFFER_COUNT::SINGLE)
  { 
    m_LAB_Core->dma.next_cb (LABC::DMA::CHAN::LOGAN_GPIO_STORE, m_uncached_memory.bus (&dma_data.cbs[4]));
  }
  else if (buff_count == LABE::DMA::BUFFER_COUNT::DOUBLE)
  {
    m_LAB_Core->dma.next_cb (LABC::DMA::CHAN::LOGAN_GPIO_STORE, m_uncached_memory.bus (&dma_data.cbs[0]));
  }

  // 3. Abort the current control block 
  m_LAB_Core->dma.abort (LABC::DMA::CHAN::LOGAN_GPIO_STORE);

  // 4. Clean buffer status
  dma_data.status[0] = dma_data.status[1] = 0;

  // 5. Run DMA channel if it was running
  if (flag)
  {
    m_LAB_Core->dma.run (LABC::DMA::CHAN::PWM_PACING);
  }
}

bool LAB_Logic_Analyzer:: 
is_running ()
{
  return (m_parent_data.is_enabled);
}

// EOF