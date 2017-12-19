add_executable(client client.cpp)
target_link_libraries(client net)
set_target_properties(client PROPERTIES COMPILE_FLAGS "-std=c++1y")

add_executable(server server.cpp)
target_link_libraries(server net)
set_target_properties(server PROPERTIES COMPILE_FLAGS "-std=c++1y")