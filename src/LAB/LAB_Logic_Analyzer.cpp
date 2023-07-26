#include "LAB_Logic_Analyzer.h"

#include "LAB.h"

#include <bitset>
#include <cstring>
#include <iostream>

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
  for (unsigned chan = 0; chan < (m_parent_data.channel_data.size ()); chan++)
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
  m_uncached_memory.map_uncached_mem (LABC::LOGAN::VC_MEM_SIZE);

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
        static_cast<uint32_t> (sizeof (uint32_t) * LABC::LOGAN::NUMBER_OF_SAMPLES),
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
        static_cast<uint32_t> (sizeof (uint32_t) * LABC::LOGAN::NUMBER_OF_SAMPLES),
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
        static_cast<uint32_t> (sizeof (uint32_t) * LABC::LOGAN::NUMBER_OF_SAMPLES),
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
  m_parent_data.is_logan_core_running = true;
}

void LAB_Logic_Analyzer:: 
stop ()
{
  //m_LAB_Core->pwm_stop (LAB_PWM_DMA_PACING_PWM_CHAN);
  m_parent_data.is_enabled = false;
  m_parent_data.is_logan_core_running = false;
}

void LAB_Logic_Analyzer:: 
fill_channel_samples_buffer ()
{
  // switch (trigger_mode ())
  // {
  //   case (LABE::LOGAN::TRIG::MODE::NONE):
  //   {
  //     fill_raw_data_buffer_using_uncached_data_buffer (); 
  //     parse_raw_data_buffer ();

  //     break;
  //   }

  //   case (LABE::LOGAN::TRIG::MODE::NORMAL):
  //   {
  //     break;
  //   }

  //   case (LABE::LOGAN::TRIG::MODE::AUTO):
  //   {
  //     break;
  //   }
  // }
}

void LAB_Logic_Analyzer:: 
fill_raw_data_buffer_using_uncached_data_buffer ()
{
  LAB_DMA_Data_Logic_Analyzer& dma_data = 
    *(static_cast<LAB_DMA_Data_Logic_Analyzer*>(m_uncached_memory.virt ()));

  switch (mode ())
  {
    case (LABE::LOGAN::MODE::SCREEN):
    {
      std::memcpy (
        m_parent_data.raw_data_buffer.data (),
        const_cast<const void*>(static_cast<volatile void*>(dma_data.rxd[0])),
        sizeof (uint32_t) * m_parent_data.samples
      );

      break;
    }

    case (LABE::LOGAN::MODE::REPEATED):
    {
      for (unsigned buff = 0; buff < 2; buff++)
      {
        if (dma_data.status[buff])
        {
          std::memcpy (
            m_parent_data.raw_data_buffer.data (),
            const_cast<const void*>(static_cast<volatile void*>(dma_data.rxd[buff])),
            sizeof (uint32_t) * m_parent_data.samples
          );
        }

        if (dma_data.status[buff ^ 1])
        {
          dma_data.status[0] = dma_data.status[1] = 0;

          break;
        }

        dma_data.status[buff] = 0;
      }
    }
  }
}

void LAB_Logic_Analyzer:: 
parse_raw_data_buffer ()
{
  LAB_Parent_Data_Logic_Analyzer& pdata = m_parent_data;

  for (unsigned samp = 0; samp < (m_parent_data.raw_data_buffer.size ()); samp++)
  {
    for (unsigned chan = 0; chan < (m_parent_data.channel_data.size ()); chan++)
    {
      pdata.channel_data[chan].samples[samp] = 
        (pdata.raw_data_buffer[samp] >> LABC::PIN::LOGIC_ANALYZER[chan]) & 0x1;    

      // For debug
      // if (samp == 0 && chan == 0)
      // {
      //   std::cout << std::bitset <32> (pdata.raw_data_buffer[samp]) << "\n";
      // }  
    }
  }
}

void LAB_Logic_Analyzer:: 
set_samples (unsigned value)
{
  m_parent_data.samples = value;

  LAB_DMA_Data_Logic_Analyzer& uncached_dma_data = 
    *(static_cast<LAB_DMA_Data_Logic_Analyzer*>(m_uncached_memory.virt ()));
  
  uncached_dma_data.cbs[0].txfr_len = static_cast<uint32_t>(sizeof (uint32_t) * value);
  uncached_dma_data.cbs[2].txfr_len = static_cast<uint32_t>(sizeof (uint32_t) * value);
  uncached_dma_data.cbs[4].txfr_len = static_cast<uint32_t>(sizeof (uint32_t) * value);
}

void LAB_Logic_Analyzer::
parse_trigger_mode ()
{
  switch (m_parent_data.trigger_mode)
  {
    case (LABE::LOGAN::TRIG::MODE::NONE):
    {
      if (m_thread_find_trigger.joinable ())
      {
        m_parent_data.find_trigger = false;
        m_thread_find_trigger.join ();
      }

      break;
    }

    case (LABE::LOGAN::TRIG::MODE::AUTO):
    {
      if (!m_thread_find_trigger.joinable ())
      {
        m_parent_data.find_trigger = true;
        m_thread_find_trigger = std::thread (&LAB_Logic_Analyzer::find_trigger_point_loop, this);
      }

      break;
    }

    case (LABE::LOGAN::TRIG::MODE::NORMAL):
    {
      if (!m_thread_find_trigger.joinable ())
      {
        m_parent_data.find_trigger = true;
        m_thread_find_trigger = std::thread (&LAB_Logic_Analyzer::find_trigger_point_loop, this);
      }

      break;
    }
  }
}

