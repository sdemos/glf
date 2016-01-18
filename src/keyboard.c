// keyboard.c
//
// keyboard abstractions
//
// author: Stephen Demos (stphndemos@gmail.com)

#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "keyboard.h"

typedef struct {
    int key;
    void (*callback) (GLFWwindow *w);
} key_callback_t;

int callbackc = 0, callbacks = 10;
key_callback_t **callbackv;

/**
 * keyboard
 *
 * this is the function to call in the main loop. It goes through all of the
 * registered keybindings and when it finds one that is pressed, it calls the
 * keybindings callback.
 *
 * arguments
 *  GLFWwindow *window - the GLFW window we are acting on
 */
void keyboard (GLFWwindow *window)
{
    for (int i = 0; i < callbackc; i++) {
        if (keys[callbackv[i]->key]) {
            callbackv[i]->callback(window);
        }
    }
}

/**
 * keyboard_add_keybinding
 *
 * register a keybinding. keybindings registered here will then be checked and
 * called in the keyboard() function
 *
 * arguments
 *  int key - the GLFW key to register the callback to
 *  void (*callback) (GLFWwindow *w) - function pointer of the callback,
 *      taking the GLFW window we are acting on as an argument
 */
void keyboard_add_keybinding (int key, void (*callback) (GLFWwindow *w))
{
    key_callback_t *k = malloc(sizeof(key_callback_t));
    k->key = key;
    k->callback = callback;

    if (callbackc == callbacks) {
        callbacks += 10;
        callbackv = realloc(callbackv, sizeof(key_callback_t *) * callbacks);
    }

    callbackv[callbackc++] = k;
}

/**
 * keyboard_callback
 *
 * the keyboard callback function to register with GLFW at initialization.
 * This function simply sets a key as pressed or not in the keys buffer,
 * the keyboard() function is the function that actually performs the callback
 * actions.
 *
 * arguments are defined by GLFW. see their documentation for details.
 */
void keyboard_callback (GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (action == GLFW_PRESS) {
        keys[key] = 1;
    } else if (action == GLFW_RELEASE) {
        keys[key] = 0;
    }
}

/**
 * keyboard_init
 *
 * initialize the keyboard functions. This should be called during program
 * initialization, after the GLFW window has been created and set as the
 * current context
 *
 * arguments
 *  GLFWwindow *window - the window to listen for keypresses on. This is the
 *      window that we will register the keyboard callback for.
 */
void keyboard_init (GLFWwindow *window)
{
    glfwSetKeyCallback(window, keyboard_callback);
    callbackv = malloc(sizeof(key_callback_t *) * callbacks);
}

/* PRE-DEFINED CALLBACKS */
/* for convinience, some common callbacks are already defined below */

/**
 * keyboard_close_window_callback
 *
 * predefined callback that calls glfwSetWindowShouldClose with GL_TRUE,
 * telling GLFW to close the current window
 *
 * arguments
 *  GLFWwindow *window - the GLFW window to act on
 */
void keyboard_close_window_callback (GLFWwindow *window)
{
    glfwSetWindowShouldClose(window, GL_TRUE);
}
