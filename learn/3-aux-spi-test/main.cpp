#include <iostream>

#include "LAB_Core.h"

int main ()
{
  LAB_Core *core = new LAB_Core ();
  
  char rxbuff[2], txbuff[2];

  txbuff[0] = 0xCA;
  txbuff[1] = 0xFF;

  core->aux_spi0_init ();

  // while (true)
  // {
  //   core->aux_spi0_write (txbuff, 2);
  //   //g_reg32_peek (core->m_aux_regs, AUX_SPI0_PEEK_REG);
  // }

  core->aux_spi0_write (txbuff, 2);

  return 0;
}