#include "LAB_MCP4XXX.h"

LAB_MCP4XXX::
LAB_MCP4XXX (MCP4XXX::PART_NUMBER part, MCP4XXX::RESISTANCE_VERSION resistance, unsigned spi_channel) 
  : MCP4XXX (part, resistance)
{
  m_SPI_channel = spi_channel;
}

LAB_MCP4XXX::
~LAB_MCP4XXX ()
{

}   

void LAB_MCP4XXX::
spi_xfer (char* rxd, char* txd, unsigned length)
{
  AikaPi::aux.spi (m_SPI_channel).xfer (rxd, txd, length);
}

// EOF