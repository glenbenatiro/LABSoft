CMAKE_MINIMUM_REQUIRED(VERSION 3.18.4)

PROJECT(LABSoft
  VERSION 0.1
  LANGUAGES CXX
)

FIND_PACKAGE(FLTK REQUIRED NO_MODULE)  # must use "NO_MODULE" 
FIND_PACKAGE(Threads REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)

include_directories(${FLTK_INCLUDE_DIRS}) 
link_directories(${FLTK_LIBRARY_DIRS}) 
add_definitions(${FLTK_DEFINITIONS})

ADD_EXECUTABLE(LABSoft
  src/main.cpp

  src/LABSoft.cpp
  src/LAB.cpp

  src/LAB_Core.cpp
  src/LAB_Oscilloscope.cpp
  src/LAB_Meter.cpp
  src/LAB_Voltmeter.cpp
  src/LAB_Ammeter.cpp
  src/LAB_Ohmmeter.cpp
  src/LAB_Function_Generator.cpp
  src/LAB_Logic_Analyzer.cpp
  src/LAB_Power_Supply.cpp
  src/LAB_Circuit_Checker.cpp

  src/LABSoft_Controller.cpp
  src/LABSoft_Controller_Oscilloscope.cpp
  src/LABSoft_Controller_Meter.cpp
  src/LABSoft_Controller_Voltmeter.cpp
  src/LABSoft_Controller_Ammeter.cpp
  src/LABSoft_Controller_Ohmmeter.cpp
  src/LABSoft_Controller_Function_Generator.cpp
  src/LABSoft_Controller_Logic_Analyzer.cpp
  src/LABSoft_Controller_Power_Supply.cpp
  src/LABSoft_Controller_Circuit_Checker.cpp
  src/LABSoft_Controller_Main.cpp

  src/LABSoft_GUI.cpp
  src/LABSoft_Oscilloscope_Display_Group.cpp
  src/LABSoft_Oscilloscope_Display.cpp

  src/Auxiliary.cpp
  src/Channel_Signals.cpp
  src/Channel_Signal.cpp
  src/LabelValue.cpp
  src/pugixml.cpp
  src/jesspi.cpp
)

TARGET_LINK_LIBRARIES(LABSoft PRIVATE fltk Threads::Threads ) # notice fltk here
#add_compile_options(-I/usr/include/cairo -I/usr/include/glib-2.0 -I/usr/lib/arm-linux-gnueabihf/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/uuid -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/uuid -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/cairo -I/usr/include/glib-2.0 -I/usr/lib/arm-linux-gnueabihf/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/uuid -I/usr/include/freetype2 -I/usr/include/libpng16)
#add_link_options(-lpthread)

