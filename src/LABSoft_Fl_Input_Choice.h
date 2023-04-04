#ifndef LABSOFT_FL_INPUT_CHOICE_H
#define LABSOFT_FL_INPUT_CHOICE_H

#include <FL/Enumerations.H>
#include <FL/Fl_Input_Choice.H>
#include <FL/names.h>

#include <iostream>

class LABSoft_Fl_Input_Choice : public Fl_Input_Choice 
{
  public: 
    LABSoft_Fl_Input_Choice (int X, int Y, int W, int H, const char *lbl = 0) : 
      Fl_Input_Choice (X, Y, W, H, lbl)
    {

    }

    int Fl_Input_Choice::InputMenuButton::handle(int e) override {
      if (!menu() || !menu()->text) return 0;
      switch (e) {
      case FL_ENTER: /* FALLTHROUGH */
      case FL_LEAVE:
        return (box() && !type()) ? 1 : 0;
      case FL_PUSH:
        if (!box()) {
          if (Fl::event_button() != 3) return 0;
        } else if (type()) {
          if (!(type() & (1 << (Fl::event_button()-1)))) return 0;
        }
        if (Fl::visible_focus()) Fl::focus(this);
        popup();
        return 1;
      case FL_KEYBOARD:
        if (!box()) return 0;
        if (Fl::event_key() == ' ' &&
            !(Fl::event_state() & (FL_SHIFT | FL_CTRL | FL_ALT | FL_META))) {
          popup();
          return 1;
        } else return 0;
      case FL_SHORTCUT:
        if (Fl_Widget::test_shortcut()) {popup(); return 1;}
        return test_shortcut() != 0;
      case FL_FOCUS: /* FALLTHROUGH */
      case FL_UNFOCUS:
        if (box() && Fl::visible_focus()) {
          redraw();
          return 1;
        }
      default:
        return 0;
      }
    }

    // void inp_cb(Fl_Widget*, void *data) override {
    //   std::cout << "hello" << std::endl;
      
    //   // Fl_Input_Choice *o=(Fl_Input_Choice *)data;
    //   // Fl_Widget_Tracker wp(o);
    //   // if (o->input ()->changed()) {
    //   //   o->Fl_Widget::set_changed();
    //   //   if (o->when() & (FL_WHEN_CHANGED|FL_WHEN_RELEASE))
    //   //     o->do_callback(FL_REASON_CHANGED);
    //   // } else {
    //   //   o->Fl_Widget::clear_changed();
    //   //   if (o->when() & FL_WHEN_NOT_CHANGED)
    //   //     o->do_callback(FL_REASON_RESELECTED);
    //   // }

    //   // if (wp.deleted()) return;

    //   // if (o->callback() != default_callback)
    //   //   o->Fl_Widget::clear_changed();
    // }
    

};

#endif