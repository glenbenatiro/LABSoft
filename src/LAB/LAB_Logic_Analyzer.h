#ifndef LAB_LOGIC_ANALYZER_H
#define LAB_LOGIC_ANALYZER_H

#include <thread>

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
    std::thread       m_thread_find_trigger;

    bool m_is_running = false;

    int m_curr_screen_buffer = 0;

    LAB_Parent_Data_Logic_Analyzer m_parent_data;

  private:
    void init_gpio_pins ();
    void init_dma       ();
    void config_dma_cb  ();  

    void fill_raw_sample_buffer_from_dma_buffer ();
    void parse_raw_sample_buffer                ();

    // Horizontal
    void set_samples (unsigned value);

    // Trigger 
    void set_trigger_condition (unsigned gpio_pin, LABE::LOGAN::TRIG::CND condition);
    void parse_trigger_mode       ();
    void find_trigger_point_loop  ();

  public:
    LAB_Logic_Analyzer (LAB_Core* _LAB_Core, LAB* _LAB);
   ~LAB_Logic_Analyzer ();

    void run  ();
    void stop  ();
    
    // State
    bool is_running () const;

    // Data 
    LAB_Parent_Data_Logic_Analyzer& parent_data ();

    // Mode
    void              mode (LABE::LOGAN::MODE mode);
    LABE::LOGAN::MODE mode ();
  
    // Horizontal
    void                  horizontal_offset       (double value);
    double                horizontal_offset       () const;
    void                  time_per_division       (double value);
    double                time_per_division       () const;
    void                  samples                 (unsigned value);
    unsigned              samples                 () const;
    void                  sampling_rate           (double value);
    double                sampling_rate           () const;

    // Get data/samples
    void                load_data_samples       ();
    double              calc_samp_count         (double time_per_div, unsigned osc_disp_num_cols);
    double              calc_samp_rate          (double time_per_div, unsigned osc_disp_num_cols);
    LABE::LOGAN::MODE   calc_mode               (double time_per_division);

    //
    void    dma_buffer_count (LABE::LOGAN::BUFFER_COUNT buffer_count);

    // Trigger  
    void trigger_mode       (LABE::LOGAN::TRIG::MODE value);
    void trigger_condition  (unsigned channel, LABE::LOGAN::TRIG::CND condition);
};

#endif