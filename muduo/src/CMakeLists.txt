list(APPEND flags "-std=c++20")

add_executable(muduo main.cpp)
include_directories(../include)
target_compile_options(muduo
	PRIVATE
		${flags}
	)

