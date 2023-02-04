#include "LABSoft_Controller_Oscilloscope.h"

#include <iostream>
#include <chrono>

#include <FL/Fl.H>

#include "Defaults.h"

LABSoft_Controller_Oscilloscope::
LABSoft_Controller_Oscilloscope (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI)
{
  m_LAB         = _LAB;
  m_LABSoft_GUI = _LABSoft_GUI;

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    channel_signals (&(m_LAB->m_Oscilloscope->m_channel_signals));

  m_LAB->m_Oscilloscope->m_channel_signals.display_width (m_LABSoft_GUI->
    oscilloscope_labsoft_oscilloscope_display_group_display->display ()-> w ());

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    update_time_per_division_labels ();

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    update_volts_per_division_labels ();
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
      // m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
      //   disable ();

      w->label ("Run");
    }
  else 
    {
      m_LAB->m_Oscilloscope->run ();
      
      // m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
      //   enable ();

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
  bool value  = w->value ();

  if (value)
  {
    m_LAB->m_Oscilloscope->channel_enable (channel);
    m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
      channel_enable (channel);
  }
  else 
  {
    m_LAB->m_Oscilloscope->channel_disable (channel);
    m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
      channel_disable (channel);
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
    update_time_per_division_labels ();
}


void LABSoft_Controller_Oscilloscope:: 
update_display ()
{
  while (m_LAB->m_Oscilloscope->m_is_running) 
  {
    // auto start = std::chrono::steady_clock::now ();

    m_LAB->m_Oscilloscope->load_data_samples ();

    m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
     load_channel_signals (&(m_LAB->m_Oscilloscope->m_channel_signals));

    // draw signals
    m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->display ()->redraw ();
    Fl::awake ();

    std::this_thread::sleep_for (std::chrono::milliseconds 
      (DISPLAY_UPDATE_SLEEP_TIME_MS));

    // auto end = std::chrono::steady_clock::now ();

    // std::chrono::duration<double, std::milli> elapsed = end - start;
    // std::cout << "Duration: " << elapsed.count () << " ms\n";
  }
}

void LABSoft_Controller_Oscilloscope::
cb_volts_per_division (Fl_Input_Choice *w, 
                       long             channel)
{
  LabelValue _LabelValue (w->value ());
  
  m_LAB->m_Oscilloscope->volts_per_division (static_cast<unsigned>(channel),
    _LabelValue.actual_value ());

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    update_volts_per_division_labels ();
}

void LABSoft_Controller_Oscilloscope::
cb_vertical_offset (Fl_Input_Choice *w, 
                    long             channel)
{
  LabelValue _LabelValue (w->value ());

  m_LAB->m_Oscilloscope->vertical_offset (static_cast<unsigned>(channel),
    _LabelValue.actual_value ());
}


void LABSoft_Controller_Oscilloscope:: 
cb_time_per_division (Fl_Input_Choice *w,
                      long             channel)
{
  LabelValue _LabelValue (w->value ());

  m_LAB->m_Oscilloscope->time_per_division (static_cast<unsigned>(channel),
    _LabelValue.actual_value (), LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_COLUMNS);

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    update_time_per_division_labels ();
}

void LABSoft_Controller_Oscilloscope::
cb_test (Fl_Light_Button *w,
         void            *data)
{
  printf ("test!\n");

  if (w->value () == 1)
  {
    m_LAB->m_LAB_Core.dma_pause (DMA_CHAN_PWM_PACING);
  }
  else 
  {
    m_LAB->m_LAB_Core.dma_play (DMA_CHAN_PWM_PACING);
  }
}

// EOFs