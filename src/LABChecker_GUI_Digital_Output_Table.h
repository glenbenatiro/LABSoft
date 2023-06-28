#ifndef LABCHECKER_GUI_DIGITAL_OUTPUT_TABLE
#define LABCHECKER_GUI_DIGITAL_OUTPUT_TABLE

#include <vector>
#include <cstdint>

#include <FL/Fl_Table.H>

#include "LAB_Digital_Circuit_Checker.h"

class LABChecker_GUI_Digital_Output_Table : public Fl_Table
{
  private:
    int       COLOR_HEADER_ROW_INPUT          = 95;
    int       COLOR_HEADER_ROW_OUTPUT         = 79;
    int       COLOR_HEADER_ROW_ACTUAL_OUTPUT  = 239;
    int       COLOR_SELECTED_CELL             = FL_WHITE;
    unsigned  ROW_HEIGHT                      = 25;
    unsigned  COL_WIDTH                       = 30;

  private:
    bool      m_values_loaded     = false;
    unsigned  m_number_of_rows    = 1;
    unsigned  m_number_of_cols    = 2;
    unsigned  m_input_bits        = 1;
    unsigned  m_output_bits       = 1;
    unsigned  m_output_count      = 2;

    std::vector<uint8_t> m_input_vals;
    std::vector<uint8_t> m_output_vals;
    std::vector<uint8_t> m_actual_output_vals;

    std::vector<std::vector<char>> m_inputs;
    std::vector<std::vector<char>> m_outputs;
    std::vector<std::vector<char>> m_actual_outputs;

  private:
    void  draw_cell                 (TableContext context, 
                                      int R = 0, 
                                      int C = 0, 
                                      int X = 0, 
                                      int Y = 0, 
                                      int W = 0, 
                                      int H = 0);

    void  init_table_dimensions     ();
    void  do_resize                 ();
    void  recalculate_rows_and_cols ();
    void  recalculate_and_resize    ();
    char  get_vector_value          (unsigned R, unsigned C) const;

  public:
    LABChecker_GUI_Digital_Output_Table (int X, int Y, int W, int H, const char* label = 0);
   ~LABChecker_GUI_Digital_Output_Table ();

    // Setter
    void resize           (unsigned new_number_of_rows, unsigned new_number_of_cols);
    void display_results  (LAB_Digital_Circuit_Checker::ScoreData score_data);
    void load_data        (LAB_Digital_Circuit_Checker::ScoreData score_data);
};

#endif