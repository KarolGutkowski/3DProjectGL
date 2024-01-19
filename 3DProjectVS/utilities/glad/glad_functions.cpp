#include "glad/glad.h"
#include "Includes/GLFW/glfw3.h"
#include "glad_functions.h"
#include <iostream>
#include "utilities/report_error/report_error.h"

void loadGlad()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        err("failed to load GL functions with glad");
}