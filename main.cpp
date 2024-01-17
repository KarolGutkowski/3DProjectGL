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
#include "models_utilities/Truck/Truck.h"
#include "textures/texture_utils.h"

void clearBuffers();
std::vector<PointLight> generatePointLights(glm::vec3 pointLightPositions[]);
void renderLight(Shader shader, const vertex_array& vao, glm::vec3 position, Camera camera);
void setUpLights(Shader& shader, std::vector<PointLight> pointLights);
void drawWithShader(Shader shader, glm::vec3 pointLightPositions[], const Model& element, glm::mat4 model);
void render_street_with_shader(Shader shader, const vertex_array& vao, std::vector<PointLight> pointLights , GLuint texture_base, GLuint texture_specular, GLuint texture_normal, int order_number);

int main(void)
{
    initializeGLFW();
    auto window = createGLFWWindow(1200,900, "3D Graphics");
    glfwMakeContextCurrent(window);
    loadGlad();
    setWindowCallbacks(window);
    setGlOptions();
    initializeImGui(window);

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
            glm::vec3(-10.0f,  2.0f,  1.0f),
            glm::vec3(10.0f, 2.0f, 1.0f),
            glm::vec3(10.0f,  2.0f, -4.0f),
            glm::vec3(-10.0f,  2.0f, -4.0f)
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
    auto scene = Scene(items, lights);

    camera.Position.y = 1.0f;
    camera.Position.x = 0.5f;
    camera.Position.z = 2.0f;

    car.attached_camera.Position = glm::vec3(-0.4f, 4.5f, -5.0f);
    car.model_matrix = glm::rotate(car.model_matrix, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //car.model_matrix = glm::translate(car.model_matrix, glm::vec3(0.0f, 0.0f, 11.0f));

    while (!glfwWindowShouldClose(window)) 
    {
        processInput(window, camera);
        clearBuffers();

        ImGuiNewFrame();

        generateImGuiWindow(camera, car.attached_camera);
        glClearColor(0.1f, 0.1f, 0.1f, 0.1f);

        for(int i=0;i<4;i++)
            renderLight(light_shader, vao, lights[i].position, camera);

        scene.render(ShaderType::Phong, camera);

        for (int i = 0; i < 20; i++)
        {
            render_street_with_shader(floor_shader, floor_vao, lights, floor_texture_base, floor_texture_specular, floor_texture_normal,i);
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
    shader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("pointLights[0].constant", 1.0f);
    shader.setFloat("pointLights[0].linear", 0.09f);
    shader.setFloat("pointLights[0].quadratic", 0.032f);

    shader.setVec3("pointLights[1].position", pointLights[1].position);
    shader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("pointLights[1].constant", 1.0f);
    shader.setFloat("pointLights[1].linear", 0.09f);
    shader.setFloat("pointLights[1].quadratic", 0.032f);

    shader.setVec3("pointLights[2].position", pointLights[2].position);
    shader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("pointLights[2].constant", 1.0f);
    shader.setFloat("pointLights[2].linear", 0.09f);
    shader.setFloat("pointLights[2].quadratic", 0.032f);

    shader.setVec3("pointLights[3].position", pointLights[3].position);
    shader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("pointLights[3].constant", 1.0f);
    shader.setFloat("pointLights[3].linear", 0.09f);
    shader.setFloat("pointLights[3].quadratic", 0.032f);
}

void renderLight(Shader shader,const vertex_array& vao, glm::vec3 position, Camera camera) {
    shader.use();

    auto model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.5f,0.5f,0.5f));
    model = glm::translate(model, position);
    shader.set4Matrix("model", model);

    auto view = camera.GetViewMatrix();
    shader.set4Matrix("view", view);

    auto projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
    shader.set4Matrix("projection", projection);

    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void render_street_with_shader(Shader shader, const vertex_array& vao, std::vector<PointLight> pointLights, GLuint texture_base, GLuint texture_specular, GLuint texture_normal, int order_number) {
    shader.use();
    setUpLights(shader, pointLights);

    auto model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0, 0.5f, -10.0f* order_number));
    model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
    shader.set4Matrix("model", model);

    auto view = camera.GetViewMatrix();
    shader.set4Matrix("view", view);

    auto projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
    shader.set4Matrix("projection", projection);

    shader.setVec3("viewPos", camera.Position);

    shader.setVec3("objectColor", glm::vec3(0.5f,0.5f,0.5f));
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
    light.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
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