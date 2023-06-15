#include "LabelValue_GUI.h"
#include "LAB.h"

#include <FL/Fl_Input_Choice.H>

class LabelValue_Controller
{
  private:
    LAB*            lab;
    LabelValue_GUI* gui;

  public:
    LabelValue_Controller (LAB* _LAB, LabelValue_GUI* _LabelValue_GUI);

    void cb_input (Fl_Input_Choice* w, void* data);
};