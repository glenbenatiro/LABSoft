# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/glen/Desktop/LABSoft

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/glen/Desktop/LABSoft/build

# Include any dependencies generated for this target.
include CMakeFiles/LABSoft.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/LABSoft.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/LABSoft.dir/flags.make

CMakeFiles/LABSoft.dir/src/main.cpp.o: CMakeFiles/LABSoft.dir/flags.make
CMakeFiles/LABSoft.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/glen/Desktop/LABSoft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/LABSoft.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LABSoft.dir/src/main.cpp.o -c /home/glen/Desktop/LABSoft/src/main.cpp

CMakeFiles/LABSoft.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LABSoft.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/glen/Desktop/LABSoft/src/main.cpp > CMakeFiles/LABSoft.dir/src/main.cpp.i

CMakeFiles/LABSoft.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LABSoft.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/glen/Desktop/LABSoft/src/main.cpp -o CMakeFiles/LABSoft.dir/src/main.cpp.s

CMakeFiles/LABSoft.dir/src/LABSoft.cpp.o: CMakeFiles/LABSoft.dir/flags.make
CMakeFiles/LABSoft.dir/src/LABSoft.cpp.o: ../src/LABSoft.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/glen/Desktop/LABSoft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/LABSoft.dir/src/LABSoft.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LABSoft.dir/src/LABSoft.cpp.o -c /home/glen/Desktop/LABSoft/src/LABSoft.cpp

CMakeFiles/LABSoft.dir/src/LABSoft.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LABSoft.dir/src/LABSoft.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/glen/Desktop/LABSoft/src/LABSoft.cpp > CMakeFiles/LABSoft.dir/src/LABSoft.cpp.i

CMakeFiles/LABSoft.dir/src/LABSoft.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LABSoft.dir/src/LABSoft.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/glen/Desktop/LABSoft/src/LABSoft.cpp -o CMakeFiles/LABSoft.dir/src/LABSoft.cpp.s

CMakeFiles/LABSoft.dir/src/LAB.cpp.o: CMakeFiles/LABSoft.dir/flags.make
CMakeFiles/LABSoft.dir/src/LAB.cpp.o: ../src/LAB.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/glen/Desktop/LABSoft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/LABSoft.dir/src/LAB.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LABSoft.dir/src/LAB.cpp.o -c /home/glen/Desktop/LABSoft/src/LAB.cpp

CMakeFiles/LABSoft.dir/src/LAB.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LABSoft.dir/src/LAB.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/glen/Desktop/LABSoft/src/LAB.cpp > CMakeFiles/LABSoft.dir/src/LAB.cpp.i

CMakeFiles/LABSoft.dir/src/LAB.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LABSoft.dir/src/LAB.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/glen/Desktop/LABSoft/src/LAB.cpp -o CMakeFiles/LABSoft.dir/src/LAB.cpp.s

CMakeFiles/LABSoft.dir/lib/AikaPi/AikaPi.cpp.o: CMakeFiles/LABSoft.dir/flags.make
CMakeFiles/LABSoft.dir/lib/AikaPi/AikaPi.cpp.o: ../lib/AikaPi/AikaPi.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/glen/Desktop/LABSoft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/LABSoft.dir/lib/AikaPi/AikaPi.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LABSoft.dir/lib/AikaPi/AikaPi.cpp.o -c /home/glen/Desktop/LABSoft/lib/AikaPi/AikaPi.cpp

CMakeFiles/LABSoft.dir/lib/AikaPi/AikaPi.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LABSoft.dir/lib/AikaPi/AikaPi.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/glen/Desktop/LABSoft/lib/AikaPi/AikaPi.cpp > CMakeFiles/LABSoft.dir/lib/AikaPi/AikaPi.cpp.i

CMakeFiles/LABSoft.dir/lib/AikaPi/AikaPi.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LABSoft.dir/lib/AikaPi/AikaPi.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/glen/Desktop/LABSoft/lib/AikaPi/AikaPi.cpp -o CMakeFiles/LABSoft.dir/lib/AikaPi/AikaPi.cpp.s

