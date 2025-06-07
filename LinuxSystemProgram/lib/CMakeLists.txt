file(GLOB_RECURSE LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE LIB_HEADERS "${CMAKE_SOURCE_DIR}/include/*.hpp")

add_library(localLib STATIC ${LIB_HEADERS} ${LIB_SOURCES})

target_include_directories(localLib
	${CMAKE_SOURCE_DIR}/include
)

