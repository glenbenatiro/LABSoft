#include <iostream>

#include <FL/Fl.H>
#include <FL/Fl_Output.H>
#include <FL/Enumerations.H>
#include <FL/Fl_Input_Choice.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Double_Window.H>

#include "LABSoft_GUI_Fl_Input_Choice_With_Scroll.h"

// globals
Fl_Output* output = nullptr;

void cb_input_choice (LABSoft_GUI_Fl_Input_Choice_With_Scroll* w, void* data)
{
  std::cout << "Callback!" << "\n";
}

int main ()
{
  Fl_Double_Window* window = new Fl_Double_Window (100, 100, 600, 400, "input-choice-with-scroll");

  LABSoft_GUI_Fl_Input_Choice_With_Scroll* input_choice = 
    new LABSoft_GUI_Fl_Input_Choice_With_Scroll (240, 165, 120, 30);

  Fl_Menu_Item menu_input_choice[] = 
  {
    {"5 V/div", 0,  0, (void*)("5"), 0, (uchar)FL_NORMAL_LABEL, 0, 12, 0},
    {"2 V/div", 0,  0, (void*)("2"), 0, (uchar)FL_NORMAL_LABEL, 0, 12, 0},
    {"1 V/div", 0,  0, (void*)("1"), 0, (uchar)FL_NORMAL_LABEL, 0, 12, 0},
    {"500 mV/div", 0,  0, (void*)("0.5"), 0, (uchar)FL_NORMAL_LABEL, 0, 12, 0},
    {"200 mV/div", 0,  0, (void*)("0.2"), 0, (uchar)FL_NORMAL_LABEL, 0, 12, 0},
    {"100 mV/div", 0,  0, (void*)("0.1"), 0, (uchar)FL_NORMAL_LABEL, 0, 12, 0},
    {"50 mV/div", 0,  0, (void*)("0.05"), 0, (uchar)FL_NORMAL_LABEL, 0, 12, 0},
    {"20 mV/div", 0,  0, (void*)("0.02"), 0, (uchar)FL_NORMAL_LABEL, 0, 12, 0},
    {"10 mV/div", 0,  0, (void*)("0.01"), 0, (uchar)FL_NORMAL_LABEL, 0, 12, 0},
    {"5 mV/div", 0,  0, (void*)("0.005"), 0, (uchar)FL_NORMAL_LABEL, 0, 12, 0},
    {"2 mV/div", 0,  0, (void*)("0.002"), 0, (uchar)FL_NORMAL_LABEL, 0, 12, 0},
    {"1 mV/div", 0,  0, (void*)("0.001"), 0, (uchar)FL_NORMAL_LABEL, 0, 12, 0},
    {"500 uV/div", 0,  0, (void*)("0.0005"), 0, (uchar)FL_NORMAL_LABEL, 0, 12, 0},
    {"200 uV/div", 0,  0, (void*)("0.0002"), 0, (uchar)FL_NORMAL_LABEL, 0, 12, 0},
    {"100 uV/div", 0,  0, (void*)("0.0001"), 0, (uchar)FL_NORMAL_LABEL, 0, 12, 0},
    {0,0,0,0,0,0,0,0,0}
  };

  input_choice->menu (menu_input_choice);
  input_choice->callback ((Fl_Callback*)(cb_input_choice), nullptr);
  input_choice->value (0);

  output = new Fl_Output (240, 205, 120, 30);

  Fl::visual (FL_DOUBLE | FL_INDEX);

  window->show ();

  return (Fl::run ());
}