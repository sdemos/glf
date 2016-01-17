cmake_minimum_required(VERSION 3.0)

project(glf C) # working title...

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_SOURCE_DIR}/cmake/Modules
   )

find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)
find_package(GLEW REQUIRED)
find_package(SOIL REQUIRED)
find_package(BARI REQUIRED)

# hooking up glfw
find_package(GLFW REQUIRED)
add_definitions(${GLFW_CFLAGS_OTHER})

include_directories(${OPENGL_INCLUDE_DIR}
                    ${GLFW_INCLUDE_DIR}
                    ${SOIL_INCLUDE_DIR}
                    ${BARI_INCLUDE_DIR}
                    include
                   )

add_subdirectory(src)
add_subdirectory(shaders)
