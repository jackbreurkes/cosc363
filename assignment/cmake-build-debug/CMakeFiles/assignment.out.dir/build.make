# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/jackbreurkes/Documents/Coding/Cpp/COSC363/assignment

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/jackbreurkes/Documents/Coding/Cpp/COSC363/assignment/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/assignment.out.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/assignment.out.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/assignment.out.dir/flags.make

CMakeFiles/assignment.out.dir/assignment.cpp.o: CMakeFiles/assignment.out.dir/flags.make
CMakeFiles/assignment.out.dir/assignment.cpp.o: ../assignment.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jackbreurkes/Documents/Coding/Cpp/COSC363/assignment/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/assignment.out.dir/assignment.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/assignment.out.dir/assignment.cpp.o -c /Users/jackbreurkes/Documents/Coding/Cpp/COSC363/assignment/assignment.cpp

CMakeFiles/assignment.out.dir/assignment.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/assignment.out.dir/assignment.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jackbreurkes/Documents/Coding/Cpp/COSC363/assignment/assignment.cpp > CMakeFiles/assignment.out.dir/assignment.cpp.i

CMakeFiles/assignment.out.dir/assignment.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/assignment.out.dir/assignment.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jackbreurkes/Documents/Coding/Cpp/COSC363/assignment/assignment.cpp -o CMakeFiles/assignment.out.dir/assignment.cpp.s

# Object files for target assignment.out
assignment_out_OBJECTS = \
"CMakeFiles/assignment.out.dir/assignment.cpp.o"

# External object files for target assignment.out
assignment_out_EXTERNAL_OBJECTS =

assignment.out: CMakeFiles/assignment.out.dir/assignment.cpp.o
assignment.out: CMakeFiles/assignment.out.dir/build.make
assignment.out: CMakeFiles/assignment.out.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/jackbreurkes/Documents/Coding/Cpp/COSC363/assignment/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable assignment.out"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/assignment.out.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/assignment.out.dir/build: assignment.out

.PHONY : CMakeFiles/assignment.out.dir/build

CMakeFiles/assignment.out.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/assignment.out.dir/cmake_clean.cmake
.PHONY : CMakeFiles/assignment.out.dir/clean

CMakeFiles/assignment.out.dir/depend:
	cd /Users/jackbreurkes/Documents/Coding/Cpp/COSC363/assignment/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/jackbreurkes/Documents/Coding/Cpp/COSC363/assignment /Users/jackbreurkes/Documents/Coding/Cpp/COSC363/assignment /Users/jackbreurkes/Documents/Coding/Cpp/COSC363/assignment/cmake-build-debug /Users/jackbreurkes/Documents/Coding/Cpp/COSC363/assignment/cmake-build-debug /Users/jackbreurkes/Documents/Coding/Cpp/COSC363/assignment/cmake-build-debug/CMakeFiles/assignment.out.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/assignment.out.dir/depend

