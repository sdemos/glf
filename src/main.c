// main.c
//
// main file for my opengl framework
//
// author: Stephen Demos (stphndemos@gmail.com)

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <SOIL.h>

#include "shader.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

GLfloat triangleVertices[] = {
    // Positions         // Colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // Bottom Right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // Bottom Left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // Top
};

GLfloat rectVertices[] = {
    // Positions         // Colors          // Texture Coords
     0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,   // Top Right
     0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,   // Bottom Right
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,   // Bottom Left
    -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,   // Top Left
};

GLuint rectIndices[] = {
    0, 1, 3
  , 1, 2, 3
};

void display (GLuint program, GLuint VAO, GLuint container_texture, GLuint smile_texture)
{
    // for now we are just going to use this program for everything
    // in the future, I should provide a way to change programs easily
    glUseProgram(program);

    // set background color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // update the uniform color
    //GLfloat timeValue = glfwGetTime();
    //GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
    //GLint ourColorLocation = glGetUniformLocation(program, "ourColor");
    //glUniform4f(ourColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    // activate the textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, container_texture);
    glUniform1i(glGetUniformLocation(program, "ourTexture1"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, smile_texture);
    glUniform1i(glGetUniformLocation(program, "ourTexture2"), 1);

    // bind the VAO we want to use
    glBindVertexArray(VAO);
    // draw the object
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // unbind the VAO
    glBindVertexArray(0);
}

void keyboard (GLFWwindow *window, int key, int scancode, int action, int mode)
{
    // close the application!
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main (int argc, char **argv)
{
    GLuint program;

    // INITIALIZATION //
    // initialize glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // get a glfw window
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to initialize GLFW window\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    // initialize glew
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return 0;
    }

    // let opengl know the size of the window it's working with
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // set a variety of glfw callback functions here
    glfwSetKeyCallback(window, keyboard);

    // compile (and link) shaders!
    if (!(program = create_shader_program("shaders/shader.vert", "shaders/shader.frag"))) {
        // any errors in create_shader_program print out detailed error messages
        // so just throw up instead.
        return 0;
    }

    // load the textures
    int width, height;
    GLuint container_texture, smile_texture;

    glGenTextures(1, &container_texture);
    glGenTextures(1, &smile_texture);

    glBindTexture(GL_TEXTURE_2D, container_texture);
    unsigned char *container_image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, container_image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(container_image);

    glBindTexture(GL_TEXTURE_2D, smile_texture);
    unsigned char *smile_image = SOIL_load_image("awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, smile_image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(smile_image);

    glBindTexture(GL_TEXTURE_2D, 0);

    // set up the buffers
    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectVertices), rectVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectIndices), rectIndices, GL_STATIC_DRAW);

#define POSITION_LOC 0
#define COLOR_LOC 1
#define TEX_LOC 2
    // Position attribute information
    glVertexAttribPointer(POSITION_LOC, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(POSITION_LOC);

    // Color attribute information
    glVertexAttribPointer(COLOR_LOC, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(COLOR_LOC);

    // Texture Coordinate attribute information
    glVertexAttribPointer(TEX_LOC, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) (6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(TEX_LOC);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // MAIN LOOP //
    // glfw's main loop
    while (!glfwWindowShouldClose(window)) {
        // check events and call appropriate callback functions
        glfwPollEvents();

        // figure out what to draw to the screen
        display(program, VAO, container_texture, smile_texture);

        // finally, swap the buffers
        glfwSwapBuffers(window);
    }

    // CLEANUP //
    // clean up after glfw
    glfwTerminate();
    return 0;
}
