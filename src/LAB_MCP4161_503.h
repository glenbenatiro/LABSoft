#ifndef LAB_MCP4161_503_H
#define LAB_MCP4161_503_H

#include "LAB_Core.h"
#include "Defaults.h"

class LAB_MCP4161_503
{
  private:
    const double    MCP4161_503_RESISTANCE        = 50'000.0; // in ohms
    const unsigned  MCP4161_503_STEPS             = 256;
    const double    MCP4161_503_STEP_RESISTANCE   = MCP4161_503_RESISTANCE / MCP4161_503_STEPS;
    const double    MCP4161_503_WIPER_RESISTANCE  = 75; // in ohms

    enum MCP4161_503_ADDR 
    {
      MCP4161_503_ADDR_V_WIPER0   = 0x0,
      MCP4161_503_ADDR_NV_WIPER0  = 0x2
    };

    enum MCP4161_503_CMD
    {
      MCP4161_503_CMD_WRITE = 0,
      MCP4161_503_CMD_INCR  = 1,
      MCP4161_503_CMD_DECR  = 2,
      MCP4161_503_CMD_READ  = 3
    };

 
    unsigned m_spi_channel;
    LAB_Core *m_LAB_Core;

  public:
    LAB_MCP4161_503 (LAB_Core *_LAB_Core, unsigned spi_chan, unsigned spi_baud);

    void init ();  
    void set_resistance_aw (double value);
    void set_resistance_wb (double value);
    void write16 (MCP4161_503_ADDR _MCP4161_503_ADDR, uint16_t data);

    void spi_write (char *txdata, unsigned count);
};

#endif

// EOF