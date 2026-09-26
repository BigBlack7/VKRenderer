# glfw
add_library(glfw3 STATIC IMPORTED GLOBAL)
set_target_properties(glfw3 PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/glfw/lib/glfw3.lib"
)
target_include_directories(glfw3 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include
)

# glm
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/glm/include
)

# shaderc
add_library(shaderc STATIC IMPORTED GLOBAL)
set_target_properties(shaderc PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/shaderc/lib/shaderc_combined.lib"
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/shaderc/lib/shaderc_shared.lib"
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/shaderc/lib/shaderc.lib"
)
target_include_directories(shaderc INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/shaderc/include
)