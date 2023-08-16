// generated by Fast Light User Interface Designer (fluid) version 1.0305

#include "LABSoft_GUI_Oscilloscope_Display.h"
labsoft_gui_oscilloscope_display::labsoft_gui_oscilloscope_display(int X, int Y, int W, int H, const char *L)
  : LABSoft_GUI_Oscilloscope_Display(X, Y, W, H, L) {
this->box(FL_FLAT_BOX);
this->color(FL_FOREGROUND_COLOR);
this->selection_color(FL_BACKGROUND_COLOR);
this->labeltype(FL_NO_LABEL);
this->labelfont(0);
this->labelsize(14);
this->labelcolor(FL_FOREGROUND_COLOR);
this->align(Fl_Align(FL_ALIGN_TOP));
this->when(FL_WHEN_RELEASE);
{ Fl_Group* o = new Fl_Group(80, 20, 610, 470);
  o->box(FL_FLAT_BOX);
  o->color((Fl_Color)103);
  o->labelsize(12);
  o->end();
} // Fl_Group* o
{ c1 vpd unit = new Fl_Box(0, 20, 3, 20, "C1 mV");
  c1 vpd unit->labelsize(9);
  c1 vpd unit->labelcolor((Fl_Color)3);
  c1 vpd unit->align(Fl_Align(FL_ALIGN_TOP_LEFT));
} // Fl_Box* c1 vpd unit
{ c2 vpd unit = new Fl_Box(40, 20, 3, 20, "C2 mV");
  c2 vpd unit->labelsize(9);
  c2 vpd unit->labelcolor((Fl_Color)6);
  c2 vpd unit->align(Fl_Align(FL_ALIGN_TOP_LEFT));
} // Fl_Box* c2 vpd unit
{ c1 volts per div first = new Fl_Box(0, 20, 3, 20, "2.345");
  c1 volts per div first->labelsize(12);
  c1 volts per div first->labelcolor((Fl_Color)3);
  c1 volts per div first->align(Fl_Align(FL_ALIGN_TOP_LEFT|FL_ALIGN_INSIDE));
} // Fl_Box* c1 volts per div first
{ c2 volts per div first = new Fl_Box(40, 20, 3, 20, "1.234");
  c2 volts per div first->labelsize(12);
  c2 volts per div first->labelcolor((Fl_Color)6);
  c2 volts per div first->align(Fl_Align(FL_ALIGN_TOP_LEFT|FL_ALIGN_INSIDE));
} // Fl_Box* c2 volts per div first
{ c1 volts per div mid = new Fl_Box(0, 255, 3, 20, "2.345");
  c1 volts per div mid->labelsize(12);
  c1 volts per div mid->labelcolor((Fl_Color)3);
  c1 volts per div mid->align(Fl_Align(FL_ALIGN_TOP_LEFT|FL_ALIGN_INSIDE));
} // Fl_Box* c1 volts per div mid
{ c2 volts per div mid = new Fl_Box(40, 255, 3, 20, "1.234");
  c2 volts per div mid->labelsize(12);
  c2 volts per div mid->labelcolor((Fl_Color)6);
  c2 volts per div mid->align(Fl_Align(FL_ALIGN_TOP_LEFT|FL_ALIGN_INSIDE));
} // Fl_Box* c2 volts per div mid
{ c1 volts per div last = new Fl_Box(0, 490, 3, 20, "2.345");
  c1 volts per div last->labelsize(12);
  c1 volts per div last->labelcolor((Fl_Color)3);
  c1 volts per div last->align(Fl_Align(FL_ALIGN_TOP_LEFT));
} // Fl_Box* c1 volts per div last
{ c2 volts per div last = new Fl_Box(40, 490, 3, 20, "1.234");
  c2 volts per div last->labelsize(12);
  c2 volts per div last->labelcolor((Fl_Color)6);
  c2 volts per div last->align(Fl_Align(FL_ALIGN_TOP_LEFT));
} // Fl_Box* c2 volts per div last
{ Fl_Box* o = new Fl_Box(80, 0, 80, 20, "Ready");
  o->box(FL_BORDER_FRAME);
  o->color((Fl_Color)1);
  o->labelfont(1);
  o->labelsize(12);
  o->labelcolor(FL_BACKGROUND2_COLOR);
} // Fl_Box* o
{ Fl_Box* o = new Fl_Box(160, 0, 20, 20, "C1");
  o->box(FL_THIN_UP_BOX);
  o->color(FL_FOREGROUND_COLOR);
  o->labelsize(12);
  o->labelcolor((Fl_Color)3);
} // Fl_Box* o
{ Fl_Box* o = new Fl_Box(180, 0, 20, 20, "C2");
  o->box(FL_THIN_UP_BOX);
  o->color(FL_FOREGROUND_COLOR);
  o->labelsize(12);
  o->labelcolor((Fl_Color)6);
} // Fl_Box* o
{ Fl_Box* o = new Fl_Box(200, 0, 20, 20, "2000 samples at 200 kHz | 2023-08-15 13:03:41:231");
  o->color(FL_FOREGROUND_COLOR);
  o->labelsize(12);
  o->labelcolor(FL_BACKGROUND2_COLOR);
  o->align(Fl_Align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE));
} // Fl_Box* o
{ Fl_Slider* o = new Fl_Slider(80, 20, 10, 470, "slider:");
  o->box(FL_NO_BOX);
  o->color((Fl_Color)3);
  o->selection_color((Fl_Color)3);
} // Fl_Slider* o
{ Fl_Slider* o = new Fl_Slider(680, 20, 10, 470, "slider:");
  o->box(FL_NO_BOX);
  o->color((Fl_Color)3);
  o->selection_color((Fl_Color)3);
} // Fl_Slider* o
{ Fl_Slider* o = new Fl_Slider(80, 20, 620, 10);
  o->type(1);
  o->box(FL_NO_BOX);
  o->color((Fl_Color)3);
  o->selection_color((Fl_Color)3);
} // Fl_Slider* o
{ time per div first = new Fl_Box(80, 490, 3, 20, "-5 ms");
  time per div first->labelsize(12);
  time per div first->labelcolor(FL_BACKGROUND2_COLOR);
} // Fl_Box* time per div first
{ time per div mid = new Fl_Box(385, 490, 3, 20, "-5 ms");
  time per div mid->labelsize(12);
  time per div mid->labelcolor(FL_BACKGROUND2_COLOR);
} // Fl_Box* time per div mid
{ time per div last = new Fl_Box(690, 490, 3, 20, "-5 ms");
  time per div last->labelsize(12);
  time per div last->labelcolor(FL_BACKGROUND2_COLOR);
  time per div last->align(Fl_Align(FL_ALIGN_LEFT));
} // Fl_Box* time per div last
end();
}
