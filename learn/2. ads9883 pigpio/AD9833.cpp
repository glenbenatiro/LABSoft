#include <iostream>
#include <unistd.h>
#include <pigpio.h>
#include <bitset>

// compile using
// g++ -Wall -pthread -o AD9833 AD9833.cpp -lpigpio -lrt

enum WaveType
{
  SINE, 
  TRIANGLE,
  SQUARE_FULL,
  SQUARE_HALF
};

constexpr int SPI_FREQUENCY = 100'000;
constexpr int SPI1_CEx = 2;
constexpr double SOURCE_CLOCK = 25'000'000.0;
constexpr bool DEBUG = 0;

class AD9833_pigpio
{
  private:
    int m_fd;

    uint16_t  m_B28 = 0,
              m_HLB = 0,
              m_FSELECT = 0,
              m_PSELECT = 0,
              m_Reset = 0,
              m_SLEEP1 = 0,
              m_SLEEP12 = 0,
              m_OPBITEN = 0,
              m_DIV2 = 0,
              m_Mode = 0;
    
    WaveType def_wavetype = SINE;
    double def_freq = 100;
    
    char m_txbuff[2], m_rxbuff[2];
  
  public:
    
    AD9833_pigpio ()
    {
      if (gpioInitialise () < 0)
      {
        printf ("gpioinitalize error! DO NOT USE CLASS! exit program!\n");
      }
      else 
      {
        m_fd = spiOpen (SPI1_CEx, SPI_FREQUENCY, (1 << 8));
        init ();

        // start ();
        printf ("AD9833_pigpio class init OK\n");
      }
    }

    ~AD9833_pigpio ()
    {
      spiClose (m_fd);
      gpioTerminate ();
      printf ("AD9833_pigpio terminated\n");
    }

    void init ()
    {
      // set initial values
      stop ();

      set_wavetype (SINE);
      set_frequency (500);
      // set_write_mode (1);

      start ();
    }

    void spiW ()
    {
      if (DEBUG)
      {
        printf ("%02x ", m_txbuff[0]);
        printf ("%02x ", m_txbuff[1]);
        printf ("\n\n");
      }
      

      spiWrite (m_fd, m_txbuff, 2);
    }

    void write_ctrl_reg ()
    {
      m_txbuff[0] = 0x00 | (m_B28 << 5) | (m_HLB << 4) | (m_FSELECT << 3) |
        (m_PSELECT << 2) | m_Reset;
        
      m_txbuff[1] = 0x00 | (m_SLEEP1 << 7) | (m_SLEEP12 << 6) | 
          (m_OPBITEN << 5) | (m_DIV2 << 3) | (m_Mode << 1);
      
        
      spiW ();
    }

    void stop ()
    {
      m_Reset = 1;
      write_ctrl_reg ();
    }

    void start ()
    {
      m_Reset = 0;
      write_ctrl_reg ();
    }

    void set_HLB (bool value)
    {
      m_HLB = value;
      write_ctrl_reg ();
    }

    void set_write_mode (bool value)
    {
      // 1 = complete write through 2 consecutive writes
      // 0 = freq reg is split into MSB and LSB
      m_B28 = value;
      write_ctrl_reg ();
    }
    
    void set_wavetype (WaveType _WaveType)
    {
      switch (_WaveType)
      {
        case SINE:
          m_OPBITEN = 0;
          m_DIV2 = 0;
          m_Mode = 0;
          break;
        case TRIANGLE:
          m_OPBITEN = 0;
          m_DIV2 = 0;
          m_Mode = 1;
          break;
        case SQUARE_FULL:
          m_OPBITEN = 1;
          m_DIV2 = 1;
          m_Mode = 0;
          break;
        case SQUARE_HALF:
          m_OPBITEN = 1;
          m_DIV2 = 0;
          m_Mode = 0;
          break;
        default:
          set_wavetype (SINE);
          break;
      }
    
      write_ctrl_reg ();
    }

    void set_frequency (double frequency)
    {
      uint32_t divider = (frequency * 268'435'456.0) / SOURCE_CLOCK;

      // note that divider is only 28 bits!
      divider &= 0x0FFFFFFF;

      printf ("DEBUG: divider: %x\n", divider);

      if (m_B28 == 1)
      {
        m_txbuff[0] = 0x00 | ((m_FSELECT + 1) << 6) | ((divider >> 8) & 0x3F);
        m_txbuff[1] = divider & 0xFF;
        spiW ();

        divider >>= 14;

        m_txbuff[0] = 0x00 | ((m_FSELECT + 1) << 6) | ((divider >> 8) & 0x3F);
        m_txbuff[1] = divider & 0xFF;
        spiW ();
      } 
      else if (m_B28 == 0)
      {
        set_HLB (0);
        m_txbuff[0] = 0x00 | ((m_FSELECT + 1) << 6) | ((divider >> 8) & 0x3F);
        m_txbuff[1] = divider & 0xFF;
        spiW ();

        divider >>= 14;

        set_HLB (1);
        m_txbuff[0] = 0x00 | ((m_FSELECT + 1) << 6) | ((divider >> 8) & 0x3F);
        m_txbuff[1] = divider & 0xFF;
        spiW ();
      }
    }

    void sel_freq_reg (bool value)
    {
      m_FSELECT = value;
      write_ctrl_reg ();
    }

    void sel_phase_reg (bool value)
    {
      m_PSELECT = value;
      write_ctrl_reg ();
    }
};


int main()
{
  AD9833_pigpio dev;

  int x = 2;
  int time = 1;

  while (x--)
  {
    dev.set_wavetype (SINE);
    dev.set_frequency (250);
    sleep (time);

    dev.set_wavetype (TRIANGLE);
    //dev.set_frequency (250);
    sleep (time);

    dev.set_wavetype (SQUARE_FULL);
    //dev.set_frequency (250);
    sleep (time);

    dev.set_wavetype (SQUARE_HALF);
    //dev.set_frequency (250);
    sleep (time);

    dev.set_wavetype (SINE);
    dev.set_frequency (100);
    sleep (time);

    dev.set_frequency (500);
    sleep (time);

    dev.set_frequency (1000);
    sleep (time);
  }

  dev.stop ();
  printf ("program done\n");

  return 0;
}