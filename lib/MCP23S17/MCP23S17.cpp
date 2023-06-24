#include "MCP23S17.h"

MCP23S17:: 
MCP23S17 (uint8_t hw_addr_bits)
{

}

void MCP23S17:: 
spi_read (char*     rxd, 
          unsigned  length)
{
  char txd[length];

  spi_xfer (rxd, txd, length);
}

void MCP23S17:: 
spi_write (char*    txd,
           unsigned length)
{
  char rxd[length];

  spi_xfer (rxd, txd, length);
}

// Setter
void MCP23S17:: 
hardware_address (uint8_t hw_addr_bits)
{
  m_hardware_address = CLIENT_ADDRESS_HEADER | hw_addr_bits;
}

uint8_t MCP23S17:: 
hardware_address () const
{
  return (m_hardware_address);
}

uint8_t MCP23S17:: 
opcode () const
{
  return (m_opcode);
}

void MCP23S17:: 
direction (MCP23S17::PORT       ort,
           unsigned             pin,
           MCP23S17::DIRECITON  direction)
{

}


uint8_t MCP23S17:: 
read_port (MCP23S17::PORT port)
{



  uint8_t data;

  spi_read_mcp ()
}

void MCP23S17:: 
write_port (MCP23S17::PORT  port,
            uint8_t         data)
{
  bool    port_bool = static_cast<bool>(port);
  uint8_t addr      = port_bool ? MCP23S17::ADDR0::GPIOB : MCP23S17::ADDR0::GPIOA;

  spi_write_mcp (data, 1);
}