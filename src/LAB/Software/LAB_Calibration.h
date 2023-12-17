#ifndef LAB_CALIBRATION_H
#define LAB_CALIBRATION_H

#include <string>
#include <unordered_map>

#include "../LAB_Module.h"

class LAB_Calibration : public LAB_Module
{
  private:
    std::string                             m_calibration_file_path_actual;
    std::string                             m_calibration_file_path_incoming;
    std::unordered_map<std::string, double> m_calibration_data_actual;
    std::unordered_map<std::string, double> m_calibration_data_incoming;

  private:
    void    load_calibration_file                       (const std::string& filename);
    void    apply_calibration_file                      ();
    void    apply_calibration_values                    ();
    void    reload_calibration_values_from_instruments  ();

    // debug
    void print_map_contents ();
  
  public:
    LAB_Calibration (LAB& _LAB);

    void    osc_adc_reference_voltage (double value);
    double  osc_adc_reference_voltage ();
    void    save_calibration_values_to_file ();
};

#endif