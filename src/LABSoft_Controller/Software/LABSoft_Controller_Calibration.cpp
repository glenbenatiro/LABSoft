#include "LABSoft_Controller_Calibration.h"

#include <cstdio>
#include <sstream>
#include <fstream>

LABSoft_Controller_Calibration::
LABSoft_Controller_Calibration (LABSoft_Controller& _LABSoft_Controller)
  : LABSoft_Controller_Unit  (_LABSoft_Controller)
{

}

void LABSoft_Controller_Calibration::
cb_show_window ()
{
  gui ().main_fl_window_calibration->set_modal ();
  gui ().main_fl_window_calibration->show ();
}

void LABSoft_Controller_Calibration::
cb_cancel ()
{
  gui ().main_fl_window_calibration->hide ();
  gui ().main_fl_window_calibration->clear_modal_states ();
}
