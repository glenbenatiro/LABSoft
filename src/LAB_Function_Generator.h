#ifndef LAB_FUNCTION_GENERATOR
#define LAB_FUNCTION_GENERATOR

#include "LAB_Core.h"
#include "Channel_Signal.h"
#include "Defaults.h"

class LAB_Function_Generator 
{
  public:
    bool            m_is_running = false;
    LAB_Core       *m_LAB_Core;
    Channel_Signal  m_channel_signal[LAB_FUNCTION_GENERATOR_NUMBER_OF_CHANNELS];

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
    // --- functions ---
         LAB_Function_Generator (LAB_Core *_LAB_Core);
    void start  (int channel);
    void stop   (int channel);
    void update ();
    void init   ();
  
    // setters
    void frequency  (int channel, double frequency);
    void period     (int channel, double period);
    void wave_type  (int channel, WaveType _WaveType);
    void amplitude  (int channel, double amplitude);
    void offset     (int channel, double offset);
    void phase      (int channel, double phase);

    // --- utility ---
    void sel_freq_reg   (bool value);
    void sel_phase_reg  (bool value);
    void set_HLB        (bool value);
    void set_write_mode (bool value);
};

#endif
