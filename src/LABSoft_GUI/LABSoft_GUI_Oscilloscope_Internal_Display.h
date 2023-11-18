#ifndef LABSOFT_GUI_OSCILLOSCOPE_INTERNAL_DISPLAY_H
#define LABSOFT_GUI_OSCILLOSCOPE_INTERNAL_DISPLAY_H

#include <FL/Fl_Widget.H>
#include <FL/Enumerations.H>

#include "../Utility/LAB_Constants.h"

class LABSoft_GUI_Oscilloscope_Internal_Display : public Fl_Widget
{
  private:
    Fl_Color m_grid_color = LABC::OSC_DISPLAY::GRID_COLOR;

    // cached values for display dimensions
    double  m_row_height                = 0;
    double  m_column_width              = 0;
    double  m_x_axis_minor_ticks_width  = 0;
    double  m_y_axis_minor_ticks_width  = 0;
    double  m_display_half_height       = 0;
    double  m_display_height_midline    = 0;

    // connections
    const LABSoft_Controller* m_presenter = nullptr;
    
    // cached values for mouse drag horizontal offset change
    int     m_mouse_down_start_x          = 0;
    int     m_mouse_down_start_y          = 0;
          
  private:
    // widget functions
    void    draw   ();
    int     handle (int event);
    void    resize (int x, int y, int w, int h);

    // draw functions
    void    draw_grid          ();
    void    draw_channels      ();
    void    draw_sample_marker (int x, int y);

    // calc
    void      calc_pixel_points                   ();        
    void      calc_cached_values_display          ();
    void      calc_cached_values_drawing          ();


    bool      calc_mark_samples                   (double time_per_division_delta_scaler, unsigned samples_to_display) const;

    void      calc_cached_values_sample_y_scaler  ();
    void      calc_cached_values_sample_x_offset  ();
    void      calc_cached_values_all              ();
    void      calc_cached_values_horizontal       ();

    int       calc_sample_y_coord                 (double sample, unsigned channel) const;
    int       calc_mouse_drag_time_per_div_scaler (int drag_x) const;       

    // callbacks
    void    cb_mouse_click  (int x, int y);
    void    cb_mouse_drag   (int x);

  public:
    LABSoft_GUI_Oscilloscope_Internal_Display (int X, int Y, int W, int H, const char* label = 0);

    void load_presenter (const LABSoft_Controller& controller);

    // update state
    void update_voltage_per_division  ();
    void update_horizontal_offset     ();
    void update_time_per_division     ();
    void update_samples               ();
    void update_sampling_rate         ();

    double row_height () const;
    double column_width () const;
};

#endif