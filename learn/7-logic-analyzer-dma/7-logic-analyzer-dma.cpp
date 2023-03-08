#include <cstring>
#include <iostream>

#include "../../lib/AikaPi/AikaPi.h"

// compile with 
// g++ 7-logic-analyzer-dma.cpp ../../lib/AikaPi/AikaPi.cpp -o 7-logic-analyzer-dma

constexpr unsigned PIN_PWM = 12;
constexpr unsigned LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES = 2000;
constexpr unsigned DMA_CHAN_RX = 8;
constexpr unsigned DMA_CHAN_PWM_PACING = 7;

struct LAB_OSCILLOSCOPE_DMA_DATA
{
  AP_DMA_CB cbs[10];

  uint32_t  samp_size,
            pwm_val, 
            adc_csd,
            txd[2];

  volatile uint32_t usecs[2],
                    states[2],
                    rxd0[LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES],
                    rxd1[LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES];
};

int main ()
{
  AikaPi AP;

  AP.pwm_init  (10'000);
  // AP.pwm_duty_cycle (0, 50);
  AP.pwm_algo  (0, AP_PWM_ALGO_MARKSPACE);
  AP.pwm_fifo_clear ();
  
  // 9.) start pwm
  AP.pwm_start (0);

  *(Utility::get_reg32 (AP.m_regs_pwm, PWM_FIF1)) = 100000;

  // 10.) pause
  std::cout << "cm pwm ctl: \n";
  Utility::disp_reg32 (AP.m_regs_cm, CM_PWMCTL);  

  std::cout << "cm pwm div: \n";
  Utility::disp_reg32 (AP.m_regs_cm, CM_PWMDIV);  

  std::cout << "pwm CTL: \n";
  Utility::disp_reg32 (AP.m_regs_pwm, PWM_CTL); 

  std::cout << "pwm rng: \n";
  Utility::disp_reg32 (AP.m_regs_pwm, PWM_RNG1); 

  std::cout << "pwm STA: \n";
  Utility::disp_reg32 (AP.m_regs_pwm, PWM_STA); 

  std::cout << "\n\npause, waiting for enter...\n";

  double pwm_freq, pwm_dc;
  
  while (true)
  {
    std::cout << "\n\nInput new freq: ";
    std::cin >> pwm_freq;
    std::cout << "\nInput duty cycle: " ;
    std::cin >> pwm_dc;

    AP.pwm_frequency  (0, pwm_freq);
    AP.pwm_duty_cycle (0, pwm_dc);
  }

  //
  AP.pwm_stop (0);

  // 11.) unmap
  //AP.unmap_periph_mem (&m_uncached_memory);

  return 0;


















  // AP_MemoryMap m_uncached_memory;

  // // 1.)
  // AP.gpio_set (PIN_PWM, AP_GPIO_FUNC_ALT0, AP_GPIO_PULL_DOWN);
  // AP.pwm_init (200'000); 

  // *(Utility::get_reg32 (AP.m_regs_pwm, PWM_DMAC)) = (1 << 31) | (15 << 8) | (15 << 0);

  // //2.) map uncached memory
  // AP.map_uncached_mem (&m_uncached_memory, sizeof (LAB_OSCILLOSCOPE_DMA_DATA));

  // //3.) set control blocks
  // AP_MemoryMap *mp = &m_uncached_memory;

  // LAB_OSCILLOSCOPE_DMA_DATA *dp = static_cast<LAB_OSCILLOSCOPE_DMA_DATA *>(m_uncached_memory.virt);
  
  // LAB_OSCILLOSCOPE_DMA_DATA dma_data = 
  // {
  //   .cbs =
  //   {
  //     // store
  //     // {
  //     //   DMA_TI_DREQ_PWM << 16 | DMA_TI_SRC_DREQ | DMA_TI_DEST_INC | DMA_TI_WAIT_RESP,
  //     //   Utility::reg_bus_addr (&(AP.m_regs_st), AP_ST_CLO),
  //     //   Utility::mem_bus_addr (&m_uncached_memory, dp->rxd0),
  //     //   static_cast<uint32_t>(4 * 2000),
  //     //   0,
  //     //   Utility::mem_bus_addr (&m_uncached_memory, &(dp->cbs[0])),
  //     //   0
  //     // },

  //     // PWM trigger
  //     {
  //       DMA_TI_DREQ_PWM << 16 | DMA_TI_DEST_DREQ | DMA_TI_WAIT_RESP,
  //       Utility::mem_bus_addr (&m_uncached_memory, &(dp->pwm_val)),
  //       Utility::reg_bus_addr (&(AP.m_regs_pwm), PWM_FIF1),
  //       4,
  //       0,
  //       Utility::mem_bus_addr (&m_uncached_memory, &(dp->cbs[0])),
  //       0
  //     }
  //   },
    
  //   .samp_size  = 4,
  //   .pwm_val    = static_cast<uint32_t>(32000),
  //   .adc_csd    = 0,
  //   .txd        = {0xffff, 0x0000},
  //   .usecs      = {0, 0},
  //   .states     = {0, 0},
  //   .rxd0       = {0, 0},
  //   .rxd1       = {0, 0},
  // };

  // // 4.) copy control block to uncached mem
  // std::memcpy (dp, &dma_data, sizeof (dma_data));

  // // 5.) reset dma on the dma chans
  // AP.dma_reset (DMA_CHAN_PWM_PACING);
  // //AP.dma_reset (DMA_CHAN_RX);

  // // 8.) start dma chans
  // //AP.dma_start (DMA_CHAN_RX, &m_uncached_memory, &(dp->cbs[0]));
  // AP.dma_start (DMA_CHAN_PWM_PACING, &m_uncached_memory, &(dp->cbs[1]));

  // // 9.) start pwm
  // AP.pwm_start (0);

  // // 10.) pause
  // std::cout << "cm pwm ctl: \n";
  // Utility::disp_reg32 (AP.m_regs_cm, CM_PWMCTL);  

  // std::cout << "cm pwm div: \n";
  // Utility::disp_reg32 (AP.m_regs_cm, CM_PWMDIV);  

  // std::cout << "pwm rng: \n";
  // Utility::disp_reg32 (AP.m_regs_pwm, PWM_RNG1); 

  // std::cout << "pwm STA: \n";
  // Utility::disp_reg32 (AP.m_regs_pwm, PWM_STA); 

  // std::cout << "\n\npause, waiting for enter...\n";
  // std::cin.get ();

  // //
  // AP.pwm_stop (0);

  // // 11.) unmap
  // //AP.unmap_periph_mem (&m_uncached_memory);

  // return 0;
}