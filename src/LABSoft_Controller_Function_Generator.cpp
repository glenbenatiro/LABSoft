#include "LABSoft_Controller_Function_Generator.h"

#include <string>

LABSoft_Controller_Function_Generator::
LABSoft_Controller_Function_Generator (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI)
{
  m_LAB         = _LAB;
  m_LABSoft_GUI = _LABSoft_GUI;
}

// callback functions
void LABSoft_Controller_Function_Generator::
cb_run_stop (Fl_Light_Button *w, 
             long             data)
{
  int channel = static_cast<int>(data);

  if (w->value () == 0)
  {
    m_LAB->m_Function_Generator.stop (channel);
  }
  else
  {
    m_LAB->m_Function_Generator.start (channel);
  }
}

void LABSoft_Controller_Function_Generator::
cb_wave_type (Fl_Input_Choice *w, 
              long             data)
{
  WaveType wave_type;

  std::string label (w->value ());
  int channel = static_cast<int>(data);

  if (label == "Sine")
    wave_type = SINE;
  else if (label == "Half Square")
    wave_type = SQUARE_HALF;
  else if (label == "Full Square")
    wave_type = SQUARE_FULL;
  else if (label == "Square")
    wave_type = SQUARE_FULL;
  else if (label == "Triangle")
    wave_type = TRIANGLE;
  else if (label == "DC")
    wave_type = DC;
  else 
    wave_type = SINE;

  m_LAB->m_Function_Generator.wave_type (channel, wave_type);
}

void LABSoft_Controller_Function_Generator:: 
cb_frequency (Fl_Input_Choice *w, 
              long             data)
{
  int channel = static_cast<int>(data);
  LabelValue _LabelValue (w->value ());

  m_LAB->m_Function_Generator.frequency (channel, _LabelValue.actual_value ());
}

void LABSoft_Controller_Function_Generator:: 
cb_period (Fl_Input_Choice *w, 
           long             data)
{
  int channel = static_cast<int>(data);
  LabelValue _LabelValue (w->value ());

  m_LAB->m_Function_Generator.period (channel, _LabelValue.actual_value ());
}

void LABSoft_Controller_Function_Generator:: 
cb_amplitude (Fl_Input_Choice *w, 
              long             data)
{
  int channel = static_cast<int>(data);
  LabelValue _LabelValue (w->value ());

  m_LAB->m_Function_Generator.amplitude (channel, _LabelValue.actual_value ());
}

void LABSoft_Controller_Function_Generator:: 
cb_offset (Fl_Input_Choice *w, 
           long             data)
{
  int channel = static_cast<int>(data);
  LabelValue _LabelValue (w->value ());

  m_LAB->m_Function_Generator.offset (channel, _LabelValue.actual_value ());
}

void LABSoft_Controller_Function_Generator:: 
cb_duty_cycle (Fl_Input_Choice *w, 
               long             data)
{
  int channel = static_cast<int>(data);
  LabelValue _LabelValue (w->value ());

  m_LAB->m_Function_Generator.duty_cycle (channel, _LabelValue.actual_value ());
}

void LABSoft_Controller_Function_Generator:: 
cb_phase (Fl_Input_Choice *w, 
          long             data)
{
  int channel = static_cast<int>(data);
  LabelValue _LabelValue (w->value ());

  m_LAB->m_Function_Generator.phase (channel, _LabelValue.actual_value ());
}

// EOF




