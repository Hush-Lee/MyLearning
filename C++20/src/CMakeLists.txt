list(APPEND flags "-std=c++20") 
include_directories(../include)
add_executable(C++20 main.cpp)
target_compile_options(C++20
	PRIVATE
		${flags}
	)
