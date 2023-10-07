#ifndef LABSOFT_GUI_OSCILLOSCOPE_DISPLAY_H
#define LABSOFT_GUI_OSCILLOSCOPE_DISPLAY_H

#include <array>
#include <vector>

#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Slider.H>

#include "LABSoft_GUI_Fl_Slider.h"
#include "../Utility/LAB_Constants.h"
#include "../Utility/LAB_Definitions.h"

class LABSoft_Controller;

// ========== LABSoft_GUI_Oscilloscope_Display_Internal ==========

class LABSoft_GUI_Oscilloscope_Display_Internal : public Fl_Widget
{
  private:
    std::array<std::vector<std::array<int, 2>>, LABC::OSC::NUMBER_OF_CHANNELS>
      m_pixel_points;

    const LAB_Parent_Data_Oscilloscope* m_parent_data = nullptr;

    // cached drawing values
    float   m_row_height                  = 0;
    float   m_column_width                = 0;
    float   m_x_axis_minor_ticks_width    = 0;
    float   m_y_axis_minor_ticks_height   = 0;
    float   m_display_half_height         = 0;
    float   m_display_height_midline      = 0;

    // cached dragging values (for horizontal offset change)
    int     m_mouse_down_start_x          = 0;
    int     m_mouse_down_start_y          = 0;
    double  m_pre_drag_horizontal_offset  = 0.0;

    //
    uint32_t m_grid_color                 = LABC::OSC_DISPLAY::GRID_COLOR;
    uint32_t m_background_color           = LABC::OSC_DISPLAY::BACKGROUND_COLOR;

  private:
    int     handle                              (int event);
    void    draw                                ();
    void    draw_grid                           ();
    void    draw_channels                       ();
    void    draw_sample_marker                  (int x, int y);
    void    calc_cached_drawing_values          ();
    int     calc_samp_y_coord                   (double sample, double y_scaler);
    double  calc_y_scaler                       (double voltage_per_division);
    double  calc_x_offset                       (unsigned channel);
    void    fill_pixel_points_backend_running   ();
    void    fill_pixel_points_backend_stopped   ();
    void    reserve_pixel_points                ();
    int     calc_mouse_drag_time_per_div_scaler (int drag_x);

  public:
    LABSoft_GUI_Oscilloscope_Display_Internal (int X              = 80,
                                               int Y              = 20,
                                               int W              = 600,
                                               int H              = 470,
                                               const char* label  = 0);
   ~LABSoft_GUI_Oscilloscope_Display_Internal ();

   void load_parent_data (const LAB_Parent_Data_Oscilloscope& parent_data);
   void fill_pixel_points ();
};

// ========== LABSoft_GUI_Oscilloscope_Display ==========

class LABSoft_GUI_Oscilloscope_Display : public Fl_Group 
{
  private:
    const LAB_Parent_Data_Oscilloscope* m_parent_data         = nullptr;
          LABSoft_Controller*           m_LABSoft_Controller  = nullptr;
          unsigned                      m_selected_channel    = 0;

  public:
    // child widgets
    Fl_Box*                 m_status                    = nullptr;
    Fl_Box*                 m_top_info                  = nullptr;
    LABSoft_GUI_Fl_Slider*  m_horizontal_offset_slider  = nullptr;
    LABSoft_GUI_Fl_Slider*  m_vertical_offset_slider    = nullptr;
    LABSoft_GUI_Fl_Slider*  m_trigger_level_slider      = nullptr;

    LABSoft_GUI_Oscilloscope_Display_Internal* m_display_internal = nullptr;

    std::array<
      Fl_Button*,
      LABC::OSC::NUMBER_OF_CHANNELS
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
      LABC::OSC::NUMBER_OF_CHANNELS
    > m_voltage_per_division_labels;

    std::array<
      Fl_Box*,
      LABC::OSC::NUMBER_OF_CHANNELS
    > m_voltage_per_division_units;

  private:
    void    draw                                            ();
    void    init_child_widgets                              ();
    void    init_child_widgets_sliders                      ();
    void    init_child_widgets_status                       ();
    void    init_child_widgets_time_per_division_labels     ();
    void    init_child_widgets_voltage_per_division_labels  ();
    void    init_child_widgets_channel_selectors            ();
    void    init_child_widgets_top_info                     ();
    void    update_gui_status                               ();
    
    double  calc_row_voltage_per_division (unsigned row, unsigned number_of_rows, const LAB_Channel_Data_Oscilloscope& cdata);

    static void cb_trigger_level_static     (Fl_Widget* w, void* data);
    static void cb_channel_selector_static  (Fl_Widget* w, void* data);
                                           
  public:
    LABSoft_GUI_Oscilloscope_Display (int X             = 0, 
                                      int Y             = 0, 
                                      int W             = 700, 
                                      int H             = 510, 
                                      const char* label = 0);

   ~LABSoft_GUI_Oscilloscope_Display ();

    void load_parent_data                   (const LAB_Parent_Data_Oscilloscope& pdata);
    void load_controller                    (LABSoft_Controller& controller);
    void update_display                     ();
    void update_gui_voltage_per_division    (unsigned channel);
    void update_gui_voltage_per_division    ();
    void update_gui_time_per_division       ();
    void update_gui_vertical_elements       ();
    void update_gui_horizontal_elements     ();
    void update_gui_trigger_level_slider    ();
    void update_gui_vertical_offset_slider  ();
    void update_gui_top_info                ();
    void select_channel                     (unsigned channel);

    LABSoft_Controller& controller () const;
    unsigned            selected_channel () const;
};

#endif