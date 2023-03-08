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
  AP_MemoryMap m_uncached_memory;
  
  // AP.cm_pwm_clk_run ();

  // Utility::disp_reg32 (AP.m_regs_cm, CM_PWMCTL);

  // if (AP.cm_pwm_clk_is_running ())
  // {
  //   std::cout << "clock is running\n";
  // }
  // else 
  // {
  //   std::cout << "clock is not running\n";
  //}

  // 1.) set pwm pin mode
  

  // // 2.) map uncached memory
  // AP.map_uncached_mem (&m_uncached_memory, sizeof (LAB_OSCILLOSCOPE_DMA_DATA));

  // // 3.) set control blocks
  // AP_MemoryMap *mp = &m_uncached_memory;

  // LAB_OSCILLOSCOPE_DMA_DATA *dp = static_cast<LAB_OSCILLOSCOPE_DMA_DATA *>(m_uncached_memory.virt);
  
  // LAB_OSCILLOSCOPE_DMA_DATA dma_data = 
  // {
  //   .cbs =
  //   {
  //     // store
  //     {
  //       DMA_TI_DREQ_PWM << 16 | DMA_TI_SRC_DREQ | DMA_TI_DEST_INC | DMA_TI_WAIT_RESP,
  //       Utility::reg_bus_addr (&(AP.m_regs_st), AP_ST_CLO),
  //       Utility::mem_bus_addr (&m_uncached_memory, dp->rxd0),
  //       static_cast<uint32_t>(4 * 2000),
  //       0,
  //       Utility::mem_bus_addr (&m_uncached_memory, &(dp->cbs[0])),
  //       0
  //     },

  //     // PWM trigger
  //     {
  //       DMA_TI_DREQ_PWM << 16 | DMA_TI_DEST_DREQ | DMA_TI_WAIT_RESP,
  //       Utility::mem_bus_addr (&m_uncached_memory, &(dp->pwm_val)),
  //       Utility::reg_bus_addr (&(AP.m_regs_pwm), PWM_FIF1),
  //       4,
  //       0,
  //       Utility::mem_bus_addr (&m_uncached_memory, &(dp->cbs[1])),
  //       0
  //     },

  //   },
    
  //   .samp_size  = 4,
  //   .pwm_val    = 0x0000FFFF,
  //   .adc_csd    = SPI_CS_TA | SPI_CS_ADCS | SPI_CS_DMAEN | SPI_CS_CLEAR,
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
  // AP.dma_reset (DMA_CHAN_RX);

  // // 8.) start dma chans
  // AP.dma_start (DMA_CHAN_RX, &m_uncached_memory, &(dp->cbs[0]));
  // AP.dma_start (DMA_CHAN_PWM_PACING, &m_uncached_memory, &(dp->cbs[1]));

  // 6.) init pwm
  //AP.pwm_init (1, 200'000);

  // 7.) enable pwm dma
  //*(Utility::get_reg32 (AP.m_regs_pwm, PWM_DMAC)) = PWM_DMAC_ENAB | PWM_ENAB

  std::cout << "before stop: " << "\n";
  Utility::disp_reg32 (AP.m_regs_cm, CM_PWMCTL);


  AP.cm_pwm_clk_stop  ();

  std::cout << "after stop: " << "\n";
  Utility::disp_reg32 (AP.m_regs_cm, CM_PWMCTL);

  //AP.cm_pwm_clk_src   (AP_CM_CLK_SRC_PLLD);
  AP.cm_pwm_clk_freq  (100'000'000);
  //AP.cm_pwm_clk_mash  (AP_CM_CLK_MASH_1STAGE);
  //AP.cm_pwm_clk_run   ();

  volatile uint32_t *reg = Utility::get_reg32 (AP.m_regs_cm, CM_PWMCTL);

  *reg = CM_PASSWD | (1 << 9) | 6 | (1 << 4);

   std::cout << "after run: " << "\n";
  Utility::disp_reg32 (AP.m_regs_cm, CM_PWMCTL);

   AP.AP_gpio_set (PIN_PWM, AP_GPIO_FUNC_ALT0, AP_GPIO_PULL_DOWN);

  *(Utility::get_reg32 (AP.m_regs_pwm, PWM_RNG1)) = 100;
  *(Utility::get_reg32 (AP.m_regs_pwm, PWM_FIF1)) = 50;
  *(Utility::get_reg32 (AP.m_regs_pwm, PWM_CTL)) = (1 << 2) | (1 << 5) | 1 << 0;

  // std::cout << "clk ctl: ";
  // Utility::disp_reg32 (AP.m_regs_cm, CM_PWMCTL);

  // 9.) start pwm
   AP.pwm_start ();

  // 10.) pause

  Utility::disp_reg32 (AP.m_regs_cm, CM_PWMCTL);

  std::cout << "pause, waiting for enter...\n";
  std::cin.get ();

  // 11.) unmap
  AP.unmap_periph_mem (&m_uncached_memory);

  return 0;
}