set(SOURCES file.c shader.c)

add_executable(main main.c ${SOURCES})

target_link_libraries(main 
                      ${GLEW_LIBRARY} 
                      ${GLFW_LIBRARIES}
                      ${SOIL_LIBRARY}
                      ${OPENGL_gl_LIBRARY}
                      ${X11_LIBRARY}
                      m # link with math system libs
                     )
