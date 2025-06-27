file(GLOB_RECURSE LIB_HEADERS "${CMAKE_SOURCE_DIR}/include/*.hpp")
file(GLOB_RECURSE LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(SDL2Lib STATIC ${LIB_HEADERS} ${LIB_SOURCES})

target_include_directories(SDL2Lib PUBLIC
	${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(SDL2Lib PUBLIC
	SDL2::SDL2
	SDL2_image::SDL2_image
)
