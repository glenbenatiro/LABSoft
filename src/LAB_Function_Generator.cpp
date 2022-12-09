#include "LAB_Function_Generator.h"

#include <cstdio>

#include "Defaults.h"
#include "Auxiliary.h"

LAB_Function_Generator::
LAB_Function_Generator (LAB_Core *_LAB_Core) 
{
  m_LAB_Core = _LAB_Core;
  init ();
}

void LAB_Function_Generator:: 
start (int channel)
{
  if (channel < LAB_FUNCTION_GENERATOR_NUMBER_OF_CHANNELS && channel >= 0)
  {
    m_channel_signal[channel].enable ();
  }
}

void LAB_Function_Generator:: 
stop (int channel)
{
  if (channel < LAB_FUNCTION_GENERATOR_NUMBER_OF_CHANNELS && channel >= 0)
  {
    m_channel_signal[channel].disable ();
  }
}

void LAB_Function_Generator:: 
init ()
{
  stop ();

  set_wavetype (SINE);
  set_frequency (500);
  // set_write_mode (1);

  start ();
}

void LAB_Function_Generator:: 
update ()
{
  m_txbuff[0] = 0x00 | (m_B28 << 5) | (m_HLB << 4) | (m_FSELECT << 3) |
    (m_PSELECT << 2) | m_Reset;
        
  m_txbuff[1] = 0x00 | (m_SLEEP1 << 7) | (m_SLEEP12 << 6) | 
    (m_OPBITEN << 5) | (m_DIV2 << 3) | (m_Mode << 1);

  m_LAB_Core->aux_spi_write (SIG_GEN_CE_PIN, m_txbuff, 2);
}

void LAB_Function_Generator::
frequency (int    channel, 
           double frequency)
{
  if (m_channel_signal[channel].is_enabled ())
  {
    if ((frequency <= SIG_GEN_MAX_FREQ) && (frequency >= SIG_GEN_MIN_FREQ))
    {
      uint32_t divider = ((frequency * SIG_GEN_2_POW_28) / 
      static_cast<double>(SIG_GEN_REF_CLOCK_HZ)) & 0x0FFFFFFF;

      if (m_B28 == 1)
      {
        m_txbuff[0] = 0x00 | ((m_FSELECT + 1) << 6) | ((divider >> 8) & 0x3F);
        m_txbuff[1] = divider & 0xFF;
        update ();

        divider >>= 14;

        m_txbuff[0] = 0x00 | ((m_FSELECT + 1) << 6) | ((divider >> 8) & 0x3F);
        m_txbuff[1] = divider & 0xFF;
        update ();
      } 
      else if (m_B28 == 0)
      {
        set_HLB (0);
        m_txbuff[0] = 0x00 | ((m_FSELECT + 1) << 6) | ((divider >> 8) & 0x3F);
        m_txbuff[1] = divider & 0xFF;
        update ();

        divider >>= 14;

        set_HLB (1);
        m_txbuff[0] = 0x00 | ((m_FSELECT + 1) << 6) | ((divider >> 8) & 0x3F);
        m_txbuff[1] = divider & 0xFF;
        update ();
      }
    }
  }
}

void LAB_Function_Generator::
period (int channel, double period)
{
  frequency (channel, (1.0 / period));
}

void LAB_Function_Generator::
wave_type  (int channel, WaveType _WaveType)
{
  switch (_WaveType)
  {
    case SINE:
      m_OPBITEN = 0;
      m_DIV2 = 0;
      m_Mode = 0;
      break;
    case TRIANGLE:
      m_OPBITEN = 0;
      m_DIV2 = 0;
      m_Mode = 1;
      break;
    case SQUARE_FULL:
      m_OPBITEN = 1;
      m_DIV2 = 1;
      m_Mode = 0;
      break;
    case SQUARE_HALF:
      m_OPBITEN = 1;
      m_DIV2 = 0;
      m_Mode = 0;
      break;
    default:
      set_wavetype (SINE);
      break;
  }

  update ();
}

void LAB_Function_Generator::
amplitude  (int    channel,
            double amplitude)
{

}

void LAB_Function_Generator::
offset (int    channel, 
        double offset)
{

}

void LAB_Function_Generator:: 
phase (int    channel,
       double phase)
{

}



// --- utility --- 
void LAB_Function_Generator::
sel_freq_reg (bool value)
{
  m_FSELECT = value;
  update ();
}

void LAB_Function_Generator::
sel_phase_reg (bool value)
{
  m_PSELECT = value;
  update ();
}

void set_HLB (bool value)
{
  m_HLB = value;
  update ();
}

void set_write_mode (bool value)
{
  // 1 = complete write through 2 consecutive writes
  // 0 = freq reg is split into MSB and LSB
  m_B28 = value;
  update ();
}

// EOFs