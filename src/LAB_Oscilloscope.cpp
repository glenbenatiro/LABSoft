#include "LAB_Oscilloscope.h"

#include "Defaults.h"

LAB_Oscilloscope::
LAB_Oscilloscope () 
  : LAB_Core (),
    m_channel_signals (LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS,
                       LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS,
                       0)
{

  m_verbose        = VERBOSE;
  m_lockstep       = LOCKSTEP;
  m_data_format    = DATA_FORMAT;
  m_sample_count   = OSCILLOSCOPE_STREAMING_SAMPLE_COUNT;

  m_fifo_name      = OSCILLOSCOPE_FIFO_NAME;

  LAB_Oscilloscope_init ();
}

LAB_Oscilloscope::~LAB_Oscilloscope ()
{
  
}

void LAB_Oscilloscope::LAB_Oscilloscope_init ()
{
  LAB_Oscilloscope_dma_init (&(m_vc_mem), m_sample_count, 0);
  LAB_Core_fifo_create(m_fifo_name);
}

void
LAB_Oscilloscope::LAB_Oscilloscope_dma_init (MEM_MAP *mp, int nsamp, int single)
{    
  ADC_DMA_DATA *dp = static_cast<ADC_DMA_DATA*>(mp->virt);

  ADC_DMA_DATA dma_data = {
    .cbs = {
      // Rx input: read data from usec clock and SPI, into 2 ping-pong buffers
      {SPI_RX_TI, REG(m_usec_regs, USEC_TIME), MEM(mp, &dp->usecs[0]),                         4, 0, CBS(1), 0}, // 0
      {SPI_RX_TI, REG(m_spi_regs,  SPI_FIFO),  MEM(mp, dp->rxd1), static_cast<uint32_t>(nsamp*4), 0, CBS(2), 0}, // 1
      {SPI_RX_TI, REG(m_spi_regs,  SPI_CS),    MEM(mp, &dp->states[0]),                        4, 0, CBS(3), 0}, // 2
      {SPI_RX_TI, REG(m_usec_regs, USEC_TIME), MEM(mp, &dp->usecs[1]),                         4, 0, CBS(4), 0}, // 3
      {SPI_RX_TI, REG(m_spi_regs,  SPI_FIFO),  MEM(mp, dp->rxd2), static_cast<uint32_t>(nsamp*4), 0, CBS(5), 0}, // 4
      {SPI_RX_TI, REG(m_spi_regs,  SPI_CS),    MEM(mp, &dp->states[1]),                        4, 0, CBS(0), 0}, // 5

      // Tx output: 2 data writes to SPI for chan 0 & 1, or both chan 0
      {SPI_TX_TI, MEM(mp, dp->txd),          REG(m_spi_regs, SPI_FIFO), 8, 0, CBS(6), 0}, // 6

      // PWM ADC trigger: wait for PWM, set sample length, trigger SPI
      {PWM_TI,    MEM(mp, &dp->pwm_val),     REG(m_pwm_regs, PWM_FIF1), 4, 0, CBS(8), 0}, // 7
      {PWM_TI,    MEM(mp, &dp->samp_size),   REG(m_spi_regs, SPI_DLEN), 4, 0, CBS(9), 0}, // 8
      {PWM_TI,    MEM(mp, &dp->adc_csd),     REG(m_spi_regs, SPI_CS),   4, 0, CBS(7), 0}, // 9
    },
    .samp_size = 2, 
    .pwm_val   = m_pwm_range, 
    .adc_csd   = SPI_TFR_ACT | SPI_AUTO_CS | SPI_DMA_EN | SPI_FIFO_CLR | ADC_CE_NUM,
    .txd       = {0xd0, static_cast<uint32_t>(m_number_of_channels > 1 ? 0xf0 : 0xd0)},
    .usecs     = {0, 0},  
    .states    = {0, 0}, 
    .rxd1      = {0}, 
    .rxd2      = {0},
  };
  
  if (single)                                 // If single-shot, stop after first Rx block
    dma_data.cbs[2].next_cb = 0;

  memcpy (dp, &dma_data, sizeof(dma_data));    // Copy DMA data into uncached memory

  LAB_Core_pwm_init (PWM_FREQ, m_pwm_range, PWM_VALUE);   // Initialise PWM, with DMA

  *REG32(m_pwm_regs, PWM_DMAC) = PWM_DMAC_ENAB | PWM_ENAB;
  *REG32(m_spi_regs, SPI_DC) = (8<<24) | (1<<16) | (8<<8) | 1;  // Set DMA priorities
  *REG32(m_spi_regs, SPI_CS) = SPI_FIFO_CLR;                    // Clear SPI FIFOs

  LAB_Core_dma_start(mp, DMA_CHAN_C, &dp->cbs[6], 0);  // Start SPI Tx DMA
  LAB_Core_dma_start(mp, DMA_CHAN_B, &dp->cbs[0], 0);  // Start SPI Rx DMA
  LAB_Core_dma_start(mp, DMA_CHAN_A, &dp->cbs[7], 0);  // Start PWM DMA, for SPI trigger
}

