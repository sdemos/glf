# copy each shader to the output shader directory
# this method prevents unecessary copying of files

set(SHADER_DIR shaders)
set(SHADERS shader.vert shader.frag)

foreach(SHADER ${SHADERS})
    add_custom_target(copy_shader_${SHADER}
        ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/${SHADER_DIR}/${SHADER}
        ${CMAKE_BINARY_DIR}/${SHADER_DIR}/${SHADER})
    add_dependencies(main copy_shader_${SHADER})
endforeach(SHADER)
