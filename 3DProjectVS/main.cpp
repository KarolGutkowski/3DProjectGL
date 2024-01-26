#include "shaders/Shader.h"
#include "utilities/glfw/glfw_utilities.h"
#include "utilities/glad/glad_functions.h"
#include <GLFW/glfw3.h>
#include "state.h"
#include "utilities/glfw/glfw_callbacks.h"
#include "utilities/gl/gl_utilities.h"
#include "opengl_abstractions/vertex_buffer/vertex_buffer.h"
#include "opengl_abstractions/vertex_array/vertex_array.h"
#include "opengl_abstractions/vertex_buffer_layout/vertex_buffer_layout.h"
#include "models_utilities/Model/Model.h"
#include "utilities/imgui/imgui_utilities.h"
#include "models_utilities/PoliceCar/PoliceCar.h"
#include "scene/Scene.h"
#include "rendered_item/RenderedItem.h"
#include "light/PointLight.h"
#include "textures/texture_utils.h"
#include "models_utilities/BezierSurface/BezierSurface.h"

void clearBuffers();
std::vector<PointLight> generatePointLights(glm::vec3 pointLightPositions[]);
void renderLight(Shader shader, const vertex_array& vao, glm::vec3 position, Camera camera, glm::vec3 color, glm::vec3 fogColor);
void setUpLights(Shader& shader, std::vector<PointLight> pointLights);
void drawWithShader(Shader shader, glm::vec3 pointLightPositions[], const Model& element, glm::mat4 model);
void render_street_with_shader(Shader shader, const vertex_array& vao, std::vector<PointLight> pointLights, GLuint texture_base, GLuint texture_specular, GLuint texture_normal, int order_number, Camera curent_camera, glm::vec3 fogColor, std::vector<SpotLight> spotLights, DirectionalLight dir_light);

