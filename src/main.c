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
#include "light.h"

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

bari_vec3 point_lights[4];
bari_vec3 cube_positions[10];

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
    GLuint mseloc = glGetUniformLocation(shader_program, "m.se");
    glUniform1f(mseloc, 32.0f);

    // set directional light properties
    GLuint ddir = glGetUniformLocation(shader_program, "dir_light.direction");
    GLuint da = glGetUniformLocation(shader_program, "dir_light.a");
    GLuint dd = glGetUniformLocation(shader_program, "dir_light.d");
    GLuint ds = glGetUniformLocation(shader_program, "dir_light.s");
    glUniform3f(ddir, -0.2f, -1.0f, -0.3f);
    glUniform3f(da, 0.2f, 0.2f, 0.2f);
    glUniform3f(dd, 0.5f, 0.5f, 0.5f);
    glUniform3f(ds, 1.0f, 1.0f, 1.0f);

    // set point light properties
    for (GLuint i = 0; i < 4; i++) {
        char spp[20], spa[20], spd[20], sps[20], spc[20], spl[20], spq[20];
        sprintf(spp, "point_lights[%d].p", i);
        sprintf(spa, "point_lights[%d].a", i);
        sprintf(spd, "point_lights[%d].d", i);
        sprintf(sps, "point_lights[%d].s", i);
        sprintf(spc, "point_lights[%d].c", i);
        sprintf(spl, "point_lights[%d].l", i);
        sprintf(spq, "point_lights[%d].q", i);
        GLuint pp = glGetUniformLocation(shader_program, spp);
        GLuint pa = glGetUniformLocation(shader_program, spa);
        GLuint pd = glGetUniformLocation(shader_program, spd);
        GLuint ps = glGetUniformLocation(shader_program, sps);
        GLuint pc = glGetUniformLocation(shader_program, spc);
        GLuint pl = glGetUniformLocation(shader_program, spl);
        GLuint pq = glGetUniformLocation(shader_program, spq);
        glUniform3f(pp, point_lights[i].x, point_lights[i].y, point_lights[i].z);
        glUniform3f(pa, 0.2f, 0.2f, 0.2f);
        glUniform3f(pd, 0.5f, 0.5f, 0.5f);
        glUniform3f(ps, 1.0f, 1.0f, 1.0f);
        glUniform1f(pc, 1.0f);
        glUniform1f(pl, 0.09f);
        glUniform1f(pq, 0.032f);
    }

    // set spotlight properties
    GLuint sa = glGetUniformLocation(shader_program, "spotlight.a");
    GLuint sd = glGetUniformLocation(shader_program, "spotlight.d");
    GLuint ss = glGetUniformLocation(shader_program, "spotlight.s");
    GLuint sp = glGetUniformLocation(shader_program, "spotlight.p");
    GLuint sc = glGetUniformLocation(shader_program, "spotlight.c");
    GLuint sl = glGetUniformLocation(shader_program, "spotlight.l");
    GLuint sq = glGetUniformLocation(shader_program, "spotlight.q");
    GLuint sdir = glGetUniformLocation(shader_program, "spotlight.direction");
    GLuint scut = glGetUniformLocation(shader_program, "spotlight.cutoff");
    GLuint soutcut = glGetUniformLocation(shader_program, "spotlight.outercutoff");
    glUniform3f(sa, 0.2f, 0.2f, 0.2f);
    glUniform3f(sd, 0.5f, 0.5f, 0.5f);
    glUniform3f(ss, 1.0f, 1.0f, 1.0f);
    glUniform3f(sp, camera_pos.x, camera_pos.y, camera_pos.z);
    glUniform1f(sc, 1.0f);
    glUniform1f(sl, 0.09f);
    glUniform1f(sq, 0.032f);
    glUniform3f(sdir, camera_front.x, camera_front.y, camera_front.z);
    glUniform1f(scut, cos(DEG_TO_RAD(12.5f)));
    glUniform1f(soutcut, cos(DEG_TO_RAD(17.5f)));

    // pass current camera position
    GLuint camera_pos_loc = glGetUniformLocation(shader_program, "camera_pos");
    glUniform3f(camera_pos_loc, camera_pos.x, camera_pos.y, camera_pos.z);

    GLuint shader_model_loc = glGetUniformLocation(shader_program, "model");
    GLuint shader_mvp_loc   = glGetUniformLocation(shader_program, "mvp");
    glBindVertexArray(shaderVAO);
    for (GLuint i = 0; i < 10; i++) {
        float angle = DEG_TO_RAD(20.0f * i);
        bari_mat4 translate = bari_translate(cube_positions[i].x, cube_positions[i].y, cube_positions[i].z);
        bari_mat4 rotate = bari_rotate(1.0f * angle, 0.3f * angle, 0.5f * angle);
        bari_mat4 model = bari_mprod4(translate, rotate);
        bari_mat4 mvp = bari_mprod4(vp, model);
        glUniformMatrix4fv(shader_model_loc, 1, GL_FALSE, BARI_VALUE_PTR(model));
        glUniformMatrix4fv(shader_mvp_loc, 1, GL_FALSE, BARI_VALUE_PTR(mvp));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glUseProgram(light_program);

    bari_mat4 light_scale  = bari_scale(0.2f, 0.2f, 0.2f);
    GLuint light_model_loc = glGetUniformLocation(light_program, "model");
    GLuint light_mvp_loc   = glGetUniformLocation(light_program, "mvp");
    glBindVertexArray(lightVAO);

    for (GLuint i = 0; i < 4; i++) {
        bari_mat4 light_translate = bari_translate(point_lights[i].x, point_lights[i].y, point_lights[i].z);
        bari_mat4 light_model = bari_mprod4(light_translate, light_scale);
        bari_mat4 light_mvp   = bari_mprod4(vp, light_model);
        glUniformMatrix4fv(light_model_loc, 1, GL_FALSE, BARI_VALUE_PTR(light_model));
        glUniformMatrix4fv(light_mvp_loc, 1, GL_FALSE, BARI_VALUE_PTR(light_mvp));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

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
    //light_pos = bari_mkvec3(-0.2f, -1.0f, -0.3f);
    //light_pos = bari_mkvec3(1.2f, 1.0f, 2.0f);
    point_lights[0] = bari_mkvec3( 0.7f,  0.2f,  2.0f);
    point_lights[1] = bari_mkvec3( 2.3f, -3.3f, -4.0f);
    point_lights[2] = bari_mkvec3(-4.0f,  2.0f, -12.0f);
    point_lights[3] = bari_mkvec3( 0.0f,  0.0f, -3.0f);

    cube_positions[0] = bari_mkvec3( 0.0f,  0.0f,  0.0f);
    cube_positions[1] = bari_mkvec3( 2.0f,  5.0f, -15.0f);
    cube_positions[2] = bari_mkvec3(-1.5f, -2.2f, -2.5f);
    cube_positions[3] = bari_mkvec3(-3.8f, -2.0f, -12.3f);
    cube_positions[4] = bari_mkvec3( 2.4f, -0.4f, -3.5f);
    cube_positions[5] = bari_mkvec3(-1.7f,  3.0f, -7.5f);
    cube_positions[6] = bari_mkvec3( 1.3f, -2.0f, -2.5f);
    cube_positions[7] = bari_mkvec3( 1.5f,  2.0f, -2.5f);
    cube_positions[8] = bari_mkvec3( 1.5f,  0.2f, -1.5f);
    cube_positions[9] = bari_mkvec3(-1.3f,  1.0f, -1.5f);

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
    GLuint diffuse_map = make_texture("assets/container2.png");
    GLuint spec_map = make_texture("assets/container2_specular.png");

    // activate the textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse_map);
    glUniform1i(glGetUniformLocation(shader_program, "m.d"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, spec_map);
    glUniform1i(glGetUniformLocation(shader_program, "m.s"), 1);

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
#define TEX_LOC 2
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

