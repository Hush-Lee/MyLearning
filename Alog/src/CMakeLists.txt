add_executable(Alog main.cpp)
include_directories(../include)
target_link_libraries(Alog
	PRIVATE
		heap
)
