#ifndef IMGUI_UTILITIES_H
#define IMGUI_UTILITIES_H

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "glfw/glfw3.h"
#include "camera/camera.h"

void initializeImGui(GLFWwindow* window);
void destroyImGuiContext();
void generateImGuiWindow(Camera& camera, Camera& camera2);
void ImGuiNewFrame();
void renderImGui();

#endif
