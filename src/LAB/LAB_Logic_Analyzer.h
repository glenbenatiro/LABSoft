#ifndef LAB_LOGIC_ANALYZER_H
#define LAB_LOGIC_ANALYZER_H

#include "LAB_Core.h"
#include "../Utility/LAB_Enumerations.h"
#include "../Utility/LAB_Definitions.h"

// forward declare LAB, #include in .cpp file
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

    // Mode
    void                        mode (LABE::LOGAN::MODE mode);
    LABE::LOGAN::MODE  mode ();
  
    // Horizontal
    void    horizontal_offset (double value);
    double  horizontal_offset () const;
    void    time_per_division (double value, unsigned disp_num_cols);
    double  time_per_division () const;    
    void    sampling_rate     (double value);

    // Get data/samples
    void                load_data_samples       ();
    void                fill_raw_sample_buffer  ();
    void                parse_raw_sample_buffer ();
    double              calc_samp_count         (double time_per_div, unsigned osc_disp_num_cols);
    double              calc_samp_rate          (double time_per_div, unsigned osc_disp_num_cols);
    LABE::LOGAN::MODE   calc_mode               (double time_per_division);

    //
    bool    is_running ();
    void    dma_buffer_count (LABE::LOGAN::BUFFER_COUNT buffer_count);
};

#endif