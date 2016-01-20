// texture.h
//
// header for texture creation
//
// author: Stephen Demos (stphndemos@gmail.com)

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <GL/gl.h>

/**
 * make_texture
 *
 * this function creates and register a 2D, RGB texture by loading the image
 * provided at the filepath using SOIL (www.lonesock.net/soil.html)
 *
 * on error, this function will print out a detailed error message, including
 * SOIL's error string if the error was with image loading, and the will call
 * exit(1)
 *
 * arguments
 *  const char *image_filepath - the relative filepath of the iamge to load
 *      as a texture. this must be a file that SOIL knows how to load (see
 *      SOIL's website (listed above) a complete list of image types SOIL can
 *      load)
 * 
 * returns
 *  GLuint that identifies the texture to OpenGL for future calls
 */
GLuint make_texture (const char *image_filepath);

#endif // _TEXTURE_H_
