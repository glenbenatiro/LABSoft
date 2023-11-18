#ifndef LAB_OSCILLOSCOPE_DISPLAY_H
#define LAB_OSCILLOSCOPE_DISPLAY_H

#include "../LAB_Module.h"
#include "../../Utility/LAB_Constants.h"

class LAB;
class LAB_Oscilloscope;

class LAB_Oscilloscope_Display : public LAB_Module
{
  using PixelPoints = std::array<std::vector<std::array<int, 2>>, 
                      LABC::OSC_DISPLAY::NUMBER_OF_CHANNELS>;

  private:
    enum class DRAW_MODE
    {
      STRETCH,
      SHRINK,
      FIT
    };

  private:
    LAB_Oscilloscope& m_osc;

    // cached values for drawing channels
    unsigned    m_height                          = 0;
    unsigned    m_width                           = 0; 
    double      m_time_per_division_delta_scaler  = 0.0;
    DRAW_MODE   m_draw_mode                       = DRAW_MODE::FIT;
    double      m_draw_window_width               = 0.0;
    unsigned    m_samples_to_display              = 0;
    unsigned    m_x_coord_draw_start              = 0;
    unsigned    m_samples_start_index             = 0;
    double      m_samp_skipper                    = 0;
    double      m_x_skipper                       = 0;      
    double      m_sample_x_offset                 = 0;

    std::array<double, LABC::OSC_DISPLAY::NUMBER_OF_CHANNELS> 
      m_sample_y_scaler = {0.0};
    
    // data
    PixelPoints m_pixel_points;
  
  private:
    void      initialize_values                   ();
    void      reserve_pixel_points                (unsigned value);
    void      calc_cached_values                  ();

    double    calc_time_per_division_delta_scaler ();
    DRAW_MODE calc_draw_mode                      (double tpd_ds);
    unsigned  calc_samples_to_display             (double tpd_ds, DRAW_MODE draw_mode);  
    unsigned  calc_draw_window_width              (double tpd_ds, DRAW_MODE draw_mode);
    unsigned  calc_x_coord_draw_start             (double tpd_ds, DRAW_MODE draw_mode);
    unsigned  calc_samples_start_index            (double tpd_ds, unsigned samples_to_display);
    double    calc_samp_skipper                   (unsigned samples_to_display, unsigned draw_window_width);
    double    calc_x_skipper                      (unsigned samples_to_display, unsigned draw_window_width);

  public:
    LAB_Oscilloscope_Display (LAB& _LAB, LAB_Oscilloscope& _LAB_Oscilloscope);

    void          update_cached_values  ();    
    void          dimensions            (unsigned width, unsigned height);
    void          load_pixel_points     ();
    PixelPoints&  pixel_points          () const;
};

#endif