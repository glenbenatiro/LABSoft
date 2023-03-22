#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>
#include <bitset>
#include <unistd.h>

#include "../../lib/AikaPi/AikaPi.h"

// compile with 
// g++ 7-logic-analyzer-dma.cpp ../../lib/AikaPi/AikaPi.cpp -o 7-logic-analyzer-dma

constexpr double    LAB_PWM_FREQUENCY = 200'000;
constexpr double    LAB_SPI_FREQUENCY = 15'000'000;
constexpr unsigned  LAB_PIN_PWM = 12;
constexpr unsigned  LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES = 2000;

// post reboot, dma channels in use are only 4 and 6
constexpr unsigned  LAB_DMA_CHAN_PWM_PACING                 = 7;
constexpr unsigned  LAB_DMA_CHAN_OSCILLOSCOPE_SPI_RX        = 8;
constexpr unsigned  LAB_DMA_CHAN_OSCILLOSCOPE_SPI_TX        = 9;
constexpr unsigned  LAB_DMA_CHAN_LOGIC_ANALYZER_GPIO_STORE  = 10;

constexpr unsigned  LAB_OSCILLOSCOPE_DMA_CHAN_XTRA          = 11;
constexpr int       LAB_OSCILLOSCOPE_ADC_CE                 = 0; // CE0 or CE1

struct LAB_DMA_Data_Oscilloscope
{
  AP_DMA_CB cbs[15];

  uint32_t  samp_size,
            pwm_val, 
            adc_csd,
            adc_csd_fifo_reset = 0x00000030,
            //txd;
            txd[LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES + 1];

  volatile uint32_t usecs[2],
                    status[2],
                    rxd0[LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES],
                    rxd1[LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES];
};

struct LAB_DMA_Data_PWM_Pacing 
{ 
  AP_DMA_CB cbs[15];

  uint32_t pwm_val;
};

constexpr unsigned VC_MEM_SIZE = 0x1000 + (2 * 8000 * 4);

