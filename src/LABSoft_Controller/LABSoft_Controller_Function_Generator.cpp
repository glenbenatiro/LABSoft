#include "LABSoft_Controller_Function_Generator.h"

#include "LABSoft_Controller.h"
#include "../Utility/LAB_LabelValue.h"
#include "../Utility/LAB_Utility_Functions.h"
#include "../Utility/LABSoft_GUI_Label_Values.h"

LABSoft_Controller_Function_Generator::
LABSoft_Controller_Function_Generator (LAB*                 _LAB, 
                                       LABSoft_GUI*         _LABSoft_GUI,
                                       LABSoft_Controller*  _LABSoft_Controller)

  : m_LAB                 (_LAB), 
    m_LABSoft_GUI         (_LABSoft_GUI),
    m_LABSoft_Controller  (_LABSoft_Controller)
{
  init_gui_values ();
}

void LABSoft_Controller_Function_Generator:: 
init_gui_values ()
{
  LAB_Function_Generator& gen = m_LAB->m_Function_Generator;
  LABSoft_GUI&            gui = *m_LABSoft_GUI;

  // Channel 0
  Fl_Choice& w = *(gui.function_generator_fl_choice_wave_type);

  w.value (w.find_index (LABS_GUI_VALUES::FUNC_GEN::WAVE_TYPE.at 
    (gen.wave_type (0)).c_str ()));

  // gui.function_generator_fl_input_choice_amplitude->value (
  //   LAB_LabelValue (gen.amplitude (0)).to_label_text 
  //   (LAB_LabelValue::UNIT::VOLT).c_str ()
  // );

  gui.function_generator_fl_input_choice_frequency->value (
    LAB_LabelValue (gen.frequency (0)).to_label_text 
    (LAB_LabelValue::UNIT::HERTZ).c_str ()
  );

  gui.function_generator_fl_input_choice_period->value (
    LAB_LabelValue (gen.period (0)).to_label_text
    (LAB_LabelValue::UNIT::SECOND).c_str ()
  );

  gui.function_generator_fl_input_choice_phase->value (
    LAB_LabelValue (gen.phase (0)).to_label_text 
    (LAB_LabelValue::UNIT::DEGREE).c_str ()
  );

  // gui.function_generator_fl_input_choice_vertical_offset->value (
  //   LAB_LabelValue (gen.vertical_offset (0)).to_label_text
  //   (LAB_LabelValue::UNIT::VOLT).c_str ()
  // );
}

void LABSoft_Controller_Function_Generator:: 
update_gui_frequency_elements ()
{
  m_LABSoft_GUI->function_generator_fl_input_choice_frequency->
    value (LAB_LabelValue (m_LAB->m_Function_Generator.frequency (0)).
    to_label_text (LAB_LabelValue::UNIT::HERTZ).c_str ());
  
  m_LABSoft_GUI->function_generator_fl_input_choice_period->
    value (LAB_LabelValue (m_LAB->m_Function_Generator.period (0)).
    to_label_text (LAB_LabelValue::UNIT::SECOND).c_str ());
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
  LABE::FUNC_GEN::WAVE_TYPE wave_type;

  if (label == "Sine")
  {
    wave_type = LABE::FUNC_GEN::WAVE_TYPE::SINE;
  }
  else if (label == "Triangle")
  {
    wave_type = LABE::FUNC_GEN::WAVE_TYPE::TRIANGLE;
  }
  else if (label == "Square")
  {
    wave_type = LABE::FUNC_GEN::WAVE_TYPE::SQUARE;
  }
  else if (label == "DC")
  {
    wave_type = LABE::FUNC_GEN::WAVE_TYPE::DC;
  }
 
  m_LAB->m_Function_Generator.wave_type (channel, wave_type);
}

