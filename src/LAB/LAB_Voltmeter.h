#ifndef LAB_VOLTMETER_H
#define LAB_VOLTMETER_H

#include "LAB_Module.h"

class LAB_Voltmeter : public LAB_Module
{
  private:
    bool m_is_backend_running   = false;
    bool m_is_frontend_running  = false;
  
  private:
    void load_oscilloscope_voltmeter_mode_settings ();

  public:
    LAB_Voltmeter (LAB& _LAB);

    bool    is_frontend_running () const;
    bool    is_backend_running  () const;
    void    run                 ();
    void    stop                ();
    void    frontend_run_stop   (bool value);
    void    backend_run_stop    (bool value);
    double  get_reading         (unsigned chan);
};

#endif
