#ifndef LABSOFT_OSCILLOSCOPE_DISPLAY_H
#define LABSOFT_OSCILLOSCOPE_DISPLAY_H

#include <array>
#include "time.h"

#include <FL/Fl_Widget.H>

#include "Defaults.h"
#include "Channel_Signals.h"

class LABSoft_Oscilloscope_Display : public Fl_Widget
{ 
  public:

    Channel_Signals m_channel_signals;

    clock_t deltaTime = 0;
    unsigned int frames = 0;
    double  frameRate = 30;
    double  averageFrameTimeMilliseconds = 33.333;
    double clockToMilliseconds(clock_t ticks);

    bool  m_is_enabled = false,
          m_enable_sample_sharing = false,
          m_is_function_generator_mode = false;

    int   m_grid_color                  = LABSOFT_OSCILLOSCOPE_DISPLAY_GRID_COLOR,
          m_background_color            = LABSOFT_OSCILLOSCOPE_DISPLAY_BACKGROUND_COLOR,
          m_default_color               = LABSOFT_OSCILLOSCOPE_DISPLAY_DEFAULT_COLOR,
          m_func_gen_ic_channel  = LABSOFT_OSCILLOSCOPE_DISPLAY_FUNCTION_GENERATOR_CHANNEL_NUMBER,
          m_adc_resolution_in_bits      = LAB_OSCILLOSCOPE_ADC_RESOLUTION_BITS,
          m_adc_resolution_in_integer   = LAB_OSCILLOSCOPE_ADC_RESOLUTION_INT;
          
          
     // int     m_channel_colors              = LABSOFT_OSCILLOSCOPE_DISPLAY_CHANNEL_COLORS;


    int   m_number_of_rows          = LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_ROWS,
          m_number_of_columns       = LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_COLUMNS,
          m_number_of_channels      = LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_CHANNELS,
          m_number_of_samples       = LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_SAMPLES,
          m_max_number_of_channels  = LABSOFT_OSCILLOSCOPE_DISPLAY_MAX_NUMBER_OF_CHANNELS;
    

    float m_time_per_division,
          m_voltage_per_division  = LABSOFT_OSCILLOSCOPE_DISPLAY_VOLTAGE_PER_DIVISION,
          m_max_voltage,
          m_adc_reference_voltage       = LAB_OSCILLOSCOPE_ADC_REFERENCE_VOLTAGE;
    

      

    float m_function_time_per_division,
          m_function_volts_per_division;

    DisplayMode m_display_mode = LABSOFT_OSCILLOSCOPE_DISPLAY_DISPLAY_MODE;

    // for display css-ish
    std::array<int, LABSOFT_OSCILLOSCOPE_DISPLAY_MAX_NUMBER_OF_CHANNELS> m_channels_graph_color = 
      LABSOFT_OSCILLOSCOPE_DISPLAY_CHANNELS_GRAPH_COLOR;

    // --- FUNCTION GENERATOR SECTION ---
    const double m_pi = 3.1415926535897932384626433832795028841971693993751058209;

    double  m_function_amplitude = LABSOFT_OSCILLOSCOPE_DISPLAY_FUNCTION_AMPLITUDE,
            m_function_frequency = LABSOFT_OSCILLOSCOPE_DISPLAY_FUNCTION_FREQUENCY,
            m_function_phase     = LABSOFT_OSCILLOSCOPE_DISPLAY_FUNCTION_PHASE,
            m_function_y_offset  = LABSOFT_OSCILLOSCOPE_DISPLAY_FUNCTION_Y_OFFSET;
    
    WaveType m_wave_type;
    
    
    // --- OSCILLOSCOPE SECTION ---
    // functions
          LABSoft_Oscilloscope_Display (int X, int Y, int W, int H, const char *label);
         ~LABSoft_Oscilloscope_Display ();

    
    void  enable ();
    void  disable ();

    void  draw ();
    void  draw_grid ();
    
    void update ();

    void  normalize_all_channels_raw_data ();
    void  normalize_channels_data_to_display ();
    void  load_and_process_samples (Channel_Signals *_Channel_Signals);
    void  draw_channels_signals ();
    
    // -- FUNCTION GENERATOR SECTION ---
    int  generate_waveform (WaveType wave_type, int channel);
    void enable_function_generator_mode ();

    // getter
    double volts_per_division (int channel);

    Channel_Signals* channel_signals ()
    {
      return &m_channel_signals;
    }

    // setters
    void volts_per_division (int channel, double value);
    void vertical_offset (int channel, double value);
    void time_per_division (int channel, double value);   
};

#endif

// EOF