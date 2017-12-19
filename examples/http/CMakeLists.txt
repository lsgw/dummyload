include_directories(
	${CMAKE_SOURCE_DIR}/http/
)

add_executable(http_test main.cpp)
target_link_libraries(http_test net http)
set_target_properties(http_test PROPERTIES COMPILE_FLAGS "-std=c++1y")
