#ifndef RENDERED_ITEM_H
#define RENDERED_ITEM_H

#include "models_utilities/Model/Model.h"
#include "shaders/Shader.h"
#include "shaders/ShaderType.h"
#include "light/PointLight.h"
#include "light/SpotLight.h"
#include "light/DirectionaLight.h"
#include "camera/camera.h"
#include "GLM/glm.hpp"

class RenderedItem
{
public:
	RenderedItem(Model& model, Shader& constantShader, Shader& gourardShader, Shader& phongShader): 
		model(model), constantShader(constantShader),
		gourardShader(gourardShader), phongShader(phongShader) {}

	void render(ShaderType shading_type,
		const std::vector<PointLight> point_lights,
		const std::vector<DirectionalLight> dir_lights,
		const std::vector<SpotLight> spot_lights,
		Camera& camera)
	{
		phongShader.use();
		setUpPointLightsWithShader(phongShader, point_lights); 
		setUpCameraWithShader(phongShader, camera);
		setUpModelWithShader(phongShader, model.model_matrix);

		model.Draw(phongShader);
	}

	void moveBy(glm::vec3 vector)
	{
		model.model_matrix = glm::translate(model.model_matrix, vector);
		//model.attached_camera.Position += glm::vec3(model.model_matrix* glm::vec4(vector,0.0f));
	}

	void rotateByDegreesAroundY(float degrees)
	{
		model.model_matrix = glm::rotate(model.model_matrix, glm::radians(degrees), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	void setUpPointLightsWithShader(Shader& shader, const std::vector<PointLight> point_lights)
	{
		for (int i = 0; i < point_lights.size(); i++)
		{
			std::string pointsArrayNameInShader = "pointLights[" + std::to_string(i) + "]";
			shader.setVec3(pointsArrayNameInShader + ".position", point_lights[i].position);
			shader.setVec3(pointsArrayNameInShader + ".ambient", point_lights[i].ambient);
			shader.setVec3(pointsArrayNameInShader + ".diffuse", point_lights[i].diffuse);
			shader.setVec3(pointsArrayNameInShader + ".specular", point_lights[i].diffuse);
			shader.setFloat(pointsArrayNameInShader + ".constant", point_lights[i].constant);
			shader.setFloat(pointsArrayNameInShader + ".linear", point_lights[i].linear);
			shader.setFloat(pointsArrayNameInShader + ".quadratic", point_lights[i].quadratic);
		}
	}
	
	void setUpModelWithShader(Shader& shader, glm::mat4 model_matrix)
	{
		shader.set4Matrix("model", model_matrix);
	}

	void setUpCameraWithShader(Shader& shader, Camera& camera)
	{
		auto view = camera.GetViewMatrix();
		shader.set4Matrix("view", view);

		auto projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
		shader.set4Matrix("projection", projection);

		shader.setVec3("viewPos", camera.Position);
	}
private:
	Model& model;
	Shader& constantShader;
	Shader& gourardShader;
	Shader& phongShader;
};

#endif // !RENDERED_ITEM_H