CMakeFiles/LABSoft.dir/src/LAB_Core.cpp.o: CMakeFiles/LABSoft.dir/flags.make
CMakeFiles/LABSoft.dir/src/LAB_Core.cpp.o: ../src/LAB_Core.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/glen/Desktop/LABSoft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/LABSoft.dir/src/LAB_Core.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LABSoft.dir/src/LAB_Core.cpp.o -c /home/glen/Desktop/LABSoft/src/LAB_Core.cpp

CMakeFiles/LABSoft.dir/src/LAB_Core.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LABSoft.dir/src/LAB_Core.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/glen/Desktop/LABSoft/src/LAB_Core.cpp > CMakeFiles/LABSoft.dir/src/LAB_Core.cpp.i

CMakeFiles/LABSoft.dir/src/LAB_Core.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LABSoft.dir/src/LAB_Core.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/glen/Desktop/LABSoft/src/LAB_Core.cpp -o CMakeFiles/LABSoft.dir/src/LAB_Core.cpp.s

CMakeFiles/LABSoft.dir/src/LAB_Oscilloscope.cpp.o: CMakeFiles/LABSoft.dir/flags.make
CMakeFiles/LABSoft.dir/src/LAB_Oscilloscope.cpp.o: ../src/LAB_Oscilloscope.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/glen/Desktop/LABSoft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/LABSoft.dir/src/LAB_Oscilloscope.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LABSoft.dir/src/LAB_Oscilloscope.cpp.o -c /home/glen/Desktop/LABSoft/src/LAB_Oscilloscope.cpp

CMakeFiles/LABSoft.dir/src/LAB_Oscilloscope.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LABSoft.dir/src/LAB_Oscilloscope.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/glen/Desktop/LABSoft/src/LAB_Oscilloscope.cpp > CMakeFiles/LABSoft.dir/src/LAB_Oscilloscope.cpp.i

CMakeFiles/LABSoft.dir/src/LAB_Oscilloscope.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LABSoft.dir/src/LAB_Oscilloscope.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/glen/Desktop/LABSoft/src/LAB_Oscilloscope.cpp -o CMakeFiles/LABSoft.dir/src/LAB_Oscilloscope.cpp.s

CMakeFiles/LABSoft.dir/src/LAB_Multimeter.cpp.o: CMakeFiles/LABSoft.dir/flags.make
CMakeFiles/LABSoft.dir/src/LAB_Multimeter.cpp.o: ../src/LAB_Multimeter.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/glen/Desktop/LABSoft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/LABSoft.dir/src/LAB_Multimeter.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LABSoft.dir/src/LAB_Multimeter.cpp.o -c /home/glen/Desktop/LABSoft/src/LAB_Multimeter.cpp

CMakeFiles/LABSoft.dir/src/LAB_Multimeter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LABSoft.dir/src/LAB_Multimeter.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/glen/Desktop/LABSoft/src/LAB_Multimeter.cpp > CMakeFiles/LABSoft.dir/src/LAB_Multimeter.cpp.i

CMakeFiles/LABSoft.dir/src/LAB_Multimeter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LABSoft.dir/src/LAB_Multimeter.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/glen/Desktop/LABSoft/src/LAB_Multimeter.cpp -o CMakeFiles/LABSoft.dir/src/LAB_Multimeter.cpp.s

CMakeFiles/LABSoft.dir/src/LAB_Function_Generator.cpp.o: CMakeFiles/LABSoft.dir/flags.make
CMakeFiles/LABSoft.dir/src/LAB_Function_Generator.cpp.o: ../src/LAB_Function_Generator.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/glen/Desktop/LABSoft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/LABSoft.dir/src/LAB_Function_Generator.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LABSoft.dir/src/LAB_Function_Generator.cpp.o -c /home/glen/Desktop/LABSoft/src/LAB_Function_Generator.cpp

