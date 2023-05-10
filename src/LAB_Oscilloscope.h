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
    LAB*             m_LAB;
    LAB_Core*        m_LAB_Core;
    AikaPi::Uncached m_uncached_memory;
    std::thread      m_trigger_thread;

  private:
    // Setup
    void init_spi       ();
    void init_pwm       ();
    void init_gpio_pins ();
    void init_dma       ();
    void init_state     ();
    void config_dma_cb  ();

    // Master Controls
    void master_run_stop (bool value);
  
    // Horizontal 
    double calc_samp_count  (double time_per_division, unsigned osc_disp_num_cols);
    double calc_samp_rate   (double time_per_div, unsigned osc_disp_num_cols);
    LE::DISPLAY_MODE disp_mode ();

    // Fill data
    void fill_raw_sample_buffer   ();
    void parse_raw_sample_buffer  ();

    // Trigger 
    void  parse_trigger         (LABC::OSC::TRIG::MODE value);
    void  search_trigger_point  ();
    void  create_trigger_block  ();

    // Display 
    LE::DISPLAY_MODE  calc_disp_mode  (double time_per_div);
    void              display_mode    (LE::DISPLAY_MODE _DISPLAY_MODE);

    // Other
    void      set_hw_sampling_rate        (double value);
    bool      is_raw_buffer_being_written (unsigned buff);
     
    // Conversion
    constexpr double    conv_raw_samp_buff_samp         (uint32_t sample, unsigned channel);
    constexpr uint32_t  extract_chan_raw_samp_buff_samp (uint32_t sample, unsigned channel);
    constexpr uint32_t  arrange_raw_chan_bits           (uint32_t sample);
    constexpr double    arranged_bits_to_actual_value   (uint32_t abs_arranged_bits, bool sign);

  public:
    LAB_Parent_Data_Oscilloscope  m_parent_data;

  public:   
    LAB_Oscilloscope (LAB_Core *_LAB_Core, LAB *_LAB);
   ~LAB_Oscilloscope ();   

    // Master controls
    void run                    ();
    void stop                   ();  
    void osc_core_run_stop      (bool value);
    void osc_frontend_run_stop  (bool value);
    
    // Vertical
    void    channel_enable_disable  (unsigned channel, bool value);
    void    voltage_per_division    (unsigned channel, double value);
    double  voltage_per_division    (unsigned channel);
    void    vertical_offset         (unsigned channel, double value);
    double  vertical_offset         (unsigned channel);
    void    scaling                 (unsigned channel, LE_OSC_SCALING _LE_OSC_SCALING);
    void    coupling                (unsigned channel, LE_OSC_COUPLING _LE_OSC_COUPLING);

    // Horizontal
    void    time_per_division (double value, unsigned osc_disp_num_cols = LABSOFT_OSCILLOSCOPE_DISPLAY::NUMBER_OF_COLUMNS);
    double  time_per_division ();
    void    sampling_rate     (double value, unsigned osc_disp_num_cols = LABSOFT_OSCILLOSCOPE_DISPLAY::NUMBER_OF_COLUMNS);
    double  sampling_rate     ();
    void    horizontal_offset (double value);
    double  horizontal_offset ();

    // Trigger 
    void                  trigger_mode      (LABC::OSC::TRIG::MODE value);
    LABC::OSC::TRIG::MODE trigger_mode      () const;
    void                  trigger_source    (unsigned chan);
    double                trigger_source    () const;
    void                  trigger_type      (LABC::OSC::TRIG::TYPE value);
    LABC::OSC::TRIG::TYPE trigger_type      () const;
    void                  trigger_condition (LABC::OSC::TRIG::CND value);
    LABC::OSC::TRIG::CND  trigger_condition () const;
    void                  trigger_level     (double value);
    double                trigger_level     () const;
    
    // Display 
    void              display_mode_frontend (LE::DISPLAY_MODE _DISPLAY_MODE); 
    LE::DISPLAY_MODE  display_mode          ();
       
    // State
    bool  is_running              ();
    bool  has_enabled_channel     ();
    void  load_data_samples       ();
    void  switch_dma_buffer       (LABC::DMA::BUFFER_COUNT buff_count);   
    void  update_dma_data         (int disp_mode);
    int   update_state            ();

    // Getters
    bool    is_osc_frontend_running ();
    bool    is_osc_core_running     ();
};

#endif
