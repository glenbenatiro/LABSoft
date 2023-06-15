#include "LabelValue_Test.h"

LabelValue_Test:: 
LabelValue_Test (int argc, char** argv)
  : controller (&lab, &gui)
{
  gui.controller = &controller;
  
  gui.window->show ();

  Fl::lock ();

  Fl::run ();
}