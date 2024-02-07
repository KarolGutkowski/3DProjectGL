#ifndef POLICE_CAR_H
#define POLICE_CAR_H

#include "models_utilities/Model/Model.h"
#include <glm/gtx/rotate_vector.hpp>

class PoliceCar : public Model
{
public:
	PoliceCar(const char* path, bool _flippedTexture = false)
	{
		current_translation = glm::vec3(0.0f);
		current_scaling = glm::vec3(1.0f);
		rotation_degrees = glm::vec3(0.0f);

		flippedTexture = _flippedTexture;
		loadModel(path);

		for(int i=0;i< meshes.size(); i++)
		{
			for (int j=0; j < meshes[i].vertices.size(); j++)
			{
				//fixing some dumb modelling
				meshes[i].vertices[j].Position.z += 10;
			}
			meshes[i].updateMesh();
		}

		has_camera_attached_to_it = true;
		attached_camera = Camera();
		attached_camera.Yaw = -90.0f;
		attached_camera.Pitch = -23.0f;

		has_lights = true;
		generateFrontLights();
	}

	void Draw(Shader& shader) override
	{
		updateLights();
		for (const auto& mesh : meshes)
			mesh.Draw(shader);
	}

	glm::vec3 camera_relative_position = glm::vec3(1.0f, 4.5f, 9.0f);

	void Translate(glm::vec3 translation_vector) override
	{
		current_translation += translation_vector;
		attached_camera.Position = current_translation + glm::rotateY(camera_relative_position, glm::radians(rotation_degrees.y+90.0f));
		updateLights();
	}

	void Scale(glm::vec3 scaling_vector) override
	{
		current_scaling *= scaling_vector;
	}

	bool initial_rotation = true;
	void Rotate(glm::vec3 rotation_vector) override
	{ 
		rotation_degrees += rotation_vector;
		if (!initial_rotation)
		{
			attached_camera.Position = current_translation + glm::rotateY(camera_relative_position, glm::radians(rotation_degrees.y + 90.0f));
			attached_camera.Yaw -= rotation_vector.y;

			updateLights();
		}
		initial_rotation = false;
	}

	std::vector<SpotLight>& getModelSpotLights() override 
	{
		return car_lights;
	}
		
	std::vector<glm::vec3>& getLightDirections() override
	{
		return initial_light_directions;
	}

private:

	void updateLights()
	{
		for (int i = 0; i < car_lights.size(); i++)
		{
			auto postion_rotation = glm::rotateY(car_lights_relative_positions[i], glm::radians(rotation_degrees.y + 90.0f));
			car_lights[i].position = current_translation + postion_rotation;

			auto direction_rotation = glm::rotateY(initial_light_directions[i], glm::radians(rotation_degrees.y + 90.0f));
			car_lights[i].direction = direction_rotation;
		}
	}
	std::vector<SpotLight> car_lights;
	std::vector<glm::vec3> car_lights_relative_positions;
	std::vector<glm::vec3> initial_light_directions;
	/*std::vector<SpotLight> back_lights;
	std::vector<glm::vec3> back_lights_relative_positions;*/

	void generateFrontLights()
	{
		car_lights_relative_positions.push_back(glm::vec3(0.885f, 0.855f, -1.25f));
		car_lights_relative_positions.push_back(glm::vec3(1.85f, 0.855f, -1.25f));
		car_lights_relative_positions.push_back(glm::vec3(0.885f, 0.855f, 2.05f));
		car_lights_relative_positions.push_back(glm::vec3(1.85f, 0.855f, 2.05f));

		auto spotLightLeft = SpotLight();
		spotLightLeft.position = car_lights_relative_positions[0];
		spotLightLeft.cutOffClose = glm::cos(glm::radians(20.5f));
		spotLightLeft.cutOffFar = glm::cos(glm::radians(30.5f));
		spotLightLeft.direction = glm::vec3(0.0f, -0.2f, -1.0f); //for now
		spotLightLeft.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
		spotLightLeft.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
		spotLightLeft.specular = glm::vec3(1.0f, 1.0f, 1.0f);
		spotLightLeft.constant = 1.0f;
		spotLightLeft.linear = 0.09f;
		spotLightLeft.quadratic = 0.032f; 

		auto spotLightRight = SpotLight();
		spotLightRight.position = car_lights_relative_positions[1];
		spotLightRight.cutOffClose = glm::cos(glm::radians(20.5f));
		spotLightRight.cutOffFar = glm::cos(glm::radians(30.5f));
		spotLightRight.direction = glm::vec3(0.0f, -0.2f, -1.0f); //for now
		spotLightRight.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
		spotLightRight.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
		spotLightRight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
		spotLightRight.constant = 1.0f;
		spotLightRight.linear = 0.09f;
		spotLightRight.quadratic = 0.032f;


		auto spotLightLeftBack = SpotLight();
		spotLightLeftBack.position = car_lights_relative_positions[2];
		spotLightLeftBack.cutOffClose = glm::cos(glm::radians(20.5f));
		spotLightLeftBack.cutOffFar = glm::cos(glm::radians(30.5f));
		spotLightLeftBack.direction = glm::vec3(0.0f, -0.8f, 1.0f); //for now
		spotLightLeftBack.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
		spotLightLeftBack.diffuse = glm::vec3(0.8f, 0.0f, 0.0f);
		spotLightLeftBack.specular = glm::vec3(1.0f, 1.0f, 1.0f);
		spotLightLeftBack.constant = 1.0f;
		spotLightLeftBack.linear = 0.09f;
		spotLightLeftBack.quadratic = 0.032f;

		auto spotLightRightBack = SpotLight();
		spotLightRightBack.position = car_lights_relative_positions[3];
		spotLightRightBack.cutOffClose = glm::cos(glm::radians(20.5f));
		spotLightRightBack.cutOffFar = glm::cos(glm::radians(30.5f));
		spotLightRightBack.direction = glm::vec3(0.0f, -0.8f, 1.0f); //for now
		spotLightRightBack.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
		spotLightRightBack.diffuse = glm::vec3(0.8f, 0.0f, 0.0f);
		spotLightRightBack.specular = glm::vec3(1.0f, 1.0f, 1.0f);
		spotLightRightBack.constant = 1.0f;
		spotLightRightBack.linear = 0.09f;
		spotLightRightBack.quadratic = 0.032f;

		car_lights.push_back(spotLightLeft);
		car_lights.push_back(spotLightRight);
		car_lights.push_back(spotLightLeftBack);
		car_lights.push_back(spotLightRightBack);

		for (int i = 0; i < car_lights.size(); i++)
		{
			initial_light_directions.push_back(car_lights[i].direction);
		}
	}

};

#endif // !POLICE_CAR_H