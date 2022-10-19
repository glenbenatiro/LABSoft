#include "LABSoft_Controller_Oscilloscope.h"

#include <iostream>
#include <chrono>

#include <FL/Fl.H>

#include "Globals.h"
#include "Defaults.h"

LABSoft_Controller_Oscilloscope::
LABSoft_Controller_Oscilloscope (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI)
{
  m_LAB = _LAB;
  m_LABSoft_GUI = _LABSoft_GUI;
}

void LABSoft_Controller_Oscilloscope:: 
cb_run_stop (Fl_Light_Button *w, 
             void            *data)
{
  if (w->value () == 0)
    {
      m_LAB->m_Oscilloscope->stop ();
      m_LAB->m_Oscilloscope->m_is_running = false;

      w->label ("Run");
    }
  else 
    {
      m_LAB->m_Oscilloscope->run ();
      m_LAB->m_Oscilloscope->m_is_running = true;

      m_thread_update_display = new std::thread
        (&LABSoft_Controller_Oscilloscope::update_display, this);

      w->label ("Stop");
    }
}


void LABSoft_Controller_Oscilloscope::
LABSoft_Controller_Oscilloscope_cb_fl_light_button_generate_sine_wave (Fl_Light_Button *w, 
                                                                       void            *data)
{

}

void LABSoft_Controller_Oscilloscope::
cb_single (Fl_Button *w, 
           void      *data)
{
  printf ("DEBUG: single button clicked\n");
}

void LABSoft_Controller_Oscilloscope:: 
cb_channel_enable_disable (Fl_Light_Button *w,
                           void            *data)
{
  int channel = *((int*)data);
  bool value = w->value ();

  // backend
  m_LAB->m_Oscilloscope->m_channel_signals.m_channel_signal_vector[channel].
    m_is_enabled = value;

  // frontend
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    m_display->m_channel_signals.m_channel_signal_vector[channel].
      m_is_enabled = value;
}


void LABSoft_Controller_Oscilloscope::
cb_time_per_division (Fl_Input_Choice *w, 
                      void            *data)
{
  double value    = globals_get_actual_value_from_label (w->value ());
  int unit_scaler = globals_get_unit_prefix_power_scaler (w->value ());

  printf ("value: %f, unit_scaler: %d\n", value, unit_scaler);

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    update_time_per_division (value, unit_scaler);
}

void LABSoft_Controller_Oscilloscope::
cb_x_offset (Fl_Input_Choice *w, 
             void            *data)
{
  double value = globals_get_actual_value_from_label (w->value ());

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    m_x_offset = value;

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    update_x_axis_labels ();
}

void LABSoft_Controller_Oscilloscope::
cb_volts_per_division (Fl_Input_Choice *w, 
                            void            *data)
{
  int channel = *((int*)data);
  double value    = globals_get_actual_value_from_label (w->value ());
  int unit_scaler = globals_get_unit_prefix_power_scaler (w->value ());

  printf ("value: %f, unit_scaler: %d\n", value, unit_scaler);

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    update_volts_per_division (value, unit_scaler, channel);
}


void LABSoft_Controller_Oscilloscope:: 
update_display ()
{
  int x = 0;

  uint32_t usec;
  std::vector<Channel_Signal> *chns = &(m_LAB->m_Oscilloscope->
    m_channel_signals.m_channel_signal_vector);
  ADC_DMA_DATA *dp = static_cast<ADC_DMA_DATA*>(m_LAB->m_LAB_Core.m_vc_mem.virt);

  while (m_LAB->m_Oscilloscope->m_is_running) {
    // to iterate over ping pong channels
    // WARNING! wala pana implement ang other channels
    // this is from single channel code
      
    for (int a = 0; a < (m_LAB->m_Oscilloscope->m_number_of_channels); a++)  {
      for (int b = 0; b < 2; b++) {
        if (dp->states[b]) {
          // copy values from uncached mem to memory on channel 1
          // WARNING! wala pana implement ang sa other channels.
          // assumed pa ni nga this for loop will only run 1 time bc a < 1

          //printf ("raw value samp: %d\n", b ? dp->rxd2[0] : dp->rxd1[0]);
          memcpy (&((*chns)[a].m_raw_values), (b ? (void *)dp->rxd2 : (void *)dp->rxd1),
            (m_LAB->m_Oscilloscope->m_number_of_samples_per_channel * 4));


          usec = dp->usecs[b];
            if (dp->states[b^1])
            {
                dp->states[0] = dp->states[1] = 0;
                //overrun_total++;
                break;
            }
            dp->states[b] = 0;
          
          for (int i=0; i < 2*4; i++)
            printf("%02X ", *(((uint8_t *)(*chns)[i].m_raw_values)+i));  
          printf("\n");
          
        }
      }

      // sample print sa daw be
      uint32_t samp = m_LAB->m_Oscilloscope->m_channel_signals.
        m_channel_signal_vector[0].m_raw_values[0];
      
      //printf ("copied samp: %d\n", samp);
    }
    
    if (x > 23)  {
      x = 0;
      printf ("tick!\n");
    }  else {
      x++;
    }

    int duration = (DISPLAY_UPDATE_RATE * 1000);
    std::this_thread::sleep_for
      (std::chrono::milliseconds(duration));
  }
}

