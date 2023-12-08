CMAKE_MINIMUM_REQUIRED(VERSION 3.18.4)

PROJECT(LABSoft
  VERSION 0.1
  LANGUAGES CXX
)

find_package(FLTK REQUIRED NO_MODULE)  # must use "NO_MODULE" 
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
# find_package(CryptoPP REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

set(THREADS_PREFER_PTHREAD_FLAG ON)

# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

include_directories(
  ${FLTK_INCLUDE_DIRS}
  ${OPENSSL_INCLUDE_DIR}  # Add this line to include OpenSSL headers
  # ${CRYPTOPP_INCLUDE_DIR}
) 

add_definitions(${FLTK_DEFINITIONS})

ADD_EXECUTABLE(LABSoft
  src/main.cpp
  src/LABSoft.cpp
  
  src/LAB/LAB_Module.cpp
  src/LAB/LAB.cpp
  src/LAB/LAB_Oscilloscope.cpp
  src/LAB/LAB_Voltmeter.cpp
  src/LAB/LAB_Function_Generator.cpp
  src/LAB/LAB_Logic_Analyzer.cpp
  src/LAB/LAB_Digital_Circuit_Checker.cpp

  src/LAB/Software/LAB_Exporter.cpp
  src/LAB/Software/LAB_Calibration.cpp
  src/LAB/Software/LAB_LABChecker_Digital.cpp
  src/LAB/Software/LAB_Oscilloscope_Display.cpp

  src/LABSoft_Presenter/LABSoft_Presenter_Unit.cpp
  src/LABSoft_Presenter/LABSoft_Presenter.cpp
  src/LABSoft_Presenter/LABSoft_Presenter_Oscilloscope.cpp
  src/LABSoft_Presenter/LABSoft_Presenter_Voltmeter.cpp
  src/LABSoft_Presenter/LABSoft_Presenter_Function_Generator.cpp
  src/LABSoft_Presenter/LABSoft_Presenter_Logic_Analyzer.cpp
  src/LABSoft_Presenter/LABSoft_Presenter_Digital_Circuit_Checker.cpp
  src/LABSoft_Presenter/LABSoft_Presenter_Main_Window.cpp

  src/LABSoft_Presenter/Software/LABSoft_Presenter_Exporter.cpp
  src/LABSoft_Presenter/Software/LABSoft_Presenter_Calibration.cpp
  src/LABSoft_Presenter/Software/LABSoft_Presenter_LABChecker_Digital.cpp
  src/LABSoft_Presenter/Software/LABSoft_Presenter_Oscilloscope_Display.cpp

  src/LABSoft_GUI/LABSoft_GUI.cpp
  src/LABSoft_GUI/LABSoft_GUI_Fl_Slider.cpp
  src/LABSoft_GUI/LABSoft_GUI_Exporter_Data_Table.cpp
  src/LABSoft_GUI/LABSoft_GUI_Oscilloscope_Display.cpp
  src/LABSoft_GUI/LABSoft_GUI_Fl_Choice_With_Scroll.cpp
  src/LABSoft_GUI/LABSoft_GUI_Logic_Analyzer_Display.cpp
  src/LABSoft_GUI/LABSoft_GUI_Fl_Input_Choice_With_Scroll.cpp
  src/LABSoft_GUI/LABSoft_GUI_Oscilloscope_Internal_Display.cpp
  src/LABSoft_GUI/LABSoft_GUI_LABChecker_Digital_Input_Table.cpp
  src/LABSoft_GUI/LABSoft_GUI_LABChecker_Digital_Output_Table.cpp
  src/LABSoft_GUI/LABSoft_GUI_Logic_Analyzer_Add_Channel_Signal_Window.cpp
  
  src/LAB_Libraries/LAB_AD9833.cpp
  src/LAB_Libraries/LAB_MCP4XXX.cpp
  src/LAB_Libraries/LAB_MCP23S17.cpp

  src/Utility/LABSoft_GUI_Label.cpp
  src/Utility/pugixml.cpp
  src/Utility/LAB_Utility_Functions.cpp
  # src/Utility/LAB_Encryptor.cpp

  lib/AikaPi/AikaPi.cpp
  lib/AD9833/AD9833.cpp
  lib/MCP4XXX/MCP4XXX.cpp
  lib/MCP23S17/MCP23S17.cpp
)

target_link_libraries(LABSoft PRIVATE
  fltk
  Threads::Threads
  ${OPENSSL_LIBRARIES}  # Add this line to link against OpenSSL
  # cryptopp-static
)

#add_compile_options(-I/usr/include/cairo -I/usr/include/glib-2.0 -I/usr/lib/arm-linux-gnueabihf/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/uuid -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/uuid -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/cairo -I/usr/include/glib-2.0 -I/usr/lib/arm-linux-gnueabihf/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/uuid -I/usr/include/freetype2 -I/usr/include/libpng16)
#add_link_options(-lpthread)

