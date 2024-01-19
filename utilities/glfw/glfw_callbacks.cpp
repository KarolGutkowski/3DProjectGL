#include "glad/glad.h"
#include "glfw_callbacks.h"
#include "state.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
}

float lastFrame = 0.0f;
int currentCameraNumber = 0;
bool was_R_key_up = true;
void processInput(GLFWwindow* window, Camera& movable_camera) {
    float currentFrame = glfwGetTime();
    float deltaTime = currentFrame - lastFrame;
    lastFrame=currentFrame;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        movable_camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
    }else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        movable_camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
    }else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        movable_camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        movable_camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
    }

   /* if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && was_R_key_up)
    {
        was_R_key_up = false;
        switch (currentCameraNumber) 
        {
            case 0:
            {
                printf("1\n");
                currentCameraNumber = 1;
                current_camera = object_camera;
                break;
            }
            case 1:
            {
                printf("0\n");
                currentCameraNumber = 0;
                current_camera = movable_camera_copy;
                break;
            }
        }
    } 

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE)
    {
        was_R_key_up = true;
    }*/
}


bool firstMouse = true;
float lastX = 0.0f;
float lastY = 0.0f;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    //if (firstMouse)
    //{
    //    lastX = xpos;
    //    lastY = ypos;
    //    firstMouse = false;
    //}

    //float xoffset = xpos - lastX;
    //float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    //lastX = xpos;
    //lastY = ypos;

    //camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  /*  camera.ProcessMouseScroll(yoffset);*/
}