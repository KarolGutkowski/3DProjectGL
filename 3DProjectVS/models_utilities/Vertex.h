#ifndef VERTEX_H
#define VERTEX_H

#include "GLM/glm.hpp"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

#endif