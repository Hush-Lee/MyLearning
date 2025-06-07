# src/CMakeLists.txt

add_executable(VulkanDemo main.cpp ${SPIRV_OUTPUTS})

target_include_directories(VulkanDemo PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(VulkanDemo PRIVATE
    VulkanDemoLib
)

