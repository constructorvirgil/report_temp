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
CMAKE_COMMAND = /home/virgil/Documents/clion-2019.3.3/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/virgil/Documents/clion-2019.3.3/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/virgil/report_temp/report_temp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/virgil/report_temp/report_temp/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/report_temp.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/report_temp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/report_temp.dir/flags.make

CMakeFiles/report_temp.dir/main.c.o: CMakeFiles/report_temp.dir/flags.make
CMakeFiles/report_temp.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/virgil/report_temp/report_temp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/report_temp.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/report_temp.dir/main.c.o   -c /home/virgil/report_temp/report_temp/main.c

CMakeFiles/report_temp.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/report_temp.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/virgil/report_temp/report_temp/main.c > CMakeFiles/report_temp.dir/main.c.i

CMakeFiles/report_temp.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/report_temp.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/virgil/report_temp/report_temp/main.c -o CMakeFiles/report_temp.dir/main.c.s

CMakeFiles/report_temp.dir/tempdb.c.o: CMakeFiles/report_temp.dir/flags.make
CMakeFiles/report_temp.dir/tempdb.c.o: ../tempdb.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/virgil/report_temp/report_temp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/report_temp.dir/tempdb.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/report_temp.dir/tempdb.c.o   -c /home/virgil/report_temp/report_temp/tempdb.c

CMakeFiles/report_temp.dir/tempdb.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/report_temp.dir/tempdb.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/virgil/report_temp/report_temp/tempdb.c > CMakeFiles/report_temp.dir/tempdb.c.i

CMakeFiles/report_temp.dir/tempdb.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/report_temp.dir/tempdb.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/virgil/report_temp/report_temp/tempdb.c -o CMakeFiles/report_temp.dir/tempdb.c.s

CMakeFiles/report_temp.dir/simple_server.c.o: CMakeFiles/report_temp.dir/flags.make
CMakeFiles/report_temp.dir/simple_server.c.o: ../simple_server.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/virgil/report_temp/report_temp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/report_temp.dir/simple_server.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/report_temp.dir/simple_server.c.o   -c /home/virgil/report_temp/report_temp/simple_server.c

CMakeFiles/report_temp.dir/simple_server.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/report_temp.dir/simple_server.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/virgil/report_temp/report_temp/simple_server.c > CMakeFiles/report_temp.dir/simple_server.c.i

CMakeFiles/report_temp.dir/simple_server.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/report_temp.dir/simple_server.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/virgil/report_temp/report_temp/simple_server.c -o CMakeFiles/report_temp.dir/simple_server.c.s

# Object files for target report_temp
report_temp_OBJECTS = \
"CMakeFiles/report_temp.dir/main.c.o" \
"CMakeFiles/report_temp.dir/tempdb.c.o" \
"CMakeFiles/report_temp.dir/simple_server.c.o"

# External object files for target report_temp
report_temp_EXTERNAL_OBJECTS =

report_temp: CMakeFiles/report_temp.dir/main.c.o
report_temp: CMakeFiles/report_temp.dir/tempdb.c.o
report_temp: CMakeFiles/report_temp.dir/simple_server.c.o
report_temp: CMakeFiles/report_temp.dir/build.make
report_temp: CMakeFiles/report_temp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/virgil/report_temp/report_temp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable report_temp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/report_temp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/report_temp.dir/build: report_temp

.PHONY : CMakeFiles/report_temp.dir/build

CMakeFiles/report_temp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/report_temp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/report_temp.dir/clean

CMakeFiles/report_temp.dir/depend:
	cd /home/virgil/report_temp/report_temp/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/virgil/report_temp/report_temp /home/virgil/report_temp/report_temp /home/virgil/report_temp/report_temp/cmake-build-debug /home/virgil/report_temp/report_temp/cmake-build-debug /home/virgil/report_temp/report_temp/cmake-build-debug/CMakeFiles/report_temp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/report_temp.dir/depend

