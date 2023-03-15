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

  // Initialize GPIO pins

  // Reset GPIO store DMA chan
  m_LAB_Core->dma_reset (LAB_DMA_CHAN_LOGIC_ANALYZER_GPIO_STORE);

  // Config DMA blocks
  config_dma_control_blocks ();

  // Start GPIO store DMA chan
  AP_MemoryMap                *ud = &(m_uncached_dma_data);
  LAB_Logic_Analyzer_DMA_Data *dd = static_cast<LAB_Logic_Analyzer_DMA_Data *>
    (ud->virt);

  m_LAB_Core->dma_start (ud, LAB_DMA_CHAN_LOGIC_ANALYZER_GPIO_STORE,
    &(dd->cbs[0]), 0);

  // initialize GPIO pins
  for (int a = 0; a < (sizeof (LAB_PIN_LOGIC_ANALYZER) / 
    sizeof (LAB_PIN_LOGIC_ANALYZER[0])); a++)
  {
    m_LAB_Core->gpio_set (LAB_PIN_LOGIC_ANALYZER[0], AP_GPIO_FUNC_INPUT,
      AP_GPIO_PULL_DOWN);
  }
}

void LAB_Logic_Analyzer:: 
run ()
{
  // in the meantime, use PWM triggering
  m_LAB_Core->pwm_start (LAB_PWM_DMA_PACING_PWM_CHAN);
  m_is_running = true;
}

void LAB_Logic_Analyzer:: 
stop ()
{
  m_LAB_Core->pwm_stop (LAB_PWM_DMA_PACING_PWM_CHAN);
  m_is_running = false;
}

// this changes PWM speed on board!! 
// verify no other are affected
void LAB_Logic_Analyzer:: 
sampling_rate (double value)
{
  // static_cast<LAB_Oscilloscope_DMA_Data *>(m_uncached_dma_data.virt)->pwm_val = 
  //   (LAB_PWM_FREQUENCY * 2) / value;

  m_LAB_Core->pwm_frequency (value, LAB_PWM_DUTY_CYCLE);
}

void LAB_Logic_Analyzer:: 
load_data_samples ()
{
  LAB_Logic_Analyzer_DMA_Data *dma_data = static_cast<LAB_Logic_Analyzer_DMA_Data *>
    (m_uncached_dma_data.virt);
  
  if (m_parent_data.graph_disp_mode == LE_GRAPH_DISP_MODE_SCREEN)
  {
    if (dma_data->status[m_curr_screen_buffer])
    {
      dma_data->status[m_curr_screen_buffer] = 0;
      m_curr_screen_buffer ^= 1;
    }

    std::memcpy (
      m_parent_data.raw_sample_buffer.data (),
      m_curr_screen_buffer ? (void *)(dma_data->rxd1) : (void *)(dma_data->rxd0),
      4 * LAB_LOGIC_ANALYZER_NUMBER_OF_SAMPLES
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
          m_curr_screen_buffer ? (void *)(dma_data->rxd1) : (void *)(dma_data->rxd0),
          4 * LAB_LOGIC_ANALYZER_NUMBER_OF_SAMPLES
        );

        // Check if the other buffer is also full. 
        // If it is, then we have a buffer overflow (both buffers full).
        if (dma_data->status[a ^ 1])
        {
          dma_data->status[0] = dma_data->status[1] = 0;

          break;
        }
        else 
        {
          dma_data->status[a] = 0;
        }
      }
    }
  }

  parse_raw_sample_buffer ();
}

void LAB_Logic_Analyzer::
parse_raw_sample_buffer()
{
  for (int a = 0; a < LAB_LOGIC_ANALYZER_NUMBER_OF_SAMPLES; a++)
  {
    for (int b = 0; b < LAB_LOGIC_ANALYZER_NUMBER_OF_CHANNELS; b++)
    {
      m_parent_data.channel_data[b].samples[a] = 
        ((m_parent_data.raw_sample_buffer[a] >> LAB_PIN_LOGIC_ANALYZER[b]) & 0x1);
    }
  }
}

