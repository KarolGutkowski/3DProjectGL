#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <iostream>

class renderer {
    void clearErrors();
    void logErrors(const char* function, const char* file, int line);
};

#endif
