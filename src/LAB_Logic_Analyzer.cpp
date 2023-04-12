#include "LAB_Logic_Analyzer.h"

#include "LAB.h"

#include <cstring>
#include <bitset>
// #include <iostream>
// #include <bitset>

LAB_Logic_Analyzer::
LAB_Logic_Analyzer (LAB_Core *_LAB_Core, LAB *_LAB)
{
  // PWM should be initialized already at this point in LAB
  m_LAB_Core  = _LAB_Core;
  m_LAB       = _LAB;

  init_logan_gpio_pins  ();
  init_logan_dma        ();
}

LAB_Logic_Analyzer::
~LAB_Logic_Analyzer ()
{
  m_LAB_Core->unmap_periph_mem (&m_uncached_dma_data_logan);
}

void LAB_Logic_Analyzer:: 
init_logan_gpio_pins ()
{
  for (int chan = 0; chan < LAB_LOGIC_ANALYZER::NUMBER_OF_CHANNELS; chan++)
  {
    m_LAB_Core->gpio_set (
      LAB_PIN_LOGIC_ANALYZER[chan],
      AP_GPIO_FUNC_INPUT,
      AP_GPIO_PULL_DOWN
    );
  }
}

void LAB_Logic_Analyzer:: 
init_logan_dma ()
{
    // Reset GPIO store DMA chan
  m_LAB_Core->dma_reset (LAB_DMA_CHAN_LOGIC_ANALYZER_GPIO_STORE);

  // Config DMA blocks
  config_dma_cb ();

  // Start GPIO store DMA chan
  AP_MemoryMap                *mp = &(m_uncached_dma_data_logan);
  LAB_DMA_Data_Logic_Analyzer *dp = static_cast<LAB_DMA_Data_Logic_Analyzer *>(mp->virt);

  m_LAB_Core->dma_start (
    mp,
     LAB_DMA_CHAN_LOGIC_ANALYZER_GPIO_STORE,
    &(dp->cbs[0]),
    0
  );
}

