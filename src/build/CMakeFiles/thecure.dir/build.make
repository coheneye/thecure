# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/woo/proj/cpp/thecure/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/woo/proj/cpp/thecure/src/build

# Include any dependencies generated for this target.
include CMakeFiles/thecure.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/thecure.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/thecure.dir/flags.make

CMakeFiles/thecure.dir/main.cc.o: CMakeFiles/thecure.dir/flags.make
CMakeFiles/thecure.dir/main.cc.o: ../main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/woo/proj/cpp/thecure/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/thecure.dir/main.cc.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/thecure.dir/main.cc.o -c /home/woo/proj/cpp/thecure/src/main.cc

CMakeFiles/thecure.dir/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/thecure.dir/main.cc.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/woo/proj/cpp/thecure/src/main.cc > CMakeFiles/thecure.dir/main.cc.i

CMakeFiles/thecure.dir/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/thecure.dir/main.cc.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/woo/proj/cpp/thecure/src/main.cc -o CMakeFiles/thecure.dir/main.cc.s

CMakeFiles/thecure.dir/main.cc.o.requires:

.PHONY : CMakeFiles/thecure.dir/main.cc.o.requires

CMakeFiles/thecure.dir/main.cc.o.provides: CMakeFiles/thecure.dir/main.cc.o.requires
	$(MAKE) -f CMakeFiles/thecure.dir/build.make CMakeFiles/thecure.dir/main.cc.o.provides.build
.PHONY : CMakeFiles/thecure.dir/main.cc.o.provides

CMakeFiles/thecure.dir/main.cc.o.provides.build: CMakeFiles/thecure.dir/main.cc.o


# Object files for target thecure
thecure_OBJECTS = \
"CMakeFiles/thecure.dir/main.cc.o"

# External object files for target thecure
thecure_EXTERNAL_OBJECTS =

thecure: CMakeFiles/thecure.dir/main.cc.o
thecure: CMakeFiles/thecure.dir/build.make
thecure: CMakeFiles/thecure.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/woo/proj/cpp/thecure/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable thecure"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/thecure.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/thecure.dir/build: thecure

.PHONY : CMakeFiles/thecure.dir/build

CMakeFiles/thecure.dir/requires: CMakeFiles/thecure.dir/main.cc.o.requires

.PHONY : CMakeFiles/thecure.dir/requires

CMakeFiles/thecure.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/thecure.dir/cmake_clean.cmake
.PHONY : CMakeFiles/thecure.dir/clean

CMakeFiles/thecure.dir/depend:
	cd /home/woo/proj/cpp/thecure/src/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/woo/proj/cpp/thecure/src /home/woo/proj/cpp/thecure/src /home/woo/proj/cpp/thecure/src/build /home/woo/proj/cpp/thecure/src/build /home/woo/proj/cpp/thecure/src/build/CMakeFiles/thecure.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/thecure.dir/depend

