#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "models_utilities/BezierSurface/TriangleVertex.h"
#include "GLM/glm.hpp"
#include <array>

class Triangle
{
public:
	Triangle(TriangleVertex A, TriangleVertex B, TriangleVertex C): vertices({A,B,C}) {}
private:
	std::array<TriangleVertex,3> vertices;
	//TriangleVertex GetVertexAtWithColor(float i, float j, glm::vec3 color);
};
#endif // !TRIANGLE_H
