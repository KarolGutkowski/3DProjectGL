#ifndef MODEL_H
#define MODEL_H

#include "shaders/Shader.h"
#include <vector>
#include "models_utilities/Mesh/Mesh.h"
#include "models_utilities/Texture.h"
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <utilities/stb_image/stb_image.h>
#include "camera/camera.h"
#include "models_utilities/Material.h"

#define AI_SCENE_FLAGS_INCOMPLETE 0x1

class Model {
public:
   /* Model(const char* path, bool _flippedTexture = false);*/

    virtual void Draw(Shader& shader) const = 0;
    glm::mat4 model_matrix = glm::mat4(1.0f);
    bool has_camera_attached_to_it = false;
    Camera attached_camera;
protected:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;
    bool flippedTexture;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);
};

#endif
