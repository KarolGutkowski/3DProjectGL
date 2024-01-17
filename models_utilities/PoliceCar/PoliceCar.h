#ifndef POLICE_CAR_H
#define POLICE_CAR_H

#include "models_utilities/Model/Model.h"

class PoliceCar : public Model
{
public:
	PoliceCar(const char* path, bool _flippedTexture = false)
	{
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
		attached_camera.Yaw = 90.0f;
		attached_camera.Pitch = -30.0f;
	}

	void Draw(Shader& shader) const override
	{
		for (const auto& mesh : meshes)
			mesh.Draw(shader);
	}

};

#endif // !POLICE_CAR_H