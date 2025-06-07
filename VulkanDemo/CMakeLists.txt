cmake_minimum_required(VERSION 3.16)
project(VulkanDemo)

set(CMAKE_CXX_STANDARD 17)

# Find Vulkan
find_package(Vulkan REQUIRED)

# Find SDL2
find_package(SDL2 REQUIRED)

# Shader compile function
function(compile_shader shader)
    get_filename_component(FILE_NAME ${shader} NAME)
    set(SPIRV_OUTPUT "${CMAKE_BINARY_DIR}/shaders/${FILE_NAME}.spv")
    add_custom_command(
        OUTPUT ${SPIRV_OUTPUT}
        COMMAND glslangValidator -V ${shader} -o ${SPIRV_OUTPUT}
        DEPENDS ${shader}
        COMMENT "Compiling shader ${FILE_NAME}"
    )
    set(SPIRV_OUTPUTS ${SPIRV_OUTPUT} ${SPIRV_OUTPUTS} PARENT_SCOPE)
endfunction()

# Compile all shaders
file(GLOB SHADERS "${CMAKE_SOURCE_DIR}/shaders/*.vert" "${CMAKE_SOURCE_DIR}/shaders/*.frag")
foreach(SHADER ${SHADERS})
    compile_shader(${SHADER})
endforeach()

# Output shaders to binary dir
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/shaders)

# Add subdirectories
add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(test)

