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
CMAKE_SOURCE_DIR = /mnt/e/test/working/cs1666-First-Team/source

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/e/test/working/cs1666-First-Team/build

# Include any dependencies generated for this target.
include app/Controllers/CMakeFiles/Controllers.dir/depend.make

# Include the progress variables for this target.
include app/Controllers/CMakeFiles/Controllers.dir/progress.make

# Include the compile flags for this target's objects.
include app/Controllers/CMakeFiles/Controllers.dir/flags.make

app/Controllers/CMakeFiles/Controllers.dir/AIController.cpp.o: app/Controllers/CMakeFiles/Controllers.dir/flags.make
app/Controllers/CMakeFiles/Controllers.dir/AIController.cpp.o: /mnt/e/test/working/cs1666-First-Team/source/app/Controllers/AIController.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/e/test/working/cs1666-First-Team/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object app/Controllers/CMakeFiles/Controllers.dir/AIController.cpp.o"
	cd /mnt/e/test/working/cs1666-First-Team/build/app/Controllers && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Controllers.dir/AIController.cpp.o -c /mnt/e/test/working/cs1666-First-Team/source/app/Controllers/AIController.cpp

app/Controllers/CMakeFiles/Controllers.dir/AIController.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Controllers.dir/AIController.cpp.i"
	cd /mnt/e/test/working/cs1666-First-Team/build/app/Controllers && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/e/test/working/cs1666-First-Team/source/app/Controllers/AIController.cpp > CMakeFiles/Controllers.dir/AIController.cpp.i

app/Controllers/CMakeFiles/Controllers.dir/AIController.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Controllers.dir/AIController.cpp.s"
	cd /mnt/e/test/working/cs1666-First-Team/build/app/Controllers && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/e/test/working/cs1666-First-Team/source/app/Controllers/AIController.cpp -o CMakeFiles/Controllers.dir/AIController.cpp.s

app/Controllers/CMakeFiles/Controllers.dir/AIController.cpp.o.requires:

.PHONY : app/Controllers/CMakeFiles/Controllers.dir/AIController.cpp.o.requires

app/Controllers/CMakeFiles/Controllers.dir/AIController.cpp.o.provides: app/Controllers/CMakeFiles/Controllers.dir/AIController.cpp.o.requires
	$(MAKE) -f app/Controllers/CMakeFiles/Controllers.dir/build.make app/Controllers/CMakeFiles/Controllers.dir/AIController.cpp.o.provides.build
.PHONY : app/Controllers/CMakeFiles/Controllers.dir/AIController.cpp.o.provides

app/Controllers/CMakeFiles/Controllers.dir/AIController.cpp.o.provides.build: app/Controllers/CMakeFiles/Controllers.dir/AIController.cpp.o


app/Controllers/CMakeFiles/Controllers.dir/KeyboardController.cpp.o: app/Controllers/CMakeFiles/Controllers.dir/flags.make
app/Controllers/CMakeFiles/Controllers.dir/KeyboardController.cpp.o: /mnt/e/test/working/cs1666-First-Team/source/app/Controllers/KeyboardController.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/e/test/working/cs1666-First-Team/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object app/Controllers/CMakeFiles/Controllers.dir/KeyboardController.cpp.o"
	cd /mnt/e/test/working/cs1666-First-Team/build/app/Controllers && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Controllers.dir/KeyboardController.cpp.o -c /mnt/e/test/working/cs1666-First-Team/source/app/Controllers/KeyboardController.cpp

app/Controllers/CMakeFiles/Controllers.dir/KeyboardController.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Controllers.dir/KeyboardController.cpp.i"
	cd /mnt/e/test/working/cs1666-First-Team/build/app/Controllers && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/e/test/working/cs1666-First-Team/source/app/Controllers/KeyboardController.cpp > CMakeFiles/Controllers.dir/KeyboardController.cpp.i

app/Controllers/CMakeFiles/Controllers.dir/KeyboardController.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Controllers.dir/KeyboardController.cpp.s"
	cd /mnt/e/test/working/cs1666-First-Team/build/app/Controllers && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/e/test/working/cs1666-First-Team/source/app/Controllers/KeyboardController.cpp -o CMakeFiles/Controllers.dir/KeyboardController.cpp.s

app/Controllers/CMakeFiles/Controllers.dir/KeyboardController.cpp.o.requires:

.PHONY : app/Controllers/CMakeFiles/Controllers.dir/KeyboardController.cpp.o.requires

app/Controllers/CMakeFiles/Controllers.dir/KeyboardController.cpp.o.provides: app/Controllers/CMakeFiles/Controllers.dir/KeyboardController.cpp.o.requires
	$(MAKE) -f app/Controllers/CMakeFiles/Controllers.dir/build.make app/Controllers/CMakeFiles/Controllers.dir/KeyboardController.cpp.o.provides.build
.PHONY : app/Controllers/CMakeFiles/Controllers.dir/KeyboardController.cpp.o.provides

app/Controllers/CMakeFiles/Controllers.dir/KeyboardController.cpp.o.provides.build: app/Controllers/CMakeFiles/Controllers.dir/KeyboardController.cpp.o


