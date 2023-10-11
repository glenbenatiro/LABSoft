#ifndef LAB_CALIBRATION_H
#define LAB_CALIBRATION_H

#include "../LAB_Module.h"

class LAB_Calibration : public LAB_Module
{
  private:
    void load_init_calibration_values   ();
    void load_values_from_default_file  ();

  public:
    LAB_Calibration (LAB& _LAB);

    void    osc_adc_reference_voltage (double value);
    double  osc_adc_reference_voltage ();
};

#endif