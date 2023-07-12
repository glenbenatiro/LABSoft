// generated by Fast Light User Interface Designer (fluid) version 1.0305

#ifndef LABSoft_GUI_h
#define LABSoft_GUI_h
#include <FL/Fl.H>
#include "../Utility/Defaults.h"
#include "LABSoft_GUI_Fl_Input_Choice_With_Scroll.h"
#include "LABSoft_GUI_Fl_Choice_With_Scroll.h"
class LABSoft_Controller;
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Light_Button.H>
#include "LABSoft_GUI_Oscilloscope_Display_Group.h"
#include <FL/Fl_Slider.H>
#include <FL/Fl_Input_Choice.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Output.H>
#include "LABSoft_GUI_Logic_Analyzer_Display_Group.h"
#include <FL/Fl_Text_Display.H>
#include "LABSoft_GUI_LABChecker_Digital_Output_Table.h"
#include <FL/Fl_Input.H>
#include "LABSoft_GUI_LABChecker_Digital_Input_Table.h"
#include <FL/Fl_Check_Button.H>

class LABSoft_GUI {
public:
  LABSoft_Controller *m_LABSoft_Controller; 
  LABSoft_GUI();
  Fl_Double_Window *main_fl_window;
  static Fl_Menu_Item menu_[];
  static Fl_Menu_Item *main_menuitem_export;
private:
  inline void cb_Exit_i(Fl_Menu_*, void*);
  static void cb_Exit(Fl_Menu_*, void*);
  inline void cb_Browse_i(Fl_Menu_*, void*);
  static void cb_Browse(Fl_Menu_*, void*);
  inline void cb_About_i(Fl_Menu_*, void*);
  static void cb_About(Fl_Menu_*, void*);
public:
  Fl_Tabs *main_fl_tabs;
private:
  inline void cb_main_fl_tabs_i(Fl_Tabs*, void*);
  static void cb_main_fl_tabs(Fl_Tabs*, void*);
public:
  Fl_Group *main_fl_group_home_tab;
  Fl_Group *main_fl_group_oscilloscope_tab;
  Fl_Button *oscilloscope_fl_button_record;
private:
  inline void cb_oscilloscope_fl_button_record_i(Fl_Button*, void*);
  static void cb_oscilloscope_fl_button_record(Fl_Button*, void*);
public:
  Fl_Light_Button *oscilloscope_fl_light_button_run_stop;
private:
  inline void cb_oscilloscope_fl_light_button_run_stop_i(Fl_Light_Button*, void*);
  static void cb_oscilloscope_fl_light_button_run_stop(Fl_Light_Button*, void*);
public:
  Fl_Button *oscilloscope_fl_button_record_config;
private:
  inline void cb_oscilloscope_fl_button_record_config_i(Fl_Button*, void*);
  static void cb_oscilloscope_fl_button_record_config(Fl_Button*, void*);
public:
  Fl_Button *oscilloscope_fl_button_single;
private:
  inline void cb_oscilloscope_fl_button_single_i(Fl_Button*, void*);
  static void cb_oscilloscope_fl_button_single(Fl_Button*, void*);
public:
  LABSoft_GUI_Oscilloscope_Display_Group *oscilloscope_labsoft_oscilloscope_display_group_display;
  LABSoft_GUI_Oscilloscope_Display *oscilloscope_labsoft_oscilloscope_display_display;
private:
  inline void cb_oscilloscope_labsoft_oscilloscope_display_display_i(LABSoft_GUI_Oscilloscope_Display*, void*);
  static void cb_oscilloscope_labsoft_oscilloscope_display_display(LABSoft_GUI_Oscilloscope_Display*, void*);
public:
  Fl_Box *oscilloscope_fl_box_display_status;
  Fl_Button *oscilloscope_fl_toggle_button_c1_selector;
private:
  inline void cb_oscilloscope_fl_toggle_button_c1_selector_i(Fl_Button*, long);
  static void cb_oscilloscope_fl_toggle_button_c1_selector(Fl_Button*, long);
public:
  Fl_Button *oscilloscope_fl_toggle_button_c2_selector;
private:
  inline void cb_oscilloscope_fl_toggle_button_c2_selector_i(Fl_Button*, long);
  static void cb_oscilloscope_fl_toggle_button_c2_selector(Fl_Button*, long);
public:
  Fl_Box *oscilloscope_fl_box_upper_info_display;
  Fl_Slider *oscilloscope_fl_slider_horizontal_offset;
  Fl_Slider *oscilloscope_fl_slider_vertical_offset;
private:
  inline void cb_oscilloscope_fl_slider_vertical_offset_i(Fl_Slider*, void*);
  static void cb_oscilloscope_fl_slider_vertical_offset(Fl_Slider*, void*);
public:
  Fl_Slider *oscilloscope_fl_slider_trigger_level;
  Fl_Group *oscilloscope_fl_group_vertical_channel_0;
  Fl_Light_Button *oscilloscope_fl_light_button_channel_0_enable;
private:
  inline void cb_oscilloscope_fl_light_button_channel_0_enable_i(Fl_Light_Button*, long);
  static void cb_oscilloscope_fl_light_button_channel_0_enable(Fl_Light_Button*, long);
public:
  Fl_Light_Button *oscilloscope_fl_light_button_channel_0_ac_coupling;
private:
  inline void cb_oscilloscope_fl_light_button_channel_0_ac_coupling_i(Fl_Light_Button*, long);
  static void cb_oscilloscope_fl_light_button_channel_0_ac_coupling(Fl_Light_Button*, long);
public:
  LABSoft_GUI_Fl_Choice_With_Scroll *oscilloscope_labsoft_gui_fl_choice_with_scroll_channel_0_scaling;
private:
  inline void cb_oscilloscope_labsoft_gui_fl_choice_with_scroll_channel_0_scaling_i(LABSoft_GUI_Fl_Choice_With_Scroll*, long);
  static void cb_oscilloscope_labsoft_gui_fl_choice_with_scroll_channel_0_scaling(LABSoft_GUI_Fl_Choice_With_Scroll*, long);
  static Fl_Menu_Item menu_oscilloscope_labsoft_gui_fl_choice_with_scroll_channel_0_scaling[];
public:
  LABSoft_GUI_Fl_Input_Choice_With_Scroll *oscilloscope_labsoft_gui_fl_input_choice_with_scroll_channel_0_voltage_per_division;
private:
  inline void cb_oscilloscope_labsoft_gui_fl_input_choice_with_scroll_channel_0_voltage_per_division_i(LABSoft_GUI_Fl_Input_Choice_With_Scroll*, long);
  static void cb_oscilloscope_labsoft_gui_fl_input_choice_with_scroll_channel_0_voltage_per_division(LABSoft_GUI_Fl_Input_Choice_With_Scroll*, long);
  static Fl_Menu_Item menu_oscilloscope_labsoft_gui_fl_input_choice_with_scroll_channel_0_voltage_per_division[];
public:
  LABSoft_GUI_Fl_Input_Choice_With_Scroll *oscilloscope_labsoft_gui_fl_input_choice_with_scroll_channel_0_vertical_offset;
private:
  inline void cb_oscilloscope_labsoft_gui_fl_input_choice_with_scroll_channel_0_vertical_offset_i(LABSoft_GUI_Fl_Input_Choice_With_Scroll*, long);
  static void cb_oscilloscope_labsoft_gui_fl_input_choice_with_scroll_channel_0_vertical_offset(LABSoft_GUI_Fl_Input_Choice_With_Scroll*, long);
  static Fl_Menu_Item menu_oscilloscope_labsoft_gui_fl_input_choice_with_scroll_channel_0_vertical_offset[];
public:
  Fl_Group *oscilloscope_fl_group_vertical_channel_1;
  Fl_Light_Button *oscilloscope_fl_light_button_channel_1_enable;
private:
  inline void cb_oscilloscope_fl_light_button_channel_1_enable_i(Fl_Light_Button*, long);
  static void cb_oscilloscope_fl_light_button_channel_1_enable(Fl_Light_Button*, long);
public:
  Fl_Light_Button *oscilloscope_fl_light_button_channel_1_ac_coupling;
private:
  inline void cb_oscilloscope_fl_light_button_channel_1_ac_coupling_i(Fl_Light_Button*, long);
  static void cb_oscilloscope_fl_light_button_channel_1_ac_coupling(Fl_Light_Button*, long);
public:
  LABSoft_GUI_Fl_Choice_With_Scroll *oscilloscope_labsoft_gui_fl_choice_with_scroll_channel_1_scaling;
private:
  inline void cb_oscilloscope_labsoft_gui_fl_choice_with_scroll_channel_1_scaling_i(LABSoft_GUI_Fl_Choice_With_Scroll*, long);
  static void cb_oscilloscope_labsoft_gui_fl_choice_with_scroll_channel_1_scaling(LABSoft_GUI_Fl_Choice_With_Scroll*, long);
  static Fl_Menu_Item menu_oscilloscope_labsoft_gui_fl_choice_with_scroll_channel_1_scaling[];
public:
  LABSoft_GUI_Fl_Input_Choice_With_Scroll *oscilloscope_labsoft_gui_fl_input_choice_with_scroll_channel_1_voltage_per_division;
private:
  inline void cb_oscilloscope_labsoft_gui_fl_input_choice_with_scroll_channel_1_voltage_per_division_i(LABSoft_GUI_Fl_Input_Choice_With_Scroll*, long);
  static void cb_oscilloscope_labsoft_gui_fl_input_choice_with_scroll_channel_1_voltage_per_division(LABSoft_GUI_Fl_Input_Choice_With_Scroll*, long);
  static Fl_Menu_Item menu_oscilloscope_labsoft_gui_fl_input_choice_with_scroll_channel_1_voltage_per_division[];
public:
  LABSoft_GUI_Fl_Input_Choice_With_Scroll *oscilloscope_labsoft_gui_fl_input_choice_with_scroll_channel_1_vertical_offset;
private:
  inline void cb_oscilloscope_labsoft_gui_fl_input_choice_with_scroll_channel_1_vertical_offset_i(LABSoft_GUI_Fl_Input_Choice_With_Scroll*, long);
  static void cb_oscilloscope_labsoft_gui_fl_input_choice_with_scroll_channel_1_vertical_offset(LABSoft_GUI_Fl_Input_Choice_With_Scroll*, long);
  static Fl_Menu_Item menu_oscilloscope_labsoft_gui_fl_input_choice_with_scroll_channel_1_vertical_offset[];
public:
  Fl_Group *oscilloscope_fl_group_horizontal;
  Fl_Input_Choice *oscilloscope_fl_input_choice_horizontal_offset;
private:
  inline void cb_oscilloscope_fl_input_choice_horizontal_offset_i(Fl_Input_Choice*, void*);
  static void cb_oscilloscope_fl_input_choice_horizontal_offset(Fl_Input_Choice*, void*);
  static Fl_Menu_Item menu_oscilloscope_fl_input_choice_horizontal_offset[];
public:
  Fl_Input_Choice *oscilloscope_fl_input_choice_time_per_division;
private:
  inline void cb_oscilloscope_fl_input_choice_time_per_division_i(Fl_Input_Choice*, void*);
  static void cb_oscilloscope_fl_input_choice_time_per_division(Fl_Input_Choice*, void*);
  static Fl_Menu_Item menu_oscilloscope_fl_input_choice_time_per_division[];
public:
  Fl_Input_Choice *oscilloscope_fl_input_choice_samples;
private:
  inline void cb_oscilloscope_fl_input_choice_samples_i(Fl_Input_Choice*, void*);
  static void cb_oscilloscope_fl_input_choice_samples(Fl_Input_Choice*, void*);
  static Fl_Menu_Item menu_oscilloscope_fl_input_choice_samples[];
public:
  Fl_Input_Choice *oscilloscope_fl_input_choice_sampling_rate;
private:
  inline void cb_oscilloscope_fl_input_choice_sampling_rate_i(Fl_Input_Choice*, void*);
  static void cb_oscilloscope_fl_input_choice_sampling_rate(Fl_Input_Choice*, void*);
  static Fl_Menu_Item menu_oscilloscope_fl_input_choice_sampling_rate[];
public:
  Fl_Group *oscilloscope_fl_group_trigger;
  Fl_Choice *oscilloscope_fl_choice_trigger_mode;
private:
  inline void cb_oscilloscope_fl_choice_trigger_mode_i(Fl_Choice*, void*);
  static void cb_oscilloscope_fl_choice_trigger_mode(Fl_Choice*, void*);
  static Fl_Menu_Item menu_oscilloscope_fl_choice_trigger_mode[];
public:
  Fl_Choice *oscilloscope_fl_choice_trigger_source;
private:
  inline void cb_oscilloscope_fl_choice_trigger_source_i(Fl_Choice*, void*);
  static void cb_oscilloscope_fl_choice_trigger_source(Fl_Choice*, void*);
  static Fl_Menu_Item menu_oscilloscope_fl_choice_trigger_source[];
public:
  Fl_Choice *oscilloscope_fl_choice_trigger_type;
private:
  inline void cb_oscilloscope_fl_choice_trigger_type_i(Fl_Choice*, void*);
  static void cb_oscilloscope_fl_choice_trigger_type(Fl_Choice*, void*);
  static Fl_Menu_Item menu_oscilloscope_fl_choice_trigger_type[];
public:
  Fl_Choice *oscilloscope_fl_choice_trigger_condition;
private:
  inline void cb_oscilloscope_fl_choice_trigger_condition_i(Fl_Choice*, void*);
  static void cb_oscilloscope_fl_choice_trigger_condition(Fl_Choice*, void*);
  static Fl_Menu_Item menu_oscilloscope_fl_choice_trigger_condition[];
public:
  Fl_Input_Choice *oscilloscope_fl_input_choice_trigger_level;
private:
  inline void cb_oscilloscope_fl_input_choice_trigger_level_i(Fl_Input_Choice*, void*);
  static void cb_oscilloscope_fl_input_choice_trigger_level(Fl_Input_Choice*, void*);
  static Fl_Menu_Item menu_oscilloscope_fl_input_choice_trigger_level[];
public:
  Fl_Group *oscilloscope_fl_group_display;
  Fl_Choice *oscilloscope_fl_choice_mode;
private:
  inline void cb_oscilloscope_fl_choice_mode_i(Fl_Choice*, void*);
  static void cb_oscilloscope_fl_choice_mode(Fl_Choice*, void*);
  static Fl_Menu_Item menu_oscilloscope_fl_choice_mode[];
public:
  Fl_Group *main_fl_group_voltmeter_tab;
  Fl_Light_Button *voltmeter_fl_light_button_run_stop;
private:
  inline void cb_voltmeter_fl_light_button_run_stop_i(Fl_Light_Button*, void*);
  static void cb_voltmeter_fl_light_button_run_stop(Fl_Light_Button*, void*);
public:
  Fl_Group *voltmeter_fl_group_values;
  Fl_Output *voltmeter_fl_output_chan0_value;
  Fl_Output *voltmeter_fl_output_chan1_value;
  Fl_Group *main_fl_group_function_generator_tab;
  Fl_Choice *function_generator_fl_choice_wave_type;
private:
  inline void cb_function_generator_fl_choice_wave_type_i(Fl_Choice*, long);
  static void cb_function_generator_fl_choice_wave_type(Fl_Choice*, long);
  static Fl_Menu_Item menu_function_generator_fl_choice_wave_type[];
public:
  Fl_Input_Choice *function_generator_fl_input_choice_amplitude;
private:
  inline void cb_function_generator_fl_input_choice_amplitude_i(Fl_Input_Choice*, long);
  static void cb_function_generator_fl_input_choice_amplitude(Fl_Input_Choice*, long);
  static Fl_Menu_Item menu_function_generator_fl_input_choice_amplitude[];
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
  Fl_Input_Choice *function_generator_fl_input_choice_phase;
private:
  inline void cb_function_generator_fl_input_choice_phase_i(Fl_Input_Choice*, long);
  static void cb_function_generator_fl_input_choice_phase(Fl_Input_Choice*, long);
  static Fl_Menu_Item menu_function_generator_fl_input_choice_phase[];
public:
  Fl_Input_Choice *function_generator_fl_input_choice_offset;
private:
  inline void cb_function_generator_fl_input_choice_offset_i(Fl_Input_Choice*, long);
  static void cb_function_generator_fl_input_choice_offset(Fl_Input_Choice*, long);
  static Fl_Menu_Item menu_function_generator_fl_input_choice_offset[];
public:
  Fl_Light_Button *function_generator_fl_light_button_run_stop;
private:
  inline void cb_function_generator_fl_light_button_run_stop_i(Fl_Light_Button*, long);
  static void cb_function_generator_fl_light_button_run_stop(Fl_Light_Button*, long);
public:
  Fl_Group *main_fl_group_logic_analyzer_tab;
  Fl_Light_Button *logic_analyzer_fl_light_button_run_stop;
private:
  inline void cb_logic_analyzer_fl_light_button_run_stop_i(Fl_Light_Button*, void*);
  static void cb_logic_analyzer_fl_light_button_run_stop(Fl_Light_Button*, void*);
public:
  Fl_Input_Choice *logic_analyzer_fl_input_choice_memory_depth;
private:
  inline void cb_logic_analyzer_fl_input_choice_memory_depth_i(Fl_Input_Choice*, void*);
  static void cb_logic_analyzer_fl_input_choice_memory_depth(Fl_Input_Choice*, void*);
  static Fl_Menu_Item menu_logic_analyzer_fl_input_choice_memory_depth[];
public:
  Fl_Input_Choice *logic_analyzer_fl_input_choice_sampling_rate;
private:
  inline void cb_logic_analyzer_fl_input_choice_sampling_rate_i(Fl_Input_Choice*, void*);
  static void cb_logic_analyzer_fl_input_choice_sampling_rate(Fl_Input_Choice*, void*);
  static Fl_Menu_Item menu_logic_analyzer_fl_input_choice_sampling_rate[];
public:
  Fl_Input_Choice *logic_analyzer_fl_input_choice_time_per_division;
private:
  inline void cb_logic_analyzer_fl_input_choice_time_per_division_i(Fl_Input_Choice*, void*);
  static void cb_logic_analyzer_fl_input_choice_time_per_division(Fl_Input_Choice*, void*);
  static Fl_Menu_Item menu_logic_analyzer_fl_input_choice_time_per_division[];
public:
  Fl_Input_Choice *logic_analyzer_fl_input_choice_position;
private:
  inline void cb_logic_analyzer_fl_input_choice_position_i(Fl_Input_Choice*, void*);
  static void cb_logic_analyzer_fl_input_choice_position(Fl_Input_Choice*, void*);
  static Fl_Menu_Item menu_logic_analyzer_fl_input_choice_position[];
public:
  Fl_Choice *logic_analyzer_fl_choice_display_mode;
private:
  inline void cb_logic_analyzer_fl_choice_display_mode_i(Fl_Choice*, void*);
  static void cb_logic_analyzer_fl_choice_display_mode(Fl_Choice*, void*);
  static Fl_Menu_Item menu_logic_analyzer_fl_choice_display_mode[];
public:
  LABSoft_GUI_Logic_Analyzer_Display_Group *logic_analyzer_labsoft_logic_analyzer_display_group_display;
  Fl_Group *main_fl_group_circuit_checker_tab;
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
  Fl_Group *main_fl_group_digital_circuit_checker_tab;
  Fl_Button *digital_circuit_checker_fl_button_load_file;
private:
  inline void cb_digital_circuit_checker_fl_button_load_file_i(Fl_Button*, void*);
  static void cb_digital_circuit_checker_fl_button_load_file(Fl_Button*, void*);
public:
  Fl_Output *digital_circuit_checker_fl_output_selected_file;
  Fl_Button *digital_circuit_checker_fl_button_unload_file;
private:
  inline void cb_digital_circuit_checker_fl_button_unload_file_i(Fl_Button*, void*);
  static void cb_digital_circuit_checker_fl_button_unload_file(Fl_Button*, void*);
public:
  Fl_Button *digital_circuit_checker_fl_button_run_checker;
private:
  inline void cb_digital_circuit_checker_fl_button_run_checker_i(Fl_Button*, void*);
  static void cb_digital_circuit_checker_fl_button_run_checker(Fl_Button*, void*);
public:
  LABSoft_GUI_LABChecker_Digital_Output_Table *digital_circuit_checker_labchecker_gui_digital_output_table_table;
  Fl_Output *digital_circuit_checker_fl_output_results;
  Fl_Button *digital_circuit_checker_fl_button_export_results;
  Fl_Group *main_fl_group_labchecker_digital;
  Fl_Choice *digital_fl_choice_input_bits;
private:
  inline void cb_digital_fl_choice_input_bits_i(Fl_Choice*, void*);
  static void cb_digital_fl_choice_input_bits(Fl_Choice*, void*);
  static Fl_Menu_Item menu_digital_fl_choice_input_bits[];
public:
  Fl_Choice *digital_fl_choice_output_bits;
private:
  inline void cb_digital_fl_choice_output_bits_i(Fl_Choice*, void*);
  static void cb_digital_fl_choice_output_bits(Fl_Choice*, void*);
  static Fl_Menu_Item menu_digital_fl_choice_output_bits[];
public:
  Fl_Input *digital_fl_input_output_count;
private:
  inline void cb_digital_fl_input_output_count_i(Fl_Input*, void*);
  static void cb_digital_fl_input_output_count(Fl_Input*, void*);
public:
  Fl_Button *digital_fl_button_create_file;
private:
  inline void cb_digital_fl_button_create_file_i(Fl_Button*, void*);
  static void cb_digital_fl_button_create_file(Fl_Button*, void*);
public:
  LABSoft_GUI_LABChecker_Digital_Input_Table *labchecker_digital_labsoft_gui_labchecker_digital_input_table_table;
  Fl_Double_Window *oscilloscope_fl_window_record_config;
  Fl_Group *oscilloscope_fl_group_record_config;
  static Fl_Menu_Item menu_Samples[];
  Fl_Button *oscilloscope_fl_button_record_config_start;
private:
  inline void cb_oscilloscope_fl_button_record_config_start_i(Fl_Button*, void*);
  static void cb_oscilloscope_fl_button_record_config_start(Fl_Button*, void*);
public:
  Fl_Button *oscilloscope_fl_button_record_config_cancel;
private:
  inline void cb_oscilloscope_fl_button_record_config_cancel_i(Fl_Button*, void*);
  static void cb_oscilloscope_fl_button_record_config_cancel(Fl_Button*, void*);
public:
  Fl_Double_Window *oscilloscope_fl_window_export;
  static Fl_Menu_Item menu_Source[];
  Fl_Group *oscilloscope_export_fl_group_output;
  Fl_Button *oscilloscope_export_fl_button_copy_to_clipboard;
  Fl_Button *oscilloscope_export_fl_button_save;
  Fl_Button *oscilloscope_export_fl_button_cancel;
  Fl_Double_Window *main_fl_window_about;
private:
  inline void cb_Close_i(Fl_Button*, void*);
  static void cb_Close(Fl_Button*, void*);
};
#endif
