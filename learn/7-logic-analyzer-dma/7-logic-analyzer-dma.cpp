#include <cstring>
#include <iostream>

#include "../../lib/AikaPi/AikaPi.h"

// compile with 
// g++ 7-logic-analyzer-dma.cpp ../../lib/AikaPi/AikaPi.cpp -o 7-logic-analyzer-dma

constexpr unsigned PIN_PWM = 12;
constexpr unsigned LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES = 2000;
constexpr unsigned DMA_CHAN_RX = 8;
constexpr unsigned DMA_CHAN_PWM_PACING = 7;

constexpr unsigned  LAB_DMA_CHAN_PWM_PACING    = 7;
constexpr unsigned  LAB_DMA_CHAN_OSCILLOSCOPE_SPI_RX        = 8;
constexpr unsigned  LAB_DMA_CHAN_OSCILLOSCOPE_SPI_TX        = 9;
constexpr unsigned  LAB_OSCILLOSCOPE_DMA_CHAN_XTRA          = 11;
constexpr unsigned  LAB_DMA_CHAN_LOGIC_ANALYZER_GPIO_STORE  = 10;

constexpr int     LAB_OSCILLOSCOPE_ADC_CE                 = 0; // CE0 or CE1

struct LAB_OSCILLOSCOPE_DMA_DATA
{
  AP_DMA_CB cbs[15];

  uint32_t  samp_size,
            pwm_val, 
            adc_csd,
            txd[2];

  volatile uint32_t usecs[2],
                    states[2],
                    rxd0[LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES],
                    rxd1[LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES];
};

struct AP_DMA_PWM_PACING_DATA_STRUCT
{
  AP_DMA_CB _AP_DMA_CB[10];

  uint32_t pwm_data;
};

constexpr unsigned VC_MEM_SIZE = 0x1000 + (2 * 8000 * 4);

