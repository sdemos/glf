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
    // Positions         // Normals           // Texture Coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

bari_vec3 light_pos;

void display (GLuint shader_program, GLuint shaderVAO, GLuint light_program, GLuint lightVAO)
{
    // set background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set up model view projection matrix
    bari_mat4 view  = camera_view();
    bari_mat4 proj  = bari_perspective(45.0f, WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);
    bari_mat4 vp    = bari_mprod4(proj, view);

    glUseProgram(shader_program);

    // set material properties
    GLuint msloc  = glGetUniformLocation(shader_program, "m.s");
    GLuint mseloc = glGetUniformLocation(shader_program, "m.se");
    glUniform3f(msloc,  0.5f, 0.5f, 0.5f);
    glUniform1f(mseloc, 32.0f);

    // set light properties
    GLuint la = glGetUniformLocation(shader_program, "l.a");
    GLuint ld = glGetUniformLocation(shader_program, "l.d");
    GLuint ls = glGetUniformLocation(shader_program, "l.s");
    GLuint lp = glGetUniformLocation(shader_program, "l.p");
    glUniform3f(la, 0.2f, 0.2f, 0.2f);
    glUniform3f(ld, 0.5f, 0.5f, 0.5f);
    glUniform3f(ls, 1.0f, 1.0f, 1.0f);
    glUniform3f(lp, light_pos.x, light_pos.y, light_pos.z);

    // pass current camera position
    bari_vec3 camera_pos = current_camera_pos();
    GLuint camera_pos_loc = glGetUniformLocation(shader_program, "camera_pos");
    glUniform3f(camera_pos_loc, camera_pos.x, camera_pos.y, camera_pos.z);

    bari_mat4 shader_model  = bari_mat4_id;
    bari_mat4 shader_mvp    = bari_mprod4(vp, shader_model);
    GLuint shader_model_loc = glGetUniformLocation(shader_program, "model");
    GLuint shader_mvp_loc   = glGetUniformLocation(shader_program, "mvp");
    glUniformMatrix4fv(shader_model_loc, 1, GL_FALSE, BARI_VALUE_PTR(shader_model));
    glUniformMatrix4fv(shader_mvp_loc, 1, GL_FALSE, BARI_VALUE_PTR(shader_mvp));

    glBindVertexArray(shaderVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glUseProgram(light_program);

    bari_mat4 light_translate = bari_translate(light_pos.x, light_pos.y, light_pos.z);
    bari_mat4 light_scale     = bari_scale(0.2f, 0.2f, 0.2f);
    bari_mat4 light_model = bari_mprod4(light_translate, light_scale);
    bari_mat4 light_mvp   = bari_mprod4(vp, light_model);
    GLuint light_model_loc = glGetUniformLocation(shader_program, "model");
    GLuint light_mvp_loc  = glGetUniformLocation(light_program, "mvp");
    glUniformMatrix4fv(light_model_loc, 1, GL_FALSE, BARI_VALUE_PTR(light_model));
    glUniformMatrix4fv(light_mvp_loc, 1, GL_FALSE, BARI_VALUE_PTR(light_mvp));

    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

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
        fprintf(stderr, "glf_init: Failed to initialize GLFW window\n");
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(window);

    // initialize glew
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "glf_init: Failed to initialize GLEW\n");
        exit(1);
    }

    // initialize the keyboard
    keyboard_init(window);

    // initialize the camera
    camera_init(window);

    // let opengl know the size of the window it's working with
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // enable various options
    glEnable(GL_DEPTH_TEST);

    // initialize lighting
    light_pos = bari_mkvec3(1.2f, 1.0f, 2.0f);

    return window;
}

int main (int argc, char **argv)
{
    GLuint shader_program, light_program;
    GLFWwindow *window;

    // INITIALIZATION //
    window = glf_init();

    // add some keybindings
    keyboard_add_keybinding(GLFW_KEY_ESCAPE, keyboard_close_window_callback);

    // compile (and link) shaders!
    shader_program = create_shader_program("shaders/shader.vert", "shaders/shader.frag");
    light_program = create_shader_program("shaders/light.vert", "shaders/light.frag");

    // for now we are just going to use this program for everything
    // in the future, I should provide a way to change programs easily
    // that is, if I ever end up needing more than one program
    glUseProgram(shader_program);

    // I need to design a better abstraction for the textures
    // although it seems I need to make all of the textures befor I bind them
    // to their texture unit, or else the old ones get blown away during the
    // texture creation
    // load the textures
    //GLuint container_texture = make_texture("assets/container.jpg");
    //GLuint smile_texture = make_texture("assets/awesomeface.png");
    GLuint texture = make_texture("assets/container2.png");

    //// activate the textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(shader_program, "m.d"), 0);

    //glActiveTexture(GL_TEXTURE1);
    //glBindTexture(GL_TEXTURE_2D, smile_texture);
    //glUniform1i(glGetUniformLocation(program, "ourTexture2"), 1);

    // set up the buffers
    GLuint shaderVAO, VBO, EBO, lightVAO;

    glGenVertexArrays(1, &lightVAO);
    glGenVertexArrays(1, &shaderVAO);
    glGenBuffers(1, &VBO);
    //glGenBuffers(1, &EBO);

    glBindVertexArray(shaderVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectIndices), rectIndices, GL_STATIC_DRAW);

#define POSITION_LOC 0
#define NORMAL_LOC 1
#define TEX_LOC 1
    // Position attribute information
    glVertexAttribPointer(POSITION_LOC, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(POSITION_LOC);

    // normal vector attribute information
    glVertexAttribPointer(NORMAL_LOC, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(NORMAL_LOC);

    // Color attribute information
    //glVertexAttribPointer(COLOR_LOC, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    //glEnableVertexAttribArray(COLOR_LOC);

    // Texture Coordinate attribute information
    glVertexAttribPointer(TEX_LOC, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) (6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(TEX_LOC);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);

    glVertexAttribPointer(POSITION_LOC, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(POSITION_LOC);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // MAIN LOOP //
    // glfw's main loop
    while (!glfwWindowShouldClose(window)) {
        // check events and call appropriate callback functions
        glfwPollEvents();

        keyboard(window);
        camera();
        display(shader_program, shaderVAO, light_program, lightVAO);

        // finally, swap the buffers
        glfwSwapBuffers(window);
    }

    // CLEANUP //
    // clean up after glfw
    glfwTerminate();
    return 0;
}