void 
LAB_Oscilloscope::LAB_Oscilloscope_stream_start ()
{
  LAB_Core_pwm_start ();
}

void 
LAB_Oscilloscope::LAB_Oscilloscope_stream_wait ()
{
  LAB_Core_dma_wait (DMA_CHAN_B);
}

void 
LAB_Oscilloscope::LAB_Oscilloscope_stream_stop ()  
{
  LAB_Core_pwm_stop ();
}

void 
LAB_Oscilloscope::LAB_Oscilloscope_read_from_adc (MEM_MAP *mp, 
                                                             char    *vals, 
                                                             int      maxlen, 
                                                             int      nsamp)  
{
  // open fifo for writing
  if ((m_fifo_write_fd = LAB_Core_fifo_open_write (m_fifo_name)) < 0)
    {
      printf ("ERROR: Can't open FIFO %s for reading\n", m_fifo_name);
      exit (0);
    }
  else
    {
      printf ("DEBUG: Opened FIFO %s for reading\n", m_fifo_name);
    }

  m_fifo_size = LAB_Core_fifo_get_free_space (m_fifo_write_fd);

  int n;

  while (m_is_oscilloscope_running)
    {
      // copy data from uncached mem to stream buff
      if ((n = LAB_Oscilloscope_stream_csv (mp, vals, maxlen, nsamp)) > 0)
        LAB_Core_fifo_write (m_fifo_write_fd, vals, n); // write data from stream buff to fifo
      else
        usleep (10);
    }
}

// Fetch samples from ADC buffer, return comma-delimited integer values
// If in lockstep mode, discard new data if FIFO isn't empty
int 
LAB_Oscilloscope::LAB_Oscilloscope_stream_csv (MEM_MAP *mp, 
                                                           char    *vals, 
                                                           int      maxlen, 
                                                           int      nsamp)
{
  uint32_t  i, 
            n, 
            usec, 
            slen = 0;

  ADC_DMA_DATA *dp = static_cast<ADC_DMA_DATA*>(mp->virt);
  
  for (n = 0; n < 2 && slen == 0; n++)
    {
      if (dp->states[n])
        {
          m_samp_total += nsamp;
          memcpy (m_rx_buff, n ? (void *)dp->rxd2 : (void *)dp->rxd1, nsamp * 4);
          usec = dp->usecs[n];

          if (dp->states[n^1])
            {
              dp->states[0] = dp->states[1] = 0;
              m_overrun_total++;
              break;
            }
            
          dp->states[n] = 0;

          if (m_usec_start == 0)
            m_usec_start = usec;

          if (!m_lockstep || LAB_Core_fifo_get_free_space (m_fifo_fd) >= m_fifo_size)
            {
              if (m_data_format == FMT_USEC)
                slen += sprintf (&vals[slen], "%u", usec - m_usec_start);

              for (i = 0; i < nsamp && slen + 20 < maxlen; i++)
                slen += sprintf (&vals[slen], "%s%4.3f", slen ? "," : "", ADC_VOLTAGE(ADC_RAW_VAL(m_rx_buff[i])));

              slen += sprintf(&vals[slen], "\n");

              if (m_verbose)
                {
                  for (int i = 0; i < nsamp * 4; i++)
                    printf("%02X ", *(((uint8_t *)m_rx_buff)+i));

                  printf("\n");
                }
            }
        }
    }

  vals[slen] = 0;

  return(slen);
}

