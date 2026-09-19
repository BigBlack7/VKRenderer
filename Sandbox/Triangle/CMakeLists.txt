project(Triangle)
file(GLOB_RECURSE MAIN CONFIGURE_DEPENDS main.cpp)
add_executable(Triangle ${MAIN})
target_link_libraries(Triangle PRIVATE Core)