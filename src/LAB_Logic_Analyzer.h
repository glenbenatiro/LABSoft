#ifndef LAB_LOGIC_ANALYZER_H
#define LAB_LOGIC_ANALYZER_H

#include "Defaults.h"
#include "LAB_Core.h"

// Forward declare LAB, include #LAB.h in cpp file
class LAB;

class LAB_Logic_Analyzer
{
  private:
    LAB*              m_LAB;
    LAB_Core*         m_LAB_Core;
    AikaPi::Uncached  m_uncached_memory;

    bool m_is_running = false;

    int m_curr_screen_buffer = 0;

  private:
    void init_gpio_pins ();
    void init_dma       ();
    void config_dma_cb  ();  

  public:
    LAB_Parent_Data_Logic_Analyzer m_parent_data;

  public:
    LAB_Logic_Analyzer (LAB_Core* _LAB_Core, LAB* _LAB);
   ~LAB_Logic_Analyzer ();

    void run  ();
    void stop  ();
    


    // Horizontal
    double  time_per_division (double value, unsigned disp_num_cols);
    double  horizontal_offset (double value);
    void    sampling_rate     (double value);

    // Display
    void              display_mode (LE::DISPLAY_MODE _DISPLAY_MODE); 
    LE::DISPLAY_MODE  display_mode ();

    // Get data/samples
    void              load_data_samples       ();
    void              fill_raw_sample_buffer  ();
    void              parse_raw_sample_buffer ();
    double            calc_samp_count         (double time_per_div, unsigned osc_disp_num_cols);
    double            calc_samp_rate          (double time_per_div, unsigned osc_disp_num_cols);
    LE::DISPLAY_MODE  calc_disp_mode          (double time_per_div);

    //
    bool    is_running ();
    void    switch_dma_buffer (LABC::DMA::BUFFER_COUNT buff_count);
};

#endif