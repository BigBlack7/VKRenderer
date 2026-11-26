find_package(Vulkan REQUIRED)

find_program(SLANGC_EXECUTABLE
    NAMES slangc
    HINTS "$ENV{VULKAN_SDK}/Bin"
    REQUIRED
)

message(STATUS "Slang compiler: ${SLANGC_EXECUTABLE}")

# ================EnvironmentCheck================
add_executable(EnvironmentCheck
    EnvironmentCheck/Main.cpp
)

set_property(TARGET EnvironmentCheck PROPERTY
    MSVC_RUNTIME_LIBRARY "MultiThreadedDLL"
)

target_link_libraries(EnvironmentCheck PRIVATE
    Vulkan::Vulkan
    glfw3
    glm
)

if(WIN32)
    target_link_libraries(EnvironmentCheck PRIVATE
        user32 gdi32 shell32
    )
endif()

# ================HelloTriangle================
add_executable(HelloTriangle
    HelloTriangle/Main.cpp
)

set_property(TARGET HelloTriangle PROPERTY
    MSVC_RUNTIME_LIBRARY "MultiThreadedDLL"
)

target_compile_definitions(HelloTriangle PRIVATE
    VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
)

target_link_libraries(HelloTriangle PRIVATE
    Vulkan::Vulkan
    glfw3
)

if(WIN32)
    target_link_libraries(HelloTriangle PRIVATE
        user32 gdi32 shell32
    )
endif()

if(MSVC)
    target_compile_options(HelloTriangle PRIVATE /utf-8)
endif()

set(HELLO_TRIANGLE_SHADER_SOURCE
    "${PROJECT_SOURCE_DIR}/Slang/HelloTriangle.slang"
)

set(HELLO_TRIANGLE_SHADER_DIR
    "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/shaders"
)

set(HELLO_TRIANGLE_SHADER_OUTPUT
    "${HELLO_TRIANGLE_SHADER_DIR}/HelloTriangle.spv"
)

add_custom_command(
    OUTPUT "${HELLO_TRIANGLE_SHADER_OUTPUT}"

    COMMAND "${CMAKE_COMMAND}" -E make_directory
    "${HELLO_TRIANGLE_SHADER_DIR}"

    COMMAND "${SLANGC_EXECUTABLE}"
    "${HELLO_TRIANGLE_SHADER_SOURCE}"
    -target spirv
    -profile spirv_1_4
    -emit-spirv-directly
    -fvk-use-entrypoint-name
    -entry vertMain
    -entry fragMain
    -o "${HELLO_TRIANGLE_SHADER_OUTPUT}"

    DEPENDS
    "${HELLO_TRIANGLE_SHADER_SOURCE}"
    "${SLANGC_EXECUTABLE}"

    COMMENT "Compiling HelloTriangle Slang shaders"
    VERBATIM
)

add_custom_target(HelloTriangleShaders
    DEPENDS "${HELLO_TRIANGLE_SHADER_OUTPUT}"
)

add_dependencies(HelloTriangle HelloTriangleShaders)