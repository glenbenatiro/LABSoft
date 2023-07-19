#ifndef LAB_VOLTMETER_H
#define LAB_VOLTMETER_H

#include <array>

#include "LAB_Core.h"
#include "../Utility/LAB_Constants.h"

// forward declare LAB, #include in .cpp file
class LAB;

class LAB_Voltmeter
{
  private:
    LAB*      m_LAB;
    LAB_Core* m_LAB_Core;
  
    bool  m_is_running       = false;
    bool  m_osc_chan_en_flag = false;

  public:
    std::array <double, LABC::OSC::NUMBER_OF_CHANNELS> m_samples = {0};

    LAB_Voltmeter (LAB_Core* _LAB_Core, LAB* _LAB);

    bool    is_running        ();
    void    run               ();
    void    stop              ();
    void    load_data_samples ();
};

#endif
