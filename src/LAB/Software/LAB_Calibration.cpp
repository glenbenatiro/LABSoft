#include "LAB_Calibration.h"

#include "../LAB.h"
#include "../../Utility/LAB_Constants.h"

#include <string>
#include <fstream>

LAB_Calibration:: 
LAB_Calibration (LAB& _LAB)
  : LAB_Module (_LAB)
{
  load_init_calibration_values ();
}

void LAB_Calibration:: 
load_init_calibration_values ()
{
  load_values_from_default_file ();
}

void LAB_Calibration:: 
load_values_from_default_file ()
{
  std::string file_path = std::string (LABC::LABSOFT::DEFAULT_CALIBRATION_FILE_PATH);

  std::ifstream file (file_path);

  if (file.is_open ())
  {

  }
  else 
  {
    throw (std::runtime_error ("cannot find default calibration file"));
  }
}

void LAB_Calibration:: 
osc_adc_reference_voltage (double value)
{
  lab ().m_Oscilloscope.adc_reference_voltage (value);
}

double LAB_Calibration:: 
osc_adc_reference_voltage ()
{
  return (lab ().m_Oscilloscope.adc_reference_voltage ());
}

// eof