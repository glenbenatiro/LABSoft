// // --- SPI ---
// Initialise SPI0, given desired clock freq; return actual value
float 
spi_init ()
{
  // initialize spi gpio pins on rpi
  gpio_set (SPI0_CE0_PIN,  GPIO_ALT0, GPIO_NOPULL);
  gpio_set (SPI0_CE1_PIN,  GPIO_ALT0, GPIO_NOPULL);
  gpio_set (SPI0_MISO_PIN, GPIO_ALT0, GPIO_PULLUP);
  gpio_set (SPI0_MOSI_PIN, GPIO_ALT0, GPIO_NOPULL);
  gpio_set (SPI0_SCLK_PIN, GPIO_ALT0, GPIO_NOPULL);

  // clear tx and rx fifo. one shot operation
  spi_clear_rxtx_fifo ();
  
  return (spi_set_clock_rate (hz));
}

void  
spi_clear_rxtx_fifo ()
{
  // *REG32 (m_spi_regs, SPI_CS) = SPI_FIFO_CLR;
  *REG32 (m_spi_regs, SPI_CS) = 0x30;
}

// Set / clear SPI chip select
void 
LAB_Core_spi_cs (int set)
{
  uint32_t csval = *REG32(m_spi_regs, SPI_CS);

  *REG32(m_spi_regs, SPI_CS) = set ? csval | 0x80 : csval & ~0x80;
}

// Transfer SPI bytes
void 
spi_xfer (uint8_t *txd, 
          uint8_t *rxd, 
          int      length)
{
  while (length--)
    {
      *REG8(m_spi_regs, SPI_FIFO) = *txd++;
      
      // wait for rx fifo to contain AT LEAST 1 byte
      while ((*REG32(m_spi_regs, SPI_CS) & (1<<17)) == 0);
      
      *rxd++ = *REG32(m_spi_regs, SPI_FIFO);
    }
}

// Disable SPI
void 
LAB_Core_spi_disable (void)
{
    *REG32(m_spi_regs, SPI_CS) = SPI_FIFO_CLR;
    *REG32(m_spi_regs, SPI_CS) = 0;
}

// Display SPI registers
void 
LAB_Core_spi_disp (void)
{
  // SPI register strings
  const char *m_spi_regstrs[] = {"CS", 
                               "FIFO", 
                               "CLK", 
                               "DLEN", 
                               "LTOH", 
                               "DC", 
                               ""};

  int i = 0;
  volatile uint32_t *p = REG32(m_spi_regs, SPI_CS);

  while (m_spi_regstrs[i][0])
    printf("%-4s %08X ", m_spi_regstrs[i++], *p++);
  
  printf("\n");
}

// page 156
float  
spi_set_clock_rate (float value)
{
  uint16_t divider = 0;

  if (value <= SPI_CLOCK_HZ)
    uint16_t divider = SPI_CLOCK_HZ / value;
  
  // set spi frequency as rounded off clock
  *REG32 (m_spi_regs, SPI_CLK) = divider;

  return (SPI_CLOCK_HZ / divider);
}