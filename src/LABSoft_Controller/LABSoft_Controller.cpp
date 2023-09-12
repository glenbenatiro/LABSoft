#include "LABSoft_Controller.h"

#include <iostream>

#include <FL/Fl.H>
#include <FL/Fl_Tabs.H>

#include "../Utility/LAB_Constants.h"

LABSoft_Controller:: 
LABSoft_Controller (LAB& _LAB, LABSoft_GUI& _LABSoft_GUI)
  : m_LAB                     (_LAB),
    m_LABSoft_GUI             (_LABSoft_GUI),
    m_Oscilloscope            (*this),
    m_Voltmeter               (*this),
    m_Function_Generator      (*this),
    m_Logic_Analyzer          (*this),
    m_Digital_Circuit_Checker (*this),
    m_Main_Window             (*this),
    m_LABChecker_Digital      (*this)
{
  load_controller_to_gui ();

  Fl::add_timeout (LABC::LABSOFT::DISPLAY_UPDATE_RATE, update_display, this);
}

void LABSoft_Controller::
load_controller_to_gui ()
{
  // 1. 
  m_LABSoft_GUI.m_LABSoft_Controller = this;

  // 2. 
  m_LABSoft_GUI.oscilloscope_labsoft_gui_oscilloscope_display->
    load_controller (*this);

  // 3.
  m_LABSoft_GUI.logic_analyzer_labsoft_gui_logic_analyzer_display-> 
    load_controller (*this);
}

void LABSoft_Controller:: 
update_gui_tab_colors ()
{
  // 63 is green
  // 52 is inactive tab gray
  // 54 is active tab gray
  
  static LABSoft_GUI& gui   = m_LABSoft_GUI;
  static Fl_Tabs&     tabs  = *(gui.main_fl_tabs);

  // 1. oscilloscope
  gui.main_fl_group_oscilloscope_tab->selection_color (
    m_LAB.m_Oscilloscope.is_running () ? 63 : 52
  );

  // 2. voltmeter
  gui.main_fl_group_voltmeter_tab->selection_color (
    m_LAB.m_Voltmeter.is_running () ? 63 : 52
  );

  // 3. function generator
  gui.main_fl_group_function_generator_tab->selection_color (
    m_LAB.m_Function_Generator.is_running () ? 63 : 52
  );

  // 4. logic analyzer
  gui.main_fl_group_logic_analyzer_tab->selection_color (
    m_LAB.m_Logic_Analyzer.is_running () ? 63 : 52
  );

  // 5. logic analyzer
  gui.main_fl_group_digital_circuit_checker_tab->selection_color (
    m_LAB.m_Digital_Circuit_Checker.is_running () ? 63 : 52
  );

  tabs.selection_color  (tabs.value ()->selection_color () == 63 ? 63 : 54);
  tabs.redraw           ();
}

void LABSoft_Controller::
update_display (void *data)
{
  LABSoft_Controller& controller = *((LABSoft_Controller*)(data));

  controller.update_gui_tab_colors                  ();
  
  controller.m_Oscilloscope.display_update_cycle    ();
  controller.m_Voltmeter.display_update_cycle       (); 
  controller.m_Logic_Analyzer.display_update_cycle  ();

  Fl::repeat_timeout (LABC::LABSOFT::DISPLAY_UPDATE_RATE, update_display, data);  
}

void LABSoft_Controller:: 
cb_tabs (Fl_Group* w, void* data)
{
  update_gui_tab_colors ();
}

LAB& LABSoft_Controller:: 
lab () const
{
  return (m_LAB);
}

LABSoft_GUI& LABSoft_Controller::
gui () const
{
  return (m_LABSoft_GUI);
}

