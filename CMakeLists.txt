cmake_minimum_required(VERSION 3.0)

project(glf C) # working title...

find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)
find_package(GLEW REQUIRED)
find_package(SOIL REQUIRED)

# hooking up glfw
find_package(GLFW REQUIRED)
add_definitions(${GLFW_CFLAGS_OTHER})

include_directories(${OPENGL_INCLUDE_DIR}
                    ${GLFW_INCLUDE_DIR}
                    ${SOIL_INCLUDE_DIR}
                    include
                   )

add_subdirectory(src)
add_subdirectory(shaders)
