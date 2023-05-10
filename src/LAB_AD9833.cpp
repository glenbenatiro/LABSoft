#include "LAB_AD9833.h"

LAB_AD9833::
LAB_AD9833 ()
  : m_spi_bb (AikaPi::SPI_BB (
    LABC::PIN::FUNC_GEN_IC_CS, 
    LABC::PIN::FUNC_GEN_IC_MISO, 
    LABC::PIN::FUNC_GEN_IC_MOSI, 
    LABC::PIN::FUNC_GEN_IC_SCLK, 
    LABC::FUNC_GEN::IC_FREQUENCY))
{


}

void LAB_AD9833:: 
init (void*    controller,
      unsigned spi_channel)
{

}

void LAB_AD9833::
spi_xfer (char* rxd, char* txd, unsigned length)
{
  // (static_cast<LAB_Core*>(m_controller))->
}