# lib/CMakeLists.txt


file(GLOB_RECURSE LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE LIB_HEADERS "${CMAKE_SOURCE_DIR}/include/*.hpp")

#add_library(VulkanDemoLib STATIC ${LIB_SOURCES} ${LIB_HEADERS})

#target_include_directories(VulkanDemoLib PUBLIC
#    ${CMAKE_SOURCE_DIR}/include
#    ${SDL2_INCLUDE_DIRS}
#)

#target_link_libraries(VulkanDemoLib PUBLIC
#    Vulkan::Vulkan
#    ${SDL2_LIBRARIES}
#)

