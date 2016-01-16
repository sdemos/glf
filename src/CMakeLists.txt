set(SOURCES file.c shader.c)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(main main.c ${SOURCES})

target_link_libraries(main 
                      ${GLEW_LIBRARY} 
                      ${GLFW_LIBRARIES}
                      ${OPENGL_gl_LIBRARY}
                      ${X11_LIBRARY}
                      m # link with math system libs
                     )
