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
        // widget functions
        void    draw   ();
        int     handle (int event);
        void    resize (int x, int y, int w, int h);

        // draw functions
        void    draw_grid          ();
        void    draw_channels      ();
        void    draw_sample_marker (int x, int y);

        // calc
        void    calc_pixel_points                   ();
        void    calc_pixel_points_osc_running       ();
        void    calc_pixel_points_osc_stopped       ();
        void    calc_skippers                       ();
        void    calc_cached_display_values          ();
        void    calc_sample_y_scaler                ();
        int     calc_sample_y_coord                 (double sample, unsigned channel);
        void    calc_sample_x_offset                ();
        int     calc_mouse_drag_time_per_div_scaler (int drag_x);

        // state
        void    reserve_pixel_points ();

        // callbacks
        void    cb_mouse_wheel  (int direction);
        void    cb_mouse_click  (int x, int y);
        void    cb_mouse_drag   ();

      public:
        // connections
        const LAB_Oscilloscope*   m_osc         = nullptr;
        const LABSoft_Controller* m_controller  = nullptr;

        // cached values
        float   m_row_height                = 0;
        float   m_column_width              = 0;
        float   m_x_axis_minor_ticks_width  = 0;
        float   m_y_axis_minor_ticks_width  = 0;
        float   m_display_half_height       = 0;
        int     m_display_height_midline    = 0;
        double  m_sample_x_offset           = 0;
        double  m_samp_skipper              = 0;
        double  m_x_skipper                 = 0;

        std::array<double, LABC::OSC_DISPLAY::NUMBER_OF_CHANNELS>
          m_sample_y_scaler = {0.0};

        // display data
        unsigned  m_pixel_points_to_display   = 1;
        bool      m_mark_samples              = false;

        std::array<std::vector<std::array<int, 2>>, 
          LABC::OSC_DISPLAY::NUMBER_OF_CHANNELS> m_pixel_points;

        // mouse drag values, for horizontal offset change
        int     m_mouse_down_start_x          = 0;
        int     m_mouse_down_start_y          = 0;
        double  m_pre_drag_horizontal_offset  = 0;

      public:
        Internal_Display (int X, int Y, int W, int H, const char* label = 0);

        // load data
        void load_oscilloscope  (const LAB_Oscilloscope& oscilloscope);
        void load_controller    (const LABSoft_Controller& controller);

        // update state
        void update_voltage_per_division  ();
        void update_horizontal_offset     ();
        void update_samples               ();
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

    // others
    std::string get_now_timestamp ();

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
    void update_trigger_source          ();
    void update_trigger_level           ();
    void update_display                 ();

    // select
    void select_channel (unsigned channel);
    
    // getters
    unsigned selected_channel () const;
};

#endif