void LAB_Logic_Analyzer::
config_dma_cb ()
{
  m_LAB_Core->map_uncached_mem  (
    &m_uncached_dma_data_logan, 
    LAB_LOGIC_ANALYZER::VC_MEM_SIZE
  );

  AP_MemoryMap                     *mp = &m_uncached_dma_data_logan;
  LAB_DMA_Data_Logic_Analyzer      *dp = static_cast<LAB_DMA_Data_Logic_Analyzer *>(mp->virt);
  LAB_DMA_Data_Logic_Analyzer dma_data = 
  {
    .cbs = 
    {
      // for dual buffer
      {
        // CB0
        LAB_DMA_TI_LOGAN_STORE, 
        Utility::reg_bus_addr (&(m_LAB_Core->m_regs_gpio), GPIO_GPLEV0),
        Utility::mem_bus_addr (mp, dp->rxd[0]),
        (uint32_t)(4 * LAB_LOGIC_ANALYZER::NUMBER_OF_SAMPLES),
        0,
        Utility::mem_bus_addr (mp, &dp->cbs[1]),
        0
      },
      { // CB1
        LAB_DMA_TI_LOGAN_STORE,
        MEM (mp, &dp->buffer_ok_flag),
        MEM (mp, &dp->status[0]),
        4,
        0,
        Utility::mem_bus_addr (mp, &dp->cbs[2]),
        0
      },
      { // CB2
        LAB_DMA_TI_LOGAN_STORE, 
        Utility::reg_bus_addr (&(m_LAB_Core->m_regs_gpio), GPIO_GPLEV0),
        Utility::mem_bus_addr (mp, dp->rxd[1]),
        (uint32_t)(4 * LAB_LOGIC_ANALYZER::NUMBER_OF_SAMPLES),
        0,
        Utility::mem_bus_addr (mp, &dp->cbs[3]),
        0
      },
      { //CB 3
        LAB_DMA_TI_LOGAN_STORE,
        MEM (mp, &dp->buffer_ok_flag),
        MEM (mp, &dp->status[1]),
        4,
        0,
        Utility::mem_bus_addr (mp, &dp->cbs[0]),
        0
      },

      // for single buffer
      {
        // CB4
        LAB_DMA_TI_LOGAN_STORE, 
        Utility::reg_bus_addr (&(m_LAB_Core->m_regs_gpio), GPIO_GPLEV0),
        Utility::mem_bus_addr (mp, dp->rxd[0]),
        (uint32_t)(4 * LAB_LOGIC_ANALYZER::NUMBER_OF_SAMPLES),
        0,
        Utility::mem_bus_addr (mp, &dp->cbs[1]),
        0
      },
      {
        // CB5
        LAB_DMA_TI_LOGAN_STORE,
        MEM (mp, &dp->buffer_ok_flag),
        MEM (mp, &dp->status[0]),
        4,
        0,
        Utility::mem_bus_addr (mp, &dp->cbs[2]),
        0
      },
    },
  };

  std::memcpy (dp, &dma_data, sizeof (dma_data));
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

// this changes PWM speed on board!! 
// verify no other are affected
void LAB_Logic_Analyzer:: 
sampling_rate (double value)
{
  // static_cast<LAB_DMA_Data_Oscilloscope *>(m_uncached_dma_data_logan.virt)->pwm_val = 
  //   (LAB_PWM_FREQUENCY * 2) / value;

  m_LAB_Core->pwm_frequency (value, LAB_PWM_DUTY_CYCLE);
}

void LAB_Logic_Analyzer:: 
load_data_samples ()
{
  LAB_DMA_Data_Logic_Analyzer *dma_data = 
    static_cast<LAB_DMA_Data_Logic_Analyzer *>(m_uncached_dma_data_logan.virt);
  
  if (m_parent_data.disp_mode == LE::DISPLAY_MODE::SCREEN)
  {
    std::memcpy (
      m_parent_data.raw_sample_buffer.data (),
      (void *)(dma_data->rxd[0]),
      4 * LAB_LOGIC_ANALYZER::NUMBER_OF_SAMPLES
    );
  }
  else if (m_parent_data.disp_mode == LE::DISPLAY_MODE::REPEATED)
  {
    for (int buff = 0; buff < 2; buff++)
    {
      std::memcpy (
        m_parent_data.raw_sample_buffer.data (),
        (void *)(dma_data->rxd[buff]),
        4 * LAB_LOGIC_ANALYZER::NUMBER_OF_SAMPLES
      );

      dma_data->status[buff] = 0;

      if (dma_data->status[buff ^ 1]) // overrun!
      {
        dma_data->status[0] = dma_data->status[1] = 0;
      }
    }
  }

  parse_raw_sample_buffer ();
}

void LAB_Logic_Analyzer::
parse_raw_sample_buffer()
{
  for (int samp_i = 0; samp_i < m_parent_data.raw_sample_buffer.size (); samp_i++)
  {
    for (int chan = 0; chan < m_parent_data.channel_data.size (); chan++)
    {
      m_parent_data.channel_data[chan].samples[samp_i] = 
        (m_parent_data.raw_sample_buffer[samp_i] >> LAB_PIN_LOGIC_ANALYZER[chan]) & 0x1;
    }
  }
}

double LAB_Logic_Analyzer:: 
time_per_division (double value, unsigned disp_num_cols)
{
  if (m_parent_data.time_per_division != value)
  {
    double new_samp_count;
    double new_samp_rate;

    // 1. Calculate the new count of samples
    if (value >= LAB_LOGIC_ANALYZER::MIN_TIME_PER_DIV_NO_ZOOM)
    {
      new_samp_count = LAB_LOGIC_ANALYZER::NUMBER_OF_SAMPLES;
    }
    else 
    {
      new_samp_count = LAB_LOGIC_ANALYZER::MAX_SAMPLING_RATE * disp_num_cols *
        value;
    }

    // 2. Calculate the new sampling rate
    if (value <= LAB_LOGIC_ANALYZER::MIN_TIME_PER_DIV_NO_ZOOM)
    {
      new_samp_rate = LAB_LOGIC_ANALYZER::MAX_SAMPLING_RATE;
    }
    else 
    {
      new_samp_rate = (LAB_LOGIC_ANALYZER::NUMBER_OF_SAMPLES) / 
        (value * disp_num_cols);
    }

    // 3. Change the graph display mode if necessary
    if (value >= LAB_LOGIC_ANALYZER::MIN_TIME_PER_DIV_DISP_SCREEN)
    {
      if (m_parent_data.disp_mode != LE::DISPLAY_MODE::SCREEN)
      {
        m_parent_data.disp_mode = LE::DISPLAY_MODE::SCREEN;
        switch_dma_buffer (LE_SPI_DMA_BUFFER_COUNT_SINGLE);
      }
    }
    else 
    {
      if (m_parent_data.disp_mode != LE::DISPLAY_MODE::REPEATED)
      {
        m_parent_data.disp_mode = LE::DISPLAY_MODE::REPEATED;
        switch_dma_buffer (LE_SPI_DMA_BUFFER_COUNT_SINGLE);
      }
    }

    // 4. Set the new time per division, sample count, and sampling rate values
    m_parent_data.time_per_division   = value;
    m_parent_data.w_samp_count  = new_samp_count;
    
    // 5. If new sampling rate is different from current, change sampling rate
    if (m_parent_data.sampling_rate != new_samp_rate)
    {
      sampling_rate (new_samp_rate);
    }
  }

  return value;
}

double LAB_Logic_Analyzer:: 
horizontal_offset (double value)
{
  m_parent_data.horizontal_offset = value;

  return (value);
}

void LAB_Logic_Analyzer:: 
switch_dma_buffer (LE_SPI_DMA_BUFFER_COUNT _LE_SPI_DMA_BUFFER_COUNT)
{
  bool is_dma_pwm_pacing_running = false;

  if (!(m_LAB_Core->is_dma_paused (LAB_DMA_CHAN_PWM_PACING)))
  {
    is_dma_pwm_pacing_running = true;
    m_LAB_Core->dma_pause (LAB_DMA_CHAN_PWM_PACING);
  }
 
  // load the next cb depending on buffer
  LAB_DMA_Data_Logic_Analyzer *dma_data = static_cast
    <LAB_DMA_Data_Logic_Analyzer *>(m_uncached_dma_data_logan.virt);
  
  volatile uint32_t *reg = Utility::get_reg32 (m_LAB_Core->m_regs_dma,
    DMA_REG (LAB_DMA_CHAN_OSCILLOSCOPE_SPI_RX, DMA_NEXTCONBK));

  if (_LE_SPI_DMA_BUFFER_COUNT == LE_SPI_DMA_BUFFER_COUNT_SINGLE)
  {
    *reg = Utility::mem_bus_addr (&m_uncached_dma_data_logan, &(dma_data->cbs[4]));
  }
  else // (buffer == LE_SPI_DMA_BUFFER_COUNT_DOUBLE)
  {
    *reg = Utility::mem_bus_addr (&m_uncached_dma_data_logan, &(dma_data->cbs[0]));
  }

  // abort current DMA control block
  m_LAB_Core->dma_abort (LAB_DMA_CHAN_OSCILLOSCOPE_SPI_RX);

  // clear buffer status
  static_cast<LAB_DMA_Data_Logic_Analyzer *>(m_uncached_dma_data_logan.virt)->
    status[0] = 0;
  static_cast<LAB_DMA_Data_Logic_Analyzer *>(m_uncached_dma_data_logan.virt)->
    status[1] = 0;

  if (is_dma_pwm_pacing_running)
  {
    m_LAB_Core->dma_play (LAB_DMA_CHAN_PWM_PACING);
  }
}

bool LAB_Logic_Analyzer:: 
is_running ()
{
  return (m_parent_data.is_enabled);
}

// EOF