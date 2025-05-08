file(GLOB SOURCES "*.cpp")
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
include_directories(../include)
include_directories(../lib)

add_executable(SDLRe ${SOURCES})
target_link_libraries(SDLRe ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} SDL2_image)
