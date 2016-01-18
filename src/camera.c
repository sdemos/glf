// camera.c
//
// camera abstractions
//
// author: Stephen Demos (stphndemos@gmail.com)

#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <bari.h>

#include "glf.h"
#include "keyboard.h"

// camera postitions
bari_vec3 camera_pos, camera_front, camera_up;

// smooth out movement
GLfloat delta_time = 0.0f;
GLfloat last_frame = 0.0f;

// mouse location
GLfloat lastx = 400, lasty = 300;

// our current pitch and yaw
GLfloat pitch = 0.0f, yaw = 0.0f;

/**
 * camera
 *
 * this function does camera calculations that must be done every frame. It
 * should be called in the main loop.
 */
void camera ()
{
    GLfloat current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;
}

/**
 * camera_mouse_callback
 *
 * the function to register with GLFW as the mouse callback. It calculates
 * pitch and yaw based on the mouse location
 *
 * arguments are specified by GLFW. See their documentation for details
 */
void camera_mouse_callback (GLFWwindow *window, double xpos, double ypos)
{
    GLfloat xoffset = xpos - lastx;
    GLfloat yoffset = lasty - ypos;
    lastx = xpos;
    lasty = ypos;

    GLfloat sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    } else if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    bari_vec3 front;
    front.x = cos(DEG_TO_RAD(yaw)) * cos(DEG_TO_RAD(pitch));
    front.y = sin(DEG_TO_RAD(pitch));
    front.z = sin(DEG_TO_RAD(yaw)) * cos(DEG_TO_RAD(pitch));
    camera_front = bari_norm3(front);
}

/**
 * camera_move_fore
 *
 * move the camera position forward in space. Registered as the callback for W
 * with the keyboard subsystem
 *
 * arguments
 *  GLFWwindow *window - this is not used
 */
void camera_move_fore (GLFWwindow *window)
{
    GLfloat camera_speed = 5.0f * delta_time;
    camera_pos = bari_vsum3(camera_pos, bari_vscale3(camera_front, camera_speed));
}

/**
 * camera_move_back
 *
 * move the camera position backward in space. Registered as the callback for S
 * with the keyboard subsystem
 *
 * arguments
 *  GLFWwindow *window - this is not used
 */
void camera_move_back (GLFWwindow *window)
{
    GLfloat camera_speed = 5.0f * delta_time;
    camera_pos = bari_vsub3(camera_pos, bari_vscale3(camera_front, camera_speed));
}

/**
 * camera_move_left
 *
 * move the camera position left in space. Registered as the callback for A
 * with the keyboard subsystem
 *
 * arguments
 *  GLFWwindow *window - this is not used
 */
void camera_move_left (GLFWwindow *window)
{
    GLfloat camera_speed = 5.0f * delta_time;
    camera_pos = bari_vsub3(camera_pos, bari_vscale3(bari_norm3(bari_cross(camera_front, camera_up)), camera_speed));
}

/**
 * camera_move_right
 *
 * move the camera position right in space. Registered as the callback for D
 * with the keyboard subsystem
 *
 * arguments
 *  GLFWwindow *window - this is not used
 */
void camera_move_right (GLFWwindow *window)
{
    GLfloat camera_speed = 5.0f * delta_time;
    camera_pos = bari_vsum3(camera_pos, bari_vscale3(bari_norm3(bari_cross(camera_front, camera_up)), camera_speed));
}

/**
 * camera_view
 *
 * get the view matrix that transforms world coordinates into view coordinates
 * using the current details of the camera position and direction
 *
 * returns
 *  the view matrix
 */
bari_mat4 camera_view ()
{
    return bari_lookat(camera_pos, bari_vsum3(camera_pos, camera_front), camera_up);
}

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
void camera_init (GLFWwindow *window)
{
    // set initial camera location
    camera_pos   = bari_mkvec3(0.0f, 0.0f, 3.0f);
    camera_front = bari_mkvec3(0.0f, 0.0f,-1.0f);
    camera_up    = bari_mkvec3(0.0f, 1.0f, 0.0f);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, camera_mouse_callback);

    // register camera movement keybindings
    keyboard_add_keybinding(GLFW_KEY_W, camera_move_fore);
    keyboard_add_keybinding(GLFW_KEY_S, camera_move_back);
    keyboard_add_keybinding(GLFW_KEY_A, camera_move_left);
    keyboard_add_keybinding(GLFW_KEY_D, camera_move_right);
}
