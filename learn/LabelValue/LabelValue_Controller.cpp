#include "LabelValue_Controller.h"
#include "../../src/LabelValue.h"

#include <iostream>

LabelValue_Controller:: 
LabelValue_Controller (LAB* _LAB, LabelValue_GUI* _LabelValue_GUI)
{
  lab = _LAB;
  gui = _LabelValue_GUI;
}

void LabelValue_Controller:: 
cb_input (Fl_Input_Choice* w, void* data)
{
  // get unit from choice
  const char* choice_text = gui->choice->text ();

  LabelValue::UNIT unit = LabelValue::get_unit_from_unit_string (choice_text);

  LabelValue lv (
    w->value    (),
    lab->value  (),
    unit
  );

  if (lv.is_valid ())
  {
    lab->value (lv.actual_value ());
  }

  LabelValue newval (lab->value ());

  gui->output->value (newval.to_label_text (unit).c_str ());
  gui->input->value (newval.to_label_text (unit).c_str ());

  lv.debug ();
}