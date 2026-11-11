find_package(Vulkan REQUIRED)

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