#ifndef GLFW_CALLBACKS_H
#define GLFW_CALLBACKS_H

#include "GLFW/glfw3.h"
#include "camera/camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void processInput(GLFWwindow* window, Camera& movable_camera);

#endif