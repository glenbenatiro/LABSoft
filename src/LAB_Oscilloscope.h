#ifndef LAB_OSCILLOSCOPE_H
#define LAB_OSCILLOSCOPE_H

#include <thread>

#include "LAB_Core.h"
#include "Defaults.h"

// forward declare LAB, include LAB.h in cpp file
class LAB;

class LAB_Oscilloscope 
{
  private:
    LAB*              m_LAB;
    LAB_Core*         m_LAB_Core;
    AikaPi::Uncached  m_uncached_memory;
    AikaPi::Uncached  m_uncached_memory_record;
    std::thread       m_thread_find_trigger;
    std::thread       m_find_trigger_timer;

  private:
    // Setup
    void init_spi       ();
    void init_pwm       ();
    void init_gpio_pins ();
    void init_dma       ();
    void init_state     ();
    void config_dma_cb  ();

    // Master Controls
    void                master_run_stop                 (bool value);

    // State 
    void                update_status                   ();
    void                status                          (LABE::OSC::STATUS _STATUS);
  
    // Horizontal 
    double              calc_samp_count                 (double time_per_division, unsigned osc_disp_num_cols);
    double              calc_samp_rate                  (double time_per_div, unsigned osc_disp_num_cols);

    // Trigger 
    void                parse_trigger                   (LABE::OSC::TRIG::MODE value);
    void                find_trigger_point_loop         ();
    bool                find_trigger_point              ();
    void                create_trigger_frame            ();
    void                find_trigger_timeout_timer      ();

    // Record
    void                config_dma_cb_record            ();

  
    // Display 
    LABE::DISPLAY::MODE calc_display_mode               (double time_per_div);

    // Other
    void                set_hw_sampling_rate            (double value);
    bool                is_raw_buffer_being_written     (unsigned buff);
    void                clear_dma_interrupt_flag        (unsigned dma_chan);

    // Data and conversion
    void                fill_raw_sample_buffer          ();
    void                parse_raw_sample_buffer         ();
    constexpr double    conv_raw_buff_get_actual_value  (uint32_t sample, unsigned channel);
    constexpr uint32_t  conv_raw_buff_get_arranged_bits (uint32_t sample, unsigned channel);
    constexpr uint32_t  conv_raw_buff_xtract_chan       (uint32_t sample, unsigned channel);
    constexpr uint32_t  conv_raw_buff_arrange_bits      (uint32_t sample);
    constexpr double    conv_raw_buff_bits_actual_value (uint32_t abs_arranged_bits, bool sign);

  public:
    LAB_Parent_Data_Oscilloscope  m_parent_data;

  public:   
    LAB_Oscilloscope (LAB_Core *_LAB_Core, LAB *_LAB);
   ~LAB_Oscilloscope ();   

    // Master controls
    void                  run                     ();
    void                  stop                    ();  
    void                  osc_core_run_stop       (bool value);
    void                  osc_frontend_run_stop   (bool value);
    void                  single                  ();
    
    // Vertical
    void                  channel_enable_disable  (unsigned channel, bool value);
    void                  voltage_per_division    (unsigned channel, double value);
    double                voltage_per_division    (unsigned channel);
    void                  vertical_offset         (unsigned channel, double value);
    double                vertical_offset         (unsigned channel);
    void                  scaling                 (unsigned channel, LABE::OSC::SCALING scaling);
    double                scaling_scaler          (unsigned channel);
    void                  coupling                (unsigned channel, LABE::OSC::COUPLING coupling);

    // Horizontal
    void                  horizontal_offset       (double value);
    double                horizontal_offset       ();
    void                  time_per_division       (double value, unsigned osc_disp_num_cols = LABC::OSC_DISPLAY::NUMBER_OF_COLUMNS);
    double                time_per_division       ();
    void                  samples                 (unsigned value);
    unsigned              samples                 ();
    void                  sampling_rate           (double value, unsigned osc_disp_num_cols = LABC::OSC_DISPLAY::NUMBER_OF_COLUMNS);
    double                sampling_rate           ();
    
    // Trigger 
    void                  trigger_mode            (LABE::OSC::TRIG::MODE value);
    LABE::OSC::TRIG::MODE trigger_mode            () const;
    void                  trigger_source          (unsigned chan);
    double                trigger_source          () const;
    void                  trigger_type            (LABE::OSC::TRIG::TYPE value);
    LABE::OSC::TRIG::TYPE trigger_type            () const;
    void                  trigger_condition       (LABE::OSC::TRIG::CND value);
    LABE::OSC::TRIG::CND  trigger_condition       () const;
    void                  trigger_level           (double value);
    double                trigger_level           () const;

    // Record
    void                  record                  ();
    
    // Display 
    void                  display_mode_frontend   (LABE::DISPLAY::MODE _DISPLAY_MODE);
    void                  display_mode            (LABE::DISPLAY::MODE _DISPLAY_MODE);
    LABE::DISPLAY::MODE   display_mode            ();
       
    // State
    bool                  is_running              ();
    bool                  has_enabled_channel     ();
    void                  load_data_samples       ();
    void                  switch_dma_buffer       (LABE::DMA::BUFFER_COUNT buff_count);   
    void                  update_dma_data         (int display_mode);
    int                   update_state            ();

    // Getters
    bool                  is_osc_frontend_running ();
    bool                  is_osc_core_running     ();
};

#endif
