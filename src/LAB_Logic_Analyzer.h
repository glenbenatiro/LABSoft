#ifndef LAB_LOGIC_ANALYZER_H
#define LAB_LOGIC_ANALYZER_H

#include "Defaults.h"
#include "LAB_Core.h"

// Forward declare LAB, include #LAB.h in cpp file
class LAB;

class LAB_Logic_Analyzer
{
  private:
    LAB       *m_LAB;
    LAB_Core  *m_LAB_Core;

    bool m_is_running = false;

    Channel_Data_Logic_Analyzer 
      m_channel_data_logic_analyzer[LAB_LOGIC_ANALYZER_NUMBER_OF_CHANNELS];
    
    AP_MemoryMap m_uncached_dma_data;

  public:
    const uint32_t m_dma_one = 0x1;
    
    LAB_Logic_Analyzer (LAB_Core *_LAB_Core, LAB *_LAB);

    void  run   ();
    void  stop  ();

    void  sampling_rate (double value);
    void  load_data_samples ();
    void  config_dma_control_blocks ();    

    // getter
    bool is_running ()
    {
      return m_is_running;
    }
};

#endif