void LAB_Logic_Analyzer::
config_dma_control_blocks ()
{
  m_LAB_Core->map_uncached_mem  (&m_uncached_dma_data, 
    LAB_LOGIC_ANALYZER_VC_MEM_SIZE);

  AP_MemoryMap *mp = &m_uncached_dma_data;

  LAB_Logic_Analyzer_DMA_Data *dp = static_cast<LAB_Logic_Analyzer_DMA_Data *>
    (m_uncached_dma_data.virt);
    
  LAB_Logic_Analyzer_DMA_Data dma_data = 
  {
    .cbs = 
    {
      // for dual buffer
      {
        // CB0
        LAB_LOGIC_ANALYZER_DMA_CB_TI_GPIO_STORE, 
        REG (m_LAB_Core->m_regs_gpio, GPIO_GPLEV0),
        MEM (mp, dp->rxd0),
        (uint32_t)(LAB_LOGIC_ANALYZER_NUMBER_OF_SAMPLES * 4),
        0,
        MEM (mp, &dp->cbs[1]),
        0
      },
      { // CB1
        LAB_LOGIC_ANALYZER_DMA_CB_TI_GPIO_STORE,
        MEM (mp, &dp->buffer_ok_flag),
        MEM (mp, &dp->status[0]),
        4,
        0,
        MEM (mp, &dp->cbs[2]),
        0
      },
      { // CB2
        LAB_LOGIC_ANALYZER_DMA_CB_TI_GPIO_STORE, 
        REG (m_LAB_Core->m_regs_gpio, GPIO_GPLEV0),
        MEM (mp, dp->rxd1),
        (uint32_t)(LAB_LOGIC_ANALYZER_NUMBER_OF_SAMPLES * 4),
        0,
        MEM (mp, &dp->cbs[3]),
        0
      },
      { //CB 3
        LAB_LOGIC_ANALYZER_DMA_CB_TI_GPIO_STORE,
        MEM (mp, &dp->buffer_ok_flag),
        MEM (mp, &dp->status[1]),
        4,
        0,
        MEM (mp, &dp->cbs[0]),
        0
      },

      // for single buffer
      {
        // CB4
        LAB_LOGIC_ANALYZER_DMA_CB_TI_GPIO_STORE, 
        REG (m_LAB_Core->m_regs_gpio, GPIO_GPLEV0),
        MEM (mp, dp->rxd0),
        (uint32_t)(LAB_LOGIC_ANALYZER_NUMBER_OF_SAMPLES * 4),
        0,
        MEM (mp, &dp->cbs[1]),
        0
      },
      {
        // CB5
        LAB_LOGIC_ANALYZER_DMA_CB_TI_GPIO_STORE,
        MEM (mp, &dp->buffer_ok_flag),
        MEM (mp, &dp->status[0]),
        4,
        0,
        MEM (mp, &dp->cbs[2]),
        0
      },
    },

    .samp_size = 2, // in number of bytes
    .usecs     = {0, 0},  
    .status    = {0, 0}, 
    .rxd0      = {0}, 
    .rxd1      = {0},
  };

  std::memcpy (dp, &dma_data, sizeof (dma_data));
}

double LAB_Logic_Analyzer:: 
time_per_division (double value, unsigned disp_num_cols)
{
  if (m_parent_data.time_per_division != value)
  {
    double new_samp_count;
    double new_samp_rate;

    // 1. Calculate the new count of samples
    if (value >= LAB_LOGIC_ANALYZER_MIN_TIME_PER_DIV_NO_ZOOM)
    {
      new_samp_count = LAB_LOGIC_ANALYZER_NUMBER_OF_SAMPLES;
    }
    else 
    {
      new_samp_count = LAB_LOGIC_ANALYZER_MAX_SAMPLING_RATE * disp_num_cols *
        value;
    }

    // 2. Calculate the new sampling rate
    if (value <= LAB_LOGIC_ANALYZER_MIN_TIME_PER_DIV_NO_ZOOM)
    {
      new_samp_rate = LAB_LOGIC_ANALYZER_MAX_SAMPLING_RATE;
    }
    else 
    {
      new_samp_rate = (LAB_LOGIC_ANALYZER_NUMBER_OF_SAMPLES) / 
        (value * disp_num_cols);
    }

    // 3. Change the graph display mode if necessary
    if (value >= LAB_LOGIC_ANALYZER_MIN_TIME_PER_DIV_GRAPH_DISP_MODE_SCREEN)
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
  LAB_Logic_Analyzer_DMA_Data *dma_data = static_cast
    <LAB_Logic_Analyzer_DMA_Data *>(m_uncached_dma_data.virt);
  
  volatile uint32_t *reg = Utility::get_reg32 (m_LAB_Core->m_regs_dma,
    DMA_REG (LAB_DMA_CHAN_OSCILLOSCOPE_SPI_RX, DMA_NEXTCONBK));

  if (_LE_SPI_DMA_BUFFER_COUNT == LE_SPI_DMA_BUFFER_COUNT_SINGLE)
  {
    *reg = Utility::mem_bus_addr (&m_uncached_dma_data, &(dma_data->cbs[4]));
  }
  else // (buffer == LE_SPI_DMA_BUFFER_COUNT_DOUBLE)
  {
    *reg = Utility::mem_bus_addr (&m_uncached_dma_data, &(dma_data->cbs[0]));
  }

  // abort current DMA control block
  m_LAB_Core->dma_abort (LAB_DMA_CHAN_OSCILLOSCOPE_SPI_RX);

  // clear buffer status
  static_cast<LAB_Logic_Analyzer_DMA_Data *>(m_uncached_dma_data.virt)->
    status[0] = 0;
  static_cast<LAB_Logic_Analyzer_DMA_Data *>(m_uncached_dma_data.virt)->
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