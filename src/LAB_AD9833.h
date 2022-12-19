#ifndef LAB_AD9833_H
#define LAB_AD9833_H

#include "LAB_Core.h"

class LAB_AD9833
{
  private: 
    LAB_Core *m_LAB_Core;

    unsigned  m_MOSI,
              m_MISO,
              m_SCLK,
              m_CS;
    
    const double AD9833_MAX_FREQUENCY = 12'500'000; // Hz
    const double AD9833_MIN_FREQUENCY = 0.1;        // Hz
    const int    AD9833_REF_CLK_HZ    = 25'000'000; 
    const int    AD9833_2_POW_28      = 268'435'456;

    // AD9833 Control Register
    uint16_t  m_B28     = 0,
              m_HLB     = 0,
              m_FSELECT = 0,
              m_PSELECT = 0,
              m_Reset   = 0,
              m_SLEEP1  = 0,
              m_SLEEP12 = 0,
              m_OPBITEN = 0,
              m_DIV2    = 0,
              m_Mode    = 0;
    
    char      m_txbuff[2] = {0x00, 0x00},
              m_rxbuff[2] = {0x00, 0x00};

  public: 
    //
          LAB_AD9833 (LAB_Core *_LAB_Core);

    void  init ();
    void  start ();
    void  stop ();
    void  update ();

    void  wave_type   (WaveType _WaveType);
    void  frequency   (double frequency);
    void  duty_cycle  (double duty_cycle);
    void  phase       (double phase);

    void sel_freq_reg   (bool value);
    void sel_phase_reg  (bool value);
    void set_HLB        (bool value);
    void set_write_mode (bool value);
};

#endif 