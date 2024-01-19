#ifndef LINCOLN_H
#define LINCOLN_H

#include "models_utilities/Model/Model.h"

class Lincoln : public Model
{
public:
	Lincoln(const char* path, bool _flippedTexture = false)
	{
		flippedTexture = _flippedTexture;
		loadModel(path);
	}

	void Draw(Shader& shader) const override
	{
		for (int i = 0; i < meshes.size(); i++)
		{
			if (i !=7 && i!=8 && i!=9)
			{
				meshes[i].Draw(shader);
			}
		}
	}
};

#endif // !LINCOLN_H
