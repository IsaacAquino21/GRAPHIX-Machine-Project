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

/* Light Instances */
Light pointLight = Light(
    glm::vec3(0.0f, 10.0f, 5.0f),
    glm::vec3(1, 1, 1),
    0.1f,
    glm::vec3(1, 1, 1),
    3.0f,
    25.0f
);

Light directionalLight = Light(
    glm::vec3(0.0f, 10.0f, 0.0f),
    glm::vec3(1, 1, 1),
    0.1f,
    glm::vec3(1, 1, 1),
    3.0f,
    25.0f
);

/* Camera Instances */
// Camera Movement were referenced from: https://learnopengl.com/Getting-started/Camera
Orthographic ortho_cam = Orthographic(
    glm::vec3(0.0f, 15.0f, 0.0f),
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

    MyShader skyboxShader = MyShader("Shaders/skybox.vert", "Shaders/skybox.frag");

    /* Create Skybox */
    Skybox skybox = Skybox();

    /* List of underwater textures */
    std::string faceSkybox[]{
        "Skybox/uw_ft.jpg", //front
        "Skybox/uw_bk.jpg", //back
        "Skybox/uw_up.jpg", //up
        "Skybox/uw_dn.jpg", //down
        "Skybox/uw_rt.jpg", //right
        "Skybox/uw_lf.jpg", //left
    };

    /* Create texture for skybox */
    SkyboxTexture skybox_uwTexture = SkyboxTexture(faceSkybox);

    MyShader modelShaders[4] = {
        MyShader("Shaders/main.vert", "Shaders/main.frag"), // with normal mapping
        MyShader("Shaders/noTBN.vert", "Shaders/texture2.frag"), // with 2 textures
        MyShader("Shaders/noTBN.vert", "Shaders/texture1.frag"), // with 1 texture only
        MyShader("Shaders/noTBN.vert", "Shaders/color.frag") // with no textures (so only color)
    };

    /* Player Model */
    Model playerModel = Model("3D/Odyssey/Odyssey.obj", glm::vec3(0.0f, 0.0f, 0.0f), 0.005f, glm::vec3(90.0f, 0.0f, 0.0f),
        modelShaders[0],
        MyTexture("3D/Odyssey/OdysseyHullTexture.png", false),
        MyTexture("3D/Odyssey/OdysseyHullEmissive.png", false),
        MyTexture("3D/Odyssey/OdysseyHullNormal.png", false)
    );

    /* Enemy Models */
    Model enemyModels[6] = {
        Model("3D/SubmarineV1.obj", glm::vec3(30.5f, -2.5f, 18.5f), 0.01f, glm::vec3(45.0f, 0.0f, 0.0f), modelShaders[3]),
        Model("3D/SubmarineV2/SubmarineV2.obj", glm::vec3(15.5f, 3.5f, 18.5f), 1.0f, glm::vec3(-30.0f, 0.0f, 0.0f), 
            modelShaders[2],
            MyTexture("3D/SubmarineV2/SubmarineV2Base.png", false)
        ),
        Model("3D/SubmarineV3/SubmarineV3.obj", glm::vec3(40.0f, 2.5f, -20.5f), 0.001f, glm::vec3(45.0f, 90.0f, 0.0f), modelShaders[0]),
        Model("3D/Dolphin/Dolphin.obj", glm::vec3(25.0f, 3.0f, -10.0f), 0.025f, glm::vec3(30.0f, 90.0f, 0.0f), 
            modelShaders[2],
            MyTexture("3D/Dolphin/Dolphin.jpg", false)
        ),
        Model("3D/SeaTurtle/SeaTurtle.obj", glm::vec3(15.0f, -2.0f, -9.5f), 0.025f, glm::vec3(45.0f, 45.0f, -120.0f),
            modelShaders[2],
            MyTexture("3D/SeaTurtle/SeaTurtle.jpg", false)),
        Model("3D/SharkV2/SharkV2.obj", glm::vec3(13.0f, -2.0f, 8.5f), 0.025f, glm::vec3(-45.0f, 90.0f, 30.0f), 
            modelShaders[2], 
            MyTexture("3D/SharkV2/SharkV2Texture.jpg", false)
        ),
    };

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ;

        if (view_select == 0) {
            curr_view = tp_camera.getView();
            curr_projection = tp_camera.getProjection();
            curr_cameraPos = tp_camera.getCameraPos();
            tp_camera.recalculateFront();
            tp_camera.recalculateViewMatrix();
        }
        if (view_select == 1) {
            curr_view = ortho_cam.getView();
            curr_projection = ortho_cam.getProjection();
            curr_cameraPos = ortho_cam.getCameraPos();
            ortho_cam.orthorecalViewMatrix();
        }

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

        for (MyShader shader : modelShaders) {
            shader.useProgram();
            shader.setMat4("projection", curr_projection);
            shader.setMat4("view", curr_view);
            shader.setVec3("cameraPos", tp_camera.getCameraPos());
            shader.setVec3("lightPos", pointLight.getLightPos());
            shader.setVec3("lightColor", pointLight.getLightColor());
            shader.setFloat("ambientStr", pointLight.getAmbientStr());
            shader.setVec3("ambientColor", pointLight.getAmbientColor());
            shader.setFloat("specStr", pointLight.getSpecularStrength());
            shader.setFloat("specPhong", pointLight.getSpecularPhong());
            shader.setFloat("intensity", pointLight.getIntensity());
            shader.setVec3("lightPos2", directionalLight.getLightPos());
            shader.setVec3("lightColor2", directionalLight.getLightColor());
            shader.setFloat("ambientStr2", directionalLight.getAmbientStr());
            shader.setVec3("ambientColor2", directionalLight.getAmbientColor());
            shader.setFloat("specStr2", directionalLight.getSpecularStrength());
            shader.setFloat("specPhong2", directionalLight.getSpecularPhong());
            shader.setFloat("intensity2", directionalLight.getIntensity());
        }

        // Draw VAO
        playerModel.draw();

        /* Loop each enemy model and render based on default position, scale, and rotation */
        for (Model enemyModel : enemyModels) enemyModel.draw(); 

        /* Render 10 fixed mines around the world*/
        /*for (int i = 0; i < 10; i++) {
            transform = glm::mat4(1.0f);
            transform = glm::translate(transform, enemyModel.getPosition());
            transform = glm::scale(transform, enemyModel.getScale());
            transform = glm::rotate(transform, glm::radians(enemyModel.getRotation().x), glm::vec3(0, 1, 0));
            transform = glm::rotate(transform, glm::radians(enemyModel.getRotation().y), glm::vec3(-1, 0, 0));
            transform = glm::rotate(transform, glm::radians(enemyModel.getRotation().z), glm::vec3(0, 0, 1));

            modelShader.useProgram();
            modelShader.setMat4("transform", transform);

            enemyModel.draw();
        }*/

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    playerModel.deleteBuffers();
    for (Model model : enemyModels) model.deleteBuffers();
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

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        view_select = 0;
    }
    
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        pointLight.cycleIntensity();
    }
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

    if (view_select == 1) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            ortho_cam.cameraPos.z -= 0.3;
            ortho_cam.cameraFront.z -= 0.3;  
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            ortho_cam.cameraPos.z += 0.3;
            ortho_cam.cameraFront.z += 0.3;
        }          

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            ortho_cam.cameraPos.x -= 0.3;
            ortho_cam.cameraFront.x -= 0.3;
        }
            
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            ortho_cam.cameraPos.x += 0.3;
            ortho_cam.cameraFront.x += 0.3;
        }
      
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
        else if (action == GLFW_RELEASE) button_down = false;
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