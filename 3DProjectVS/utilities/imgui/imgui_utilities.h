#ifndef IMGUI_UTILITIES_H
#define IMGUI_UTILITIES_H

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
//#include "glfw/glfw3.h"
#include "camera/camera.h"
#include "light/DirectionaLight.h"
#include "light/SpotLight.h"
#include <vector>
#include "models_utilities/BezierSurface/BezierSurface.h"

void initializeImGui(GLFWwindow* window);
void destroyImGuiContext();
void generateImGuiWindow(Camera& camera, Camera& camera2, glm::vec3& fog_color, DirectionalLight& dir_light, std::vector<SpotLight>& police_car_lights, BezierSurface& bezier, Camera_Option& chosen_camera);
void ImGuiNewFrame();
void renderImGui();

#endif
