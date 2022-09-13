// generated by Fast Light User Interface Designer (fluid) version 1.0305

#ifndef LABSoft_GUI_h
#define LABSoft_GUI_h
#include <FL/Fl.H>
#include "LAB.h"
class LABSoft_Controller;
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Button.H>
#include "LABSoft_Oscilloscope_Display_Group.h"
#include <FL/Fl_Input_Choice.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_File_Browser.H>

class LABSoft_GUI {
public:
  LAB *m_LAB; 
  LABSoft_Controller *m_LABSoft_Controller; 
  LABSoft_GUI();
  Fl_Double_Window *main_fl_window;
  Fl_Light_Button *oscilloscope_fl_light_button_run_stop;
  LABSoft_Oscilloscope_Display_Group *oscilloscope_labsoft_oscilloscope_display_group_display;
  Fl_Group *oscilloscope_fl_group_vertical_channel_1;
  Fl_Light_Button *oscilloscope_fl_light_button_channel_1_enable;
  Fl_Input_Choice *oscilloscope_fl_input_choice_channel_1_volts_per_division;
  Fl_Input_Choice *oscilloscope_fl_input_choice_channel_1_vertical_offset;
  Fl_Group *oscilloscope_fl_group_vertical_channel_2;
  Fl_Light_Button *oscilloscope_fl_light_button_channel_2_enable;
  Fl_Input_Choice *oscilloscope_fl_input_choice_channel_2_volts_per_division;
  Fl_Input_Choice *oscilloscope_fl_input_choice_channel_2_vertical_offset;
  Fl_Group *oscilloscope_fl_group_horizontal;
  Fl_Input_Choice *oscilloscope_fl_input_choice_time_per_division;
  Fl_Input_Choice *oscilloscope_fl_input_choice_horizontal_offset;
  Fl_Group *oscilloscope_fl_group_trigger;
  Fl_Group *oscilloscope_fl_group_display;
  Fl_Output *voltmeter_fl_output_value;
  Fl_Choice *voltmeter_fl_choice_unit;
private:
  inline void cb_voltmeter_fl_choice_unit_i(Fl_Choice*, void*);
  static void cb_voltmeter_fl_choice_unit(Fl_Choice*, void*);
  static Fl_Menu_Item menu_voltmeter_fl_choice_unit[];
public:
  Fl_Light_Button *voltmeter_fl_light_button_run_stop;
private:
  inline void cb_voltmeter_fl_light_button_run_stop_i(Fl_Light_Button*, void*);
  static void cb_voltmeter_fl_light_button_run_stop(Fl_Light_Button*, void*);
public:
  Fl_Light_Button *voltmeter_fl_light_button_generate_random_values;
private:
  inline void cb_voltmeter_fl_light_button_generate_random_values_i(Fl_Light_Button*, void*);
  static void cb_voltmeter_fl_light_button_generate_random_values(Fl_Light_Button*, void*);
public:
  Fl_Output *ammeter_fl_output_value;
  Fl_Choice *ammeter_fl_choice_unit;
  static Fl_Menu_Item menu_ammeter_fl_choice_unit[];
  Fl_Light_Button *ammeter_fl_light_button_run_stop;
  Fl_Output *ohmmeter_fl_output_value;
  Fl_Choice *ohmmeter_fl_choice_unit;
  static Fl_Menu_Item menu_ohmmeter_fl_choice_unit[];
  Fl_Light_Button *ohmmeter_fl_light_button_run_stop;
  Fl_Light_Button *function_generator_fl_light_button_start_stop;
private:
  inline void cb_function_generator_fl_light_button_start_stop_i(Fl_Light_Button*, void*);
  static void cb_function_generator_fl_light_button_start_stop(Fl_Light_Button*, void*);
public:
  LABSoft_Oscilloscope_Display_Group *function_generator_labsoft_oscilloscope_display_group_display;
  Fl_Group *function_generator_fl_group_horizontal;
  Fl_Input_Choice *function_generator_fl_input_choice_horizontal_position;
  static Fl_Menu_Item menu_function_generator_fl_input_choice_horizontal_position[];
  Fl_Input_Choice *function_generator_fl_input_choice_time_per_division;
private:
  inline void cb_function_generator_fl_input_choice_time_per_division_i(Fl_Input_Choice*, void*);
  static void cb_function_generator_fl_input_choice_time_per_division(Fl_Input_Choice*, void*);
  static Fl_Menu_Item menu_function_generator_fl_input_choice_time_per_division[];
public:
  Fl_Group *function_generator_fl_group_vertical;
  Fl_Input_Choice *function_generator_fl_input_choice_vertical_offset;
  static Fl_Menu_Item menu_function_generator_fl_input_choice_vertical_offset[];
  Fl_Input_Choice *function_generator_fl_input_choice_volts_per_division;
private:
  inline void cb_function_generator_fl_input_choice_volts_per_division_i(Fl_Input_Choice*, void*);
  static void cb_function_generator_fl_input_choice_volts_per_division(Fl_Input_Choice*, void*);
  static Fl_Menu_Item menu_function_generator_fl_input_choice_volts_per_division[];
public:
  Fl_Group *function_generator_fl_group_parameters;
  Fl_Input_Choice *function_generator_fl_input_choice_wave_type;
private:
  inline void cb_function_generator_fl_input_choice_wave_type_i(Fl_Input_Choice*, void*);
  static void cb_function_generator_fl_input_choice_wave_type(Fl_Input_Choice*, void*);
  static Fl_Menu_Item menu_function_generator_fl_input_choice_wave_type[];
public:
  Fl_Input_Choice *function_generator_fl_input_choice_frequency;
private:
  inline void cb_function_generator_fl_input_choice_frequency_i(Fl_Input_Choice*, void*);
  static void cb_function_generator_fl_input_choice_frequency(Fl_Input_Choice*, void*);
  static Fl_Menu_Item menu_function_generator_fl_input_choice_frequency[];
public:
  Fl_Input_Choice *function_generator_fl_input_choice_period;
private:
  inline void cb_function_generator_fl_input_choice_period_i(Fl_Input_Choice*, void*);
  static void cb_function_generator_fl_input_choice_period(Fl_Input_Choice*, void*);
  static Fl_Menu_Item menu_function_generator_fl_input_choice_period[];
public:
  Fl_Input_Choice *function_generator_fl_input_choice_amplitude;
private:
  inline void cb_function_generator_fl_input_choice_amplitude_i(Fl_Input_Choice*, void*);
  static void cb_function_generator_fl_input_choice_amplitude(Fl_Input_Choice*, void*);
  static Fl_Menu_Item menu_function_generator_fl_input_choice_amplitude[];
public:
  Fl_Input_Choice *function_generator_fl_input_choice_offset;
private:
  inline void cb_function_generator_fl_input_choice_offset_i(Fl_Input_Choice*, void*);
  static void cb_function_generator_fl_input_choice_offset(Fl_Input_Choice*, void*);
  static Fl_Menu_Item menu_function_generator_fl_input_choice_offset[];
public:
  Fl_Input_Choice *function_generator_fl_input_choice_duty_cycle;
  static Fl_Menu_Item menu_function_generator_fl_input_choice_duty_cycle[];
  Fl_Input_Choice *function_generator_fl_input_choice_phase;
private:
  inline void cb_function_generator_fl_input_choice_phase_i(Fl_Input_Choice*, void*);
  static void cb_function_generator_fl_input_choice_phase(Fl_Input_Choice*, void*);
  static Fl_Menu_Item menu_function_generator_fl_input_choice_phase[];
public:
  Fl_Light_Button *power_supply_fl_light_button_positive_12_volts;
private:
  inline void cb_power_supply_fl_light_button_positive_12_volts_i(Fl_Light_Button*, void*);
  static void cb_power_supply_fl_light_button_positive_12_volts(Fl_Light_Button*, void*);
public:
  Fl_Light_Button *power_supply_fl_light_button_negative_12_volts;
private:
  inline void cb_power_supply_fl_light_button_negative_12_volts_i(Fl_Light_Button*, void*);
  static void cb_power_supply_fl_light_button_negative_12_volts(Fl_Light_Button*, void*);
public:
  Fl_Light_Button *power_supply_fl_light_button_positive_5_volts;
private:
  inline void cb_power_supply_fl_light_button_positive_5_volts_i(Fl_Light_Button*, void*);
  static void cb_power_supply_fl_light_button_positive_5_volts(Fl_Light_Button*, void*);
public:
  Fl_Light_Button *power_supply_fl_light_button_master_enable_disable;
private:
  inline void cb_power_supply_fl_light_button_master_enable_disable_i(Fl_Light_Button*, void*);
  static void cb_power_supply_fl_light_button_master_enable_disable(Fl_Light_Button*, void*);
public:
  Fl_File_Browser *circuit_checker_fl_file_browser_file_browser;
  Fl_Button *circuit_checker_fl_button_start;
  Fl_Output *circuit_checker_fl_output_similarity_score;
  Fl_Group *alignment_guides;
  Fl_Box *inner_height;
  Fl_Box *outer_height;
  Fl_Box *inner_height_crosswise;
};
#endif
