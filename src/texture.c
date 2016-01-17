// texture.c
//
// texture creation
//
// author: Stephen Demos (stphndemos@gmail.com)

#include <stdio.h>

#include <GL/glew.h>

#include <SOIL.h>

#include <texture.h>

/**
 * make_texture
 *
 * this function creates and register a 2D, RGB texture by loading the image
 * provided at the filepath using SOIL (www.lonesock.net/soil.html)
 *
 * on error, this function will print out a detailed error message, including
 * SOIL's error string if the error was with image loading, and then will
 * return 0
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
GLuint make_texture (const char *image_filepath)
{
    GLuint texture;
    int width, height;
    unsigned char *image;

    glGenTextures(1, &texture);
    if (!(image = SOIL_load_image(image_filepath,
                    &width, &height, 0,
                    SOIL_LOAD_RGB))) {
        fprintf(stderr, "error: make_texture: image SOIL_load_iamge");
        fprintf(stderr, "error: make_texture: SOIL error string: %s\n",
                result_string_pointer);
        return 0;
    }

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}
