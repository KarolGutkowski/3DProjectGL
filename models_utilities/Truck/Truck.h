#ifndef TRUCK_H
#define TRUCK_H

#include "models_utilities/Model/Model.h"

class Truck : public Model
{
public:
	Truck(const char* path, bool _flippedTexture = false)
	{
		flippedTexture = _flippedTexture;
		loadModel(path);
	}

	void Draw(Shader& shader) const override
	{
		for (const auto& mesh : meshes)
			mesh.Draw(shader);
	}
};

#endif // !TRUCK_H