CMakeFiles/LABSoft.dir/src/LAB_Function_Generator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LABSoft.dir/src/LAB_Function_Generator.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/glen/Desktop/LABSoft/src/LAB_Function_Generator.cpp > CMakeFiles/LABSoft.dir/src/LAB_Function_Generator.cpp.i

CMakeFiles/LABSoft.dir/src/LAB_Function_Generator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LABSoft.dir/src/LAB_Function_Generator.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/glen/Desktop/LABSoft/src/LAB_Function_Generator.cpp -o CMakeFiles/LABSoft.dir/src/LAB_Function_Generator.cpp.s

CMakeFiles/LABSoft.dir/src/LAB_Logic_Analyzer.cpp.o: CMakeFiles/LABSoft.dir/flags.make
CMakeFiles/LABSoft.dir/src/LAB_Logic_Analyzer.cpp.o: ../src/LAB_Logic_Analyzer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/glen/Desktop/LABSoft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/LABSoft.dir/src/LAB_Logic_Analyzer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LABSoft.dir/src/LAB_Logic_Analyzer.cpp.o -c /home/glen/Desktop/LABSoft/src/LAB_Logic_Analyzer.cpp

CMakeFiles/LABSoft.dir/src/LAB_Logic_Analyzer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LABSoft.dir/src/LAB_Logic_Analyzer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/glen/Desktop/LABSoft/src/LAB_Logic_Analyzer.cpp > CMakeFiles/LABSoft.dir/src/LAB_Logic_Analyzer.cpp.i

CMakeFiles/LABSoft.dir/src/LAB_Logic_Analyzer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LABSoft.dir/src/LAB_Logic_Analyzer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/glen/Desktop/LABSoft/src/LAB_Logic_Analyzer.cpp -o CMakeFiles/LABSoft.dir/src/LAB_Logic_Analyzer.cpp.s

CMakeFiles/LABSoft.dir/src/LAB_Circuit_Checker.cpp.o: CMakeFiles/LABSoft.dir/flags.make
CMakeFiles/LABSoft.dir/src/LAB_Circuit_Checker.cpp.o: ../src/LAB_Circuit_Checker.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/glen/Desktop/LABSoft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/LABSoft.dir/src/LAB_Circuit_Checker.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LABSoft.dir/src/LAB_Circuit_Checker.cpp.o -c /home/glen/Desktop/LABSoft/src/LAB_Circuit_Checker.cpp

CMakeFiles/LABSoft.dir/src/LAB_Circuit_Checker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LABSoft.dir/src/LAB_Circuit_Checker.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/glen/Desktop/LABSoft/src/LAB_Circuit_Checker.cpp > CMakeFiles/LABSoft.dir/src/LAB_Circuit_Checker.cpp.i

CMakeFiles/LABSoft.dir/src/LAB_Circuit_Checker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LABSoft.dir/src/LAB_Circuit_Checker.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/glen/Desktop/LABSoft/src/LAB_Circuit_Checker.cpp -o CMakeFiles/LABSoft.dir/src/LAB_Circuit_Checker.cpp.s

CMakeFiles/LABSoft.dir/src/LABSoft_Controller.cpp.o: CMakeFiles/LABSoft.dir/flags.make
CMakeFiles/LABSoft.dir/src/LABSoft_Controller.cpp.o: ../src/LABSoft_Controller.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/glen/Desktop/LABSoft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/LABSoft.dir/src/LABSoft_Controller.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LABSoft.dir/src/LABSoft_Controller.cpp.o -c /home/glen/Desktop/LABSoft/src/LABSoft_Controller.cpp

CMakeFiles/LABSoft.dir/src/LABSoft_Controller.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LABSoft.dir/src/LABSoft_Controller.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/glen/Desktop/LABSoft/src/LABSoft_Controller.cpp > CMakeFiles/LABSoft.dir/src/LABSoft_Controller.cpp.i

CMakeFiles/LABSoft.dir/src/LABSoft_Controller.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LABSoft.dir/src/LABSoft_Controller.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/glen/Desktop/LABSoft/src/LABSoft_Controller.cpp -o CMakeFiles/LABSoft.dir/src/LABSoft_Controller.cpp.s

CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Oscilloscope.cpp.o: CMakeFiles/LABSoft.dir/flags.make
CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Oscilloscope.cpp.o: ../src/LABSoft_Controller_Oscilloscope.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/glen/Desktop/LABSoft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Oscilloscope.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Oscilloscope.cpp.o -c /home/glen/Desktop/LABSoft/src/LABSoft_Controller_Oscilloscope.cpp

CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Oscilloscope.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Oscilloscope.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/glen/Desktop/LABSoft/src/LABSoft_Controller_Oscilloscope.cpp > CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Oscilloscope.cpp.i

CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Oscilloscope.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Oscilloscope.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/glen/Desktop/LABSoft/src/LABSoft_Controller_Oscilloscope.cpp -o CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Oscilloscope.cpp.s

CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Multimeter.cpp.o: CMakeFiles/LABSoft.dir/flags.make
CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Multimeter.cpp.o: ../src/LABSoft_Controller_Multimeter.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/glen/Desktop/LABSoft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Multimeter.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Multimeter.cpp.o -c /home/glen/Desktop/LABSoft/src/LABSoft_Controller_Multimeter.cpp

CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Multimeter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Multimeter.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/glen/Desktop/LABSoft/src/LABSoft_Controller_Multimeter.cpp > CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Multimeter.cpp.i

CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Multimeter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Multimeter.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/glen/Desktop/LABSoft/src/LABSoft_Controller_Multimeter.cpp -o CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Multimeter.cpp.s

CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Function_Generator.cpp.o: CMakeFiles/LABSoft.dir/flags.make
CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Function_Generator.cpp.o: ../src/LABSoft_Controller_Function_Generator.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/glen/Desktop/LABSoft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Function_Generator.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Function_Generator.cpp.o -c /home/glen/Desktop/LABSoft/src/LABSoft_Controller_Function_Generator.cpp

CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Function_Generator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Function_Generator.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/glen/Desktop/LABSoft/src/LABSoft_Controller_Function_Generator.cpp > CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Function_Generator.cpp.i

CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Function_Generator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Function_Generator.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/glen/Desktop/LABSoft/src/LABSoft_Controller_Function_Generator.cpp -o CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Function_Generator.cpp.s

CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Logic_Analyzer.cpp.o: CMakeFiles/LABSoft.dir/flags.make
CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Logic_Analyzer.cpp.o: ../src/LABSoft_Controller_Logic_Analyzer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/glen/Desktop/LABSoft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Logic_Analyzer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Logic_Analyzer.cpp.o -c /home/glen/Desktop/LABSoft/src/LABSoft_Controller_Logic_Analyzer.cpp

CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Logic_Analyzer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Logic_Analyzer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/glen/Desktop/LABSoft/src/LABSoft_Controller_Logic_Analyzer.cpp > CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Logic_Analyzer.cpp.i

CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Logic_Analyzer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Logic_Analyzer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/glen/Desktop/LABSoft/src/LABSoft_Controller_Logic_Analyzer.cpp -o CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Logic_Analyzer.cpp.s

CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Circuit_Checker.cpp.o: CMakeFiles/LABSoft.dir/flags.make
CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Circuit_Checker.cpp.o: ../src/LABSoft_Controller_Circuit_Checker.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/glen/Desktop/LABSoft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Building CXX object CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Circuit_Checker.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Circuit_Checker.cpp.o -c /home/glen/Desktop/LABSoft/src/LABSoft_Controller_Circuit_Checker.cpp

CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Circuit_Checker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Circuit_Checker.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/glen/Desktop/LABSoft/src/LABSoft_Controller_Circuit_Checker.cpp > CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Circuit_Checker.cpp.i

CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Circuit_Checker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Circuit_Checker.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/glen/Desktop/LABSoft/src/LABSoft_Controller_Circuit_Checker.cpp -o CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Circuit_Checker.cpp.s

CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Main.cpp.o: CMakeFiles/LABSoft.dir/flags.make
CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Main.cpp.o: ../src/LABSoft_Controller_Main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/glen/Desktop/LABSoft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_17) "Building CXX object CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Main.cpp.o -c /home/glen/Desktop/LABSoft/src/LABSoft_Controller_Main.cpp

CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/glen/Desktop/LABSoft/src/LABSoft_Controller_Main.cpp > CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Main.cpp.i

CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/glen/Desktop/LABSoft/src/LABSoft_Controller_Main.cpp -o CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Main.cpp.s

CMakeFiles/LABSoft.dir/src/LABSoft_GUI.cpp.o: CMakeFiles/LABSoft.dir/flags.make
CMakeFiles/LABSoft.dir/src/LABSoft_GUI.cpp.o: ../src/LABSoft_GUI.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/glen/Desktop/LABSoft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_18) "Building CXX object CMakeFiles/LABSoft.dir/src/LABSoft_GUI.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LABSoft.dir/src/LABSoft_GUI.cpp.o -c /home/glen/Desktop/LABSoft/src/LABSoft_GUI.cpp

CMakeFiles/LABSoft.dir/src/LABSoft_GUI.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LABSoft.dir/src/LABSoft_GUI.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/glen/Desktop/LABSoft/src/LABSoft_GUI.cpp > CMakeFiles/LABSoft.dir/src/LABSoft_GUI.cpp.i

CMakeFiles/LABSoft.dir/src/LABSoft_GUI.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LABSoft.dir/src/LABSoft_GUI.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/glen/Desktop/LABSoft/src/LABSoft_GUI.cpp -o CMakeFiles/LABSoft.dir/src/LABSoft_GUI.cpp.s

CMakeFiles/LABSoft.dir/src/LABSoft_Oscilloscope_Display_Group.cpp.o: CMakeFiles/LABSoft.dir/flags.make
CMakeFiles/LABSoft.dir/src/LABSoft_Oscilloscope_Display_Group.cpp.o: ../src/LABSoft_Oscilloscope_Display_Group.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/glen/Desktop/LABSoft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_19) "Building CXX object CMakeFiles/LABSoft.dir/src/LABSoft_Oscilloscope_Display_Group.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LABSoft.dir/src/LABSoft_Oscilloscope_Display_Group.cpp.o -c /home/glen/Desktop/LABSoft/src/LABSoft_Oscilloscope_Display_Group.cpp

CMakeFiles/LABSoft.dir/src/LABSoft_Oscilloscope_Display_Group.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LABSoft.dir/src/LABSoft_Oscilloscope_Display_Group.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/glen/Desktop/LABSoft/src/LABSoft_Oscilloscope_Display_Group.cpp > CMakeFiles/LABSoft.dir/src/LABSoft_Oscilloscope_Display_Group.cpp.i

CMakeFiles/LABSoft.dir/src/LABSoft_Oscilloscope_Display_Group.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LABSoft.dir/src/LABSoft_Oscilloscope_Display_Group.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/glen/Desktop/LABSoft/src/LABSoft_Oscilloscope_Display_Group.cpp -o CMakeFiles/LABSoft.dir/src/LABSoft_Oscilloscope_Display_Group.cpp.s

CMakeFiles/LABSoft.dir/src/LABSoft_Oscilloscope_Display.cpp.o: CMakeFiles/LABSoft.dir/flags.make
CMakeFiles/LABSoft.dir/src/LABSoft_Oscilloscope_Display.cpp.o: ../src/LABSoft_Oscilloscope_Display.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/glen/Desktop/LABSoft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_20) "Building CXX object CMakeFiles/LABSoft.dir/src/LABSoft_Oscilloscope_Display.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LABSoft.dir/src/LABSoft_Oscilloscope_Display.cpp.o -c /home/glen/Desktop/LABSoft/src/LABSoft_Oscilloscope_Display.cpp

