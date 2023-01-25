#include "LAB_AD9833.h"

#include <cstdio>

LAB_AD9833::
LAB_AD9833 (LAB_Core *_LAB_Core): m_LAB_Core (_LAB_Core)
{
  m_CS    = LAB_FUNCTION_GENERATOR_FUNCTION_GENERATOR_IC_CS;
  m_MISO  = LAB_FUNCTION_GENERATOR_FUNCTION_GENERATOR_IC_MISO;
  m_MOSI  = LAB_FUNCTION_GENERATOR_FUNCTION_GENERATOR_IC_MOSI;
  m_SCLK  = LAB_FUNCTION_GENERATOR_FUNCTION_GENERATOR_IC_SCLK;

  init ();
}

void LAB_AD9833:: 
init ()
{
  // initialize SPI connection with LAB_Core
  m_LAB_Core->bb_spi_open (
    m_CS,
    m_MISO,
    m_MOSI,
    m_SCLK,
    LAB_FUNCTION_GENERATOR_FUNCTION_GENERATOR_IC_BAUD,
    LAB_FUNCTION_GENERATOR_FUNCTION_GENERATOR_IC_FLAGS 
  );

  // initialize actual chip
  stop ();

  wave_type (TRIANGLE);
  frequency (250);

  // start ();
}

void LAB_AD9833:: 
wave_type (WaveType _WaveType)
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
    case SQUARE:
      wave_type (SQUARE_FULL);
      break;
    case DC:
      // add here code for DC signal
      break;
    default:
      wave_type (SINE);
      break;
  }

  update_ctrl_reg ();
}

void LAB_AD9833:: 
frequency (double frequency)
{
  printf ("freq labad9833: %f\n", frequency);

  if ((frequency <= AD9833_MAX_FREQUENCY) && (frequency >= AD9833_MIN_FREQUENCY))
  {
    uint32_t divider = ((frequency * AD9833_2_POW_28) / 
    static_cast<double>(AD9833_REF_CLK_HZ));
    divider &= 0x0FFFFFFF;

    if (m_B28 == 1)
    {
      m_txbuff[0] = 0x00 | ((m_FSELECT + 1) << 6) | ((divider >> 8) & 0x3F);
      m_txbuff[1] = divider & 0xFF;
      write_reg ();

      divider >>= 14;

      m_txbuff[0] = 0x00 | ((m_FSELECT + 1) << 6) | ((divider >> 8) & 0x3F);
      m_txbuff[1] = divider & 0xFF;
      write_reg ();
    } 
    else if (m_B28 == 0)
    {
      set_HLB (0);
      m_txbuff[0] = 0x00 | ((m_FSELECT + 1) << 6) | ((divider >> 8) & 0x3F);
      m_txbuff[1] = divider & 0xFF;
      write_reg ();

      divider >>= 14;

      set_HLB (1);
      m_txbuff[0] = 0x00 | ((m_FSELECT + 1) << 6) | ((divider >> 8) & 0x3F);
      m_txbuff[1] = divider & 0xFF;
      write_reg ();
    }
  }
}

void LAB_AD9833:: 
duty_cycle (double duty_cycle)
{

}

void LAB_AD9833:: 
phase (double phase)
{

}

//



void LAB_AD9833:: 
start ()
{
  m_Reset = 0;
  update_ctrl_reg ();
}

void LAB_AD9833:: 
stop () 
{
  m_Reset = 1;
  update_ctrl_reg ();
}

void LAB_AD9833:: 
update_ctrl_reg ()
{
  m_txbuff[0] = 0x00 | (m_B28 << 5) | (m_HLB << 4) | (m_FSELECT << 3) |
    (m_PSELECT << 2) | m_Reset;
        
  m_txbuff[1] = 0x00 | (m_SLEEP1 << 7) | (m_SLEEP12 << 6) | 
    (m_OPBITEN << 5) | (m_DIV2 << 3) | (m_Mode << 1);

  write_reg ();
}

void LAB_AD9833:: 
write_reg ()
{
  if (DEBUG)
    printf ("txbuff[0] txbuff[1]: %02x %02x\n", m_txbuff[0], m_txbuff[1]);

  m_LAB_Core->bb_spi_xfer (m_CS, m_txbuff, 2);
}

//

// --- utility --- 
void LAB_AD9833::
sel_freq_reg (bool value)
{
  m_FSELECT = value;
  update_ctrl_reg ();
}

void LAB_AD9833::
sel_phase_reg (bool value)
{
  m_PSELECT = value;
  update_ctrl_reg ();
}

void LAB_AD9833::
set_HLB (bool value)
{
  m_HLB = value;
  update_ctrl_reg ();
}

void LAB_AD9833::
set_write_mode (bool value)
{
  // 1 = complete write through 2 consecutive writes
  // 0 = freq reg is split into MSB and LSB
  m_B28 = value;
  update_ctrl_reg ();
}