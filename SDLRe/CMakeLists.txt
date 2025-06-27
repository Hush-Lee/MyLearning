cmake_minimum_required(VERSION 3.30)
project(SDLRe  LANGUAGES CXX)

set(CXX_STANDARD 20)
set(CXX_STANdARF_REQUIRED OFF)
set(CXX_EXTENTIONS OFF)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG  ${CMAKE_SOURCE_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release)



add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(test)
enable_testing()


