#ifndef GLFW_UTILITIES_H
#define GLFW_UTILITIES_H

#include <GLFW/glfw3.h>

void initializeGLFW();
GLFWwindow* createGLFWWindow(const int width, const int height, const char* window_title);
void setWindowHints();
void setWindowCallbacks(GLFWwindow* window);

#endif