void LAB_Logic_Analyzer:: 
find_trigger_point_loop ()
{

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
  
  if (mode () == LABE::LOGAN::MODE::SCREEN)
  {
    std::memcpy (
      m_parent_data.raw_data_buffer.data (),
      const_cast<const void*>(static_cast<volatile void*>(dma_data.rxd[0])),
      sizeof (uint32_t) * LABC::LOGAN::NUMBER_OF_SAMPLES
    );
  }
  else if (mode () == LABE::LOGAN::MODE::REPEATED)
  {
    for (int buff = 0; buff < 2; buff++)
    {
      if (dma_data.status[buff])
      {
        std::memcpy (
          m_parent_data.raw_data_buffer.data (),
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
  for (int samp = 0; samp < (m_parent_data.raw_data_buffer.size ()); samp++)
  {
    for (int chan = 0; chan < (m_parent_data.channel_data.size ()); chan++)
    {
      m_parent_data.channel_data[chan].samples[samp] = 
        (m_parent_data.raw_data_buffer[samp] >> LABC::PIN::LOGIC_ANALYZER[chan])
        & 0x1;
    }
  }
}

double LAB_Logic_Analyzer:: 
calc_samp_count (double time_per_div, unsigned osc_disp_num_cols)
{
  if (time_per_div >= LABC::LOGAN::MIN_TIME_PER_DIV_NO_ZOOM)
  {
    return (LABC::LOGAN::NUMBER_OF_SAMPLES);
  }
  else 
  {
    return (LABC::LOGAN::MAX_SAMPLING_RATE * osc_disp_num_cols *
      time_per_div);
  }
}

double LAB_Logic_Analyzer:: 
calc_samp_rate (double time_per_div, unsigned osc_disp_num_cols)
{
  if (time_per_div <= LABC::LOGAN::MIN_TIME_PER_DIV_NO_ZOOM)
  {
    return (LABC::LOGAN::MAX_SAMPLING_RATE);
  }
  else 
  {
    return (
      LABC::LOGAN::NUMBER_OF_SAMPLES / (time_per_div * osc_disp_num_cols)
    );
  }
}

LABE::LOGAN::MODE LAB_Logic_Analyzer:: 
calc_mode (double time_per_division)
{
  LABE::LOGAN::MODE mode;

  if (time_per_division < LABC::LOGAN::MIN_TIME_PER_DIVISION_SCREEN)
  {
    mode = LABE::LOGAN::MODE::REPEATED;
  } 
  else if (m_parent_data.time_per_division < LABC::LOGAN::MIN_TIME_PER_DIVISION_SCREEN &&
    time_per_division >= LABC::LOGAN::MIN_TIME_PER_DIVISION_SCREEN)
  {
    mode = LABE::LOGAN::MODE::SCREEN;
  }

  return (mode);
}

LAB_Parent_Data_Logic_Analyzer& LAB_Logic_Analyzer:: 
parent_data ()
{
  return (m_parent_data);
}

void LAB_Logic_Analyzer:: 
mode (LABE::LOGAN::MODE mode)
{

}

LABE::LOGAN::MODE LAB_Logic_Analyzer:: 
mode ()
{
  return (m_parent_data.mode);
}

// --- Horizontal

void LAB_Logic_Analyzer:: 
horizontal_offset (double value)
{
  m_parent_data.horizontal_offset = value;
}

double LAB_Logic_Analyzer:: 
horizontal_offset () const
{
  return (m_parent_data.horizontal_offset);
}

void LAB_Logic_Analyzer:: 
time_per_division (double value)
{
  // double            new_samp_count  = calc_samp_count (value, disp_num_cols);
  // double            new_samp_rate   = calc_samp_rate  (value, disp_num_cols);
  // //LABE::OSC::MODE  new_display_mode   = calc_display_mode  (value);

  // m_parent_data.time_per_division = value;
  // m_parent_data.w_samp_count      = new_samp_count;
  // m_parent_data.sampling_rate     = new_samp_rate;

  // //display_mode          (new_display_mode);
  // //set_hw_sampling_rate  (m_parent_data.sampling_rate);
}

double LAB_Logic_Analyzer:: 
time_per_division () const
{
  return (m_parent_data.time_per_division);
}

void LAB_Logic_Analyzer::
samples (unsigned value)
{

}

unsigned LAB_Logic_Analyzer::
samples () const
{
  return (m_parent_data.samples);
}

void LAB_Logic_Analyzer::
sampling_rate (double value)
{

}

double LAB_Logic_Analyzer::
sampling_rate () const
{
  return (m_parent_data.sampling_rate);
}

void LAB_Logic_Analyzer:: 
dma_buffer_count (LABE::LOGAN::BUFFER_COUNT buffer_count)
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
  if (buffer_count == LABE::LOGAN::BUFFER_COUNT::SINGLE)
  { 
    m_LAB_Core->dma.next_cb (LABC::DMA::CHAN::LOGAN_GPIO_STORE, m_uncached_memory.bus (&dma_data.cbs[4]));
  }
  else if (buffer_count == LABE::LOGAN::BUFFER_COUNT::DOUBLE)
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

void LAB_Logic_Analyzer:: 
trigger (unsigned channel, LABE::LOGAN::TRIG::CND condition)
{

}

// EOF