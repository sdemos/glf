// keyboard.h
//
// keyboard abstractions
//
// author: Stephen Demos (stphndemos@gmail.com)

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// keep track of key presses
// I am probably going to remove this in the future
// Once I am convinced that adding keybinding callbacks is a good way to do this
char keys[1024];

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
void keyboard (GLFWwindow *window);

/**
 * keyboard_add_keybinding
 *
 * register a keybinding. keybindings registered here will then be checked and
 * called in the keyboard function
 *
 * arguments
 *  int key - the GLFW key to register the callback to
 *  void (*callback) (GLFWwindow *w) - function pointer of the callback,
 *      taking the GLFW window we are acting on as an argument
 */
void keyboard_add_keybinding (int key, void (*callback) (GLFWwindow *w));

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
void keyboard_init (GLFWwindow *window);

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
void keyboard_close_window_callback (GLFWwindow *window);

#endif // _KEYBOARD_H_
