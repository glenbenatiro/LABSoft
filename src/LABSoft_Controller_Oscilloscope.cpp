#include "LABSoft_Controller_Oscilloscope.h"

#include <iostream>
#include <chrono>


#include <FL/Fl.H>


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
      // backend
      m_LAB->m_Oscilloscope->stop ();

      // frontend
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

  if (value)
  {
    m_LAB->m_Oscilloscope->channel_enable (channel);
  }
  else 
  {
    m_LAB->m_Oscilloscope->channel_disable (channel);
  }
}

void LABSoft_Controller_Oscilloscope::
cb_x_offset (Fl_Input_Choice *w, 
             void            *data)
{
  double value = LabelValue (w->value ()).actual_value ();

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
      m_channel_signals.m_chans);
  ADC_DMA_DATA *dp = static_cast<ADC_DMA_DATA*>(m_LAB->m_LAB_Core.m_vc_mem.virt);

  // 1. get block from backend
  while (m_LAB->m_Oscilloscope->m_is_running) 
  {
    // std::chrono::time_point<std::chrono::high_resolution_clock> start = 
    //   std::chrono::high_resolution_clock::now ();

   // to iterate over ping pong channels
    // WARNING! wala pana implement ang other channels
    // this is from single channel code

    clock_gettime (CLOCK_MONOTONIC, &tp1);
      
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

    // int duration = (DISPLAY_UPDATE_RATE * 1000);

    // clock_gettime(CLOCK_MONOTONIC, &tp2);

    // struct timespec duration = diff (tp1, tp2);
    // printf ("fifload:%01ld %09ld,", duration.tv_sec, duration.tv_nsec);

        
    // std::chrono::time_point<std::chrono::high_resolution_clock> end = 
    // std::chrono::high_resolution_clock::now ();

    // std::chrono::duration<int, std::nano> elapsed = end - start;

    // std::cout << std::fixed << elapsed.count () << "\n";

    // std::this_thread::sleep_for (std::chrono::nanoseconds (40'000'000));
  }
}

void LABSoft_Controller_Oscilloscope::
cb_volts_per_division (Fl_Input_Choice *w, 
                       long             data)
{
  unsigned channel = static_cast<unsigned>(data);
  LabelValue _LabelValue (w->value ());
  
  m_LAB->m_Oscilloscope->volts_per_division (channel, _LabelValue.actual_value ());

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    update_volts_per_division_labels (channel);
}

void LABSoft_Controller_Oscilloscope::
cb_vertical_offset (Fl_Input_Choice *w, 
                    long             data)
{
  unsigned channel = static_cast<unsigned>(data);
  LabelValue _LabelValue (w->value ());

  m_LAB->m_Oscilloscope->vertical_offset (channel, _LabelValue.actual_value ());
}


void LABSoft_Controller_Oscilloscope:: 
cb_time_per_division (Fl_Input_Choice *w,
                      long             data)
{
  int channel = static_cast<int>(data);
  LabelValue _LabelValue (w->value ());

  // frontnend stuff
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    time_per_division (channel, _LabelValue.actual_value ());
  
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    update_time_per_division_labels ();

  m_LAB->m_Oscilloscope->sample_rate (channel, LabelValue (w->value ()).actual_value ());
}

// EOFs