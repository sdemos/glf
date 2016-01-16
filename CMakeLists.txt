cmake_minimum_required(VERSION 3.0)

project(glf C) # working title...

find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)
find_package(GLEW REQUIRED)

# hooking up glfw
find_package(glfw3 REQUIRED)
add_definitions(${GLFW_CFLAGS_OTHER})
include_directories(${OPENGL_INCLUDE_DIR}
                    ${GLFW_INCLUDE_DIRS}
                   )

#find_package(pthread)
#find_package(xrandr)
#find_package(xi)

add_subdirectory(src)

# we want to copy the shaders over to the build directory
set(SHADER_DIR shaders)
add_custom_target(copy_shaders
    ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/${SHADER_DIR}
    ${CMAKE_BINARY_DIR}/${SHADER_DIR})
add_dependencies(main copy_shaders)
