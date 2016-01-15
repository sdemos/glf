// main.c
//
// main file for my opengl framework
//
// author: Stephen Demos (stphndemos@gmail.com)

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

GLfloat triangleVertices[] = {
    -0.5f, -0.5f,  0.0f,
     0.5f, -0.5f,  0.0f,
     0.0f,  0.5f,  0.0f
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
    buffer = malloc(file_size + 1);
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

GLuint create_shader_program ()
{
    GLuint vert, frag, program;
    GLint success;
    GLchar infoLog[512];

    // compile the vertex and fragment shaders
    vert = compile_shader("shaders/shader.vert", GL_VERTEX_SHADER);
    frag = compile_shader("shaders/shader.frag", GL_FRAGMENT_SHADER);

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

void display ()
{
    // set background color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // set up the VBO
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
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
    glfwWindowHint(GLFW_WINDOW_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_WINDOW_VERSION_MINOR, 3);

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
    program = create_shader_program();

    // MAIN LOOP //
    // glfw's main loop
    while (!glfwWindowShouldClose(window)) {
        // check events and call appropriate callback functions
        glfwPollEvents();

        // figure out what to draw to the screen
        display();

        // finally, swap the buffers
        glfwSwapBuffers(window);
    }

    // CLEANUP //
    // clean up after glfw
    glfwTerminate();
    return 0;
}
