#include "LAB_MCP4XXX.h"

LAB_MCP4XXX::
LAB_MCP4XXX (MCP4XXX::PART_NUMBER part, MCP4XXX::RESISTANCE_VERSION resistance) 
  : MCP4XXX (part, resistance)
{

}

LAB_MCP4XXX::
~LAB_MCP4XXX ()
{

}   

void LAB_MCP4XXX::
init (void* controller, unsigned spi_channel)
{
  m_controller  = controller;
  m_spi_channel = spi_channel;
}

void LAB_MCP4XXX::
spi_xfer (char *rxd, char *txd, unsigned length)
{
  (static_cast<LAB_Core*>(m_controller))->aux.spi (m_spi_channel).xfer (rxd, txd, length);
}

// EOF