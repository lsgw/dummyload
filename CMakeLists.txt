cmake_minimum_required(VERSION 2.8)
project(dummyload)

set (CMAKE_SYSTEM_NAME linux)

set (CMAKE_C_COMPILER             "gcc")
set (CMAKE_C_FLAGS                "-Wall -std=c99 -static")
set (CMAKE_C_FLAGS_DEBUG          "-g")
set (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set (CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG")
set (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")

set (CMAKE_CXX_COMPILER             "g++")
set (CMAKE_CXX_FLAGS                "-Wall -Wl,-Bstatic -lstdc++ -lsupc++ -Wl,-Bdynamic")
set (CMAKE_CXX_FLAGS_DEBUG          "-g")
set (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set (CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")


set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/")
set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/lib/")

include_directories(
	${CMAKE_SOURCE_DIR}/base/
	${CMAKE_SOURCE_DIR}/src/
)
add_subdirectory(base)
add_subdirectory(src)
add_subdirectory(http)
add_subdirectory(examples)

