// camera.h
//
// camera abstractions
//
// author: Stephen Demos (stphndemos@gmail.com)

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <bari.h>

/**
 * camera
 *
 * this function does camera calculations that must be done every frame. It
 * should be called in the main loop.
 */
void camera ();

/**
 * camera_view
 *
 * get the view matrix that transforms world coordinates into view coordinates
 * using the current details of the camera position and direction
 *
 * returns
 *  the view matrix
 */
bari_mat4 camera_view ();

/**
 * camera_init
 *
 * initialize the camera. This function sets the initial camera location,
 * registers the mouse callback, and registers WASD with the keyboard
 * subsystem. This function requires that the keyboard subsystem is
 * initialized first
 *
 * arguments
 *  GLFWwindow *window - the window that the camera is in (mostly to register
 *      the mouse callback function)
 */
void camera_init (GLFWwindow *window);

#endif // _CAMERA_H_
