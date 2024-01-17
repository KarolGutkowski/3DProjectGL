#include "GLFW/glfw3.h"
#include "utilities/report_error/report_error.h"
#include "glfw_utilities.h"
#include "glfw_callbacks.h"

void initializeGLFW()
{
    if (!glfwInit())
        err("failed to initialize glfw");
}

GLFWwindow* createGLFWWindow(const int width, const int height, const char* window_title) {
    setWindowHints();

    auto window = glfwCreateWindow(width, height, window_title, NULL, NULL);
    if (!window) {
        glfwTerminate();
        err("failed to create glfw window");
    }
    return window;
}

void setWindowCallbacks(GLFWwindow* window) {
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, keyCallback);
    /*glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);*/
}

void setWindowHints() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
#ifdef __APPLE__
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //this specifically seems important
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
}
