include_directories(
	${CMAKE_SOURCE_DIR}/http/
)

set(src
	ReadSmallFile.cpp
	SystemInfo.cpp
	Imagepng.cpp
	jsonxx.cc
	main.cpp
)

add_executable(procmon ${src})
target_link_libraries(procmon net http)
set_target_properties(procmon PROPERTIES COMPILE_FLAGS "-std=c++1y")
