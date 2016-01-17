#ifndef _SHADER_H_
#define _SHADER_H_

#include <GL/gl.h>

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
GLuint create_shader_program (const char *vert_file, const char *frag_file);

#endif // _SHADER_H_
