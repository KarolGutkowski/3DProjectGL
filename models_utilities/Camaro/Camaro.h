#ifndef CAMARO_H
#define CAMARO_H

#include "models_utilities/Model/Model.h"

class Camaro : public Model
{
public:
	Camaro(const char* path, bool _flippedTexture = false)
	{
		flippedTexture = _flippedTexture;
		loadModel(path);
	}

	void Draw(Shader& shader) const override
	{
		for (int i = 0; i < meshes.size(); i++)
		{
			meshes[i].Draw(shader);
		}
	}
};

#endif // !LINCOLN_H
