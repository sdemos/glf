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

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

GLfloat triangleVertices[] = {
    // Positions         // Colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // Bottom Right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // Bottom Left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // Top
};

GLfloat rectVertices[] = {
    // Positions
     0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // Top Right
     0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // Bottom Right
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   // Bottom Left
    -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 0.0f    // Top Left
};

GLuint rectIndices[] = {
    0, 1, 3
  , 1, 2, 3
};

char *get_file_contents (const char *filepath)
{
    FILE *fp;
    long  file_size;
    char *buffer;

    // open the file
    fp = fopen(filepath, "r");
    if (!fp) {
        perror("error: get_file_contents: fp fopen");
        fprintf(stderr, "error: get_file_contents: fp fopen problem child: %s\n", filepath);
        return NULL;
    }

    // find the end of the file, and make a buffer based on the length
    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp);
    rewind(fp);

    // allocate a buffer with our newfound knowledge
    buffer = calloc(1, file_size + 1);
    if (!buffer) {
        perror("error: get_file_contents: buffer malloc");
        fprintf(stderr, "error: get_file_contents: buffer malloc problem child: %s\n", filepath);
        fclose(fp);
        return NULL;
    }

    // read file_size bytes from fp into the buffer
    if (fread(buffer, file_size, 1, fp) != 1) {
        perror("error: get_file_contents: fp fread");
        fprintf(stderr, "error: get_file_contents: fp fread problem child: %s\n", filepath);
        fclose(fp);
        free(buffer);
        return NULL;
    }

    // close up the file now that we are done
    fclose(fp);
    return buffer;
}

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

GLuint create_shader_program (char *vert_file, char *frag_file)
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

void display (GLuint program, GLuint VAO)
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
        printf("Failed to initialize GLFW window\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    // initialize glew
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW\n");
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
    // Position attribute information
    glVertexAttribPointer(POSITION_LOC, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(POSITION_LOC);

    // Color attribute information
    glVertexAttribPointer(COLOR_LOC, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(COLOR_LOC);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // MAIN LOOP //
    // glfw's main loop
    while (!glfwWindowShouldClose(window)) {
        // check events and call appropriate callback functions
        glfwPollEvents();

        // figure out what to draw to the screen
        display(program, VAO);

        // finally, swap the buffers
        glfwSwapBuffers(window);
    }

    // CLEANUP //
    // clean up after glfw
    glfwTerminate();
    return 0;
}
