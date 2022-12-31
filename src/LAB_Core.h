#ifndef LAB_CORE_H
#define LAB_CORE_H

#include "../lib/AikaPi.h"

#include "Defaults.h"

constexpr int SAMPLE_SIZE       = 4; // 4 bytes per sample
constexpr int PING_BONG_BUFFER  = 2; // 2 buffers that form a ping pong buffer
constexpr int BUFFER_LENGTH     = LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES * SAMPLE_SIZE; 
constexpr int BUFFER_COUNT      = LAB_OSCILLOSCOPE_NUMBER_OF_CHANNELS;
constexpr int VC_MEM_SIZE       = (PAGE_SIZE + (BUFFER_LENGTH * BUFFER_COUNT * PING_BONG_BUFFER));

typedef struct {
  DMA_CB cbs[NUM_CBS];

  uint32_t samp_size, 
           pwm_val, 
           adc_csd, 
           txd[2];

  volatile uint32_t usecs[2], 
                    states[2], 
                    rxd1[LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES], 
                    rxd2[LAB_OSCILLOSCOPE_NUMBER_OF_SAMPLES];
} 
ADC_DMA_DATA;

class LAB_Core : public AikaPi
{
  private: 

  public:
    LAB_Core ();
};

#endif