#ifndef LAB_OSCILLOSCOPE_H
#define LAB_OSCILLOSCOPE_H

#include "LAB_Core.h"
#include "Defaults.h"

// forward declare LAB, include LAB.h in cpp file
class LAB;

class LAB_Oscilloscope 
{
  private:
    LAB          *m_LAB;
    LAB_Core     *m_LAB_Core;
    AP_MemoryMap  m_uncached_dma_data_osc;

  public:
    LAB_Parent_Data_Oscilloscope m_parent_data;
    int m_curr_screen_buffer = 0;  
  
    // --- Functions --- 
    LAB_Oscilloscope  (LAB_Core *_LAB_Core, LAB *_LAB);
   ~LAB_Oscilloscope  ();

    // Setup
    void init_spi       ();
    void init_pwm       ();
    void init_gpio_pins ();
    void init_dma       ();
    void init_state     ();
    void config_dma_cb  ();

    // Master controls
    void run                    ();
    void stop                   ();  
    void master_run_stop        (bool value);
    void osc_core_run_stop      (bool value);
    void osc_frontend_run_stop  (bool value);

    // Vertical
    void  channel_enable_disable  (unsigned channel, bool value);
    void  voltage_per_division    (unsigned channel, double value);
    void  vertical_offset         (unsigned channel, double value);
    void  scaling                 (unsigned channel, LE_OSC_SCALING _LE_OSC_SCALING);
    void  coupling                (unsigned channel, LE_OSC_COUPLING _LE_OSC_COUPLING);

    // Horizontal
    double  time_per_division (double value, unsigned osc_disp_num_cols);
    double  horizontal_offset (double value);
    void    sampling_rate     (double value);

    // 
    bool  is_running              ();
    void  load_data_samples       ();
    int   parse_raw_sample_buffer ();
    bool  has_enabled_channel     ();
    void  switch_dma_buffer       (int buffer);   
    void  update_dma_data         (int graph_disp_mode);
    int   update_state            ();

    // getters
    double voltage_per_division (unsigned channel)
    {
      return (m_parent_data.channel_data[channel].voltage_per_division);
    }
};

#endif
