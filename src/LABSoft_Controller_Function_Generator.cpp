#include "LABSoft_Controller_Function_Generator.h"

LABSoft_Controller_Function_Generator::
LABSoft_Controller_Function_Generator (LAB* _LAB, LABSoft_GUI* _LABSoft_GUI)
  : m_LAB (_LAB), m_LABSoft_GUI (_LABSoft_GUI)
{
  init_gui ();
}

void LABSoft_Controller_Function_Generator:: 
init_gui ()
{
  
}

void LABSoft_Controller_Function_Generator::
cb_run_stop (Fl_Light_Button* w, 
             long             channel)
{
  if (w->value () == 0)
  {
    m_LAB->m_Function_Generator.stop (channel);
  }
  else
  {
    m_LAB->m_Function_Generator.run (channel);
  }
}

void LABSoft_Controller_Function_Generator::
cb_wave_type (Fl_Choice* w, 
              long       channel)
{
  std::string label (w->text ());
  LABC::FUNC_GEN::WAVE_TYPE wave_type;

  if (label == "Sine")
  {
    wave_type = LABC::FUNC_GEN::WAVE_TYPE::SINE;
  }
  else if (label == "Triangle")
  {
    wave_type = LABC::FUNC_GEN::WAVE_TYPE::TRIANGLE;
  }
  else if (label == "Square")
  {
    wave_type = LABC::FUNC_GEN::WAVE_TYPE::SQUARE;
  }
  else if (label == "DC")
  {
    wave_type = LABC::FUNC_GEN::WAVE_TYPE::DC;
  }
 
  m_LAB->m_Function_Generator.wave_type (channel, wave_type);
}

void LABSoft_Controller_Function_Generator:: 
cb_amplitude (Fl_Input_Choice* w, 
              long             channel)
{
  LabelValue lv (
    w->value (),
    m_LAB->m_Function_Generator.amplitude (channel),
    LabelValue::TYPE::VOLTS
  );

  if (lv.is_valid_label_text ())
  {
    if (lv.actual_value () >= LABC::FUNC_GEN::MIN_AMPLITUDE &&
      lv.actual_value () <= LABC::FUNC_GEN::MAX_AMPLITUDE)
    {
      m_LAB->m_Function_Generator.amplitude (channel, lv.actual_value ());
    }
  }

  w->value (LabelValue (m_LAB->m_Function_Generator.amplitude (channel)).
    to_label_text (LabelValue::TYPE::VOLTS).c_str ());
}


void LABSoft_Controller_Function_Generator:: 
cb_frequency (Fl_Input_Choice *w, 
              long             channel)
{
  LabelValue lv (
    w->value (),
    m_LAB->m_Function_Generator.frequency (channel),
    LabelValue::TYPE::HERTZ
  );

  if (lv.is_valid_label_text ())
  {
    if (lv.actual_value () >= LABC::FUNC_GEN::MIN_FREQUENCY &&
      lv.actual_value () <= LABC::FUNC_GEN::MAX_FREQUENCY)
    {
      m_LAB->m_Function_Generator.frequency (channel, lv.actual_value ());
    }
  }

  w->value (LabelValue (m_LAB->m_Function_Generator.frequency (channel)).
    to_label_text (LabelValue::TYPE::HERTZ).c_str ());
}

void LABSoft_Controller_Function_Generator:: 
cb_period (Fl_Input_Choice* w, 
           long             channel)
{
  LabelValue lv (
    w->value (),
    m_LAB->m_Function_Generator.period (channel),
    LabelValue::TYPE::SECONDS
  );

  if (lv.is_valid_label_text ())
  {
    if (lv.actual_value () >= LABC::FUNC_GEN::MIN_PERIOD &&
      lv.actual_value () <= LABC::FUNC_GEN::MAX_PERIOD)
    {
      m_LAB->m_Function_Generator.period (channel, lv.actual_value ());
    }
  }

  w->value (LabelValue (m_LAB->m_Function_Generator.period (channel)).
    to_label_text (LabelValue::TYPE::SECONDS).c_str ());
}

void LABSoft_Controller_Function_Generator:: 
cb_phase (Fl_Input_Choice* w, 
          long             channel)
{
  LabelValue lv (
    w->value (),
    m_LAB->m_Function_Generator.phase (channel),
    LabelValue::TYPE::DEGREES
  );

  if (lv.is_valid_label_text ())
  {
    if (lv.actual_value () >= LABC::FUNC_GEN::MIN_PHASE &&
      lv.actual_value () <= LABC::FUNC_GEN::MAX_PHASE)
    {
      m_LAB->m_Function_Generator.phase (channel, lv.actual_value ());
    }
  }

  w->value (LabelValue (m_LAB->m_Function_Generator.phase (channel)).
    to_label_text (LabelValue::TYPE::DEGREES).c_str ());
}


void LABSoft_Controller_Function_Generator:: 
cb_offset (Fl_Input_Choice* w, 
           long             channel)
{
  LabelValue lv (
    w->value (),
    m_LAB->m_Function_Generator.vertical_offset (channel),
    LabelValue::TYPE::VOLTS
  );

  if (lv.is_valid_label_text ())
  {
    if (lv.actual_value () >= LABC::FUNC_GEN::MIN_VERTICAL_OFFSET &&
      lv.actual_value () <= LABC::FUNC_GEN::MAX_VERTICAL_OFFSET)
    {
      m_LAB->m_Function_Generator.vertical_offset (channel, lv.actual_value ());
    }
  }

  w->value (LabelValue (m_LAB->m_Function_Generator.vertical_offset (channel)).
    to_label_text (LabelValue::TYPE::VOLTS).c_str ());
}

// EOF




