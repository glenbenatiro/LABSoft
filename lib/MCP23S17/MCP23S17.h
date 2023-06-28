#ifndef MCP23S17_H
#define MCP23S17_H

#include <cstdint>

class MCP23S17
{
  public:
    enum class PORT
    {
      A = 0,
      B = 1
    };

  private:
    enum class DIRECTION
    {
      OUTPUT  = 0,
      INPUT   = 1
    };

    enum class ADDR0 // ADDR_IOCON_BANK_0
    {
      IODIRA    = 0x00,
      IODIRB    = 0x01,
      IPOLA     = 0x02,
      IPOLB     = 0x03,
      GPINTENA  = 0x04,
      GPINTENB  = 0x05,
      DEFVALA   = 0x06,
      DEFVALB   = 0x07,
      INTCONA   = 0x08,
      INTCONB   = 0x09,
      IOCON     = 0x0A,  
  //  IOCON     = 0x0B // IOCON is shared between ports A and B
      GPPUA     = 0x0C,
      GPPUB     = 0x0D,
      INTFA     = 0x0E,
      INTFB     = 0x0C,
      INTCAPA   = 0x10,
      INTCAPB   = 0x11,
      GPIOA     = 0x12,
      GPIOB     = 0x13,
      OLATA     = 0x14,
      OLATB     = 0x15
    };

    const uint8_t CLIENT_ADDRESS_HEADER = 0x20;
  
  private:
    uint8_t m_hardware_address;
    uint8_t m_opcode;

  private:
    void spi_read       (char* rxd, unsigned length);
    void spi_write      (char* txd, unsigned length);
    void spi_write_mcp  (char* rxd, unsigned length);

  protected:
    virtual void spi_xfer (char* rxd, char* txd, unsigned length) = 0;

  public:
    MCP23S17 (uint8_t hw_addr_bits);

    // Setter
    void hardware_address (uint8_t hw_addr_bits);

    // Getter
    uint8_t hardware_address  () const;
    uint8_t opcode            () const;

    void direction (PORT port, unsigned pin, DIRECTION direction);

    uint8_t read_port   (PORT port);
    void    write_port  (PORT port, uint8_t data);
};

#endif