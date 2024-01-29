#include "BezierSurface.h"
#include "opengl_abstractions/vertex_buffer/vertex_buffer.h"
#include "opengl_abstractions/vertex_array/vertex_array.h"
#include "opengl_abstractions/vertex_buffer_layout/vertex_buffer_layout.h"
#include "rendered_item/RenderedItem.h"

BezierSurface::BezierSurface(int triangulization_level, Shader& phongShader): triangulization_level(triangulization_level),
	phongShader(phongShader)
{
	triangles = std::vector<Triangle>();
	InitializeControlPoints();
	bernstein_calculator = Bernstein();

	for (int i = 0; i < triangulization_level; i++)
	{
		for (int j = 0; j < triangulization_level; j++)
		{ 
			auto upperTriangle = GetUpperRightTriangleAt(i, j);
			auto lowerTriangle = GetLowerRightTriangleAt(i, j);

			triangles.push_back(upperTriangle);
			triangles.push_back(lowerTriangle);
		}
	}
}

void BezierSurface::InitializeControlPoints()
{
	for (int i = 0; i < control_points.size(); i++)
	{
		for (int j = 0; j < control_points[i].size(); j++)
		{
			control_points[i][j] = isInTheMiddleOf4by4(i, j) ? 
				default_low_control_point_value : default_high_control_point_value;
		}
	}
}

void BezierSurface::ChangeControlPointTo(float value, int i, int j)
{
	control_points[i][j] = value;
}

bool BezierSurface::isInTheMiddleOf4by4(int i, int j)
{
	return ((i == 1 || i == 2) && (j == 1 || j == 2));
}

Triangle BezierSurface::GetUpperRightTriangleAt(int i, int j)
{
	float upper_x = (i * 2.0f) / (float)triangulization_level - 1.0f;
	float upper_y = 1.0f - (j * 2.0f) / (float)triangulization_level;
	float step = 2.0f/ (float)triangulization_level;

	auto v1 = glm::vec3(upper_x, upper_y, ZValueAt(upper_x, upper_y));
	auto v2 = glm::vec3(upper_x + step, upper_y, ZValueAt(upper_x + step, upper_y));
	auto v3 = glm::vec3(upper_x, upper_y-step, ZValueAt(upper_x, upper_y - step));

	return Triangle(v1, v2, v3);
}

Triangle BezierSurface::GetLowerRightTriangleAt(int i, int j)
{
	float lower_x = ((i + 1) * 2.0f) / (float)triangulization_level - 1.0f;
	float lower_y = 1.0f - ((j + 1) * 2.0f) / (float)triangulization_level;
	float step = 2.0f / (float)triangulization_level;

	auto v1 = glm::vec3(lower_x, lower_y, ZValueAt(lower_x, lower_y));
	auto v2 = glm::vec3(lower_x - step, lower_y, ZValueAt(lower_x - step, lower_y));
	auto v3 = glm::vec3(lower_x, lower_y + step, ZValueAt(lower_x, lower_y + step));
	
	return Triangle(v1, v2, v3);
}

float BezierSurface::ZValueAt(float x, float y)
{
	float z = 0.0f;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			float t_x = (x + 1.0f) / 2.0f;
			float t_y = (y + 1.0f) / 2.0f;
			z += control_points[i][j] * bernstein_calculator.Value(t_x, i, 3) * bernstein_calculator.Value(t_y, j, 3);
		}
	}

	return z;
}

glm::vec3 BezierSurface::NormalToPointAt(float u, float v)
{
	int n = 3;
	int m = 3;
	auto Pu = glm::vec3(0, 0, 0);
	for (int i = 0; i <= n - 1; i++)
	{
		for (int j = 0; j <= m; j++)
		{
			auto zVect1 = glm::vec3((i + 1) / (float)3, j / (float)3, control_points[i + 1][j]);
			auto zVect2 = glm::vec3(i / (float)3, j / (float)3, control_points[i][ j]);

			Pu += (zVect1 - zVect2) * bernstein_calculator.Value(u, i, n - 1) * bernstein_calculator.Value(v, j, m);
		}
	}
	Pu *= n;

	auto Pv = glm::vec3(0, 0, 0);
	for (int i = 0; i <= n; i++)
	{
		for (int j = 0; j <= m - 1; j++)
		{
			auto zVect1 = glm::vec3(i / (float)3, (j + 1) / (float)3, control_points[i][ j + 1]);
			auto zVect2 = glm::vec3(i / (float)3, j / (float)3, control_points[i][ j]);

			Pv += (zVect1 - zVect2) * bernstein_calculator.Value(u, i, n) * bernstein_calculator.Value(v, j, m - 1);
		}
	}
	Pv *= m;

	auto N = glm::cross(Pu, Pv);
	return N;
}


void BezierSurface::render(ShaderType shading_type, const std::vector<PointLight> point_lights,
	const DirectionalLight dir_light, const std::vector<SpotLight> spot_lights,
	Camera& camera, glm::vec3 fogColor)
{
	for (const auto& triangle : triangles)
	{ 
		for (int i = 0; i < 3; i++)
		{
			vertices.push_back(triangle.vertices.at(i));
			normals.push_back(triangle.vertex_normals.at(i));
			colors.push_back(triangle.vertex_colors.at(i));
		}
	}

	auto vertices_vbo = vertex_buffer(&vertices[0], vertices.size()*sizeof(float));
	auto normals_vbo = vertex_buffer(&normals[0], normals.size() * sizeof(float));
	auto colors_vbo = vertex_buffer(&colors[0], colors.size() * sizeof(float));

	auto vertices_layout = vertex_buffer_layout();
	vertices_layout.push<float>(3);

	auto normals_layout = vertex_buffer_layout();
	normals_layout.push<float>(3);

	auto colors_layout = vertex_buffer_layout();
	colors_layout.push<float>(3);

	auto vao = vertex_array();
	vao.add_buffer_in_new_vbo(vertices_vbo, vertices_layout);
	vao.add_buffer_in_new_vbo(normals_vbo, normals_layout);
	vao.add_buffer_in_new_vbo(colors_vbo, colors_layout);

	auto model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-10.0f, -1.0f, -10.0f));
	model = glm::scale(model, glm::vec3(10.0f, 30.0f, 20.0f)); //uniform scaling x10 for visibility
	auto view = camera.GetViewMatrix();
	auto projection = glm::perspective(glm::radians(camera.Zoom), (float)1200 / (float)900, 0.1f, 100.0f);

	auto shader = phongShader; // TODO change this later to any shader type chosen in rendering
	shader.use();
	
	RenderedItem::setUpPointLightsWithShader(phongShader, point_lights);
	RenderedItem::setUpCameraWithShader(phongShader, camera);
	RenderedItem::setUpModelWithShader(phongShader, model);
	RenderedItem::setUpFog(phongShader, fogColor);
	RenderedItem::setUpSpotLights(phongShader, spot_lights);
	RenderedItem::setUpDirectionalLight(phongShader, dir_light);

	vao.bind();
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

