include_directories(
	${CMAKE_SOURCE_DIR}/http/
)

add_executable(dummyload dummyload.cpp)
target_link_libraries(dummyload base net http)
set_target_properties(dummyload PROPERTIES COMPILE_FLAGS "-std=c++1y")
