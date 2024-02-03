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
		const DirectionalLight dir_light,
		const std::vector<SpotLight> spot_lights,
		Camera& camera,
		glm::vec3 fogColor)
	{
		phongShader.use();
		setUpPointLightsWithShader(phongShader, point_lights); 
		setUpCameraWithShader(phongShader, camera);
		setUpModelWithShader(phongShader, model.get_model_matrix());
		setUpFog(phongShader, fogColor);
		setUpSpotLights(phongShader, spot_lights);
		setUpDirectionalLight(phongShader, dir_light);

		model.Draw(phongShader);
	}

	void moveBy(glm::vec3 vector)
	{
		model.Translate(vector);
	}

	glm::vec3 get_current_position()
	{
		return model.current_translation;
	}

	void rotateByDegreesAroundY(float degrees)
	{
		model.Rotate(glm::vec3(0.0f, degrees, 0.0f));
	}

	static void setUpFog(Shader& shader, glm::vec3 fogColor)
	{
		shader.setVec3("fogColor", fogColor);
	}

	static void setUpSpotLights(Shader& shader, const std::vector<SpotLight> spot_lights)
	{
		for (int i = 0; i < spot_lights.size(); i++)
		{
			std::string spotLightsNameInShader = "spotLights[" + std::to_string(i) + "]";
			shader.setVec3(spotLightsNameInShader + ".position", spot_lights[i].position);
			shader.setVec3(spotLightsNameInShader + ".ambient", spot_lights[i].ambient);
			shader.setVec3(spotLightsNameInShader + ".diffuse", spot_lights[i].diffuse);
			shader.setVec3(spotLightsNameInShader + ".specular", spot_lights[i].specular);
			shader.setVec3(spotLightsNameInShader + ".direction", spot_lights[i].direction);
			shader.setFloat(spotLightsNameInShader + ".constant", spot_lights[i].constant);
			shader.setFloat(spotLightsNameInShader + ".linear", spot_lights[i].linear);
			shader.setFloat(spotLightsNameInShader + ".quadratic", spot_lights[i].quadratic);
			shader.setFloat(spotLightsNameInShader + ".cutOffClose", spot_lights[i].cutOffClose);
			shader.setFloat(spotLightsNameInShader + ".cutOffFar", spot_lights[i].cutOffFar);
		}
	}

	static void setUpDirectionalLight(Shader& shader, const DirectionalLight dir_light)
	{
		shader.setVec3("directionalLight.direction", dir_light.direction);
		shader.setVec3("directionalLight.ambient", dir_light.ambient);
		shader.setVec3("directionalLight.diffuse", dir_light.diffuse);
		shader.setVec3("directionalLight.specular", dir_light.specular);
	}


	static void setUpPointLightsWithShader(Shader& shader, const std::vector<PointLight> point_lights)
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
	
	static void setUpModelWithShader(Shader& shader, glm::mat4 model_matrix)
	{
		shader.set4Matrix("model", model_matrix);
	}

	static void setUpCameraWithShader(Shader& shader, Camera& camera)
	{
		auto view = camera.GetViewMatrix();
		shader.set4Matrix("view", view);
		 
		auto projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
		shader.set4Matrix("projection", projection);

		shader.setVec3("viewPos", camera.Position);
	}

	std::vector<SpotLight>& getItemsLights()
	{
		return model.getModelSpotLights();
	}
private:
	Model& model;
	Shader& constantShader;
	Shader& gourardShader;
	Shader& phongShader;
};

#endif // !RENDERED_ITEM_H

