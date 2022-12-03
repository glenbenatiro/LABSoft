#ifndef LAB_OSCILLOSCOPE_H
#define LAB_OSCILLOSCOPE_H

#include <thread>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

#include <FL/Fl_Light_Button.H>

#include "LAB_Core.h"
#include "Auxiliary.h"

#include "Channel_Signals.h"

class LAB_Oscilloscope 
{
  public:
    LAB_Core *m_LAB_Core;

    int         m_fifo_read_fd,
                m_fifo_write_fd,
                m_number_of_channels = LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS;

    const char *m_fifo_name;

    uint32_t    m_fifo_size,
                m_pwm_range;

    float       m_voltage_per_div = 1,
                m_seconds_per_div = 1;

    
    
    
  public:
    uint32_t m_samp_total,
             m_rx_buff[MAX_SAMPS],
             m_overrun_total,
             m_usec_start;

    char     m_stream_buff[STREAM_BUFFLEN];

    char txtbuff [20000];


    int      m_lockstep,
             m_verbose,
             m_data_format = 0,
             m_sample_count = LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES_PER_CHANNEL,
             m_sample_rate= LAB_OSCILLOSCOPE_SAMPLE_RATE,
             fifo_in;
    
    bool     m_is_oscilloscope_running = false;

    int fifo_write_fd = 0,
        fifo_read_fd  = 0;

    
    int fifo_fd = 0;
    int discard = 0;
    int use_fifo = 0;

    float fifo_vals[MAX_SAMPS];


    bool m_is_generate_random_values_running = false;

    std::thread *m_thread_ADC_streaming,
                *m_thread_ADC_reading;

    Channel_Signals m_channel_signals;


    // functions
    LAB_Oscilloscope                     (LAB_Core *_LAB_Core);
   ~LAB_Oscilloscope                     ();

    void LAB_Oscilloscope_init           ();
    void LAB_Oscilloscope_dma_init       (MemoryMap *mp, int nsamp, int single);

    void LAB_Oscilloscope_stream_start   ();
    void LAB_Oscilloscope_stream_wait    ();
    void LAB_Oscilloscope_stream_stop    ();
    int_fast16_t LAB_Oscilloscope_stream_csv     ();
    
    // int  LAB_Oscilloscope_stream_csv     (MemoryMap *mp, char *vals, int maxlen, int nsamp);

    void LAB_Oscilloscope_start ();
    void LAB_Oscilloscope_stop ();
    void LAB_Oscilloscope_read_from_adc   (MemoryMap *mp, char *vals, int maxlen, int nsamp);

    int LAB_Oscilloscope_write_to_fifo (float *vals, int maxvals);

    int LAB_Oscilloscope_is_fifo(char *fname);

    void LAB_Oscilloscope_stream_run     ();

    void LAB_Oscilloscope_generate_sine_wave ();

    // callback functions
    void LAB_Oscilloscope_cb_oscilloscope_fl_light_button_enable_disable (Fl_Light_Button *w, void *data);

    // getters
    Channel_Signals* channel_signals ();
};

#endif
