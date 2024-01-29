#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "GLM/glm.hpp"
#include <array>

class Triangle
{
public:
	Triangle(glm::vec3 A, glm::vec3 B, glm::vec3 C): vertices({A,B,C})
	{
		glm::vec3 color_grey = glm::vec3(0.2f); // for testing i want them to be somewhat visible
		vertex_colors = { color_grey , color_grey , color_grey };
		
		glm::vec3 edge1 = B - A;
		glm::vec3 edge2 = C - A;
		glm::vec3 default_normal_value = glm::cross(edge1, edge2);
		vertex_normals = { default_normal_value , default_normal_value , default_normal_value };
	}
	std::array<glm::vec3, 3> vertex_colors;
	std::array<glm::vec3, 3> vertex_normals;
	std::array<glm::vec3, 3> vertices;
};
#endif // !TRIANGLE_H