int main(void)
{
    initializeGLFW();
    auto window = createGLFWWindow(1200,900, "3D Graphics");
    glfwMakeContextCurrent(window);
    loadGlad();
    setWindowCallbacks(window);
    setGlOptions();
    initializeImGui(window);

    BezierSurface bezier = BezierSurface(1);

    float vertices[] = {
            // positions          // normals
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    float floor_vertices[] = {
        // positions          // normals            //texture
        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f, 1.0f, //top left
        0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f, 1.0f, // top right
        0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f, 0.0f, //bottom right
        0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f, 0.0f, //bottom right
        -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f, 0.0f, // bottom left
        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f, 1.0f //top left
    };

    auto vbo = vertex_buffer(vertices, sizeof(vertices));

    auto vao = vertex_array();
    auto layout = vertex_buffer_layout();
    layout.push<float>(3);
    layout.push<float>(3);

    vao.add_buffer(vbo, layout);
    vao.unbind();

    auto shader = Shader("./shaders/shader.glsl");
    auto light_shader = Shader("./shaders/light_source_shader.glsl");

    auto car = PoliceCar("./models/car/cars.obj", false);

    glm::vec3 pointLightPositions[] = {
            glm::vec3(0.0f,  0.5f,  1.0f),
            glm::vec3(3.0f, 2.0f, 3.0f),
            glm::vec3(3.0f,  2.0f, -8.0f),
            glm::vec3(-3.0f,  2.0f, -9.0f)
    };

    auto floor_vbo = vertex_buffer(floor_vertices, sizeof(floor_vertices));
    auto floor_vao = vertex_array();
    auto floor_layout = vertex_buffer_layout();
    floor_layout.push<float>(3);
    floor_layout.push<float>(3);
    floor_layout.push<float>(2);
    floor_vao.add_buffer(floor_vbo, floor_layout);
    floor_vao.unbind();
    GLuint floor_texture_base = loadTexture("./textures/road_texture/Road_001_basecolor.jpg");
    GLuint floor_texture_specular = loadTexture("./textures/road_texture/Road_001_roughness.jpg");
    GLuint floor_texture_normal = loadTexture("./textures/road_texture/Road_001_normal.jpg");

    auto floor_shader = Shader("./shaders/floor_shader.glsl");

    auto carShader = Shader("./shaders/police-car-shader.glsl");
    auto police_car_model = PoliceCar("./models/car/cars.obj");
    
    auto lights = generatePointLights(pointLightPositions);

    auto police_car = RenderedItem(car, carShader, carShader, carShader);
    std::vector<RenderedItem> items { police_car };

    auto fog_color = glm::vec3(163 / 255.0f, 234 / 255.0f, 255 / 255.0f);

    auto scene = Scene(items, lights, fog_color);

    camera.Position.y = 1.0f;
    camera.Position.x = 0.5f;
    camera.Position.z = 2.0f;

    car.attached_camera.Position = glm::vec3(1.0f, 4.5f, 6.0f);
    car.Rotate(glm::vec3(0.0f, -90.0f, 0.0f));
    //car.model_matrix = glm::translate(car.model_matrix, glm::vec3(0.0f, 0.0f, 11.0f));
    while (!glfwWindowShouldClose(window))  
    {
        Camera current_camera = car.attached_camera;
        processInput(window, camera);
        clearBuffers();

        ImGuiNewFrame();

        generateImGuiWindow(camera, current_camera, fog_color, scene.getDirLight(), police_car.getItemsLights());
        glClearColor(fog_color.x, fog_color.y, fog_color.z, 0.1f);


        for (int i = 0; i < lights.size(); i++)
        {
            lights[0].position.z -= 0.01f;
            renderLight(light_shader, vao, lights[i].position, current_camera, lights[i].diffuse, fog_color);
        }
        carShader = Shader("./shaders/police-car-shader.glsl");
        scene.render(ShaderType::Phong, current_camera);

        auto floor_shader = Shader("./shaders/floor_shader.glsl");
        for (int i = 0; i < 20; i++)
        {
            render_street_with_shader(floor_shader, floor_vao, lights, floor_texture_base, floor_texture_specular, floor_texture_normal,i, current_camera, fog_color, scene.getSpotLights(), scene.getDirLight());
        }
        renderImGui();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    destroyImGuiContext();

    glfwTerminate();
    return 0;
}


void drawWithShader(Shader shader, std::vector<PointLight> pointLights, const Model& element, glm::mat4 model) {
    // don't forget to enable shader before setting uniforms
    shader.use();
    setUpLights(shader, pointLights);
    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    shader.set4Matrix("projection", projection);
    shader.set4Matrix("view", view);

    // render the loaded model
    shader.set4Matrix("model", model);

    element.Draw(shader);
}

void setUpLights(Shader& shader, std::vector<PointLight> pointLights) {
    shader.setVec3("viewPos", camera.Position);

    shader.setVec3("pointLights[0].position", pointLights[0].position);
    shader.setVec3("pointLights[0].ambient", pointLights[0].ambient);
    shader.setVec3("pointLights[0].diffuse", pointLights[0].diffuse);
    shader.setVec3("pointLights[0].specular", pointLights[0].specular);
    shader.setFloat("pointLights[0].constant", pointLights[0].constant);
    shader.setFloat("pointLights[0].linear", pointLights[0].linear);
    shader.setFloat("pointLights[0].quadratic", pointLights[0].quadratic);

    shader.setVec3("pointLights[1].position", pointLights[1].position);
    shader.setVec3("pointLights[1].ambient", pointLights[1].ambient);
    shader.setVec3("pointLights[1].diffuse", pointLights[1].diffuse);
    shader.setVec3("pointLights[1].specular", pointLights[1].specular);
    shader.setFloat("pointLights[1].constant", pointLights[1].constant);
    shader.setFloat("pointLights[1].linear", pointLights[1].linear);
    shader.setFloat("pointLights[1].quadratic", pointLights[1].quadratic);

    shader.setVec3("pointLights[2].position", pointLights[2].position);
    shader.setVec3("pointLights[2].ambient", pointLights[2].ambient);
    shader.setVec3("pointLights[2].diffuse", pointLights[2].diffuse);
    shader.setVec3("pointLights[2].specular", pointLights[2].specular);
    shader.setFloat("pointLights[2].constant", pointLights[2].constant);
    shader.setFloat("pointLights[2].linear", pointLights[2].linear);
    shader.setFloat("pointLights[2].quadratic", pointLights[2].quadratic);

    shader.setVec3("pointLights[3].position", pointLights[3].position);
    shader.setVec3("pointLights[3].ambient", pointLights[3].ambient);
    shader.setVec3("pointLights[3].diffuse", pointLights[3].diffuse);
    shader.setVec3("pointLights[3].specular", pointLights[3].specular);
    shader.setFloat("pointLights[3].constant", pointLights[3].constant);
    shader.setFloat("pointLights[3].linear", pointLights[3].linear);
    shader.setFloat("pointLights[3].quadratic", pointLights[3].quadratic);
}

void renderLight(Shader shader, const vertex_array& vao, glm::vec3 position, Camera camera, glm::vec3 color, glm::vec3 fogColor) {
    shader.use();

    auto model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(0.5f,0.5f,0.5f));
    shader.set4Matrix("model", model);

    auto view = camera.GetViewMatrix();
    shader.set4Matrix("view", view);

    auto projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
    shader.set4Matrix("projection", projection);

    shader.setVec3("color", color);

    shader.setVec3("viewPos", camera.Position);
    shader.setVec3("fogColor", fogColor);

    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void render_street_with_shader(Shader shader, const vertex_array& vao, std::vector<PointLight> pointLights, GLuint texture_base, GLuint texture_specular, GLuint texture_normal, int order_number, Camera curent_camera, glm::vec3 fogColor, std::vector<SpotLight> spotLights, DirectionalLight dir_light) {
    shader.use();
    setUpLights(shader, pointLights);
    RenderedItem::setUpSpotLights(shader, spotLights);
    RenderedItem::setUpDirectionalLight(shader, dir_light);

    float last_camera_postion_checkpoint = std::ceil(curent_camera.Position.z / 10.0f)*10.0f;

    auto model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0, 0.5f, -10.0f* order_number + last_camera_postion_checkpoint));
    model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
    shader.set4Matrix("model", model);

    auto view = curent_camera.GetViewMatrix();
    shader.set4Matrix("view", view);

    auto projection = glm::perspective(glm::radians(curent_camera.Zoom), (float)1200 / (float)900, 0.1f, 100.0f);
    shader.set4Matrix("projection", projection);

    shader.setVec3("viewPos", curent_camera.Position);

    shader.setVec3("fogColor", fogColor);

    vao.bind();

    glActiveTexture(GL_TEXTURE0);
    shader.setInt("texture_diffuse", 0);
    glBindTexture(GL_TEXTURE_2D, texture_base);

    glActiveTexture(GL_TEXTURE1);
    shader.setInt("texture_specular", 1);
    glBindTexture(GL_TEXTURE_2D, texture_specular);

    glActiveTexture(GL_TEXTURE2);
    shader.setInt("texture_normal", 2);
    glBindTexture(GL_TEXTURE_2D, texture_normal);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    vao.unbind();
}

