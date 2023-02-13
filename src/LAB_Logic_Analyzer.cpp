#include "LAB_Logic_Analyzer.h"

#include <cstring>
#include <iostream>
#include <bitset>

LAB_Logic_Analyzer::
LAB_Logic_Analyzer (LAB_Core *_LAB_Core)
{
  m_LAB_Core = _LAB_Core;

  init_uncached_dma_data ();
}

void LAB_Logic_Analyzer:: 
init_uncached_dma_data ()
{
  m_LAB_Core->map_uncached_mem (&m_uncached_dma_data, LAB_LOGIC_ANALYZER_VC_MEM_SIZE);
  m_LAB_Core->dma_reset (LAB_LOGIC_ANALYZER_DMA_CHAN_GPIO_STORE);

  // m_LAB_Core->dma_reset (LAB_OSCILLOSCOPE_DMA_CHAN_PWM_PACING);

  LAB_LOGIC_ANALYZER_DMA_DATA *dp = 
    static_cast<LAB_LOGIC_ANALYZER_DMA_DATA *>(m_uncached_dma_data.virt);
  
  MemoryMap *mp = &m_uncached_dma_data;

  uint32_t m_pwm_range       = (LAB_PWM_FREQUENCY * 2) / LAB_LOGIC_ANALYZER_MAX_SAMPLING_RATE;
  
  LAB_LOGIC_ANALYZER_DMA_DATA dma_data = 
  {
    .cbs = 
    {
      // for dual buffer
      // CB0
      {
        LAB_LOGIC_ANALYZER_DMA_CB_TI_GPIO_STORE, 
        Utility::reg_bus_addr (&(m_LAB_Core->m_regs_gpio), GPIO_GPLEV0),
        Utility::mem_bus_addr (mp, dp->rxd0),
        (uint32_t)(LAB_LOGIC_ANALYZER_NUMBER_OF_SAMPLES * 4),
        0,
        Utility::mem_bus_addr (mp, &dp->cbs[1]),
        0
      },
      // CB1
      {
        LAB_LOGIC_ANALYZER_DMA_CB_TI_GPIO_STORE, 
        Utility::reg_bus_addr (&(m_LAB_Core->m_regs_dma), DMA_CS),
        Utility::mem_bus_addr (mp, &dp->states[0]),
        (uint32_t)(LAB_LOGIC_ANALYZER_NUMBER_OF_SAMPLES * 4),
        0,
        Utility::mem_bus_addr (mp, &dp->cbs[2]),
        0
      },
      // CB2
      {
        LAB_LOGIC_ANALYZER_DMA_CB_TI_GPIO_STORE, 
        Utility::reg_bus_addr (&(m_LAB_Core->m_regs_gpio), GPIO_GPLEV0),
        Utility::mem_bus_addr (mp, dp->rxd1),
        (uint32_t)(LAB_LOGIC_ANALYZER_NUMBER_OF_SAMPLES * 4),
        0,
        Utility::mem_bus_addr (mp, &dp->cbs[3]),
        0
      },
      // CB3
      {
        LAB_LOGIC_ANALYZER_DMA_CB_TI_GPIO_STORE, 
        Utility::reg_bus_addr (&(m_LAB_Core->m_regs_dma), DMA_CS),
        Utility::mem_bus_addr (mp, &dp->states[1]),
        (uint32_t)(LAB_LOGIC_ANALYZER_NUMBER_OF_SAMPLES * 4),
        0,
        Utility::mem_bus_addr (mp, &dp->cbs[4]),
        0
      },
      // CB4
      { 
        DMA_CB_TI_PWM, 
        MEM(mp, &dp->pwm_data),   
        REG(m_LAB_Core->m_regs_pwm, PWM_FIF1), 
        4, 
        0, 
        Utility::mem_bus_addr (mp, &dp->cbs[0]),
        0
      }, 


      // for single buffer
      // CB5
      {
        LAB_LOGIC_ANALYZER_DMA_CB_TI_GPIO_STORE, 
        Utility::reg_bus_addr (&(m_LAB_Core->m_regs_gpio), GPIO_GPLEV0),
        Utility::mem_bus_addr (mp, dp->rxd0),
        (uint32_t)(LAB_LOGIC_ANALYZER_NUMBER_OF_SAMPLES * 4),
        0,
        Utility::mem_bus_addr (mp, &dp->cbs[6]),
        0
      },

      // CB6
      { 
        DMA_CB_TI_PWM, 
        MEM(mp, &dp->pwm_data),   
        REG(m_LAB_Core->m_regs_pwm, PWM_FIF1), 
        4, 
        0, 
        Utility::mem_bus_addr (mp, &dp->cbs[5]),
        0
      }
    },

    .samp_size = 2, // in number of bytes
    .pwm_data   = m_pwm_range, 
    .usecs     = {0, 0},  
    .states    = {0, 0}, 
    .rxd0      = {0}, 
    .rxd1      = {0},
  };

  std::memcpy (static_cast<LAB_OSCILLOSCOPE_DMA_DATA *>(m_uncached_dma_data.virt), &dma_data, sizeof (dma_data));

  *(Utility::get_reg32 (m_LAB_Core->m_regs_pwm, PWM_DMAC)) = PWM_DMAC_ENAB | PWM_ENAB;

  sampling_rate (LAB_OSCILLOSCOPE_SAMPLING_RATE);

  m_LAB_Core->dma_start (mp, LAB_LOGIC_ANALYZER_DMA_CHAN_GPIO_STORE, &dp->cbs[0], 0);  // Start SPI Tx DMA
  // m_LAB_Core->dma_start(mp, LAB_OSCILLOSCOPE_DMA_CHAN_PWM_PACING, &dp->cbs[7], 0);  // Start PWM DMA, for SPI trigger
}

