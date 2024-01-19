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
		std::vector<PointLight>& point_lights,
		/*std::vector<SpotLight>& spot_lights,*/
		glm::vec3 fog_color): 
		items(items), point_lights(point_lights), fogColor(fog_color), spot_lights(spot_lights) {}

	void addModel(RenderedItem& item)
	{
		items.push_back(item);
	}

	void addPointLight(PointLight point_light)
	{
		point_lights.push_back(point_light);
	}

	int current_rotation_sign = 1;
	void render(ShaderType shading_type, Camera camera)
	{
		items[0].moveBy(glm::vec3(0.0f, 0.0f, -0.2f));
		//items[0].rotateByDegreesAroundY(current_rotation_sign * 0.1f); 
		//current_rotation_sign *= -1;
		spot_lights = items[0].getItemsLights();
		items[0].render(shading_type, point_lights, /*dir_lights,*/ spot_lights, camera, fogColor);
	}

	std::vector<SpotLight> getSpotLights()
	{
		return items[0].getItemsLights();;
	}

private:
	glm::vec3 fogColor;
	std::vector<RenderedItem> items;
	std::vector<PointLight>& point_lights;
	/*std::vector<DirectionalLight>& dir_lights;*/
	std::vector<SpotLight> spot_lights;
};


#endif //!SCENE_H