#include "LABSoft.h"

LABSoft::LABSoft (int argc, char** argv) {
  m_LAB   = new LAB ();
  m_LABSoft_GUI = new LABSoft_GUI (argc, argv, m_LAB);
}

LABSoft::~LABSoft() {
}
