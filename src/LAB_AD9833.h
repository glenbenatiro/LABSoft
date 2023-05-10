#ifndef LAB_AD9833_H
#define LAB_AD9833_H

#include "../lib/AD9833/AD9833.h"
#include "LAB_Core.h"
#include "Defaults.h"

class LAB_AD9833 : public AD9833
{
  // https://www.analog.com/media/en/technical-documentation/data-sheets/ad9833.pdf
  private:
    AikaPi::SPI_BB m_spi_bb;
  
  public: 
    LAB_AD9833 ();

    void init     (void* controller, unsigned spi_channel = 1);
    void spi_xfer (char* rxd, char* txd, unsigned length);
};

#endif 