void LAB_Oscilloscope::
LAB_Oscilloscope_start ()
{
  if (!m_is_oscilloscope_running)
    {
      // ensure DMA channels are stopped
      LAB_Core_dma_stop(DMA_CHAN_A);
      LAB_Core_dma_stop(DMA_CHAN_B);
      LAB_Core_dma_stop(DMA_CHAN_C);

      // pause for a 10 milliseconds
      std::this_thread::sleep_for(std::chrono::milliseconds(10));

      MEM_MAP *mp      = &(m_vc_mem);

      printf ("VALUE OF M_VC_MEM: %p\n", &m_vc_mem);
      ADC_DMA_DATA *dp = static_cast<ADC_DMA_DATA*>(mp->virt);

      // start DMA channels
      LAB_Core_dma_start(mp, DMA_CHAN_C, &dp->cbs[6], 0);  // Start dSPI Tx DMA
      LAB_Core_dma_start(mp, DMA_CHAN_B, &dp->cbs[0], 0);  // Start SPI Rx DMA
      LAB_Core_dma_start(mp, DMA_CHAN_A, &dp->cbs[7], 0);  // Start PWM DMA, for SPI trigger

      // start pwm
      LAB_Core_pwm_start ();

      printf ("DEBUG: start stream\n");

      m_is_oscilloscope_running = true;

      m_thread_ADC_streaming 
      = new std::thread (&LAB_Oscilloscope::LAB_Oscilloscope_read_from_adc,
                         this,
                         mp,
                         m_stream_buff,
                         STREAM_BUFFLEN,
                         m_sample_count);

      m_thread_ADC_reading
      = new std::thread (&LAB_Oscilloscope::LAB_Oscilloscope_write_to_fifo,
                         this,
                         fifo_vals,
                         MAX_SAMPS);
      
    }
}

int LAB_Oscilloscope:: 
LAB_Oscilloscope_write_to_fifo (float *vals, int maxvals)
{
  // open fifo 
  if (!LAB_Core_fifo_is_fifo (m_fifo_name) ||
      (m_fifo_read_fd = open (m_fifo_name, O_RDONLY | O_NONBLOCK)) == -1)
    {
      printf ("ERROR: Can't open FIFO %s for writing\n", m_fifo_name);
    }
  else 
    {
      printf ("DEBUG: Opened FIFO %s for writing\n", m_fifo_name);
    }

  // actual loop
  int i, n, nvals=0, done=0;
  char *s;

  while (m_is_oscilloscope_running)
    {
      while (!done && (n = read(m_fifo_read_fd, &txtbuff[fifo_in], sizeof(txtbuff)-fifo_in-1)) > 0)
      {
          txtbuff[fifo_in + n] = 0;
          if ((s=strchr(&txtbuff[fifo_in], '\n')) != 0)
          {
              s = txtbuff;
              while (!done && (i = strcspn(s, " ,\t\r\n")) > 0 && nvals < maxvals)
              {
                  if (!discard)
                  {
                    vals[nvals++] = atof(s);
                    printf ("%f\n", atof(s));
                  }
                    
                      
                      
                  s += i;
                  if (*s == '\n')
                  {
                      if ((i=strlen(s+1)) > 0)
                      {
                          strcpy(txtbuff, s+1);
                          fifo_in = i;
                      }
                      else
                          fifo_in = 0;
                      done = 0;
                  }
                  else while (*s==',' || *s==' ' || *s=='\t' || *s=='\r')
                      s++;
              }
              discard = 0;
          }
          else if ((fifo_in += n) >= sizeof(txtbuff)-2)
          {
              discard = 1;
              fifo_in = nvals = 0;
          }
          // if (verbose && nvals)
          // {
          //     for (i=0; i<nvals; i++)
          //         printf("%1.3f ", vals[i]);
          //     printf("\n");
          // }
      }
    }
  
  return 0;
}


void LAB_Oscilloscope::
LAB_Oscilloscope_stop ()
{
  if (m_is_oscilloscope_running)
    {
      m_is_oscilloscope_running = false;
      m_thread_ADC_streaming->join ();
      m_thread_ADC_reading->join ();

      LAB_Core_pwm_stop ();

      LAB_Core_dma_stop(DMA_CHAN_A);
      LAB_Core_dma_stop(DMA_CHAN_B);
      LAB_Core_dma_stop(DMA_CHAN_C);

      printf ("DEBUG: streaming stop\n");
    }
}

// callback functions
void LAB_Oscilloscope::
LAB_Oscilloscope_cb_oscilloscope_fl_light_button_enable_disable (Fl_Light_Button *w, 
                                                                       void            *data)
{
  if (w->value () == 0)
    {
      LAB_Oscilloscope_stop ();

      w->label ("Disabled");
    } 
  else 
    {
      w->label ("Enabled");

      LAB_Oscilloscope_start ();
    }
}

// for debug
void LAB_Oscilloscope:: 
LAB_Oscilloscope_generate_sine_wave ()
{

}

// getters
Channel_Signals* LAB_Oscilloscope:: 
channel_signals ()
{
  return &(m_channel_signals);
}



// EOF