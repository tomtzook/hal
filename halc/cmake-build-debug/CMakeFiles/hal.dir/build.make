# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /home/tomtzook/ides/clion-2019.1.4/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/tomtzook/ides/clion-2019.1.4/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/tomtzook/projects/hal/halc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tomtzook/projects/hal/halc/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/hal.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/hal.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/hal.dir/flags.make

CMakeFiles/hal.dir/base/hal_base.c.o: CMakeFiles/hal.dir/flags.make
CMakeFiles/hal.dir/base/hal_base.c.o: ../base/hal_base.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tomtzook/projects/hal/halc/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/hal.dir/base/hal_base.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/hal.dir/base/hal_base.c.o   -c /home/tomtzook/projects/hal/halc/base/hal_base.c

CMakeFiles/hal.dir/base/hal_base.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hal.dir/base/hal_base.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tomtzook/projects/hal/halc/base/hal_base.c > CMakeFiles/hal.dir/base/hal_base.c.i

CMakeFiles/hal.dir/base/hal_base.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hal.dir/base/hal_base.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tomtzook/projects/hal/halc/base/hal_base.c -o CMakeFiles/hal.dir/base/hal_base.c.s

CMakeFiles/hal.dir/base/hal_base_dio.c.o: CMakeFiles/hal.dir/flags.make
CMakeFiles/hal.dir/base/hal_base_dio.c.o: ../base/hal_base_dio.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tomtzook/projects/hal/halc/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/hal.dir/base/hal_base_dio.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/hal.dir/base/hal_base_dio.c.o   -c /home/tomtzook/projects/hal/halc/base/hal_base_dio.c

CMakeFiles/hal.dir/base/hal_base_dio.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hal.dir/base/hal_base_dio.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tomtzook/projects/hal/halc/base/hal_base_dio.c > CMakeFiles/hal.dir/base/hal_base_dio.c.i

CMakeFiles/hal.dir/base/hal_base_dio.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hal.dir/base/hal_base_dio.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tomtzook/projects/hal/halc/base/hal_base_dio.c -o CMakeFiles/hal.dir/base/hal_base_dio.c.s

CMakeFiles/hal.dir/util/lookup_table.c.o: CMakeFiles/hal.dir/flags.make
CMakeFiles/hal.dir/util/lookup_table.c.o: ../util/lookup_table.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tomtzook/projects/hal/halc/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/hal.dir/util/lookup_table.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/hal.dir/util/lookup_table.c.o   -c /home/tomtzook/projects/hal/halc/util/lookup_table.c

CMakeFiles/hal.dir/util/lookup_table.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hal.dir/util/lookup_table.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tomtzook/projects/hal/halc/util/lookup_table.c > CMakeFiles/hal.dir/util/lookup_table.c.i

CMakeFiles/hal.dir/util/lookup_table.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hal.dir/util/lookup_table.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tomtzook/projects/hal/halc/util/lookup_table.c -o CMakeFiles/hal.dir/util/lookup_table.c.s

# Object files for target hal
hal_OBJECTS = \
"CMakeFiles/hal.dir/base/hal_base.c.o" \
"CMakeFiles/hal.dir/base/hal_base_dio.c.o" \
"CMakeFiles/hal.dir/util/lookup_table.c.o"

# External object files for target hal
hal_EXTERNAL_OBJECTS =

libhal.a: CMakeFiles/hal.dir/base/hal_base.c.o
libhal.a: CMakeFiles/hal.dir/base/hal_base_dio.c.o
libhal.a: CMakeFiles/hal.dir/util/lookup_table.c.o
libhal.a: CMakeFiles/hal.dir/build.make
libhal.a: CMakeFiles/hal.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tomtzook/projects/hal/halc/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C static library libhal.a"
	$(CMAKE_COMMAND) -P CMakeFiles/hal.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hal.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/hal.dir/build: libhal.a

.PHONY : CMakeFiles/hal.dir/build

CMakeFiles/hal.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/hal.dir/cmake_clean.cmake
.PHONY : CMakeFiles/hal.dir/clean

CMakeFiles/hal.dir/depend:
	cd /home/tomtzook/projects/hal/halc/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tomtzook/projects/hal/halc /home/tomtzook/projects/hal/halc /home/tomtzook/projects/hal/halc/cmake-build-debug /home/tomtzook/projects/hal/halc/cmake-build-debug /home/tomtzook/projects/hal/halc/cmake-build-debug/CMakeFiles/hal.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/hal.dir/depend
