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
update_data_display ()
{
  char label[20];

  // osc
  std::snprintf (
    label, 
    sizeof (label), 
    "%.3f", 
    lab ().m_Oscilloscope.calibration ().adc_reference_voltage (0)
  );
  
  gui ().calibration_fl_input_osc_chan_0_adc_vref->value (label);

  std::snprintf (
    label, 
    sizeof (label), 
    "%.3f", 
    lab ().m_Oscilloscope.calibration ().adc_reference_voltage (1)
  );
  
  gui ().calibration_fl_input_osc_chan_1_adc_vref->value (label);

  std::snprintf (
    label, 
    sizeof (label), 
    "%.3f", 
    lab ().m_Oscilloscope.calibration ().scaling_corrector_half_to_unity (0)
  );
  
  gui ().calibration_fl_input_osc_chan_0_scaling_corrector_half_to_unity->value (label);

  std::snprintf (
    label, 
    sizeof (label), 
    "%.3f", 
    lab ().m_Oscilloscope.calibration ().scaling_corrector_half_to_unity (1)
  );
  
  gui ().calibration_fl_input_osc_chan_1_scaling_corrector_half_to_unity->value (label);

  // ohm

  std::snprintf (
    label, 
    sizeof (label), 
    "%.3f", 
    lab ().m_Ohmmeter.calibration ().vref (0)
  );
  
  gui ().calibration_fl_input_chan_0_ohm_vref->value (label);
  
  std::snprintf (
    label, 
    sizeof (label), 
    "%.3f", 
    lab ().m_Ohmmeter.calibration ().r1 (0)
  );
  
  gui ().calibration_fl_input_chan_0_ohm_r1->value (label);
}

void LABSoft_Presenter_Calibration::
cb_show_window ()
{
  gui ().main_fl_window_calibration->set_modal ();
  gui ().main_fl_window_calibration->show ();

  update_data_display ();
}

void LABSoft_Presenter_Calibration::
cb_cancel ()
{
  gui ().main_fl_window_calibration->hide ();
  gui ().main_fl_window_calibration->clear_modal_states ();
}

void LABSoft_Presenter_Calibration::
cb_save_to_file ()
{
  lab ().m_Calibration.save_calibration_values_to_file ();

  fl_message("Calibration values successfully saved to file.");
}

void LABSoft_Presenter_Calibration:: 
cb_osc_adc_vref (Fl_Input* w, long data)
{
  try 
  {
    double value = std::stod (std::string (w->value ()));

    lab ().m_Oscilloscope.calibration ().adc_reference_voltage (data, value);
  }
  catch (const std::exception& e)
  {

  }

  char label[20];

  std::snprintf (
    label, 
    sizeof (label), 
    "%.3f", 
    lab ().m_Oscilloscope.calibration ().adc_reference_voltage (data)
  );
  
  w->value (label);
}

void LABSoft_Presenter_Calibration:: 
cb_osc_scaling_corrector_half_to_unity (Fl_Input* w, long data)
{
  try 
  {
    double value = std::stod (std::string (w->value ()));

    lab ().m_Oscilloscope.calibration ().scaling_corrector_half_to_unity (data, value);
  }
  catch (const std::exception& e)
  {

  }

  char label[20];

  std::snprintf (
    label, 
    sizeof (label), 
    "%.3f", 
    lab ().m_Oscilloscope.calibration ().scaling_corrector_half_to_unity (data)
  );
  
  w->value (label);
}

void LABSoft_Presenter_Calibration:: 
cb_ohm_vref (Fl_Input* w, long data)
{
  try 
  {
    double value = std::stod (std::string (w->value ()));

    lab ().m_Ohmmeter.calibration ().vref (data, value);
  }
  catch (const std::exception& e)
  {

  }

  char label[20];

  std::snprintf (
    label, 
    sizeof (label), 
    "%.3f", 
    lab ().m_Ohmmeter.calibration ().vref (data)
  );
  
  w->value (label);
}

void LABSoft_Presenter_Calibration:: 
cb_ohm_r1 (Fl_Input* w, long data)
{
  try 
  {
    double value = std::stod (std::string (w->value ()));

    lab ().m_Ohmmeter.calibration ().r1 (data, value);
  }
  catch (const std::exception& e)
  {

  }

  char label[20];

  std::snprintf (
    label, 
    sizeof (label), 
    "%.3f", 
    lab ().m_Ohmmeter.calibration ().r1 (data)
  );
  
  w->value (label);
}
