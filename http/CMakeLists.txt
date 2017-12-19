aux_source_directory(./ src)
add_library(http STATIC ${src})
target_link_libraries(http pthread net)
set_target_properties(http PROPERTIES COMPILE_FLAGS "-std=c++1y")