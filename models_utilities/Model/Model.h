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
#include "light/SpotLight.h"

#define AI_SCENE_FLAGS_INCOMPLETE 0x1

class Model {
public:
    virtual void Draw(Shader& shader) const = 0;

    virtual void Rotate(glm::vec3 rotation_vector) = 0;
    virtual void Scale(glm::vec3 scaling_vector) = 0;
    virtual void Translate(glm::vec3 translation_vector) = 0;
    bool has_lights = false;
    virtual std::vector<SpotLight>& getModelSpotLights() = 0;

    glm::mat4 model_matrix = glm::mat4(1.0f);
    bool has_camera_attached_to_it = false;
    Camera attached_camera;

    glm::mat4 get_model_matrix()
    {
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, current_translation);
        model = glm::scale(model, current_scaling);
        model = glm::rotate(model, glm::radians(rotation_degrees.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation_degrees.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation_degrees.z), glm::vec3(0.0f, 0.0f, 1.0f));

        return model;
    }
protected:
    glm::vec3 current_translation;
    glm::vec3 rotation_degrees;
    glm::vec3 current_scaling;

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
