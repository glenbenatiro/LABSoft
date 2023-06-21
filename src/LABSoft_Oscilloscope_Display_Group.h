#ifndef LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_H
#define LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_H

#include <array>

#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Button.H>

#include "Defaults.h"
#include "LABSoft_Oscilloscope_Display.h"

// Forward declare LABSoft_GUI
class LABSoft_GUI;

class LABSoft_Oscilloscope_Display_Group : public Fl_Group
{
  private: 
    friend class LABSoft_Controller_Oscilloscope;

    unsigned m_channel_selected = 0;

    LAB_Parent_Data_Oscilloscope* m_parent_data = nullptr;
    LABSoft_Oscilloscope_Display* m_display     = nullptr;

    Fl_Box*     m_display_status                = nullptr;
    Fl_Box*     m_upper_info_display            = nullptr;
    Fl_Slider*  m_horizontal_offset             = nullptr;
    Fl_Slider*  m_vertical_offset               = nullptr;
    Fl_Slider*  m_trigger_level                 = nullptr;

    std::array<
      Fl_Button*,
      LABC::OSC::NUMBER_OF_CHANNELS
    > m_channel_selectors;
    
    std::array<
      Fl_Box*, 
      LABSOFT_OSCILLOSCOPE_DISPLAY::NUMBER_OF_COLUMNS + 1
    > m_x_labels; 

    std::array<
      std::array<
        Fl_Box*,
        LABSOFT_OSCILLOSCOPE_DISPLAY::NUMBER_OF_ROWS + 1
      >,
      LABC::OSC::NUMBER_OF_CHANNELS
    > m_y_labels;
   
    std::array<
      Fl_Box*, 
      LABC::OSC::NUMBER_OF_CHANNELS
    > m_y_label_units;

  private:
    void    update_gui_y_label_unit           (unsigned channel);
    bool    is_approx_zero                (double x, double epsilon = 1e-12);
    double  calc_row_voltage_per_division (int row, unsigned number_of_rows, LAB_Channel_Data_Oscilloscope& chan);
    void    display (LABSoft_Oscilloscope_Display* _LABSoft_Oscilloscope_Display);

  public:    
    LABSoft_Oscilloscope_Display_Group (int X, int Y, int W, int H, const char* label = 0);
    
    // Draw
    void  draw ();

    // Load data
    void  load_osc_parent_data            (LAB_Parent_Data_Oscilloscope& parent_data);
    void  reserve_pixel_points            ();
    void  fill_pixel_points               ();
    void channel_selector                 (unsigned channel);

    // Update data
    void  update_gui_time_per_division    ();
    void  update_gui_upper_left_info      ();
    void  update_all_display_information  ();
    void  update_display_status           ();   

    // GUI updates
    void  update_gui_vertical_offset      (); 
    void  update_gui_voltage_per_division (unsigned channel);
    void  update_gui_voltage_per_division ();
    void  update_gui_vertical_elements    ();

    // Getter
    LABSoft_Oscilloscope_Display* display ()
    {
      return (m_display);
    }
};

#endif