#include "LABSoft_Controller_Oscilloscope.h"

#include <iostream>
#include <chrono>

#include <FL/Fl.H>

#include "Auxiliary.h"
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
      m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
        m_display->m_is_enabled = false;

      w->label ("Run");
    }
  else 
    {
      m_LAB->m_Oscilloscope->run ();
      m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
        m_display->m_is_enabled = true;

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
                           long             data)
{
  int channel = static_cast<int>(data);
  bool value = w->value ();
  
  // backend
  m_LAB->m_Oscilloscope->m_channel_signals.m_channel_signal_vector[channel].
    m_is_enabled = value;

  // frontend
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    m_display->m_channel_signals.m_channel_signal_vector[channel].
      m_is_enabled = value;

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    update_volts_per_division_labels ();
}

void LABSoft_Controller_Oscilloscope::
cb_x_offset (Fl_Input_Choice *w, 
             void            *data)
{
  double value = g_get_actual_value_from_label (w->value ());

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    m_x_offset = value;

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    update_x_axis_labels ();
}




void LABSoft_Controller_Oscilloscope:: 
update_display ()
{
  int x = 0;

  uint32_t usec;
  std::vector<Channel_Signal> *chns = &(m_LABSoft_GUI->
    oscilloscope_labsoft_oscilloscope_display_group_display->m_display->
      m_channel_signals.m_channel_signal_vector);
  ADC_DMA_DATA *dp = static_cast<ADC_DMA_DATA*>(m_LAB->m_LAB_Core.m_vc_mem.virt);

  // 1. get block from backend
  while (m_LAB->m_Oscilloscope->m_is_running) 
  {
    // to iterate over ping pong channels
    // WARNING! wala pana implement ang other channels
    // this is from single channel code
      
    for (int a = 0; a < (m_LAB->m_Oscilloscope->m_number_of_channels); a++)  
    {
      for (int b = 0; b < 2; b++) 
      {
        if (dp->states[b]) 
        {
          // copy values from uncached mem to memory on channel 1
          // WARNING! wala pana implement ang sa other channels.
          // assumed pa ni nga this for loop will only run 1 time bc a < 1

          // copy a block of data from uncached mem right to oscilloscope display chn signals
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
        }
      }
    }
    
    

    LABSoft_Oscilloscope_Display *display = m_LABSoft_GUI-> 
      oscilloscope_labsoft_oscilloscope_display_group_display->m_display;

    // normalize raw ADC data: m_raw_values -> m_values
    display->normalize_all_channels_raw_data ();

    // sample skip to get samples to display: m_values -> m_pixel_points
    display->normalize_channels_data_to_display ();

    // draw signals
    display->redraw ();
    Fl::awake ();

    int duration = (DISPLAY_UPDATE_RATE * 1000);
    std::this_thread::sleep_for
      (std::chrono::milliseconds(duration));
  }
}

void LABSoft_Controller_Oscilloscope::
cb_volts_per_division (Fl_Input_Choice *w, 
                       long             data)
{
  int channel = static_cast<int>(data);
  ValueStruct _ValueStruct (w->value ());

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    volts_per_division (channel, _ValueStruct.actual_value ());
  
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    update_volts_per_division_labels (channel);
}

void LABSoft_Controller_Oscilloscope::
cb_vertical_offset (Fl_Input_Choice *w, 
                    long             data)
{
  int channel = static_cast<int>(data);
  ValueStruct _ValueStruct (w->value ());

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    vertical_offset (channel, _ValueStruct);
  
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    update_volts_per_division_labels (channel);
}


void LABSoft_Controller_Oscilloscope:: 
cb_time_per_division (Fl_Input_Choice *w,
                      long             data)
{
  int channel = static_cast<int>(data);
  ValueStruct _ValueStruct (w->value ());

  // frontnend stuff
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    time_per_division (channel, _ValueStruct.actual_value ());
  
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    update_time_per_division_labels ();

  // backend
  float freq = (m_LAB->m_Oscilloscope->m_number_of_samples_per_channel / 
    m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
      m_number_of_columns) / (_ValueStruct.actual_value ());
  
  m_LAB->m_Oscilloscope->sample_rate (freq);
}

// EOF