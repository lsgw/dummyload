# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/xiaoge/Desktop/dummyload

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/xiaoge/Desktop/dummyload/build

# Include any dependencies generated for this target.
include http/CMakeFiles/http.dir/depend.make

# Include the progress variables for this target.
include http/CMakeFiles/http.dir/progress.make

# Include the compile flags for this target's objects.
include http/CMakeFiles/http.dir/flags.make

http/CMakeFiles/http.dir/HttpResponse.cpp.o: http/CMakeFiles/http.dir/flags.make
http/CMakeFiles/http.dir/HttpResponse.cpp.o: ../http/HttpResponse.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/xiaoge/Desktop/dummyload/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object http/CMakeFiles/http.dir/HttpResponse.cpp.o"
	cd /home/xiaoge/Desktop/dummyload/build/http && g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/http.dir/HttpResponse.cpp.o -c /home/xiaoge/Desktop/dummyload/http/HttpResponse.cpp

http/CMakeFiles/http.dir/HttpResponse.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/http.dir/HttpResponse.cpp.i"
	cd /home/xiaoge/Desktop/dummyload/build/http && g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/xiaoge/Desktop/dummyload/http/HttpResponse.cpp > CMakeFiles/http.dir/HttpResponse.cpp.i

http/CMakeFiles/http.dir/HttpResponse.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/http.dir/HttpResponse.cpp.s"
	cd /home/xiaoge/Desktop/dummyload/build/http && g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/xiaoge/Desktop/dummyload/http/HttpResponse.cpp -o CMakeFiles/http.dir/HttpResponse.cpp.s

http/CMakeFiles/http.dir/HttpResponse.cpp.o.requires:
.PHONY : http/CMakeFiles/http.dir/HttpResponse.cpp.o.requires

http/CMakeFiles/http.dir/HttpResponse.cpp.o.provides: http/CMakeFiles/http.dir/HttpResponse.cpp.o.requires
	$(MAKE) -f http/CMakeFiles/http.dir/build.make http/CMakeFiles/http.dir/HttpResponse.cpp.o.provides.build
.PHONY : http/CMakeFiles/http.dir/HttpResponse.cpp.o.provides

http/CMakeFiles/http.dir/HttpResponse.cpp.o.provides.build: http/CMakeFiles/http.dir/HttpResponse.cpp.o

http/CMakeFiles/http.dir/HttpServer.cpp.o: http/CMakeFiles/http.dir/flags.make
http/CMakeFiles/http.dir/HttpServer.cpp.o: ../http/HttpServer.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/xiaoge/Desktop/dummyload/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object http/CMakeFiles/http.dir/HttpServer.cpp.o"
	cd /home/xiaoge/Desktop/dummyload/build/http && g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/http.dir/HttpServer.cpp.o -c /home/xiaoge/Desktop/dummyload/http/HttpServer.cpp

http/CMakeFiles/http.dir/HttpServer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/http.dir/HttpServer.cpp.i"
	cd /home/xiaoge/Desktop/dummyload/build/http && g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/xiaoge/Desktop/dummyload/http/HttpServer.cpp > CMakeFiles/http.dir/HttpServer.cpp.i

http/CMakeFiles/http.dir/HttpServer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/http.dir/HttpServer.cpp.s"
	cd /home/xiaoge/Desktop/dummyload/build/http && g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/xiaoge/Desktop/dummyload/http/HttpServer.cpp -o CMakeFiles/http.dir/HttpServer.cpp.s

http/CMakeFiles/http.dir/HttpServer.cpp.o.requires:
.PHONY : http/CMakeFiles/http.dir/HttpServer.cpp.o.requires

http/CMakeFiles/http.dir/HttpServer.cpp.o.provides: http/CMakeFiles/http.dir/HttpServer.cpp.o.requires
	$(MAKE) -f http/CMakeFiles/http.dir/build.make http/CMakeFiles/http.dir/HttpServer.cpp.o.provides.build
.PHONY : http/CMakeFiles/http.dir/HttpServer.cpp.o.provides

http/CMakeFiles/http.dir/HttpServer.cpp.o.provides.build: http/CMakeFiles/http.dir/HttpServer.cpp.o

http/CMakeFiles/http.dir/HttpContext.cpp.o: http/CMakeFiles/http.dir/flags.make
http/CMakeFiles/http.dir/HttpContext.cpp.o: ../http/HttpContext.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/xiaoge/Desktop/dummyload/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object http/CMakeFiles/http.dir/HttpContext.cpp.o"
	cd /home/xiaoge/Desktop/dummyload/build/http && g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/http.dir/HttpContext.cpp.o -c /home/xiaoge/Desktop/dummyload/http/HttpContext.cpp

http/CMakeFiles/http.dir/HttpContext.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/http.dir/HttpContext.cpp.i"
	cd /home/xiaoge/Desktop/dummyload/build/http && g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/xiaoge/Desktop/dummyload/http/HttpContext.cpp > CMakeFiles/http.dir/HttpContext.cpp.i

http/CMakeFiles/http.dir/HttpContext.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/http.dir/HttpContext.cpp.s"
	cd /home/xiaoge/Desktop/dummyload/build/http && g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/xiaoge/Desktop/dummyload/http/HttpContext.cpp -o CMakeFiles/http.dir/HttpContext.cpp.s

