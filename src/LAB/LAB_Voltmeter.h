#ifndef LAB_VOLTMETER_H
#define LAB_VOLTMETER_H

#include <array>

#include "LAB_Module.h"
#include "../Utility/LAB_Constants.h"

class LAB_Voltmeter : public LAB_Module
{
  private:
    bool  m_is_running       = false;
    bool  m_osc_chan_en_flag = false;

  public:
    std::array <double, LABC::OSC::NUMBER_OF_CHANNELS> m_samples = {0};
  
  public:
    LAB_Voltmeter (LAB& _LAB);

    bool  is_running        ();
    void  run               ();
    void  stop              ();
    void  load_data_samples ();
};

#endif
