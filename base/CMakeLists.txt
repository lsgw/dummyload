set(src 
	Condition.cpp
	Mutex.cpp
	Thread.cpp
	ThreadPool.cpp
)

#file(GLOB_RECURSE src "./*.cpp")  #递归搜索子目录
#aux_source_directory(./ src)

add_library(base STATIC ${src})
target_link_libraries(base pthread)
set_target_properties(base PROPERTIES COMPILE_FLAGS "-std=c++1y")