void clearBuffers() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

std::vector<PointLight> generatePointLights(glm::vec3 pointLightPositions[])
{
    std::vector<PointLight> lights;
    
    PointLight light = PointLight();
    light.position = pointLightPositions[0];
    light.ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    light.diffuse = glm::vec3(0.8f, 0.0f, 0.0f);
    light.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    light.constant = 1.0f;
    light.linear = 0.09f;
    light.quadratic = 0.032f;

    PointLight light1 = PointLight();
    light1.position = pointLightPositions[1];
    light1.ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    light1.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    light1.specular = glm::vec3( 1.0f, 1.0f, 1.0f);
    light1.constant = 1.0f;
    light1.linear = 0.09f;
    light1.quadratic = 0.032f;

    PointLight light2 = PointLight();
    light2.position = pointLightPositions[2];
    light2.ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    light2.diffuse = glm::vec3( 0.8f, 0.8f, 0.8f);
    light2.specular = glm::vec3( 1.0f, 1.0f, 1.0f);
    light2.constant = 1.0f;
    light2.linear = 0.09f;
    light2.quadratic = 0.032f;

    PointLight light3 = PointLight();
    light3.position = pointLightPositions[3];
    light3.ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    light3.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    light3.specular = glm::vec3( 1.0f, 1.0f, 1.0f);
    light3.constant = 1.0f;
    light3.linear = 0.09f;
    light3.quadratic = 0.032f;

    lights.push_back(light);
    lights.push_back(light1);
    lights.push_back(light2);
    lights.push_back(light3);

    return lights;
}