file(GLOB SOURCES "*.cpp")
include_directories(../include)

add_executable(SDLRE ${SOURCES})

target_link_libraries(SDLRE
	PRIVATE
	SDL2Lib
)

add_custom_command(TARGET SDLRE POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:SDLRE>/assets
)