void LABSoft_Controller_Function_Generator:: 
cb_amplitude (Fl_Input_Choice* w, 
              long             channel)
{
  LAB_LabelValue lv (
    w->value (),
    m_LAB->m_Function_Generator.amplitude (channel),
    LAB_LabelValue::UNIT::VOLT
  );

  if (lv.is_valid ())
  {
    if (LABF::is_within_range (
      lv.actual_value (),
      LABC::FUNC_GEN::MIN_AMPLITUDE,
      LABC::FUNC_GEN::MAX_AMPLITUDE
    ))
    {
      m_LAB->m_Function_Generator.amplitude (channel, lv.actual_value ());
    }
  }

  w->value (LAB_LabelValue (m_LAB->m_Function_Generator.amplitude (channel)).
    to_label_text (LAB_LabelValue::UNIT::VOLT).c_str ());
}


void LABSoft_Controller_Function_Generator:: 
cb_frequency (Fl_Input_Choice *w, 
              long             channel)
{
  LAB_LabelValue lv (
    w->value (),
    m_LAB->m_Function_Generator.frequency (channel),
    LAB_LabelValue::UNIT::HERTZ
  );

  if (lv.is_valid ())
  {
    if (LABF::is_within_range (
      lv.actual_value (), 
      LABC::FUNC_GEN::MIN_FREQUENCY,
      LABC::FUNC_GEN::MAX_FREQUENCY
    ))
    {
      m_LAB->m_Function_Generator.frequency (channel, lv.actual_value ());
    }
  }

  update_gui_frequency_elements ();
}

void LABSoft_Controller_Function_Generator:: 
cb_period (Fl_Input_Choice* w, 
           long             channel)
{
  LAB_LabelValue lv (
    w->value (),
    m_LAB->m_Function_Generator.period (channel),
    LAB_LabelValue::UNIT::SECOND
  );
  
  if (lv.is_valid ())
  {
    if (LABF::is_within_range (
      lv.actual_value (), 
      LABC::FUNC_GEN::MIN_PERIOD,
      LABC::FUNC_GEN::MAX_PERIOD
    ))
    {
      m_LAB->m_Function_Generator.period (channel, lv.actual_value ());
    }
  }

  update_gui_frequency_elements ();
}

void LABSoft_Controller_Function_Generator:: 
cb_phase (Fl_Input_Choice* w, 
          long             channel)
{
  LAB_LabelValue lv (
    w->value (),
    m_LAB->m_Function_Generator.phase (channel),
    LAB_LabelValue::UNIT::DEGREE
  );

  if (lv.is_valid ())
  {
    if (LABF::is_within_range (
      lv.actual_value (), 
      LABC::FUNC_GEN::MIN_PHASE,
      LABC::FUNC_GEN::MAX_PHASE
    ))
    {
      m_LAB->m_Function_Generator.phase (channel, lv.actual_value ());
    }
  }

  w->value (LAB_LabelValue (m_LAB->m_Function_Generator.phase (channel)).
    to_label_text (LAB_LabelValue::UNIT::DEGREE).c_str ());
}


void LABSoft_Controller_Function_Generator:: 
cb_vertical_offset (Fl_Input_Choice*  w, 
                    long              channel)
{
  LAB_LabelValue lv (
    w->value (),
    m_LAB->m_Function_Generator.vertical_offset (channel),
    LAB_LabelValue::UNIT::VOLT
  );

  if (lv.is_valid ())
  {
    if (LABF::is_within_range (
      lv.actual_value (), 
      LABC::FUNC_GEN::MIN_VERTICAL_OFFSET,
      LABC::FUNC_GEN::MAX_VERTICAL_OFFSET
    ))
    {
      m_LAB->m_Function_Generator.vertical_offset (channel, lv.actual_value ());
    }
  }

  w->value (LAB_LabelValue (m_LAB->m_Function_Generator.vertical_offset (channel)).
    to_label_text (LAB_LabelValue::UNIT::VOLT).c_str ());
}

// EOF



