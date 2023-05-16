#include "LAB_AD9833.h"

#include <iostream>

LAB_AD9833::
LAB_AD9833 (unsigned CS, unsigned MISO, unsigned MOSI, unsigned SCLK, double baud)
  : m_spi_bb (AikaPi::SPI_BB (CS, MISO, MOSI, SCLK, baud, AP::SPI::MODE::_2))
  // according to AD9833 datasheet, data is sampled on clock falling edge,
  // that's why SPI mode is 1
{
  reset ();
}

LAB_AD9833::
~LAB_AD9833 ()
{
  reset ();
}

void LAB_AD9833::
spi_xfer (char* rxd, char* txd, unsigned length)
{
  m_spi_bb.xfer (rxd, txd, length);
}