int main ()
{
  AikaPi AP;
  AP_MemoryMap m_uncached_memory;

  // Map uncached memory
  AP.map_uncached_mem (& m_uncached_memory, VC_MEM_SIZE);

  // Set GPIO pin of PWM
  AP.gpio_set (PIN_PWM, AP_GPIO_FUNC_ALT0, AP_GPIO_PULL_DOWN);

  // Init PWM settings
  AP.pwm_init     (200'000);
  AP.pwm_algo     (0, AP_PWM_ALGO_MARKSPACE);
  AP.pwm_use_fifo (0, 1);

  //*(Utility::get_reg32 (AP.m_regs_pwm, PWM_CTL)) |= (1 << 2);

  // Enable PWM DMA and set DREQ threshold
  *(Utility::get_reg32 (AP.m_regs_pwm, PWM_DMAC)) = (1 << 31) | (8 << 8) | (1 << 0);

  // Initialize SPI 
  AP.spi_init (10'000'000.0);

  *(Utility::get_reg32 (AP.m_regs_spi, SPI_DC)) = 
    (8<<24) | (1<<16) | (8<<8) | 1;

  // Setup PWM control blocks
  AP_MemoryMap *mp = &m_uncached_memory;

  LAB_OSCILLOSCOPE_DMA_DATA *dp = static_cast<LAB_OSCILLOSCOPE_DMA_DATA *>
    (m_uncached_memory.virt);
  
  LAB_OSCILLOSCOPE_DMA_DATA adc_dma_data = 
  {
    .cbs = 
    {
      // control blocks for SPI_RX dual buffer
      { // CB 0
        DMA_CB_TI_SPI_RX,
        Utility::reg_bus_addr (&(AP.m_regs_spi), SPI_FIFO),  
        Utility::mem_bus_addr (mp, dp->rxd0),       
        (uint32_t)(2000*4),  
        0, 
        Utility::mem_bus_addr (mp, &dp->cbs[1]),
        0
      },
      { // CB 1
        DMA_CB_TI_SPI_RX, 
        Utility::reg_bus_addr (&(AP.m_regs_spi),  SPI_CS),
        Utility::mem_bus_addr (mp, &dp->states[0]),
        4,
        0,
        Utility::mem_bus_addr (mp, &dp->cbs[2]),
        0
      }, 
      { // CB 2
        DMA_CB_TI_SPI_RX, 
        Utility::reg_bus_addr (&(AP.m_regs_spi),  SPI_FIFO),
        Utility::mem_bus_addr (mp, dp->rxd1),       
        (uint32_t)(2000*4),  
        0, 
        Utility::mem_bus_addr (mp, &dp->cbs[3]), 
        0
      }, 
      { // CB 3
        DMA_CB_TI_SPI_RX, 
        Utility::reg_bus_addr (&(AP.m_regs_spi),  SPI_CS),    
        Utility::mem_bus_addr (mp, &dp->states[1]), 
        4,                                              
        0, 
        Utility::mem_bus_addr (mp, &dp->cbs[0]), 
        0
      }, 


      // control blocks for SPI_RX single buffer
      { // CB 4
        DMA_CB_TI_SPI_RX, 
        Utility::reg_bus_addr (&(AP.m_regs_spi),  SPI_FIFO),
        Utility::mem_bus_addr (mp, dp->rxd0),       
        (uint32_t)(2000*4),  
        0, 
        Utility::mem_bus_addr (mp, &dp->cbs[5]), 
        0
      },  
      { // CB 5
        DMA_CB_TI_SPI_RX, 
        Utility::reg_bus_addr (&(AP.m_regs_spi),  SPI_CS),    
        Utility::mem_bus_addr (mp, &dp->states[1]), 
        4,                                              
        0, 
        Utility::mem_bus_addr (mp, &dp->cbs[4]), 
        0
      },  


      // // Control Blocks for SPI TX
      // { // CB 6
      //   DMA_CB_TI_SPI_TX,  
      //   MEM(mp, dp->txd),        
      //   REG(AP.m_regs_spi, SPI_FIFO), 
      //   8, 
      //   0, 
      //   Utility::mem_bus_addr (mp, &dp->cbs[6]), 
      //   0
      // }, 

      // Control Blocks for SPI TX
      { // CB 6
        DMA_CB_TI_SPI_TX,  
        MEM(mp, dp->txd),        
        REG(AP.m_regs_spi, SPI_FIFO), 
        8, 
        0, 
        Utility::mem_bus_addr (mp, &dp->cbs[6]), 
        0
      }, 


      // PWM ADC trigger: wait for PWM, set sample length, trigger SPI
      { //CB 7
        DMA_CB_TI_PWM, 
        MEM(mp, &dp->pwm_val),   
        REG(AP.m_regs_pwm, PWM_FIF1), 
        4, 
        0, 
        Utility::mem_bus_addr (mp, &dp->cbs[8]), 
        0
      }, 
      // PWM ADC trigger: wait for PWM, set sample length, trigger SPI
      { //CB 8
        DMA_CB_TI_PWM, 
        MEM(mp, &dp->pwm_val),   
        REG(AP.m_regs_pwm, PWM_FIF1), 
        4, 
        0, 
        Utility::mem_bus_addr (mp, &dp->cbs[7]), 
        0
      }, 

      { // CB 9
        DMA_CB_TI_PWM, 
        MEM(mp, &dp->samp_size), 
        REG(AP.m_regs_spi, SPI_DLEN), 
        4, 
        0, 
        Utility::mem_bus_addr (mp, &dp->cbs[10]), 
        0
      },
      { // CB 10
        DMA_CB_TI_PWM, 
        MEM(mp, &dp->adc_csd),   
        REG(AP.m_regs_spi, SPI_CS),   
        4, 
        0, 
        Utility::mem_bus_addr (mp, &dp->cbs[9]), 
        0
      },
    },

    .samp_size = 4, // in number of bytes
    .pwm_val   = 250, 
    .adc_csd   = SPI_CS_TA | SPI_CS_ADCS | SPI_CS_DMAEN | SPI_CS_CLEAR | LAB_OSCILLOSCOPE_ADC_CE,
    // .txd       = {0xd0, static_cast<uint32_t>(m_number_of_channels > 1 ? 0xf0 : 0xd0)},
    .txd       = {0xffff, 0x0000},
    .usecs     = {0, 0},  
    .states    = {0, 0}, 
    .rxd0      = {0}, 
    .rxd1      = {0},
  };
  
  // Copy control blocks to uncached mem
  std::memcpy (dp, &adc_dma_data, sizeof (adc_dma_data));

  // Reset DMA chan
  AP.dma_reset (DMA_CHAN_PWM_PACING);
  AP.dma_reset (LAB_DMA_CHAN_OSCILLOSCOPE_SPI_TX);
  AP.dma_reset (LAB_DMA_CHAN_OSCILLOSCOPE_SPI_TX);
  AP.dma_reset (LAB_OSCILLOSCOPE_DMA_CHAN_XTRA);

  // Start DMA chan
  AP_MemoryMap              *ud = &(m_uncached_memory);
  LAB_OSCILLOSCOPE_DMA_DATA *dd = static_cast<LAB_OSCILLOSCOPE_DMA_DATA *>(ud->virt);

  AP.dma_start (ud, LAB_DMA_CHAN_OSCILLOSCOPE_SPI_TX,     &(dd->cbs[6]), 0);  // Start SPI Tx DMA
  AP.dma_start (ud, LAB_DMA_CHAN_OSCILLOSCOPE_SPI_RX,     &(dd->cbs[0]), 0);  // Start SPI Rx DMA
  AP.dma_start (ud, LAB_OSCILLOSCOPE_DMA_CHAN_XTRA,       &(dd->cbs[9]), 0);  // Start SPI Rx DMALAB_OSCILLOSCOPE_DMA_CHAN_XTRA
  AP.dma_start (ud, DMA_CHAN_PWM_PACING,                  &(dd->cbs[7]), 0);
  // Start PWM
  AP.pwm_start (0);

  // pause
  double freq, dc;

  while (1)
  {
    std::cout << "\n\nFreq: ";
    std::cin >> freq;
    std::cout << "Duty Cycle: ";
    std::cin >> dc;

    double range = AP.pwm_frequency (0, freq);
    double pwm_val = range * (dc / 100.0);

    (static_cast<LAB_OSCILLOSCOPE_DMA_DATA *>(m_uncached_memory.virt))->
      pwm_val = pwm_val;

    *(Utility::get_reg32 (AP.m_regs_pwm, PWM_DAT1)) = pwm_val;

    std::cout << "pwm val: " << (static_cast<LAB_OSCILLOSCOPE_DMA_DATA *>
      (m_uncached_memory.virt))->pwm_val << "\n";
  }

  // Free mapped memory upon exit
  AP.unmap_periph_mem (&m_uncached_memory);

  return 0;
}