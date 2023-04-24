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

    AP_MemoryMap m_uncached_dma_data_logan;

    bool m_is_running = false;

    int m_curr_screen_buffer = 0;

  public:
    LAB_Parent_Data_Logic_Analyzer m_parent_data;
    
    // --- Functions ---
    LAB_Logic_Analyzer (LAB_Core *_LAB_Core, LAB *_LAB);
    ~LAB_Logic_Analyzer ();

    // Setup
    void    init_logan_gpio_pins  ();
    void    init_logan_dma        ();
    void    config_dma_cb ();  

    // Master controls
    void    run   ();
    void    stop  ();
    
    // Vertical 
    void    channel_enable_disable (unsigned channel, bool value);

    // Horizontal
    double  time_per_division (double value, unsigned disp_num_cols);
    double  horizontal_offset (double value);
    void    sampling_rate     (double value);

    // Display
    void    disp_mode (LE::DISPLAY_MODE _DISPLAY_MODE); 

    // Get data/samples
    void    load_data_samples ();
    void    parse_raw_sample_buffer ();

    //
    bool    is_running ();
    void    switch_dma_buffer (LE_SPI_DMA_NUMBER_OF_BUFFERS _LE_SPI_DMA_NUMBER_OF_BUFFERS);
};

#endif