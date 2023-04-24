#include "LAB_MCP4161_503.h"

#include <iostream>

LAB_MCP4161_503::
LAB_MCP4161_503 (LAB_Core *_LAB_Core, unsigned spi_chan, unsigned spi_baud)
{
  m_LAB_Core = _LAB_Core;
}

// --- change to fit LAB_Core

void LAB_MCP4161_503:: 
init ()
{
  // master initialization of the AUX SPI is done at 
  // LAB Function Generator cpp
}

void LAB_MCP4161_503:: 
spi_write (char *txdata, unsigned count)
{
  m_LAB_Core->aux_spi0_write (txdata, count);
}

// ---

void LAB_MCP4161_503:: 
set_resistance_aw (double value)
{
  set_resistance_wb (MCP4161_503_RESISTANCE - value);
}

void LAB_MCP4161_503:: 
set_resistance_wb (double value)
{
  if (value >= 0 && value <= MCP4161_503_RESISTANCE)
  {
    int scaler = 0;
    double resistance = 0;

    while ((resistance + MCP4161_503_WIPER_RESISTANCE) < value)
    {
      resistance = MCP4161_503_STEP_RESISTANCE * scaler++;
    }

    std::cout << "wiper data to write: " << scaler << "\n";

    write16 (MCP4161_503_ADDR_V_WIPER0, scaler);
  }
}

void LAB_MCP4161_503:: 
write16 (MCP4161_503_ADDR _MCP4161_503_ADDR, uint16_t data)
{
  uint16_t rxdata_temp = (_MCP4161_503_ADDR << 12) | 
    (MCP4161_503_CMD_WRITE << 10) | static_cast<uint16_t>(data);

  char txdata[2] = 
  {
    rxdata_temp & 0xff,
    (rxdata_temp >> 8) & 0xff
  };
  
  spi_write (txdata, 2);

}

// EOF 