CMakeFiles/LABSoft.dir/src/LABSoft_Oscilloscope_Display.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LABSoft.dir/src/LABSoft_Oscilloscope_Display.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/glen/Desktop/LABSoft/src/LABSoft_Oscilloscope_Display.cpp > CMakeFiles/LABSoft.dir/src/LABSoft_Oscilloscope_Display.cpp.i

CMakeFiles/LABSoft.dir/src/LABSoft_Oscilloscope_Display.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LABSoft.dir/src/LABSoft_Oscilloscope_Display.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/glen/Desktop/LABSoft/src/LABSoft_Oscilloscope_Display.cpp -o CMakeFiles/LABSoft.dir/src/LABSoft_Oscilloscope_Display.cpp.s

CMakeFiles/LABSoft.dir/src/LABSoft_Logic_Analyzer_Display_Group.cpp.o: CMakeFiles/LABSoft.dir/flags.make
CMakeFiles/LABSoft.dir/src/LABSoft_Logic_Analyzer_Display_Group.cpp.o: ../src/LABSoft_Logic_Analyzer_Display_Group.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/glen/Desktop/LABSoft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_21) "Building CXX object CMakeFiles/LABSoft.dir/src/LABSoft_Logic_Analyzer_Display_Group.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LABSoft.dir/src/LABSoft_Logic_Analyzer_Display_Group.cpp.o -c /home/glen/Desktop/LABSoft/src/LABSoft_Logic_Analyzer_Display_Group.cpp

CMakeFiles/LABSoft.dir/src/LABSoft_Logic_Analyzer_Display_Group.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LABSoft.dir/src/LABSoft_Logic_Analyzer_Display_Group.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/glen/Desktop/LABSoft/src/LABSoft_Logic_Analyzer_Display_Group.cpp > CMakeFiles/LABSoft.dir/src/LABSoft_Logic_Analyzer_Display_Group.cpp.i

CMakeFiles/LABSoft.dir/src/LABSoft_Logic_Analyzer_Display_Group.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LABSoft.dir/src/LABSoft_Logic_Analyzer_Display_Group.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/glen/Desktop/LABSoft/src/LABSoft_Logic_Analyzer_Display_Group.cpp -o CMakeFiles/LABSoft.dir/src/LABSoft_Logic_Analyzer_Display_Group.cpp.s

CMakeFiles/LABSoft.dir/src/Channel_Signals.cpp.o: CMakeFiles/LABSoft.dir/flags.make
CMakeFiles/LABSoft.dir/src/Channel_Signals.cpp.o: ../src/Channel_Signals.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/glen/Desktop/LABSoft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_22) "Building CXX object CMakeFiles/LABSoft.dir/src/Channel_Signals.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LABSoft.dir/src/Channel_Signals.cpp.o -c /home/glen/Desktop/LABSoft/src/Channel_Signals.cpp

CMakeFiles/LABSoft.dir/src/Channel_Signals.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LABSoft.dir/src/Channel_Signals.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/glen/Desktop/LABSoft/src/Channel_Signals.cpp > CMakeFiles/LABSoft.dir/src/Channel_Signals.cpp.i

CMakeFiles/LABSoft.dir/src/Channel_Signals.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LABSoft.dir/src/Channel_Signals.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/glen/Desktop/LABSoft/src/Channel_Signals.cpp -o CMakeFiles/LABSoft.dir/src/Channel_Signals.cpp.s

CMakeFiles/LABSoft.dir/src/LabelValue.cpp.o: CMakeFiles/LABSoft.dir/flags.make
CMakeFiles/LABSoft.dir/src/LabelValue.cpp.o: ../src/LabelValue.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/glen/Desktop/LABSoft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_23) "Building CXX object CMakeFiles/LABSoft.dir/src/LabelValue.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LABSoft.dir/src/LabelValue.cpp.o -c /home/glen/Desktop/LABSoft/src/LabelValue.cpp

CMakeFiles/LABSoft.dir/src/LabelValue.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LABSoft.dir/src/LabelValue.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/glen/Desktop/LABSoft/src/LabelValue.cpp > CMakeFiles/LABSoft.dir/src/LabelValue.cpp.i

