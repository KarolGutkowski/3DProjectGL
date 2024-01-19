#include "renderer.h"

void renderer::clearErrors() {
    while(glGetError() != GL_NO_ERROR);
}

void renderer::logErrors(const char* function, const char* file, int line) {
    while(auto error = glGetError()) {
        std::cout << "[OpenGL ERROR](" <<  error << "): at function call:" << function
        << " file:" << file << " line:" << line << std::endl;
    }
}

