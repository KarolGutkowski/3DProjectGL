#ifndef DE_LOREAN_H
#define DE_LOREAN_H

class DeLorean : public Model
{
public:
	DeLorean(const char* path, bool _flippedTexture = false)
	{
		flippedTexture = _flippedTexture;
		loadModel(path);
	}

	void Draw(Shader& shader) const override
	{
		for (int i = 0; i < meshes.size(); i++)
		{
			if (i < 15)
			{
				meshes[i].Draw(shader);
			}
		}
	}
};

#endif // !DE_LOREAN_H