CMakeFiles/LABSoft.dir/src/LabelValue.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LABSoft.dir/src/LabelValue.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/glen/Desktop/LABSoft/src/LabelValue.cpp -o CMakeFiles/LABSoft.dir/src/LabelValue.cpp.s

CMakeFiles/LABSoft.dir/src/pugixml.cpp.o: CMakeFiles/LABSoft.dir/flags.make
CMakeFiles/LABSoft.dir/src/pugixml.cpp.o: ../src/pugixml.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/glen/Desktop/LABSoft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_24) "Building CXX object CMakeFiles/LABSoft.dir/src/pugixml.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LABSoft.dir/src/pugixml.cpp.o -c /home/glen/Desktop/LABSoft/src/pugixml.cpp

CMakeFiles/LABSoft.dir/src/pugixml.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LABSoft.dir/src/pugixml.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/glen/Desktop/LABSoft/src/pugixml.cpp > CMakeFiles/LABSoft.dir/src/pugixml.cpp.i

CMakeFiles/LABSoft.dir/src/pugixml.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LABSoft.dir/src/pugixml.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/glen/Desktop/LABSoft/src/pugixml.cpp -o CMakeFiles/LABSoft.dir/src/pugixml.cpp.s

CMakeFiles/LABSoft.dir/src/LAB_AD9833.cpp.o: CMakeFiles/LABSoft.dir/flags.make
CMakeFiles/LABSoft.dir/src/LAB_AD9833.cpp.o: ../src/LAB_AD9833.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/glen/Desktop/LABSoft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_25) "Building CXX object CMakeFiles/LABSoft.dir/src/LAB_AD9833.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LABSoft.dir/src/LAB_AD9833.cpp.o -c /home/glen/Desktop/LABSoft/src/LAB_AD9833.cpp

CMakeFiles/LABSoft.dir/src/LAB_AD9833.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LABSoft.dir/src/LAB_AD9833.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/glen/Desktop/LABSoft/src/LAB_AD9833.cpp > CMakeFiles/LABSoft.dir/src/LAB_AD9833.cpp.i

CMakeFiles/LABSoft.dir/src/LAB_AD9833.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LABSoft.dir/src/LAB_AD9833.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/glen/Desktop/LABSoft/src/LAB_AD9833.cpp -o CMakeFiles/LABSoft.dir/src/LAB_AD9833.cpp.s

# Object files for target LABSoft
LABSoft_OBJECTS = \
"CMakeFiles/LABSoft.dir/src/main.cpp.o" \
"CMakeFiles/LABSoft.dir/src/LABSoft.cpp.o" \
"CMakeFiles/LABSoft.dir/src/LAB.cpp.o" \
"CMakeFiles/LABSoft.dir/lib/AikaPi/AikaPi.cpp.o" \
"CMakeFiles/LABSoft.dir/src/LAB_Core.cpp.o" \
"CMakeFiles/LABSoft.dir/src/LAB_Oscilloscope.cpp.o" \
"CMakeFiles/LABSoft.dir/src/LAB_Multimeter.cpp.o" \
"CMakeFiles/LABSoft.dir/src/LAB_Function_Generator.cpp.o" \
"CMakeFiles/LABSoft.dir/src/LAB_Logic_Analyzer.cpp.o" \
"CMakeFiles/LABSoft.dir/src/LAB_Circuit_Checker.cpp.o" \
"CMakeFiles/LABSoft.dir/src/LABSoft_Controller.cpp.o" \
"CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Oscilloscope.cpp.o" \
"CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Multimeter.cpp.o" \
"CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Function_Generator.cpp.o" \
"CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Logic_Analyzer.cpp.o" \
"CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Circuit_Checker.cpp.o" \
"CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Main.cpp.o" \
"CMakeFiles/LABSoft.dir/src/LABSoft_GUI.cpp.o" \
"CMakeFiles/LABSoft.dir/src/LABSoft_Oscilloscope_Display_Group.cpp.o" \
"CMakeFiles/LABSoft.dir/src/LABSoft_Oscilloscope_Display.cpp.o" \
"CMakeFiles/LABSoft.dir/src/LABSoft_Logic_Analyzer_Display_Group.cpp.o" \
"CMakeFiles/LABSoft.dir/src/Channel_Signals.cpp.o" \
"CMakeFiles/LABSoft.dir/src/LabelValue.cpp.o" \
"CMakeFiles/LABSoft.dir/src/pugixml.cpp.o" \
"CMakeFiles/LABSoft.dir/src/LAB_AD9833.cpp.o"

