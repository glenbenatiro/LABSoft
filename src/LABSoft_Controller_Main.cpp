#include "LABSoft_Controller_Main.h"

LABSoft_Controller_Main:: 
LABSoft_Controller_Main (LAB *_LAB, LABSoft_GUI *_LABSoft_GUI)
{
  m_LAB         = _LAB;
  m_LABSoft_GUI = _LABSoft_GUI;
}

void LABSoft_Controller_Main:: 
LABSoft_Controller_Main_cb_update_display (void *data)
{
  printf ("Update!\n");

  // sure??
  LABSoft_GUI *gui = static_cast<LABSoft_GUI *>(data);
  gui->main_fl_window->flush ();

  Fl::repeat_timeout (DISPLAY_UPDATE_RATE, LABSoft_Controller_Main_cb_update_display, (void *)gui);
}