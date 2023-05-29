#include "LABSoft_Controller.h"

#include <FL/Fl.H>

#include "Defaults.h"

LABSoft_Controller:: 
LABSoft_Controller (LAB* _LAB, LABSoft_GUI* _LABSoft_GUI)
  : m_LAB                 (_LAB),
    m_LABSoft_GUI         (_LABSoft_GUI),
    m_Oscilloscope        (_LAB, _LABSoft_GUI, this),
    m_Voltmeter           (_LAB, _LABSoft_GUI, this),
    m_Function_Generator  (_LAB, _LABSoft_GUI, this),
    m_Logic_Analyzer      (_LAB, _LABSoft_GUI, this),
    m_Circuit_Checker     (_LAB, _LABSoft_GUI, this)   
{
  Fl::add_timeout (
    LABC::LABSOFT::DISPLAY_UPDATE_RATE,
    update_display,
    this
  );
}

void LABSoft_Controller::
update_display (void *data)
{
  LABSoft_Controller& controller = *((LABSoft_Controller*)(data));

  controller.m_Oscilloscope.display_update_cycle    ();
  controller.m_Voltmeter.display_update_cycle       (); 
  controller.m_Logic_Analyzer.display_update_cycle  ();

  Fl::repeat_timeout (LABC::LABSOFT::DISPLAY_UPDATE_RATE, update_display, data);  
}

/**
 * @brief Sets the selection color of the specific LAB module FL Tab to green
 *        if it is active, or to the default color if inactive
 */
void LABSoft_Controller::
tab_selection_color_toggle (Fl_Group* tab, 
                            bool      value)
{
  tab->selection_color (static_cast<uint32_t>(
    value ? LABC::LABSOFT::FL_TAB_GROUP_COLOR::GREEN : 
            LABC::LABSOFT::FL_TAB_GROUP_COLOR::DEFAULT
    )
  );

  main_tab_selection_color ();
}

/**
 * @brief Sets the selection color of the main FL Tab widget to whatever the
 *        selection color is of the currently selected tab 
 */
void LABSoft_Controller:: 
main_tab_selection_color ()
{
  m_LABSoft_GUI->main_fl_tabs->selection_color (
    m_LABSoft_GUI->main_fl_tabs->value ()->selection_color ()
  );

  m_LABSoft_GUI->main_fl_tabs->redraw ();
}

void LABSoft_Controller:: 
cb_main_fl_tabs (Fl_Tabs* w,
                 void*    data)
{
  main_tab_selection_color ();
}