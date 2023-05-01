#ifndef LAB_MCP4XXX_H
#define LAB_MCP4XXX_H

#include "../lib/MCP4XXX/MCP4XXX.h"
#include "LAB_Core.h"

class LAB_MCP4XXX : public MCP4XXX
{
  // https://ww1.microchip.com/downloads/en/DeviceDoc/22059b.pdf

  private:

  public:
    LAB_MCP4XXX (MCP4XXX::PART_NUMBER part, MCP4XXX::RESISTANCE_VERSION resistance);
   ~LAB_MCP4XXX ();

   void init      (void* controller, unsigned spi_channel = 1);
   void spi_xfer  (char *rxd, char *txd, unsigned length);
};

#endif

// EOF