// generated by Fast Light User Interface Designer (fluid) version 1.0305

#ifndef LABSoft_GUI_h
#define LABSoft_GUI_h
#include <FL/Fl.H>
#include "Defaults.h"
class LABSoft_Controller;
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Light_Button.H>
#include "LABSoft_Oscilloscope_Display_Group.h"
#include <FL/Fl_Input_Choice.H>
#include <FL/Fl_Output.H>
#include "LABSoft_Logic_Analyzer_Display_Group.h"
#include <FL/Fl_Choice.H>
#include <FL/Fl_Text_Display.H>

class LABSoft_GUI {
public:
  LABSoft_Controller *m_LABSoft_Controller; 
  LABSoft_GUI();
  Fl_Double_Window *main_fl_window;
  static Fl_Menu_Item menu_[];
  Fl_Button *oscilloscope_fl_button_run_stop;
private:
  inline void cb_oscilloscope_fl_button_run_stop_i(Fl_Button*, void*);
  static void cb_oscilloscope_fl_button_run_stop(Fl_Button*, void*);
public:
  Fl_Button *oscilloscope_fl_button_single;
private:
  inline void cb_oscilloscope_fl_button_single_i(Fl_Button*, void*);
  static void cb_oscilloscope_fl_button_single(Fl_Button*, void*);
public:
  Fl_Light_Button *oscilloscope_fl_light_button_test;
private:
  inline void cb_oscilloscope_fl_light_button_test_i(Fl_Light_Button*, void*);
  static void cb_oscilloscope_fl_light_button_test(Fl_Light_Button*, void*);
public:
  LABSoft_Oscilloscope_Display_Group *oscilloscope_labsoft_oscilloscope_display_group_display;
  Fl_Group *oscilloscope_fl_group_vertical_channel_1;
  Fl_Light_Button *oscilloscope_fl_light_button_channel_1_enable;
private:
  inline void cb_oscilloscope_fl_light_button_channel_1_enable_i(Fl_Light_Button*, long);
  static void cb_oscilloscope_fl_light_button_channel_1_enable(Fl_Light_Button*, long);
public:
  Fl_Input_Choice *oscilloscope_fl_input_choice_channel_1_volts_per_division;
private:
  inline void cb_oscilloscope_fl_input_choice_channel_1_volts_per_division_i(Fl_Input_Choice*, long);
  static void cb_oscilloscope_fl_input_choice_channel_1_volts_per_division(Fl_Input_Choice*, long);
  static Fl_Menu_Item menu_oscilloscope_fl_input_choice_channel_1_volts_per_division[];
public:
  Fl_Input_Choice *oscilloscope_fl_input_choice_channel_1_vertical_offset;
private:
  inline void cb_oscilloscope_fl_input_choice_channel_1_vertical_offset_i(Fl_Input_Choice*, long);
  static void cb_oscilloscope_fl_input_choice_channel_1_vertical_offset(Fl_Input_Choice*, long);
  static Fl_Menu_Item menu_oscilloscope_fl_input_choice_channel_1_vertical_offset[];
public:
  Fl_Group *oscilloscope_fl_group_vertical_channel_2;
  Fl_Light_Button *oscilloscope_fl_light_button_channel_2_enable;
private:
  inline void cb_oscilloscope_fl_light_button_channel_2_enable_i(Fl_Light_Button*, long);
  static void cb_oscilloscope_fl_light_button_channel_2_enable(Fl_Light_Button*, long);
public:
  Fl_Input_Choice *oscilloscope_fl_input_choice_channel_2_volts_per_division;
private:
  inline void cb_oscilloscope_fl_input_choice_channel_2_volts_per_division_i(Fl_Input_Choice*, long);
  static void cb_oscilloscope_fl_input_choice_channel_2_volts_per_division(Fl_Input_Choice*, long);
  static Fl_Menu_Item menu_oscilloscope_fl_input_choice_channel_2_volts_per_division[];
public:
  Fl_Input_Choice *oscilloscope_fl_input_choice_channel_2_vertical_offset;
private:
  inline void cb_oscilloscope_fl_input_choice_channel_2_vertical_offset_i(Fl_Input_Choice*, long);
  static void cb_oscilloscope_fl_input_choice_channel_2_vertical_offset(Fl_Input_Choice*, long);
  static Fl_Menu_Item menu_oscilloscope_fl_input_choice_channel_2_vertical_offset[];
public:
  Fl_Group *oscilloscope_fl_group_horizontal;
  Fl_Input_Choice *oscilloscope_fl_input_choice_time_per_division;
private:
  inline void cb_oscilloscope_fl_input_choice_time_per_division_i(Fl_Input_Choice*, long);
  static void cb_oscilloscope_fl_input_choice_time_per_division(Fl_Input_Choice*, long);
  static Fl_Menu_Item menu_oscilloscope_fl_input_choice_time_per_division[];
public:
  Fl_Input_Choice *oscilloscope_fl_input_choice_horizontal_offset;
private:
  inline void cb_oscilloscope_fl_input_choice_horizontal_offset_i(Fl_Input_Choice*, void*);
  static void cb_oscilloscope_fl_input_choice_horizontal_offset(Fl_Input_Choice*, void*);
  static Fl_Menu_Item menu_oscilloscope_fl_input_choice_horizontal_offset[];
public:
  Fl_Group *oscilloscope_fl_group_trigger;
  Fl_Input_Choice *oscilloscope_fl_input_choice_trigger_level;
  Fl_Group *oscilloscope_fl_group_display;
  Fl_Input_Choice *oscilloscope_fl_input_choice_display_mode;
  static Fl_Menu_Item menu_oscilloscope_fl_input_choice_display_mode[];
  Fl_Output *voltmeter_fl_output_chan1_value;
  Fl_Output *voltmeter_fl_output_chan2_value;
  Fl_Button *voltmeter_fl_button_run_stop;
private:
  inline void cb_voltmeter_fl_button_run_stop_i(Fl_Button*, void*);
  static void cb_voltmeter_fl_button_run_stop(Fl_Button*, void*);
public:
  Fl_Light_Button *function_generator_fl_light_button_run_stop;
private:
  inline void cb_function_generator_fl_light_button_run_stop_i(Fl_Light_Button*, long);
  static void cb_function_generator_fl_light_button_run_stop(Fl_Light_Button*, long);
public:
  Fl_Input_Choice *function_generator_fl_input_choice_wave_type;
private:
  inline void cb_function_generator_fl_input_choice_wave_type_i(Fl_Input_Choice*, long);
  static void cb_function_generator_fl_input_choice_wave_type(Fl_Input_Choice*, long);
  static Fl_Menu_Item menu_function_generator_fl_input_choice_wave_type[];
public:
  Fl_Input_Choice *function_generator_fl_input_choice_frequency;
private:
  inline void cb_function_generator_fl_input_choice_frequency_i(Fl_Input_Choice*, long);
  static void cb_function_generator_fl_input_choice_frequency(Fl_Input_Choice*, long);
  static Fl_Menu_Item menu_function_generator_fl_input_choice_frequency[];
public:
  Fl_Input_Choice *function_generator_fl_input_choice_period;
private:
  inline void cb_function_generator_fl_input_choice_period_i(Fl_Input_Choice*, long);
  static void cb_function_generator_fl_input_choice_period(Fl_Input_Choice*, long);
  static Fl_Menu_Item menu_function_generator_fl_input_choice_period[];
public:
  Fl_Input_Choice *function_generator_fl_input_choice_amplitude;
private:
  inline void cb_function_generator_fl_input_choice_amplitude_i(Fl_Input_Choice*, long);
  static void cb_function_generator_fl_input_choice_amplitude(Fl_Input_Choice*, long);
  static Fl_Menu_Item menu_function_generator_fl_input_choice_amplitude[];
public:
  Fl_Input_Choice *function_generator_fl_input_choice_offset;
private:
  inline void cb_function_generator_fl_input_choice_offset_i(Fl_Input_Choice*, long);
  static void cb_function_generator_fl_input_choice_offset(Fl_Input_Choice*, long);
  static Fl_Menu_Item menu_function_generator_fl_input_choice_offset[];
public:
  Fl_Input_Choice *function_generator_fl_input_choice_duty_cycle;
private:
  inline void cb_function_generator_fl_input_choice_duty_cycle_i(Fl_Input_Choice*, long);
  static void cb_function_generator_fl_input_choice_duty_cycle(Fl_Input_Choice*, long);
  static Fl_Menu_Item menu_function_generator_fl_input_choice_duty_cycle[];
public:
  Fl_Input_Choice *function_generator_fl_input_choice_phase;
private:
  inline void cb_function_generator_fl_input_choice_phase_i(Fl_Input_Choice*, long);
  static void cb_function_generator_fl_input_choice_phase(Fl_Input_Choice*, long);
  static Fl_Menu_Item menu_function_generator_fl_input_choice_phase[];
public:
  Fl_Light_Button *logic_analyzer_fl_light_button_master_run_stop;
private:
  inline void cb_logic_analyzer_fl_light_button_master_run_stop_i(Fl_Light_Button*, void*);
  static void cb_logic_analyzer_fl_light_button_master_run_stop(Fl_Light_Button*, void*);
public:
  Fl_Input_Choice *logic_analyzer_fl_input_choice_memory_depth;
  static Fl_Menu_Item menu_logic_analyzer_fl_input_choice_memory_depth[];
  Fl_Input_Choice *logic_analyzer_fl_input_choice_sample_rate;
  static Fl_Menu_Item menu_logic_analyzer_fl_input_choice_sample_rate[];
  Fl_Input_Choice *logic_analyzer_fl_input_choice_time_per_division;
  static Fl_Menu_Item menu_logic_analyzer_fl_input_choice_time_per_division[];
  Fl_Input_Choice *logic_analyzer_fl_input_choice_position;
  static Fl_Menu_Item menu_logic_analyzer_fl_input_choice_position[];
  LABSoft_Logic_Analyzer_Display_Group *logic_analyzer_labsoft_logic_analyzer_display_group_display;
  Fl_Choice *logic_analyzer_fl_choice_display_mode;
  static Fl_Menu_Item menu_logic_analyzer_fl_choice_display_mode[];
  Fl_Button *circuit_checker_fl_button_choose_circuit_checker_file;
private:
  inline void cb_circuit_checker_fl_button_choose_circuit_checker_file_i(Fl_Button*, void*);
  static void cb_circuit_checker_fl_button_choose_circuit_checker_file(Fl_Button*, void*);
public:
  Fl_Button *circuit_checker_fl_button_start_circuit_checking;
private:
  inline void cb_circuit_checker_fl_button_start_circuit_checking_i(Fl_Button*, void*);
  static void cb_circuit_checker_fl_button_start_circuit_checking(Fl_Button*, void*);
public:
  Fl_Output *circuit_checker_fl_output_selected_circuit_checker_file;
  Fl_Output *circuit_checker_fl_output_similarity_score;
  Fl_Text_Display *circuit_checker_fl_text_display_logger;
  Fl_Group *alignment_guides;
  Fl_Box *inner_height;
  Fl_Box *outer_height;
  Fl_Box *inner_height_crosswise;
};
#endif
