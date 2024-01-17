#ifndef SCENE_H
#define SCENE_H

#include "light/PointLight.h"
#include "light/DirectionaLight.h"
#include "light/SpotLight.h"
#include "rendered_item/RenderedItem.h"
#include "shaders/ShaderType.h"
#include "camera/camera.h"

class Scene
{
public:
	Scene(std::vector<RenderedItem> items,
		std::vector<PointLight> point_lights): items(items), point_lights(point_lights){}

	void addModel(RenderedItem& item)
	{
		items.push_back(item);
	}
	void addPointLight(PointLight point_light)
	{
		point_lights.push_back(point_light);
	}

	void render(ShaderType shading_type, Camera camera)
	{
		for (auto item: items)
		{
			//item.rotateByDegreesAroundY(glfwGetTime() * 0.01f);
			//item.moveBy(glm::vec3(-glfwGetTime() * 0.001f, 0.0f, 0.0f));
			item.render(shading_type, point_lights, dir_lights, spot_lights, camera);
		}
	}

private:
	std::vector<RenderedItem> items;
	std::vector<PointLight> point_lights;
	std::vector<DirectionalLight> dir_lights;
	std::vector<SpotLight> spot_lights;
};


#endif //!SCENE_H