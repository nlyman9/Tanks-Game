# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/apc47/Documents/my_fork/cs1666-First-Team

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/apc47/Documents/my_fork/cs1666-First-Team/build

# Include any dependencies generated for this target.
include CMakeFiles/Tanks.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Tanks.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Tanks.dir/flags.make

CMakeFiles/Tanks.dir/GameStartUp.cpp.o: CMakeFiles/Tanks.dir/flags.make
CMakeFiles/Tanks.dir/GameStartUp.cpp.o: ../GameStartUp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/apc47/Documents/my_fork/cs1666-First-Team/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Tanks.dir/GameStartUp.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Tanks.dir/GameStartUp.cpp.o -c /home/apc47/Documents/my_fork/cs1666-First-Team/GameStartUp.cpp

CMakeFiles/Tanks.dir/GameStartUp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Tanks.dir/GameStartUp.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/apc47/Documents/my_fork/cs1666-First-Team/GameStartUp.cpp > CMakeFiles/Tanks.dir/GameStartUp.cpp.i

CMakeFiles/Tanks.dir/GameStartUp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Tanks.dir/GameStartUp.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/apc47/Documents/my_fork/cs1666-First-Team/GameStartUp.cpp -o CMakeFiles/Tanks.dir/GameStartUp.cpp.s

CMakeFiles/Tanks.dir/GameStartUp.cpp.o.requires:

.PHONY : CMakeFiles/Tanks.dir/GameStartUp.cpp.o.requires

CMakeFiles/Tanks.dir/GameStartUp.cpp.o.provides: CMakeFiles/Tanks.dir/GameStartUp.cpp.o.requires
	$(MAKE) -f CMakeFiles/Tanks.dir/build.make CMakeFiles/Tanks.dir/GameStartUp.cpp.o.provides.build
.PHONY : CMakeFiles/Tanks.dir/GameStartUp.cpp.o.provides

CMakeFiles/Tanks.dir/GameStartUp.cpp.o.provides.build: CMakeFiles/Tanks.dir/GameStartUp.cpp.o


# Object files for target Tanks
Tanks_OBJECTS = \
"CMakeFiles/Tanks.dir/GameStartUp.cpp.o"

# External object files for target Tanks
Tanks_EXTERNAL_OBJECTS =

Tanks: CMakeFiles/Tanks.dir/GameStartUp.cpp.o
Tanks: CMakeFiles/Tanks.dir/build.make
Tanks: CMakeFiles/Tanks.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/apc47/Documents/my_fork/cs1666-First-Team/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Tanks"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Tanks.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Tanks.dir/build: Tanks

.PHONY : CMakeFiles/Tanks.dir/build

CMakeFiles/Tanks.dir/requires: CMakeFiles/Tanks.dir/GameStartUp.cpp.o.requires

.PHONY : CMakeFiles/Tanks.dir/requires

CMakeFiles/Tanks.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Tanks.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Tanks.dir/clean

CMakeFiles/Tanks.dir/depend:
	cd /home/apc47/Documents/my_fork/cs1666-First-Team/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/apc47/Documents/my_fork/cs1666-First-Team /home/apc47/Documents/my_fork/cs1666-First-Team /home/apc47/Documents/my_fork/cs1666-First-Team/build /home/apc47/Documents/my_fork/cs1666-First-Team/build /home/apc47/Documents/my_fork/cs1666-First-Team/build/CMakeFiles/Tanks.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Tanks.dir/depend

