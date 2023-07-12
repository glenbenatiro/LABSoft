#ifndef LABSOFT_GUI_OSCILLOSCOPE_DISPLAY_H
#define LABSOFT_GUI_OSCILLOSCOPE_DISPLAY_H

#include <FL/Fl_Widget.H>

#include "../Utility/Defaults.h"

class LABSoft_GUI_Oscilloscope_Display : public Fl_Widget
{ 
  private:
    LAB_Parent_Data_Oscilloscope* m_parent_data = nullptr;

  private:
    // Cached drawing values
    float     m_x_axis_minor_ticks_width  = 0;
    float     m_y_axis_minor_ticks_height = 0;
    float     m_column_width              = 0;
    float     m_row_height                = 0;
    float     m_display_half_height       = 0;
    unsigned  m_display_height_midline    = 0;

    int     m_mouse_down_start_x          = 0;
    int     m_mouse_down_start_y          = 0;
    double  m_pre_drag_horizontal_offset  = 0.0; 
  
  private:
    int handle            (int event);
    int calc_samp_y_coord (double sample, double y_scaler);
    double  calc_y_scaler (double voltage_per_division);

  public:
     // --- Functions ---
    LABSoft_GUI_Oscilloscope_Display        (int X, int Y, int W, int H, const char *label = 0);
   ~LABSoft_GUI_Oscilloscope_Display        ();
    
    // Draw
    void draw                           ();
    void draw_grid                      ();
    void draw_channels                  ();
    void draw_sample_marker             (int x, int y);

    // Data load/init
    void  calc_cached_drawing_values    ();
    void  load_osc_parent_data          (LAB_Parent_Data_Oscilloscope& parent_data);
    void  reserve_pixel_points          ();
    void  fill_pixel_points             ();
    void  fill_pixel_points_osc_running ();
    void  fill_pixel_points_osc_stopped ();

    // For horizontal offset
    double  calc_x_offset               (unsigned channel);
    void    mouse_down_start            (int x, int y);
    int     mouse_down_start_x          ();
    int     mouse_down_start_y          ();
    int     calc_time_per_div_scaler    (int drag_x);
    void    pre_drag_horizontal_offset  (double value);
    double  pre_drag_horizontal_offset  ();
};

#endif

// EOF