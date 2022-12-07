#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "MyShader.h"
#include "MyTexture.h"
#include "Skybox.h"
#include "SkyboxTexture.h"
#include "Model.h"

/* Screen Resolution */
float screenWidth = 900.0f;
float screenHeight = 900.0f;

// Camera Movement were referenced from: https://learnopengl.com/Getting-started/Camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

double last_x, last_y;
float yaw = -90.0f, pitch = -30.0f;
bool button_down = false;

/**
* Handles the Key Presses
**/
void Key_Callback(
    GLFWwindow* window,
    int key,      // key code
    int scanCode, // scan code
    int action,   // press or release
    int mods      // modifier keys
) {
    const float cameraSpeed = 0.1f;

    // movement of camera
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront; // moves forward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront; // moves backward
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; // moves to the left
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; // moves to the right

    // rotation of camera
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        pitch += cameraSpeed; // rotates upwards
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        pitch -= cameraSpeed; // rotates downwards
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        yaw -= cameraSpeed;   // rotates leftwards
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        yaw += cameraSpeed;   // rotates rightwards
}

/**
* Handles the Mouse Presses
**/
void Mouse_Callback(
    GLFWwindow* window,
    int button, // key code
    int action, // press or release
    int mods    // modifier keys
) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        // if left mouse button was click, set button_down flag to true
        if (action == GLFW_PRESS) {
            glfwGetCursorPos(window, &last_x, &last_y);
            button_down = true;
        }
        // if left mouse button is released, set button_down flag to false
        if (action == GLFW_RELEASE) button_down = false;
    }
}

/**
* Handles the Cursor Movement
**/
void Cursor_Callback(
    GLFWwindow* window,
    double xpos, // cursor's x position
    double ypos  // cursor's y position
) {
    // only activates when mouse has been clicked and not released (dragging activity)
    if (button_down) {
        // determines the distance from the click and the release (offset)
        float xoffset = xpos - last_x;
        float yoffset = last_y - ypos;
        last_x = xpos;
        last_y = ypos;

        // takes into consideration the intended sensitivity
        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        // modifies the yaw and pitch values based on the offset of mouse movement
        yaw += xoffset;
        pitch += yoffset;
    }
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(screenWidth, screenHeight, "GRAPHIX - Machine Project", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Initialize glad */
    gladLoadGL();

    glEnable(GL_DEPTH_TEST);

    glfwSetKeyCallback(window, Key_Callback);
    glfwSetMouseButtonCallback(window, Mouse_Callback);
    glfwSetCursorPosCallback(window, Cursor_Callback);

    MyShader mainShader = MyShader("Shaders/main.vert", "Shaders/main.frag");

    MyShader skyboxShader = MyShader("Shaders/skybox.vert", "Shaders/skybox.frag");

    MyTexture modelTexture = MyTexture("3D/OdysseyHullTexture.png", false);

    /* Create Skybox */
    Skybox skybox = Skybox();

    /* Create texture for skybox */
    //underwater textures
    std::string faceSkybox[]{
        "Skybox/uw_lf.jpg", //left
        "Skybox/uw_rt.jpg", //right
        "Skybox/uw_up.jpg", //up
        "Skybox/uw_dn.jpg", //down
        "Skybox/uw_ft.jpg", //front
        "Skybox/uw_bk.jpg"  //back
    };

    SkyboxTexture skybox_uwTexture = SkyboxTexture(faceSkybox);

    const char* objFilename = "3D/Odyssey.obj";
    Model model = Model(objFilename, mainShader);

    
    //Temp - remove later
    //projection
    
    glm::mat4 projection = glm::perspective(
        glm::radians(60.0f),
        screenHeight / screenWidth, //aspect ratio
        0.1f, //0 < zNear < zFar
        1000.0f
    );
    float x, y, z;
    x = y = z = 0.0f;

    float scale = 0.005f;

    // Position of Light
    glm::vec3 lightPos = glm::vec3(3, -10, 0);
    // Light color
    glm::vec3 lightColor = glm::vec3(1, 1, 1);

    // Ambient Strength
    float ambientStr = 0.1f;

    // Ambient Color
    glm::vec3 ambientColor = lightColor;

    // Specular Strength
    float specStr = 0.5f;

    // Specular Phong
    float specPhong = 16;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        // re-calculates the cameraFront based on the yaw and pitch
        cameraFront = glm::normalize(direction);

        /* View matrix*/
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(x, y, -2.5f));
        transform = glm::scale(transform, glm::vec3(scale));
        transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0, 1, 0));

        glm::mat4 sky_view = glm::mat4(1.f);
        sky_view = glm::mat4(glm::mat3(view));

        skyboxShader.useProgram();
        skyboxShader.setMat4("view", sky_view);
        skyboxShader.setMat4("projection", projection);


        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);

        glBindVertexArray(skybox.getVAO());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_uwTexture.getTexture());
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        mainShader.useProgram();

        mainShader.setMat4("transform", transform);
        mainShader.setMat4("projection", projection);
        mainShader.setMat4("view", view);
        mainShader.setVec3("lighPos", lightPos);
        mainShader.setVec3("lightColor", lightColor);
        mainShader.setFloat("ambientStr", ambientStr);
        mainShader.setVec3("ambientColor", ambientColor);
        mainShader.setVec3("cameraPos", cameraPos);
        mainShader.setFloat("specStr", specStr);
        mainShader.setFloat("specPhong", specPhong);
        mainShader.setTexture("tex0", modelTexture.getTexId(), 0);

        // Draw VAO
        model.draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    model.deleteBuffers();
    skybox.deleteBuffers();

    glfwTerminate();
    return 0;
}