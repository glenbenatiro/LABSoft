#ifndef LAB_LOGIC_ANALYZER_H
#define LAB_LOGIC_ANALYZER_H

#include <vector>

#include "Defaults.h"
#include "LAB_Core.h"

// Forward declare LAB, include #LAB.h in cpp file
class LAB;

class LAB_Logic_Analyzer
{
  private:
    LAB       *m_LAB;
    LAB_Core  *m_LAB_Core;

    AP_MemoryMap m_uncached_dma_data;

    bool m_is_running = false;

    int m_curr_screen_buffer = 0;

    LAB_Parent_Data_Logic_Analyzer m_parent_data;

  public:
    LAB_Logic_Analyzer (LAB_Core *_LAB_Core, LAB *_LAB);

    void    run   ();
    void    stop  ();
    void    sampling_rate (double value);
    void    load_data_samples ();
    void    config_dma_control_blocks ();   
    void    graph_disp_mode (LE_GRAPH_DISP_MODE _LE_GRAPH_DISP_MODE); 
    double  time_per_division (double value, unsigned disp_num_cols);
    void    parse_raw_sample_buffer ();
    void    switch_dma_buffer (LE_SPI_DMA_BUFFER_COUNT _LE_SPI_DMA_BUFFER_COUNT);

    // getter
    bool is_running ();

    LAB_Parent_Data_Logic_Analyzer* parent_data ();
};

#endif