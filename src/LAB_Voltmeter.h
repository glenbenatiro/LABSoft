#ifndef LAB_VOLTMETER_H
#define LAB_VOLTMETER_H

#include "LAB_Core.h"
#include "Defaults.h"

// forward declare LAB, #include "LAB.h" in Voltmeter cpp file
class LAB;

class LAB_Voltmeter
{
  private:
    LAB_Core  *m_LAB_Core;
    LAB       *m_LAB;

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
