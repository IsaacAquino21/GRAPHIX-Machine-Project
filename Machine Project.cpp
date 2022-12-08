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
#include "Perspective.h"
#include "Orthographic.h"
#include "Light.h"

/* Screen Resolution */
float screenWidth = 900.0f;
float screenHeight = 900.0f;



/* Global Variables used for switching view and projection */
float view_select = 0;
glm::mat4 curr_view;
glm::mat4 curr_projection;
glm::vec3 curr_cameraPos;

// Position of Light
glm::vec3 lightPos = glm::vec3(0.0f, 10.0f, 5.0f);
// Light color
glm::vec3 lightColor = glm::vec3(1, 1, 1);

// Ambient Strength
float ambientStr = 0.1f;

// Ambient Color
glm::vec3 ambientColor = lightColor;

// Specular Strength
float specStr = 0.05f;

// Specular Phong
float specPhong = 16;
/* Light Instances */
Light pointLight = Light(glm::vec3(0.0f, 10.0f, 5.0f),
    glm::vec3(1, 1, 1),
    0.1f,
    glm::vec3(1, 1, 1),
    0.05f,
    16
    );


/* Camera Instances */
// Camera Movement were referenced from: https://learnopengl.com/Getting-started/Camera
Orthographic ortho_cam = Orthographic(glm::vec3(0.0f, 15.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, -1.0f), 
    glm::vec3(0.0f, 0.0f, 0.0f), 
    -10.0f, 
    10.0f, 
    -10.0f, 
    10.0f, 
    0.0f, 
    100.0f
);

Perspective tp_camera = Perspective(glm::vec3(-8.0f, 2.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f), 
    glm::vec3(0.0f, 0.0f, 1.0f), 
    screenHeight, 
    screenWidth,
    60.0f, 
    0.1f, 
    50.0f
);

double last_x, last_y;
bool button_down = false; // used for checking if mouse is clicked

/* Function Prototypes */
void Key_Callback(GLFWwindow* window, int key, int scanCode, int action, int mods);
void Mouse_Callback(GLFWwindow* window, int button, int action, int mods);
void Cursor_Callback(GLFWwindow* window, double xpos, double ypos);

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

    MyShader modelShader = MyShader("Shaders/main.vert", "Shaders/main.frag");

    MyShader skyboxShader = MyShader("Shaders/skybox.vert", "Shaders/skybox.frag");

    MyTexture modelTexture = MyTexture("3D/Odyssey/OdysseyHullTexture.png", false);
    MyTexture normTexture = MyTexture("3D/Odyssey/OdysseyHullNormal.png", false);
    MyTexture lightsTexture = MyTexture("3D/Odyssey/OdysseyHullEmissive.png", false);

    /* Create Skybox */
    Skybox skybox = Skybox();

    /* Create texture for skybox */
    //underwater textures
    std::string faceSkybox[]{
        "Skybox/uw_ft.jpg", //front
        "Skybox/uw_bk.jpg", //back
        "Skybox/uw_up.jpg", //up
        "Skybox/uw_dn.jpg", //down
        "Skybox/uw_rt.jpg", //right
        "Skybox/uw_lf.jpg", //left
    };

    SkyboxTexture skybox_uwTexture = SkyboxTexture(faceSkybox);

    Model playerModel = Model("3D/Odyssey/Odyssey.obj", 0);
    Model enemy1Model = Model("3D/Submarine.obj", 1);
    Model enemy2Model = Model("3D/MossyRocks.obj", 2);
    //Model enemy3Model = Model("3D/Mine.obj", 3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
        
    float x, y, z;
    x = y = z = 0.0f;

    float scale = 0.005f;




    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        tp_camera.recalculateFront();
        tp_camera.recalculateViewMatrix();

        if (view_select == 0) {
            curr_view = tp_camera.getView();
            curr_projection = tp_camera.getProjection();
        }
        if (view_select == 1) {
            curr_view = ortho_cam.getView();
            curr_projection = ortho_cam.getProjection();
        }

        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(x, y, z));
        transform = glm::scale(transform, glm::vec3(scale));
        transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0, 1, 0));

        glm::mat4 sky_view = glm::mat4(1.f);
        sky_view = glm::mat4(glm::mat3(curr_view));

        skyboxShader.useProgram();
        skyboxShader.setMat4("view", sky_view);
        skyboxShader.setMat4("projection", curr_projection);

        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);

        glBindVertexArray(skybox.getVAO());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_uwTexture.getTexture());
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        modelShader.useProgram();
        modelShader.setMat4("transform", transform);
        modelShader.setMat4("projection", curr_projection);
        modelShader.setMat4("view", curr_view);
        modelShader.setVec3("lighPos", lightPos);
        modelShader.setVec3("lightColor", lightColor);
        modelShader.setFloat("ambientStr", ambientStr);
        modelShader.setVec3("ambientColor", ambientColor);
        modelShader.setVec3("cameraPos", tp_camera.getCameraPos());
        modelShader.setFloat("specStr", specStr);
        modelShader.setFloat("specPhong", specPhong);
        modelShader.setTexture("tex0", modelTexture.getTexId(), 0);
        modelShader.setTexture("norm_tex", normTexture.getTexId(), 1);
        modelShader.setTexture("tex1", lightsTexture.getTexId(), 2);

        // Draw VAO
        playerModel.draw();

        /* Submarine */
        transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(7.5f, -2.5f, 7.5f));
        transform = glm::scale(transform, glm::vec3(0.0025f));
        transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0, 1, 0));

        modelShader.useProgram();
        modelShader.setMat4("transform", transform);

        enemy1Model.draw();
        for (int i = -5; i < 5; i++) {
            /* Enemy 2 - Stones */
            transform = glm::mat4(1.0f);
            transform = glm::translate(transform, glm::vec3(10.0f * i, -20.0f, -5.0f * i));
            transform = glm::scale(transform, glm::vec3(1.0f));
            transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0, 1, 0));

            modelShader.useProgram();
            modelShader.setMat4("transform", transform);

            enemy2Model.draw();
        }

        /* Enemy 3 - AkulaClassAtackSubmarine */
        /*transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(-7.5f, 5.5f, -7.5f));
        transform = glm::scale(transform, glm::vec3(0.01f));
        transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0, 1, 0));

        modelShader.useProgram();
        modelShader.setMat4("transform", transform);

        enemy3Model.draw();*/

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    playerModel.deleteBuffers();
    skybox.deleteBuffers();

    glfwTerminate();
    return 0;
}

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
        tp_camera.moveForward(cameraSpeed);
        
        

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        tp_camera.moveBackward(cameraSpeed);
       
        
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        tp_camera.moveLeft(cameraSpeed);
       
        
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        tp_camera.moveRight(cameraSpeed);
        
        

    // rotation of camera
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        tp_camera.addPitch(cameraSpeed);
        
        
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        tp_camera.subPitch(cameraSpeed);
       

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        tp_camera.subYaw(cameraSpeed);
        
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        tp_camera.addYaw(cameraSpeed);
        

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        view_select = 0;
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {    
        view_select = 1; 
    }
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
        tp_camera.addYaw(xoffset);
        tp_camera.addPitch(yoffset);
                    
    }
}