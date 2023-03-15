#include "LABSoft_Controller_Oscilloscope.h"

#include <iostream>

#include <FL/Fl.H>

#include "Defaults.h"

LABSoft_Controller_Oscilloscope::
LABSoft_Controller_Oscilloscope (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI)
{
  m_LAB         = _LAB;
  m_LABSoft_GUI = _LABSoft_GUI;

  // Link the LAB_Oscilloscope_Parent_Data struct from LAB_Oscilloscope
  // to the LABSoft_Oscilloscope_Display_Group class in the GUI
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    load_osc_parent_data (&(m_LAB->m_Oscilloscope.m_parent_data));
  
  // reserve() the pixel point vectors in the LAB_Oscilloscope_Parent_Data
  // struct 
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    reserve_pixel_points ();
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
  unsigned  channel = static_cast<unsigned>(data);
  bool      value   = w->value ();

  m_LAB->m_Oscilloscope.channel_enable_disable (channel, value);
}

void LABSoft_Controller_Oscilloscope::
cb_horizontal_offset (Fl_Input_Choice *w, 
             void            *data)
{
  LabelValue _LabelValue (w->value ());

  m_LAB->m_Oscilloscope.horizontal_offset (_LabelValue.actual_value ());

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    update_time_per_division_labels ();
}

void LABSoft_Controller_Oscilloscope::
cb_voltage_per_division (Fl_Input_Choice *w, 
                       long             channel)
{
  LabelValue _LabelValue (w->value ());
  
  m_LAB->m_Oscilloscope.voltage_per_division (static_cast<unsigned>(channel),
    _LabelValue.actual_value ());

  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    update_voltage_per_division_labels ();
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
    (w->value () == 0) ? LE_OSC_COUPLING_DC : LE_OSC_COUPLING_AC);
}

void LABSoft_Controller_Oscilloscope:: 
cb_scaling (Fl_Choice *w,
            long       channel)
{
  LE_OSC_SCALING scale;

  switch (w->value ())
  {
    case 0: // x2
      scale = LE_OSC_SCALING_DOUBLE;
      break;
    
    case 1: // x0.5
      scale = LE_OSC_SCALING_HALF;
      break; 
    
    case 2: // x0.25
      scale = LE_OSC_SCALING_QUARTER;
      break;
    
    case 3: // x0.125
      scale = LE_OSC_SCALING_EIGHTH;
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

  // Backend
  m_LAB->m_Oscilloscope.time_per_division (_LabelValue.actual_value (),
    LABSOFT_OSCILLOSCOPE_DISPLAY_NUMBER_OF_COLUMNS);

  // Frontend
  m_LABSoft_GUI->oscilloscope_labsoft_oscilloscope_display_group_display-> 
    update_time_per_division_labels ();



  // TO-DO: fix this up lmao

  char text[50];

  LAB_Parent_Data_Oscilloscope *osc = &(m_LAB->m_Oscilloscope.m_parent_data);
  
  sprintf (text, "%3.3f samples at %s", osc->w_samp_count, 
    LabelValue (osc->sampling_rate, LE_UNIT_HZ).to_label_text ().c_str ());

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