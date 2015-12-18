add_executable(main main.c)

target_link_libraries(main 
                      ${GLEW_LIBRARY} 
                      ${GLFW3_LIBRARY} 
                      ${OPENGL_gl_LIBRARY}
                      ${X11_LIBRARY}
                     )
