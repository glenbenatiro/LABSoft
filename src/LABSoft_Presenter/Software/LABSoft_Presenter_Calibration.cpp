#include "LABSoft_Presenter_Calibration.h"

#include <cstdio>
#include <string>

#include "../../LAB/LAB.h"
#include "../LABSoft_Presenter.h"
#include "../../LABSoft_GUI/LABSoft_GUI.h"

LABSoft_Presenter_Calibration::
LABSoft_Presenter_Calibration (LABSoft_Presenter& _LABSoft_Presenter)
  : LABSoft_Presenter_Unit  (_LABSoft_Presenter)
{

}

void LABSoft_Presenter_Calibration:: 
update_data ()
{
  char label[20];

  // osc
  // adc reference voltage 
  std::snprintf (label, sizeof (label), "%.3f", 
    lab ().m_Calibration.osc_adc_reference_voltage ());
  
  gui ().calibration_fl_input_osc_adc_ref_voltage->value (label);
}

void LABSoft_Presenter_Calibration::
cb_show_window ()
{
  gui ().main_fl_window_calibration->set_modal ();
  gui ().main_fl_window_calibration->show ();

  update_data ();
}

void LABSoft_Presenter_Calibration::
cb_cancel ()
{
  gui ().main_fl_window_calibration->hide ();
  gui ().main_fl_window_calibration->clear_modal_states ();
}

void LABSoft_Presenter_Calibration::
cb_save ()
{

}

void LABSoft_Presenter_Calibration:: 
cb_osc_adc_ref_voltage (Fl_Input* w, 
                        void*     data)
{
  try
  {
    double value = std::stod (std::string (w->value ()));

    lab ().m_Calibration.osc_adc_reference_voltage (value);
  }
  catch (const std::exception& e)
  {
    
  }
  
  update_data ();
}
