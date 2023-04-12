#ifndef LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_H
#define LABSOFT_OSCILLOSCOPE_DISPLAY_GROUP_H

#include <array>

#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>

#include "Defaults.h"
#include "LabelValue.h"
#include "LABSoft_Oscilloscope_Display.h"

class LABSoft_Oscilloscope_Display_Group : public Fl_Group
{
  private: 
    // --- Variables ---
    Fl_Box*                       m_upper_osc_disp_info = nullptr;
    LAB_Parent_Data_Oscilloscope *m_parent_data_osc     = nullptr;

    std::array <Fl_Box*, LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_COLUMNS + 1>
      m_x_labels; 

    std::array <std::array <Fl_Box*, LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_ROWS + 1>, 
      LAB_OSCILLOSCOPE::NUMBER_OF_CHANNELS> m_y_labels;

    std::array<Fl_Box*, LAB_OSCILLOSCOPE::NUMBER_OF_CHANNELS> m_y_label_units;

    // --- Functions ---
    void    update_y_label_unit           (unsigned channel);
    bool    is_approx_zero                (double x, double epsilon = 1e-12);
    double  calc_row_voltage_per_division (int row, unsigned number_of_rows, LAB_Channel_Data_Oscilloscope& chan);
        
  public:   
    LABSoft_Oscilloscope_Display  *m_display;
  
    // --- Functions --
    LABSoft_Oscilloscope_Display_Group (int X, int Y, int W, int H, const char *label = 0);
    
    // Draw
    void  draw ();

    // Load data
    void  load_osc_parent_data  (LAB_Parent_Data_Oscilloscope *parent_data);
    int   reserve_pixel_points  ();
    void  fill_pixel_points     ();

    // Update data
    void  update_voltage_per_division_labels  ();
    void  update_voltage_per_division_labels  (unsigned channel);
    void  update_time_per_division_labels     ();
    void  update_upper_osc_disp_info          (); 
};

#endif