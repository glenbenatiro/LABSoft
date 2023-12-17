#include "LAB_Calibration.h"

#include "../LAB.h"
#include "../../Utility/LAB_Constants.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

LAB_Calibration:: 
LAB_Calibration (LAB& _LAB)
  : LAB_Module (_LAB)
{
  load_calibration_file     (LABC::LABSOFT::DEFAULT_CALIBRATION_FILE_PATH);
  apply_calibration_file    ();
  apply_calibration_values  ();

  // print_map_contents ();
}

void LAB_Calibration:: 
load_calibration_file (const std::string& filename)
{
  std::unordered_map<std::string, double> calibration_data;

  std::ifstream file (filename);

  if (file.is_open ())
  {
    std::string line;

    while (std::getline (file, line))
    {
      std::istringstream  iss (line);
      std::string         key;
      double              value;

      // skip lines starting with pound sign, it is a comment
      if (line[0] == '#')
      {
        continue;
      }
      else 
      {
        size_t equal_sign_pos = line.find ('=');

        if (equal_sign_pos != std::string::npos)
        {
          key   = line.substr (0, equal_sign_pos);
          value = std::stod (line.substr (equal_sign_pos + 1));

          calibration_data[key] = value;
        }
      }
    }

    file.close ();

    m_calibration_data_incoming       = calibration_data;
    m_calibration_file_path_incoming  = filename;
  }
  else 
  {
    std::string error = "Cannot find/load calibration file in path provided:  " + filename;

    throw (std::runtime_error (error));
  }
}

void LAB_Calibration::
apply_calibration_file ()
{
  m_calibration_data_actual       = m_calibration_data_incoming;
  m_calibration_file_path_actual  = m_calibration_file_path_incoming;
}

void LAB_Calibration:: 
apply_calibration_values ()
{
  // osc
  lab ().m_Oscilloscope.calibration ().adc_reference_voltage (0, 
    m_calibration_data_actual.at ("osc_chan_0_adc_vref"));

  lab ().m_Oscilloscope.calibration ().adc_reference_voltage (1, 
    m_calibration_data_actual.at ("osc_chan_1_adc_vref"));

  lab ().m_Oscilloscope.calibration ().scaling_corrector_half_to_unity (0,
    m_calibration_data_actual.at ("osc_chan_0_scaling_corrector_half_to_unity"));

  lab ().m_Oscilloscope.calibration ().scaling_corrector_half_to_unity (1,
    m_calibration_data_actual.at ("osc_chan_1_scaling_corrector_half_to_unity"));

  // ohmmeter
  lab ().m_Ohmmeter.calibration ().r1 (0, 
    m_calibration_data_actual.at ("ohm_chan_0_r1"));
  
  lab ().m_Ohmmeter.calibration ().vref (0,
    m_calibration_data_actual.at ("ohm_chan_0_vref"));
}

// debug
void LAB_Calibration:: 
print_map_contents ()
{
  for (const auto& pair : m_calibration_data_actual)
  {
    std::cout << pair.first << " = " << pair.second << std::endl;
  }
}

void LAB_Calibration:: 
save_calibration_values_to_file ()
{
  std::ofstream file (m_calibration_file_path_actual);

  reload_calibration_values_from_instruments ();

  if (file.is_open ())
  {
    for (const auto& pair :m_calibration_data_actual)
    {
      file << pair.first << "=" << pair.second << "\n";
    }

    file.close ();
  }
  else 
  {
    throw ("Cannot find/open calibration file in path: " + m_calibration_file_path_actual);
  }
}

void LAB_Calibration::
reload_calibration_values_from_instruments ()
{
  // osc
  m_calibration_data_actual.at ("osc_chan_0_adc_vref") = 
    lab ().m_Oscilloscope.calibration ().adc_reference_voltage (0);

  m_calibration_data_actual.at ("osc_chan_1_adc_vref") = 
    lab ().m_Oscilloscope.calibration ().adc_reference_voltage (1);

  m_calibration_data_actual.at ("osc_chan_0_scaling_corrector_half_to_unity") = 
    lab ().m_Oscilloscope.calibration ().scaling_corrector_half_to_unity (0);
  
  m_calibration_data_actual.at ("osc_chan_1_scaling_corrector_half_to_unity") = 
    lab ().m_Oscilloscope.calibration ().scaling_corrector_half_to_unity (1);

  // ohmmeter
  m_calibration_data_actual.at ("ohm_chan_0_r1") = 
    lab ().m_Ohmmeter.calibration ().r1 (0);
    
  m_calibration_data_actual.at ("ohm_chan_0_vref") = 
    lab ().m_Ohmmeter.calibration ().vref (0);  
}

// eof