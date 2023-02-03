#ifndef LAB_CORE_H
#define LAB_CORE_H

#include "../lib/AikaPi/AikaPi.h"

#include "Defaults.h"

constexpr unsigned SAMPLE_SIZE    = 4; // 4 bytes per sample
constexpr unsigned BUFFER_LENGTH  = LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES * SAMPLE_SIZE;
constexpr unsigned BUFFER_COUNT   = 2;
constexpr unsigned VC_MEM_SIZE    = PAGE_SIZE + (BUFFER_COUNT * BUFFER_LENGTH * LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS);

typedef struct {
  DMA_CB cbs[NUM_CBS];

  uint32_t samp_size, 
           pwm_val, 
           adc_csd, 
           txd[2];

  volatile uint32_t usecs[2], 
                    states[2], 
                    rxd0[LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES], 
                    rxd1[LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES];
} ADC_DMA_DATA;

class LAB_Core : public AikaPi
{
  private: 

  public:
    LAB_Core ();
};

#endif