app/Controllers/CMakeFiles/Controllers.dir/NetworkController.cpp.o: app/Controllers/CMakeFiles/Controllers.dir/flags.make
app/Controllers/CMakeFiles/Controllers.dir/NetworkController.cpp.o: /mnt/e/test/working/cs1666-First-Team/source/app/Controllers/NetworkController.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/e/test/working/cs1666-First-Team/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object app/Controllers/CMakeFiles/Controllers.dir/NetworkController.cpp.o"
	cd /mnt/e/test/working/cs1666-First-Team/build/app/Controllers && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Controllers.dir/NetworkController.cpp.o -c /mnt/e/test/working/cs1666-First-Team/source/app/Controllers/NetworkController.cpp

app/Controllers/CMakeFiles/Controllers.dir/NetworkController.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Controllers.dir/NetworkController.cpp.i"
	cd /mnt/e/test/working/cs1666-First-Team/build/app/Controllers && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/e/test/working/cs1666-First-Team/source/app/Controllers/NetworkController.cpp > CMakeFiles/Controllers.dir/NetworkController.cpp.i

app/Controllers/CMakeFiles/Controllers.dir/NetworkController.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Controllers.dir/NetworkController.cpp.s"
	cd /mnt/e/test/working/cs1666-First-Team/build/app/Controllers && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/e/test/working/cs1666-First-Team/source/app/Controllers/NetworkController.cpp -o CMakeFiles/Controllers.dir/NetworkController.cpp.s

app/Controllers/CMakeFiles/Controllers.dir/NetworkController.cpp.o.requires:

.PHONY : app/Controllers/CMakeFiles/Controllers.dir/NetworkController.cpp.o.requires

app/Controllers/CMakeFiles/Controllers.dir/NetworkController.cpp.o.provides: app/Controllers/CMakeFiles/Controllers.dir/NetworkController.cpp.o.requires
	$(MAKE) -f app/Controllers/CMakeFiles/Controllers.dir/build.make app/Controllers/CMakeFiles/Controllers.dir/NetworkController.cpp.o.provides.build
.PHONY : app/Controllers/CMakeFiles/Controllers.dir/NetworkController.cpp.o.provides

app/Controllers/CMakeFiles/Controllers.dir/NetworkController.cpp.o.provides.build: app/Controllers/CMakeFiles/Controllers.dir/NetworkController.cpp.o


# Object files for target Controllers
Controllers_OBJECTS = \
"CMakeFiles/Controllers.dir/AIController.cpp.o" \
"CMakeFiles/Controllers.dir/KeyboardController.cpp.o" \
"CMakeFiles/Controllers.dir/NetworkController.cpp.o"

# External object files for target Controllers
Controllers_EXTERNAL_OBJECTS =

app/Controllers/libControllers.a: app/Controllers/CMakeFiles/Controllers.dir/AIController.cpp.o
app/Controllers/libControllers.a: app/Controllers/CMakeFiles/Controllers.dir/KeyboardController.cpp.o
app/Controllers/libControllers.a: app/Controllers/CMakeFiles/Controllers.dir/NetworkController.cpp.o
app/Controllers/libControllers.a: app/Controllers/CMakeFiles/Controllers.dir/build.make
app/Controllers/libControllers.a: app/Controllers/CMakeFiles/Controllers.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/e/test/working/cs1666-First-Team/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library libControllers.a"
	cd /mnt/e/test/working/cs1666-First-Team/build/app/Controllers && $(CMAKE_COMMAND) -P CMakeFiles/Controllers.dir/cmake_clean_target.cmake
	cd /mnt/e/test/working/cs1666-First-Team/build/app/Controllers && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Controllers.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
app/Controllers/CMakeFiles/Controllers.dir/build: app/Controllers/libControllers.a

.PHONY : app/Controllers/CMakeFiles/Controllers.dir/build

app/Controllers/CMakeFiles/Controllers.dir/requires: app/Controllers/CMakeFiles/Controllers.dir/AIController.cpp.o.requires
app/Controllers/CMakeFiles/Controllers.dir/requires: app/Controllers/CMakeFiles/Controllers.dir/KeyboardController.cpp.o.requires
app/Controllers/CMakeFiles/Controllers.dir/requires: app/Controllers/CMakeFiles/Controllers.dir/NetworkController.cpp.o.requires

.PHONY : app/Controllers/CMakeFiles/Controllers.dir/requires

app/Controllers/CMakeFiles/Controllers.dir/clean:
	cd /mnt/e/test/working/cs1666-First-Team/build/app/Controllers && $(CMAKE_COMMAND) -P CMakeFiles/Controllers.dir/cmake_clean.cmake
.PHONY : app/Controllers/CMakeFiles/Controllers.dir/clean

app/Controllers/CMakeFiles/Controllers.dir/depend:
	cd /mnt/e/test/working/cs1666-First-Team/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/e/test/working/cs1666-First-Team/source /mnt/e/test/working/cs1666-First-Team/source/app/Controllers /mnt/e/test/working/cs1666-First-Team/build /mnt/e/test/working/cs1666-First-Team/build/app/Controllers /mnt/e/test/working/cs1666-First-Team/build/app/Controllers/CMakeFiles/Controllers.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : app/Controllers/CMakeFiles/Controllers.dir/depend

