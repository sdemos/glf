cmake_minimum_required(VERSION 3.3)

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