# External object files for target LABSoft
LABSoft_EXTERNAL_OBJECTS =

LABSoft: CMakeFiles/LABSoft.dir/src/main.cpp.o
LABSoft: CMakeFiles/LABSoft.dir/src/LABSoft.cpp.o
LABSoft: CMakeFiles/LABSoft.dir/src/LAB.cpp.o
LABSoft: CMakeFiles/LABSoft.dir/lib/AikaPi/AikaPi.cpp.o
LABSoft: CMakeFiles/LABSoft.dir/src/LAB_Core.cpp.o
LABSoft: CMakeFiles/LABSoft.dir/src/LAB_Oscilloscope.cpp.o
LABSoft: CMakeFiles/LABSoft.dir/src/LAB_Multimeter.cpp.o
LABSoft: CMakeFiles/LABSoft.dir/src/LAB_Function_Generator.cpp.o
LABSoft: CMakeFiles/LABSoft.dir/src/LAB_Logic_Analyzer.cpp.o
LABSoft: CMakeFiles/LABSoft.dir/src/LAB_Circuit_Checker.cpp.o
LABSoft: CMakeFiles/LABSoft.dir/src/LABSoft_Controller.cpp.o
LABSoft: CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Oscilloscope.cpp.o
LABSoft: CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Multimeter.cpp.o
LABSoft: CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Function_Generator.cpp.o
LABSoft: CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Logic_Analyzer.cpp.o
LABSoft: CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Circuit_Checker.cpp.o
LABSoft: CMakeFiles/LABSoft.dir/src/LABSoft_Controller_Main.cpp.o
LABSoft: CMakeFiles/LABSoft.dir/src/LABSoft_GUI.cpp.o
LABSoft: CMakeFiles/LABSoft.dir/src/LABSoft_Oscilloscope_Display_Group.cpp.o
LABSoft: CMakeFiles/LABSoft.dir/src/LABSoft_Oscilloscope_Display.cpp.o
LABSoft: CMakeFiles/LABSoft.dir/src/LABSoft_Logic_Analyzer_Display_Group.cpp.o
LABSoft: CMakeFiles/LABSoft.dir/src/Channel_Signals.cpp.o
LABSoft: CMakeFiles/LABSoft.dir/src/LabelValue.cpp.o
LABSoft: CMakeFiles/LABSoft.dir/src/pugixml.cpp.o
LABSoft: CMakeFiles/LABSoft.dir/src/LAB_AD9833.cpp.o
LABSoft: CMakeFiles/LABSoft.dir/build.make
LABSoft: /usr/lib/arm-linux-gnueabihf/libfltk.so
LABSoft: CMakeFiles/LABSoft.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/glen/Desktop/LABSoft/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_26) "Linking CXX executable LABSoft"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/LABSoft.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/LABSoft.dir/build: LABSoft

.PHONY : CMakeFiles/LABSoft.dir/build

CMakeFiles/LABSoft.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/LABSoft.dir/cmake_clean.cmake
.PHONY : CMakeFiles/LABSoft.dir/clean

CMakeFiles/LABSoft.dir/depend:
	cd /home/glen/Desktop/LABSoft/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/glen/Desktop/LABSoft /home/glen/Desktop/LABSoft /home/glen/Desktop/LABSoft/build /home/glen/Desktop/LABSoft/build /home/glen/Desktop/LABSoft/build/CMakeFiles/LABSoft.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/LABSoft.dir/depend