http/CMakeFiles/http.dir/HttpContext.cpp.o.requires:
.PHONY : http/CMakeFiles/http.dir/HttpContext.cpp.o.requires

http/CMakeFiles/http.dir/HttpContext.cpp.o.provides: http/CMakeFiles/http.dir/HttpContext.cpp.o.requires
	$(MAKE) -f http/CMakeFiles/http.dir/build.make http/CMakeFiles/http.dir/HttpContext.cpp.o.provides.build
.PHONY : http/CMakeFiles/http.dir/HttpContext.cpp.o.provides

http/CMakeFiles/http.dir/HttpContext.cpp.o.provides.build: http/CMakeFiles/http.dir/HttpContext.cpp.o

http/CMakeFiles/http.dir/HttpRequest.cpp.o: http/CMakeFiles/http.dir/flags.make
http/CMakeFiles/http.dir/HttpRequest.cpp.o: ../http/HttpRequest.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/xiaoge/Desktop/dummyload/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object http/CMakeFiles/http.dir/HttpRequest.cpp.o"
	cd /home/xiaoge/Desktop/dummyload/build/http && g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/http.dir/HttpRequest.cpp.o -c /home/xiaoge/Desktop/dummyload/http/HttpRequest.cpp

http/CMakeFiles/http.dir/HttpRequest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/http.dir/HttpRequest.cpp.i"
	cd /home/xiaoge/Desktop/dummyload/build/http && g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/xiaoge/Desktop/dummyload/http/HttpRequest.cpp > CMakeFiles/http.dir/HttpRequest.cpp.i

http/CMakeFiles/http.dir/HttpRequest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/http.dir/HttpRequest.cpp.s"
	cd /home/xiaoge/Desktop/dummyload/build/http && g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/xiaoge/Desktop/dummyload/http/HttpRequest.cpp -o CMakeFiles/http.dir/HttpRequest.cpp.s

http/CMakeFiles/http.dir/HttpRequest.cpp.o.requires:
.PHONY : http/CMakeFiles/http.dir/HttpRequest.cpp.o.requires

http/CMakeFiles/http.dir/HttpRequest.cpp.o.provides: http/CMakeFiles/http.dir/HttpRequest.cpp.o.requires
	$(MAKE) -f http/CMakeFiles/http.dir/build.make http/CMakeFiles/http.dir/HttpRequest.cpp.o.provides.build
.PHONY : http/CMakeFiles/http.dir/HttpRequest.cpp.o.provides

http/CMakeFiles/http.dir/HttpRequest.cpp.o.provides.build: http/CMakeFiles/http.dir/HttpRequest.cpp.o

# Object files for target http
http_OBJECTS = \
"CMakeFiles/http.dir/HttpResponse.cpp.o" \
"CMakeFiles/http.dir/HttpServer.cpp.o" \
"CMakeFiles/http.dir/HttpContext.cpp.o" \
"CMakeFiles/http.dir/HttpRequest.cpp.o"

# External object files for target http
http_EXTERNAL_OBJECTS =

../lib/libhttp.a: http/CMakeFiles/http.dir/HttpResponse.cpp.o
../lib/libhttp.a: http/CMakeFiles/http.dir/HttpServer.cpp.o
../lib/libhttp.a: http/CMakeFiles/http.dir/HttpContext.cpp.o
../lib/libhttp.a: http/CMakeFiles/http.dir/HttpRequest.cpp.o
../lib/libhttp.a: http/CMakeFiles/http.dir/build.make
../lib/libhttp.a: http/CMakeFiles/http.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library ../../lib/libhttp.a"
	cd /home/xiaoge/Desktop/dummyload/build/http && $(CMAKE_COMMAND) -P CMakeFiles/http.dir/cmake_clean_target.cmake
	cd /home/xiaoge/Desktop/dummyload/build/http && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/http.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
http/CMakeFiles/http.dir/build: ../lib/libhttp.a
.PHONY : http/CMakeFiles/http.dir/build

http/CMakeFiles/http.dir/requires: http/CMakeFiles/http.dir/HttpResponse.cpp.o.requires
http/CMakeFiles/http.dir/requires: http/CMakeFiles/http.dir/HttpServer.cpp.o.requires
http/CMakeFiles/http.dir/requires: http/CMakeFiles/http.dir/HttpContext.cpp.o.requires
http/CMakeFiles/http.dir/requires: http/CMakeFiles/http.dir/HttpRequest.cpp.o.requires
.PHONY : http/CMakeFiles/http.dir/requires

http/CMakeFiles/http.dir/clean:
	cd /home/xiaoge/Desktop/dummyload/build/http && $(CMAKE_COMMAND) -P CMakeFiles/http.dir/cmake_clean.cmake
.PHONY : http/CMakeFiles/http.dir/clean

http/CMakeFiles/http.dir/depend:
	cd /home/xiaoge/Desktop/dummyload/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xiaoge/Desktop/dummyload /home/xiaoge/Desktop/dummyload/http /home/xiaoge/Desktop/dummyload/build /home/xiaoge/Desktop/dummyload/build/http /home/xiaoge/Desktop/dummyload/build/http/CMakeFiles/http.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : http/CMakeFiles/http.dir/depend

