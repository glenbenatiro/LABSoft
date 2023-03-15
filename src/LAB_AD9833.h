#ifndef LAB_AD9833_H
#define LAB_AD9833_H

#include "Defaults.h"
#include "LAB_Core.h"

constexpr int LAB_FUNCTION_GENERATOR_FUNCTION_GENERATOR_IC_MOSI   = 2;
constexpr int LAB_FUNCTION_GENERATOR_FUNCTION_GENERATOR_IC_MISO   = 3;
constexpr int LAB_FUNCTION_GENERATOR_FUNCTION_GENERATOR_IC_SCLK   = 4;
constexpr int LAB_FUNCTION_GENERATOR_FUNCTION_GENERATOR_IC_CS     = 14;
constexpr int LAB_FUNCTION_GENERATOR_FUNCTION_GENERATOR_IC_BAUD   = 10'000;
constexpr int LAB_FUNCTION_GENERATOR_FUNCTION_GENERATOR_IC_FLAGS  = 2;

class LAB_AD9833
{
  private: 
    LAB_Core *m_LAB_Core;

    int       DEBUG = 1;

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
    void  update_ctrl_reg ();
    void  write_reg ();

    void  wave_type   (LE_WAVE_TYPE _WAVETYPE);
    void  frequency   (double frequency);
    void  period      (double value);
    void  duty_cycle  (double duty_cycle);
    void  phase       (double phase);

    void sel_freq_reg   (bool value);
    void sel_phase_reg  (bool value);
    void set_HLB        (bool value);
    void set_write_mode (bool value);
};

#endif 