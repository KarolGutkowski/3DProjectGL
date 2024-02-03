#ifndef BEZIER_SURFACE_H
#define BEZIER_SURFACE_H

#include <array>
#include <vector>
#include "models_utilities/BezierSurface/Triangle.h"
#include "models_utilities/BezierSurface/Bernstein.h"
#include "shaders/ShaderType.h"
#include "light/PointLight.h"
#include "light/DirectionaLight.h"
#include "light/SpotLight.h"
#include "camera/camera.h"
#include "GLM/glm.hpp"
#include "shaders/Shader.h"

class BezierSurface
{
public:
	BezierSurface(int triangulization_level, Shader& phongShader);
	void render(ShaderType shading_type,
		const std::vector<PointLight> point_lights,
		const DirectionalLight dir_light,
		const std::vector<SpotLight> spot_lights,
		Camera& camera,
		glm::vec3 fogColor);
	void ChangeControlPointTo(float value, int i, int j);
	void controlPointsHaveChanged()
	{
		recalculate_bezier = true;
	}
	std::array<std::array<float, 4>, 4> control_points;
private:
	void generateTriangles();
	void InitializeControlPoints();
	Triangle GetUpperRightTriangleAt(int i, int j);
	Triangle GetLowerRightTriangleAt(int i, int j);
	float ZValueAt(float x, float y);
	bool isInTheMiddleOf4by4(int i, int j);

	int triangulization_level;
	std::vector<Triangle> triangles;
	const float default_low_control_point_value = 1.0f;
	const float default_high_control_point_value = 0.2f;
	Bernstein bernstein_calculator;
	glm::vec3 NormalToPointAt(float u, float v);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> colors;
	bool recalculate_bezier;
	/*Shader& constantShader;
	Shader& gourardShader;*/
	Shader& phongShader;
};




#endif // !
