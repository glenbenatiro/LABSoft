#ifndef LAB_OHMMETER_H
#define LAB_OHMMETER_H

#include "LAB_Module.h"
#include "../Utility/LAB_Definitions.h"

class LAB_Ohmmeter : public LAB_Module 
{
  private:
    LAB_Parent_Data_Ohmmeter m_parent_data;

    class Calibration
    {
      private:
        LAB_Parent_Data_Ohmmeter& m_pdata;

      public:
        Calibration (LAB_Parent_Data_Ohmmeter& _LAB_Parent_Data_Ohmmeter);

        void r1     (unsigned channel, double value);
        void vref   (unsigned channel, double value);

        double r1   (unsigned channel) const;
        double vref (unsigned channel) const;
    } m_calibration;
  
  public:
    LAB_Ohmmeter (LAB& LAB);

    bool    is_frontend_running () const;
    bool    is_backend_running  () const;
    void    run                 ();
    void    stop                ();
    void    frontend_run_stop   (bool value);
    void    backend_run_stop    (bool value);
    double  get_reading         (unsigned channel);

    Calibration& calibration ();
};

#endif