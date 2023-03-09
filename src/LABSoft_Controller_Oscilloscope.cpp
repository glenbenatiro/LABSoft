#include "LABSoft_Controller_Oscilloscope.h"

#include <iostream>

#include <FL/Fl.H>

#include "Defaults.h"

LABSoft_Controller_Oscilloscope::
LABSoft_Controller_Oscilloscope (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI)
{
  m_LAB         = _LAB;
  m_LABSoft_GUI = _LABSoft_GUI;

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    channel_signals (&(m_LAB->m_Oscilloscope.m_channel_signals));

  m_LAB->m_Oscilloscope.m_channel_signals.display_width (m_LABSoft_GUI->
    oscilloscope_labsoft_oscilloscope_display_group_display->display ()-> w ());

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    update_time_per_division_labels ();

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    update_volts_per_division_labels ();
}

void LABSoft_Controller_Oscilloscope:: 
cb_run_stop (Fl_Button *w, 
             void      *data)
{
  if (m_LAB->m_Oscilloscope.is_running ())
  {
    m_LAB->m_Oscilloscope.stop ();
      
    w->label ("Run");
  }
  else 
  { 
    m_LAB->m_Oscilloscope.run ();
      
    w->label ("Stop");
    
    m_LABSoft_GUI->voltmeter_fl_button_run_stop->label ("Run");
  }
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
    m_LAB->m_Oscilloscope.channel_enable (channel);
    m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
      channel_enable (channel);
  }
  else 
  {
    m_LAB->m_Oscilloscope.channel_disable (channel);
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
cb_volts_per_division (Fl_Input_Choice *w, 
                       long             channel)
{
  LabelValue _LabelValue (w->value ());
  
  m_LAB->m_Oscilloscope.volts_per_division (static_cast<unsigned>(channel),
    _LabelValue.actual_value ());

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    update_volts_per_division_labels ();
}

void LABSoft_Controller_Oscilloscope::
cb_vertical_offset (Fl_Input_Choice *w, 
                    long             channel)
{
  LabelValue _LabelValue (w->value ());

  m_LAB->m_Oscilloscope.vertical_offset (static_cast<unsigned>(channel),
    _LabelValue.actual_value ());
}

void LABSoft_Controller_Oscilloscope::
cb_ac_coupling (Fl_Light_Button *w, 
                long             channel)
{
  m_LAB->m_Oscilloscope.coupling (static_cast<unsigned>(channel),
    (w->value () == 0) ? LABE_OSC_COUPLING_DC : LABE_OSC_COUPLING_AC);
}

void LABSoft_Controller_Oscilloscope:: 
cb_scaling (Fl_Choice *w,
            long       channel)
{
  LABE_OSC_SCALING scale;

  switch (w->value ())
  {
    case 0: // x2
      scale = LABE_OSC_SCALING_DOUBLE;
      break;
    
    case 1: // x0.5
      scale = LABE_OSC_SCALING_HALF;
      break; 
    
    case 2: // x0.25
      scale = LABE_OSC_SCALING_QUARTER;
      break;
    
    case 3: // x0.125
      scale = LABE_OSC_SCALING_EIGHTH;
      break;
    
    default:
      break;
  }

  m_LAB->m_Oscilloscope.scaling (static_cast<unsigned>(channel), scale);
}

void LABSoft_Controller_Oscilloscope:: 
cb_time_per_division (Fl_Input_Choice *w,
                      long             channel)
{
  LabelValue _LabelValue (w->value ());

  m_LAB->m_Oscilloscope.time_per_division (_LabelValue.actual_value (), 
    LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_COLUMNS);

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    update_time_per_division_labels ();

  char text[50];
  
  Channel_Signal_Oscilloscope *osc = &(m_LAB->m_Oscilloscope.
    m_channel_signals.m_chans[0].osc);

  // sprintf (text, "%3.3f samples at %s", osc->samples, 
  //   LabelValue (osc->sampling_rate, LE_UNIT_HZ).to_label_text ().c_str ());

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display->
    update_upper_osc_disp_info (text);
}

void LABSoft_Controller_Oscilloscope::
cb_test (Fl_Light_Button *w,
         void            *data)
{
  printf ("test!\n");

  if (w->value () == 1)
  {
    m_LAB->m_LAB_Core.dma_pause (LAB_DMA_CHAN_PWM_PACING);
  }
  else 
  {
    m_LAB->m_LAB_Core.dma_play (LAB_DMA_CHAN_PWM_PACING);
  }
}

// EOFs