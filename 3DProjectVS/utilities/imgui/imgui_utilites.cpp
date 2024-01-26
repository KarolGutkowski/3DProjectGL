#include "imgui_utilities.h"

void initializeImGui(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void destroyImGuiContext() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void generateImGuiWindow(Camera& camera, Camera& camera2, glm::vec3& fog_color, 
    DirectionalLight& dir_light, std::vector<SpotLight>& police_car_lights) 
{
    ImGuiIO& io = ImGui::GetIO();
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Edit scene");

    if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None)) {
        if (ImGui::BeginTabItem("Movable camera")) {
            ImGui::SliderFloat("camera X", &camera.Position.x, -100.0f, 100.0f);
            ImGui::SliderFloat("camera Y", &camera.Position.y, -100.0f, 100.0f);
            ImGui::SliderFloat("camera Z", &camera.Position.z, -100.0f, 100.0f);

            ImGui::SliderFloat("yaw", &camera.Yaw, -180.0f, 180.0f);
            ImGui::SliderFloat("pitch", &camera.Pitch, -89.0f, 89.0f);
            ImGui::SliderFloat3("up", (float *) &camera.WorldUp, 0.0f, 1.0f);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Object camera"))
        {
            ImGui::SliderFloat("camera X", &camera2.Position.x, -100.0f, 100.0f);
            ImGui::SliderFloat("camera Y", &camera2.Position.y, -100.0f, 100.0f);
            ImGui::SliderFloat("camera Z", &camera2.Position.z, -100.0f, 100.0f);

            ImGui::SliderFloat("yaw", &camera2.Yaw, -180.0f, 180.0f);
            ImGui::SliderFloat("pitch", &camera2.Pitch, -89.0f, 89.0f);
            ImGui::SliderFloat3("up", (float *) &camera2.WorldUp, 0.0f, 1.0f);
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Car lights"))
        {
            ImGui::SliderFloat3("front left direction", &police_car_lights[0].direction.x, -1.0f, 1.0f);
            ImGui::SliderFloat3("front right direction", &police_car_lights[1].direction.x, -1.0f, 1.0f);
            ImGui::SliderFloat3("back left direction", &police_car_lights[2].direction.x, -1.0f, 1.0f);
            ImGui::SliderFloat3("back right direction", &police_car_lights[3].direction.x, -1.0f, 1.0f);

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Scenery"))
        {
            ImGui::SliderFloat3("surronding color", &fog_color.x, 0.0f, 1.0f);

            ImGui::SliderFloat3("directional light color", &dir_light.diffuse.x, 0.0f, 1.0f);

            if (ImGui::Button("Day"))
            {
                fog_color = glm::vec3(163 / 255.0f, 234 / 255.0f, 255 / 255.0f);
                dir_light.diffuse = glm::vec3(1.0f);
            }

            if (ImGui::Button("Night"))
            {
                fog_color = glm::vec3(0.1f, 0.1f, 0.1f);
                dir_light.diffuse = glm::vec3(0.0f);
            }

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    if (ImGui::Button("Button"))
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();

}

void ImGuiNewFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void renderImGui() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}