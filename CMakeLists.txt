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
  src/LABSoft_GUI.cpp
  src/LABSoft_Oscilloscope_Display.cpp
  src/LABSoft_Function_Generator_Display.cpp
  src/LAB.cpp
  src/LAB_Core.cpp
  src/LAB_Oscilloscope.cpp
  src/LAB_Function_Generator.cpp
)

TARGET_LINK_LIBRARIES(LABSoft PRIVATE fltk Threads::Threads ) # notice fltk here
#add_compile_options(-I/usr/include/cairo -I/usr/include/glib-2.0 -I/usr/lib/arm-linux-gnueabihf/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/uuid -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/uuid -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/cairo -I/usr/include/glib-2.0 -I/usr/lib/arm-linux-gnueabihf/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/uuid -I/usr/include/freetype2 -I/usr/include/libpng16)
#add_link_options(-lpthread)

