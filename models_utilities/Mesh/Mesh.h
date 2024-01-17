#ifndef MESH_H
#define MESH_H

#include "models_utilities/Vertex.h"
#include "models_utilities/Texture.h"
#include <vector>
#include "shaders/Shader.h"

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(Shader& shader) const;
    void updateMesh();
private:
    unsigned int VAO, VBO, EBO;
    void setupMesh();
};

#endif
