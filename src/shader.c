// shader.c
//
// functions for compiling and linking shader files
//
// author: Stephen Demos (stphndemos@gmail.com)

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include "file.h"

#include "shader.h"

/**
 * compile_shader
 *
 * this is a private helper function for create_shader_program that compiles
 * a shader given the relative filepath on disk and the type of the shader
 *
 * if any errors occur during the shader compilation, including reading the
 * given shader file or compiling the shader, a detailed error message is
 * printed to stderr and 0 is returned
 *
 * arguments:
 *  const char *shader_filepath - the relative filepath of the shader to
 *      compile
 *  GLenum shader_type - the type of shader to compile the given file as
 *
 * returns:
 *  a GLuint that identifies the shader to OpenGL, or 0 on error (see above)
 */
GLuint compile_shader (const char *shader_filepath, GLenum shader_type)
{
    GLchar *shader_source;
    GLuint shader;
    GLint success;
    GLchar infoLog[512];

    // get the contents of the file
    if (!(shader_source = get_file_contents(shader_filepath))) {
        // failed to get shader file!
        // get_file_contents should've already errored
        // so just pass the error up the chain
        return 0;
    }

    // create our brand new shader
    shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, (const GLchar**)&shader_source, NULL);
    glCompileShader(shader);

    // now we want to actually check for shader compilation errors
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "error: compile_shader: shader failed to compile - %s\n", shader_filepath);
        fprintf(stderr, "error: compile_shader: shader compile error - \n%s\n", infoLog);
        return 0;
    }

    // clean up the shader_source string
    free(shader_source);

    // finally, return set up shader
    return shader;
}

/**
 * create_shader_program
 * 
 * create a shader program using the provided vertex and fragment shaders
 * this means compiling both of the shaders, and then linking them
 *
 * if any errors occur during this function, including reading the files,
 * compilation, and linking errors, a detailed error message is printed to
 * stderr and 0 is returned
 *
 * arguments:
 *  const char *vert_file - the relative filepath of the vertex shader to
 *      compile and link
 *  const char *frag_file = the relative filepath of the fragment shader to
 *      compile and link
 *
 * returns:
 *  the GLuint identifier of the program that is registered with OpenGL,
 *  or 0 on error (see above)
 */
GLuint create_shader_program (const char *vert_file, const char *frag_file)
{
    GLuint vert, frag, program;
    GLint success;
    GLchar infoLog[512];

    // compile the vertex and fragment shaders
    if (!(vert = compile_shader(vert_file, GL_VERTEX_SHADER))
     || !(frag = compile_shader(frag_file, GL_FRAGMENT_SHADER))) {
        // compile shader should've printed out a more detailed error
        // so just throw this back up to the caller
        return 0;
    }

    // create a new program to link the shaders to
    program = glCreateProgram();

    // hook it all up
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    // now check for linker errors
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        fprintf(stderr, "error: create_shader_program: shader failed to link");
        fprintf(stderr, "error: create_shader_program: shader link error - \n%s\n", infoLog);
        return 0;
    }

    return program;
}
