# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /root/dev/mimilib

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/dev/mimilib/build

# Include any dependencies generated for this target.
include src/init/test/CMakeFiles/test.bin.dir/depend.make

# Include the progress variables for this target.
include src/init/test/CMakeFiles/test.bin.dir/progress.make

# Include the compile flags for this target's objects.
include src/init/test/CMakeFiles/test.bin.dir/flags.make

src/init/test/CMakeFiles/test.bin.dir/main.cpp.o: src/init/test/CMakeFiles/test.bin.dir/flags.make
src/init/test/CMakeFiles/test.bin.dir/main.cpp.o: ../src/init/test/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/dev/mimilib/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/init/test/CMakeFiles/test.bin.dir/main.cpp.o"
	cd /root/dev/mimilib/build/src/init/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test.bin.dir/main.cpp.o -c /root/dev/mimilib/src/init/test/main.cpp

src/init/test/CMakeFiles/test.bin.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.bin.dir/main.cpp.i"
	cd /root/dev/mimilib/build/src/init/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/dev/mimilib/src/init/test/main.cpp > CMakeFiles/test.bin.dir/main.cpp.i

src/init/test/CMakeFiles/test.bin.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.bin.dir/main.cpp.s"
	cd /root/dev/mimilib/build/src/init/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/dev/mimilib/src/init/test/main.cpp -o CMakeFiles/test.bin.dir/main.cpp.s

# Object files for target test.bin
test_bin_OBJECTS = \
"CMakeFiles/test.bin.dir/main.cpp.o"

# External object files for target test.bin
test_bin_EXTERNAL_OBJECTS =

../bin/test.bin: src/init/test/CMakeFiles/test.bin.dir/main.cpp.o
../bin/test.bin: src/init/test/CMakeFiles/test.bin.dir/build.make
../bin/test.bin: ../lib/libmimi.a
../bin/test.bin: src/init/test/CMakeFiles/test.bin.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/dev/mimilib/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../../../bin/test.bin"
	cd /root/dev/mimilib/build/src/init/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test.bin.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/init/test/CMakeFiles/test.bin.dir/build: ../bin/test.bin

.PHONY : src/init/test/CMakeFiles/test.bin.dir/build

src/init/test/CMakeFiles/test.bin.dir/clean:
	cd /root/dev/mimilib/build/src/init/test && $(CMAKE_COMMAND) -P CMakeFiles/test.bin.dir/cmake_clean.cmake
.PHONY : src/init/test/CMakeFiles/test.bin.dir/clean

src/init/test/CMakeFiles/test.bin.dir/depend:
	cd /root/dev/mimilib/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/dev/mimilib /root/dev/mimilib/src/init/test /root/dev/mimilib/build /root/dev/mimilib/build/src/init/test /root/dev/mimilib/build/src/init/test/CMakeFiles/test.bin.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/init/test/CMakeFiles/test.bin.dir/depend

