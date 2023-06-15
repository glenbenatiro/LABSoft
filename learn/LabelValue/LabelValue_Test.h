#include <FL/Fl.H>

#include "LAB.h"
#include "LabelValue_GUI.h"
#include "LabelValue_Controller.h"

class LabelValue_Test
{
  private:
    LAB                   lab;
    LabelValue_GUI        gui;
    LabelValue_Controller controller;

  public:
    LabelValue_Test (int argc, char** argv);
};