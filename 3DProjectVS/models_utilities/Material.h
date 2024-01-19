#ifndef MATERIAL_H
#define MATERIAL_H

#include "GLM/glm.hpp"

struct Material {
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	glm::vec3 Ambient;
	float Shininess;
};

#endif // !1