void LAB_Logic_Analyzer:: 
master_run ()
{
  m_is_running = true;
}

void LAB_Logic_Analyzer:: 
master_stop ()
{
  m_is_running = false;
}

// this changes PWM speed on board!! 
// verify no other are affected
void LAB_Logic_Analyzer:: 
sampling_rate (double value)
{
  // static_cast<LAB_OSCILLOSCOPE_DMA_DATA *>(m_uncached_adc_dma_data.virt)->pwm_val = 
  //   (LAB_PWM_FREQUENCY * 2) / value;

  m_LAB_Core->pwm_frequency (value, LAB_PWM_DUTY_CYCLE);
}

void LAB_Logic_Analyzer:: 
load_data_samples ()
{
  // auto start = std::chrono::steady_clock::now ();

  uint32_t raw_data_buffer [LAB_LOGIC_ANALYZER_NUMBER_OF_SAMPLES];

  LAB_LOGIC_ANALYZER_DMA_DATA *dp = 
    static_cast<LAB_LOGIC_ANALYZER_DMA_DATA *>(m_uncached_dma_data.virt);
  
  for (int b = 0; b < 2; b++)
  {
    if (dp->states[b])
    {
      std::memcpy (raw_data_buffer, b ? (void *)(dp->rxd1) : (void *)(dp->rxd0), 
        LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES * 4);

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

  std::cout << "logic analyz samps: " << std::bitset<32> (raw_data_buffer) << "\n";
    

  // if (m_channel_signals.m_chans[0].osc.osc_disp_mode == OSC_DISP_MODE_SCREEN)
  // {
  //   if (dp->states[m_curr_screen_buffer])
  //   {
  //     dp->states[m_curr_screen_buffer] = 0;
  //     m_curr_screen_buffer ^= 1;
  //   }

  //   std::memcpy (
  //     raw_data_buffer, 
  //     m_curr_screen_buffer ? (void *)(dp->rxd1) : (void *)(dp->rxd0),
  //     LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES * 4
  //   );
  // }
  // else if (m_channel_signals.m_chans[0].osc.osc_disp_mode == OSC_DISP_MODE_REPEATED)
  // {
  //   for (int b = 0; b < 2; b++)
  //   {
  //     if (dp->states[b])
  //     {
  //       std::memcpy (raw_data_buffer, b ? (void *)(dp->rxd1) : (void *)(dp->rxd0), 
  //         m_number_of_samples_per_channel * 4);

  //       if (dp->states[b ^ 1])
  //       {
  //         dp->states[0] = dp->states[1] = 0;
  //         // overrun
  //         break;
  //       }

  //       dp->states[b] = 0;

  //       // if (usec_start == 0)
  //       // {
  //       //   usec_start = dp->usecs[b];
  //       // }
  //     }
  //   }
  // }

  // skip processing of data if all channels are disabled
  // HARD CODED FOR 2 CHANNELS LAB
  // if (m_channel_signals.m_chans[0].is_enabled () || 
  //   m_channel_signals.m_chans[1].is_enabled ())
  // {
  //   double data;
  //   uint32_t shift_size = 32 / LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS;

  //   // go through all the samples in one go
  //   for (int samp = 0; samp < LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES; samp++)
  //   {
  //     // distribute the actual samples to each channel from a 32 bit sample
  //     for (int chan = 0; chan < LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS; chan++)
  //     {
  //       if (m_channel_signals.m_chans[chan].is_enabled ())
  //       {
  //         // START hard coded

  //         uint16_t temp1 = (raw_data_buffer[samp]) >> (16 * chan);
                    
  //         uint16_t temp2 = ((temp1 << 6) | (temp1 >> 10)) & 0x0FFF;  

  //         // END hard coded

  //         // get MSB to determine sign
  //         bool sign = temp2 >> (LAB_OSCILLOSCOPE_ADC_RESOLUTION_BITS - 1);

  //         // mask temp2 to mask out MSB sign bit
  //         temp2 = temp2 & ((LAB_OSCILLOSCOPE_ADC_RESOLUTION_INT - 1) >> 1);
      
  //         if (sign)
  //         {
  //           data = static_cast<double>(temp2) * LAB_OSCILLOSCOPE_ADC_CONVERSION_CONSTANT;
  //         }
  //         else 
  //         {
  //           data = (static_cast<double>(temp2) * LAB_OSCILLOSCOPE_ADC_CONVERSION_CONSTANT) -  
  //               LAB_OSCILLOSCOPE_ADC_REFERENCE_VOLTAGE;
  //         }

  //         m_channel_signals.m_chans[chan].osc.voltage_samples[samp] = data;
  //       }
  //     }
  //   }
  // }
  
  // auto end = std::chrono::steady_clock::now ();
  // std::chrono::duration<double, std::milli> elapsed = end - start;
  // std::cout << "Duration: " << elapsed.count () << " ms\n";
}

// EOF
