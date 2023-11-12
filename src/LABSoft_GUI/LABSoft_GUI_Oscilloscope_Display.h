#ifndef LABSOFT_GUI_OSCILLOSCOPE_DISPLAY_H
#define LABSOFT_GUI_OSCILLOSCOPE_DISPLAY_H

#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Widget.H>

#include "LABSoft_GUI_Fl_Slider.h"

#include "../LAB/LAB_Oscilloscope.h"

class LABSoft_Controller;

class LABSoft_GUI_Oscilloscope_Display : public Fl_Group
{
  private:
    class Internal_Display : public Fl_Widget
    {
      private:
        friend class LABSoft_GUI_Oscilloscope_Display;

      private:
        // connections
        const LAB_Oscilloscope*   m_osc         = nullptr;
        const LABSoft_Controller* m_controller  = nullptr;

        // cached values for display dimensions
        double  m_row_height                = 0;
        double  m_column_width              = 0;
        double  m_x_axis_minor_ticks_width  = 0;
        double  m_y_axis_minor_ticks_width  = 0;
        double  m_display_half_height       = 0;
        double  m_display_height_midline    = 0;

        // cached values for drawing channels graphs
        unsigned  m_pixel_points_to_display         = 0;
        unsigned  m_samples_to_display              = 0;
        unsigned  m_x_coord_draw_start              = 0;
        unsigned  m_samples_start_index             = 0;
        double    m_samp_skipper                    = 0;
        double    m_x_skipper                       = 0;      
        double    m_sample_x_offset                 = 0;
        bool      m_mark_samples                    = false;
        double    m_time_per_division_delta_scaler  = 0;

        std::array<double, LABC::OSC_DISPLAY::NUMBER_OF_CHANNELS>
          m_sample_y_scaler = {0.0};

        // cached values for mouse drag horizontal offset change
        int     m_mouse_down_start_x          = 0;
        int     m_mouse_down_start_y          = 0;
        double  m_pre_drag_horizontal_offset  = 0;

        // display data
        std::array<std::vector<std::array<int, 2>>, 
          LABC::OSC_DISPLAY::NUMBER_OF_CHANNELS> m_pixel_points;
              
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

        double    calc_time_per_division_delta_scaler () const;
        unsigned  calc_samples_to_display             () const;
        unsigned  calc_pixel_points_to_display        (double time_per_division_delta_scaler) const;
        unsigned  calc_x_coord_draw_start             (double time_per_division_delta_scaler) const;
        unsigned  calc_samples_start_index            (double time_per_division_delta_scaler, unsigned samples_to_display) const;
        double    calc_samp_skipper                   (unsigned samples_to_display, unsigned pixel_points_to_display) const;
        double    calc_x_skipper                      (unsigned samples_to_display, unsigned pixel_points_to_display) const;
        bool      calc_mark_samples                   (double time_per_division_delta_scaler, unsigned samples_to_display) const;

        void      calc_cached_values_sample_y_scaler  ();
        void      calc_cached_values_sample_x_offset  ();
        void      calc_cached_values_all              ();
        void      calc_cached_values_horizontal       ();

        int       calc_sample_y_coord                 (double sample, unsigned channel) const;
        int       calc_mouse_drag_time_per_div_scaler (int drag_x) const;       

        // state
        void    reserve_pixel_points ();

        // callbacks
        void    cb_mouse_wheel  (int direction);
        void    cb_mouse_click  (int x, int y);
        void    cb_mouse_drag   (int x);

      public:
        Internal_Display (int X, int Y, int W, int H, const char* label = 0);

        // load data
        void load_oscilloscope  (const LAB_Oscilloscope& oscilloscope);
        void load_controller    (const LABSoft_Controller& controller);

        // update state
        void update_voltage_per_division  ();
        void update_horizontal_offset     ();
        void update_time_per_division     ();
        void update_samples               ();
        void update_sampling_rate         ();
    };

  private:
    // state
    unsigned m_selected_channel = 0;

    // connections
    const LAB_Oscilloscope*   m_osc         = nullptr;
    const LABSoft_Controller* m_controller  = nullptr;

    // widgets
    Internal_Display*       m_internal_display          = nullptr; 
    Fl_Box*                 m_status                    = nullptr;
    Fl_Box*                 m_top_info                  = nullptr;
    LABSoft_GUI_Fl_Slider*  m_horizontal_offset_slider  = nullptr;
    LABSoft_GUI_Fl_Slider*  m_vertical_offset_slider    = nullptr;
    LABSoft_GUI_Fl_Slider*  m_trigger_level_slider      = nullptr;

    std::array<
      Fl_Button*, 
      LABC::OSC_DISPLAY::NUMBER_OF_CHANNELS
    > m_channel_selectors;

    std::array<
      Fl_Box*, 
      LABC::OSC_DISPLAY::NUMBER_OF_COLUMNS + 1
    > m_time_per_division_labels;

    std::array<
      std::array<
        Fl_Box*, 
        LABC::OSC_DISPLAY::NUMBER_OF_ROWS + 1
      >, 
      LABC::OSC_DISPLAY::NUMBER_OF_CHANNELS
    > m_voltage_per_division_labels;

    std::array<
      Fl_Box*,
      LABC::OSC_DISPLAY::NUMBER_OF_CHANNELS
    > m_voltage_per_division_units;

  private:
    // widget functions
    void draw ();

    // init widgets
    void init_child_widgets                             ();
    void init_child_widgets_internal_display            ();
    void init_child_widgets_sliders                     ();
    void init_child_widgets_status                      ();
    void init_child_widgets_voltage_per_division_labels ();
    void init_child_widgets_voltage_per_division_units  (int X, unsigned chan);
    void init_child_widgets_time_per_division_labels    ();
    void init_child_widgets_channel_selectors           ();
    void init_child_widgets_top_info                    ();

    // callbacks
    static void cb_trigger_level_static     (Fl_Widget* w, void* data);
    static void cb_channel_selector_static  (Fl_Widget* w, void* data);

    // update gui
    void update_gui_status                  ();
    void update_gui_top_info                ();
    void update_gui_voltage_per_division    ();
    void update_gui_voltage_per_division    (unsigned channel);
    void update_gui_time_per_division       ();
    void update_gui_trigger_level_slider    ();
    void update_gui_vertical_offset_slider  ();
    void update_gui_vertical_elements       ();
    void update_gui_horizontal_elements     ();
    
    // calc
    double calc_row_voltage_per_division  (unsigned channel, unsigned row);
    double calc_col_time_per_division     (unsigned col);

    // getter
    LABSoft_Controller& controller () const;

  public:
    LABSoft_GUI_Oscilloscope_Display (int X, int Y, int W, int H, const char* label = 0);
   ~LABSoft_GUI_Oscilloscope_Display ();   

    // load data
    void load_oscilloscope  (const LAB_Oscilloscope& oscilloscope);
    void load_controller    (const LABSoft_Controller& controller);

    // update state
    void update_channel_enable_disable  ();
    void update_voltage_per_division    ();
    void update_vertical_offset         ();
    void update_horizontal_offset       ();
    void update_time_per_division       ();
    void update_samples                 ();
    void update_sampling_rate           ();
    void update_trigger_source          ();
    void update_trigger_level           ();
    void update_display                 ();
    
    // select
    void select_channel (unsigned channel);
    
    // getters
    unsigned selected_channel () const;
};

#endif