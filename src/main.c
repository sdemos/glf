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

#include <bari.h>

#include "glf.h"
#include "shader.h"
#include "texture.h"
#include "keyboard.h"
#include "camera.h"

GLfloat cubeVerts[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

bari_vec3 cubePositions[10];

void display (GLuint program, GLuint VAO, GLuint container_texture, GLuint smile_texture)
{
    // for now we are just going to use this program for everything
    // in the future, I should provide a way to change programs easily
    glUseProgram(program);

    // set background color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // update the uniform color
    //GLfloat timeValue = glfwGetTime();
    //GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
    //GLint ourColorLocation = glGetUniformLocation(program, "ourColor");
    //glUniform4f(ourColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    // set up transformation matrix
    //bari_mat4 transform = bari_mprod4(bari_scale(0.5, 0.5, 0.5), bari_rotate_z(M_PI / 2));
    //bari_mat4 rotate = bari_rotate_z(DEG_TO_RAD((GLfloat)glfwGetTime() * 50.0f));
    //bari_mat4 translate = bari_translate(0.5f, -0.5f, 0.0f);
    //bari_mat4 transform = bari_mprod4(translate, rotate);
    //GLuint transformLoc = glGetUniformLocation(program, "transform");
    //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform);

    // set up model view projection matrix
    bari_mat4 view = camera_view();
    bari_mat4 proj = bari_perspective(45.0f, WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);
    bari_mat4 vp   = bari_mprod4(proj, view);
    GLuint mvpLoc  = glGetUniformLocation(program, "mvp");

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
    for (GLuint i = 0; i < 10; i++) {
        float angle = DEG_TO_RAD(20.0f * i);
        bari_mat4 translate = bari_translate(cubePositions[i].x, cubePositions[i].y, cubePositions[i].z);
        bari_mat4 rotate = bari_rotate(1.0f * angle, 0.3f * angle, 0.5f * angle);
        bari_mat4 model = bari_mprod4(translate, rotate);
        bari_mat4 mvp = bari_mprod4(vp, model);
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, BARI_VALUE_PTR(mvp));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // unbind the VAO
    glBindVertexArray(0);
}

GLFWwindow *glf_init ()
{
    GLFWwindow *window;

    // initialize glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // get a glfw window
    if (!(window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL", NULL, NULL))) {
        fprintf(stderr, "Failed to initialize GLFW window\n");
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(window);

    // initialize glew
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return 0;
    }

    // initialize the keyboard
    keyboard_init(window);

    // initialize the camera
    camera_init(window);

    // let opengl know the size of the window it's working with
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // enable various options
    glEnable(GL_DEPTH_TEST);

    return window;
}

int main (int argc, char **argv)
{
    GLuint program;
    GLFWwindow *window;

    cubePositions[0] = bari_mkvec3( 0.0f,  0.0f,  0.0f);
    cubePositions[1] = bari_mkvec3( 2.0f,  5.0f, -15.0f);
    cubePositions[2] = bari_mkvec3(-1.5f, -2.2f, -2.5f);
    cubePositions[3] = bari_mkvec3(-3.8f, -2.0f, -12.3f);
    cubePositions[4] = bari_mkvec3( 2.4f, -0.4f, -3.5f);
    cubePositions[5] = bari_mkvec3(-1.7f,  3.0f, -7.5f);
    cubePositions[6] = bari_mkvec3( 1.3f, -2.0f, -2.5f);
    cubePositions[7] = bari_mkvec3( 1.5f,  2.0f, -2.5f);
    cubePositions[8] = bari_mkvec3( 1.5f,  0.2f, -1.5f);
    cubePositions[9] = bari_mkvec3(-1.3f,  1.0f, -1.5f);

    // INITIALIZATION //
    if (!(window = glf_init())) {
        return 1;
    }

    // add some keybindings
    keyboard_add_keybinding(GLFW_KEY_ESCAPE, keyboard_close_window_callback);

    // compile (and link) shaders!
    if (!(program = create_shader_program("shaders/shader.vert", "shaders/shader.frag"))) {
        // any errors in create_shader_program print out detailed error messages
        // so just throw up instead.
        return 1;
    }

    // load the textures
    GLuint container_texture = make_texture("assets/container.jpg");
    GLuint smile_texture = make_texture("assets/awesomeface.png");

    // set up the buffers
    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectIndices), rectIndices, GL_STATIC_DRAW);

#define POSITION_LOC 0
#define COLOR_LOC 1
#define TEX_LOC 2
    // Position attribute information
    glVertexAttribPointer(POSITION_LOC, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(POSITION_LOC);

    // Color attribute information
    //glVertexAttribPointer(COLOR_LOC, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    //glEnableVertexAttribArray(COLOR_LOC);

    // Texture Coordinate attribute information
    glVertexAttribPointer(TEX_LOC, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(TEX_LOC);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // MAIN LOOP //
    // glfw's main loop
    while (!glfwWindowShouldClose(window)) {
        // check events and call appropriate callback functions
        glfwPollEvents();

        keyboard(window);
        camera();
        display(program, VAO, container_texture, smile_texture);

        // finally, swap the buffers
        glfwSwapBuffers(window);
    }

    // CLEANUP //
    // clean up after glfw
    glfwTerminate();
    return 0;
}