int main ()
{
  AikaPi AP; 

  AP_MemoryMap m_uncached_dma_data_osc;

  AP.map_uncached_mem (&m_uncached_dma_data_osc, VC_MEM_SIZE * 2);

  // ----------

  // Init PWM
  AP.pwm_reset    ();
  AP.pwm_init     (LAB_PWM_FREQUENCY);
  AP.pwm_algo     (0, AP_PWM_ALGO_MARKSPACE);
  AP.pwm_use_fifo (0, 1);
  *(Utility::get_reg32 (AP.m_regs_pwm, PWM_DMAC)) = (1 << 31) | (8 << 8) | (1 << 0);
  AP.gpio_set     (LAB_PIN_PWM, AP_GPIO_FUNC_ALT0, AP_GPIO_PULL_DOWN);

  // Init SPI
  AP.spi_init (LAB_SPI_FREQUENCY);
  *(Utility::get_reg32 (AP.m_regs_spi, SPI_DC)) = (8<<24) | (1<<16) | (8<<8) | 1;

  // --- Prepare osc control blocks ---

  // Prepare control blocks
  AP_MemoryMap *mp = &m_uncached_dma_data_osc;

  LAB_DMA_Data_Oscilloscope *dp = static_cast<LAB_DMA_Data_Oscilloscope *>
    (m_uncached_dma_data_osc.virt);

  LAB_DMA_Data_Oscilloscope osc_dma_data = 
  {
    .cbs = 
    {
      // For double buffer SPI RX
      { // CB 0
        DMA_CB_TI_SPI_RX,
        Utility::reg_bus_addr (&(AP.m_regs_spi), SPI_FIFO),
        Utility::mem_bus_addr (mp, dp->rxd0),
        (uint32_t)(4 * LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES),
        0,
        Utility::mem_bus_addr (mp, &dp->cbs[1]),
        0
      },
      { // CB 1
        DMA_CB_TI_SPI_RX, 
        Utility::reg_bus_addr (&(AP.m_regs_spi), SPI_CS),
        Utility::mem_bus_addr (mp, &dp->status[0]),
        4,
        0,
        Utility::mem_bus_addr (mp, &dp->cbs[2]),
        0
      }, 
      { // CB 2
        DMA_CB_TI_SPI_RX,
        Utility::reg_bus_addr (&(AP.m_regs_spi), SPI_FIFO),
        Utility::mem_bus_addr (mp, dp->rxd1),
        (uint32_t)(4 * LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES),
        0,
        Utility::mem_bus_addr (mp, &dp->cbs[3]),
        0
      }, 
      { // CB 3
        DMA_CB_TI_SPI_RX, 
        Utility::reg_bus_addr (&(AP.m_regs_spi), SPI_CS),
        Utility::mem_bus_addr (mp, &dp->status[1]),
        4,
        0,
        Utility::mem_bus_addr (mp, &dp->cbs[0]),
        0
      }, 



      // For single buffer SPI RX
      { // CB 4
        DMA_CB_TI_SPI_RX,
        Utility::reg_bus_addr (&(AP.m_regs_spi), SPI_FIFO),
        Utility::mem_bus_addr (mp, dp->rxd0),
        (uint32_t)(4 * LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES),
        0,
        Utility::mem_bus_addr (mp, &dp->cbs[5]),
        0
      },  
      { // CB 5
        DMA_CB_TI_SPI_RX, 
        Utility::reg_bus_addr (&(AP.m_regs_spi), SPI_CS),
        Utility::mem_bus_addr (mp, &dp->status[0]),
        4,
        0,
        Utility::mem_bus_addr (mp, &dp->cbs[4]),
        0
      },  



      // For SPI TX
      { // CB 6
        DMA_CB_TI_SPI_TX,  
        Utility::mem_bus_addr (mp, dp->txd),
        Utility::reg_bus_addr (&(AP.m_regs_spi), SPI_FIFO),
        (uint32_t)((4 * LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES) + 4), 
        0, 
        Utility::mem_bus_addr (mp, &dp->cbs[6]), 
        0
      }, 



      // For PWM pacing
      { //CB 7
        DMA_CB_TI_PWM, 
        Utility::mem_bus_addr (mp, &dp->pwm_val),   
        Utility::reg_bus_addr (&(AP.m_regs_pwm), PWM_FIF1), 
        4, 
        0, 
        Utility::mem_bus_addr (mp, &dp->cbs[8]), 
        0
      }, 
      { // CB 8
        DMA_CB_TI_PWM, 
        Utility::mem_bus_addr (mp, &dp->adc_csd_fifo_reset), 
        Utility::reg_bus_addr (&(AP.m_regs_spi), SPI_CS), 
        4, 
        0, 
        Utility::mem_bus_addr (mp, &dp->cbs[9]), 
        0
      },
      { // CB 9
        DMA_CB_TI_PWM, 
        Utility::mem_bus_addr (mp, &dp->samp_size), 
        Utility::reg_bus_addr (&(AP.m_regs_spi), SPI_DLEN), 
        4, 
        0, 
        Utility::mem_bus_addr (mp, &dp->cbs[10]), 
        0
      },
      { // CB 10
        DMA_CB_TI_PWM, 
        Utility::mem_bus_addr (mp, &dp->adc_csd),   
        Utility::reg_bus_addr (&(AP.m_regs_spi), SPI_CS),   
        4, 
        0, 
        Utility::mem_bus_addr (mp, &dp->cbs[7]), 
        0
      },
    },

    .samp_size = 4, // in number of bytes
    .pwm_val   = 250, 
    .adc_csd   = SPI_CS_TA | SPI_CS_ADCS | SPI_CS_DMAEN | LAB_OSCILLOSCOPE_ADC_CE,
    .usecs     = {0, 0},  
    .status    = {0, 0}, 
    .rxd0      = {0}, 
    .rxd1      = {0},
  };

  std::fill_n(osc_dma_data.txd, LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES + 1, 0x0000ffff);
  osc_dma_data.txd[0] = 0x7D00080;

  // Copy control blocks to uncached mem
  std::memcpy (dp, &osc_dma_data, sizeof (osc_dma_data));

  // Init and start DMA 
  AP.dma_reset (LAB_DMA_CHAN_PWM_PACING);
  AP.dma_reset (LAB_DMA_CHAN_OSCILLOSCOPE_SPI_TX);
  AP.dma_reset (LAB_DMA_CHAN_OSCILLOSCOPE_SPI_RX);

  AP_MemoryMap              *ud = &(m_uncached_dma_data_osc);
  LAB_DMA_Data_Oscilloscope *dd = static_cast<LAB_DMA_Data_Oscilloscope *>(ud->virt);

  AP.dma_start (ud, LAB_DMA_CHAN_OSCILLOSCOPE_SPI_TX,       &(dd->cbs[6]), 0);  // Start SPI Tx DMA
  AP.dma_start (ud, LAB_DMA_CHAN_OSCILLOSCOPE_SPI_RX,       &(dd->cbs[0]), 0);  // Start SPI Rx DMA
  AP.dma_start (ud, LAB_DMA_CHAN_PWM_PACING, &(dd->cbs[7]), 0);  // Start SPI Rx DMALAB_OSCILLOSCOPE_DMA_CHAN_XTRA

  // AP_MemoryMap            *ud_pwm = &(m_uncached_dma_data_pwm);
  // LAB_DMA_Data_PWM_Pacing *dd_pwm = static_cast<LAB_DMA_Data_PWM_Pacing *>(ud_pwm->virt);

  // AP.dma_start (ud_pwm, LAB_DMA_CHAN_PWM_PACING, &(dd_pwm->cbs[0]), 0);  // Start SPI Rx DMA

  // Start PWM
  AP.pwm_start  (0);
  //AP.pwm_stop   (0);

  while (1)
  {
    double      freq, dc;
    std::string user_input;

    std::cout << "\n";
    std::cout << "Freq      : ";
    std::cin  >> freq;
    std::cout << "Duty Cycle: ";
    std::cin  >> dc;

    double range    = AP.pwm_frequency (0, freq);
    double pwm_val  = range * (dc / 100.0);

    (static_cast<LAB_DMA_Data_Oscilloscope *>(m_uncached_dma_data_osc.virt))->
      pwm_val = pwm_val;

    // Ignore to the end of line
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "type exit to exit: ";
    std::getline (std::cin, user_input);

    if (user_input == "exit")
    {
      // Free mapped memory upon exit
      AP.pwm_stop (0);

      AP.dma_pause (LAB_DMA_CHAN_OSCILLOSCOPE_SPI_TX);
      AP.dma_pause (LAB_DMA_CHAN_OSCILLOSCOPE_SPI_RX);
      AP.dma_pause (LAB_DMA_CHAN_PWM_PACING);

      AP.dma_reset (LAB_DMA_CHAN_OSCILLOSCOPE_SPI_TX);
      AP.dma_reset (LAB_DMA_CHAN_OSCILLOSCOPE_SPI_RX);
      AP.dma_reset (LAB_DMA_CHAN_PWM_PACING);

      //AP.unmap_periph_mem (&m_uncached_dma_data_osc);

      return 0;
    }
  }

  return 0;
}