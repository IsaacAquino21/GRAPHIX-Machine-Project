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
#define SCREEN_WIDTH 900.0f
#define SCREEN_HEIGHT 900.0f

#define THIRD_POV_X 6.0f
#define THIRD_POV_Y 2.5f
#define FIRST_POV_X -6.0f

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
Orthographic ortho_camera = Orthographic(
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

Perspective tp_camera = Perspective(glm::vec3(THIRD_POV_X, THIRD_POV_Y, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 1.0f), 
    SCREEN_HEIGHT,
    SCREEN_WIDTH,
    60.0f,
    0.1f,
    50.0f
);

Perspective fp_camera = Perspective(glm::vec3(FIRST_POV_X, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 1.0f),
    SCREEN_HEIGHT,
    SCREEN_WIDTH,
    60.0f,
    0.1f,
    100.0f
);

glm::vec3 playerPos = glm::vec3(0.0f);
float rotX = 90.0f;

/* Global Variables used for switching view and projection */
float view_select = 0;
glm::mat4 curr_view;
glm::mat4 curr_projection;
glm::vec3 curr_cameraPos;

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
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GRAPHIX - Machine Project", NULL, NULL);
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

    MyShader modelShaders[5] = {
        MyShader("Shaders/main.vert", "Shaders/main.frag"), // with normal mapping
        MyShader("Shaders/noTBN.vert", "Shaders/texture2.frag"), // with 2 textures
        MyShader("Shaders/noTBN.vert", "Shaders/texture1.frag"), // with 1 texture only
        MyShader("Shaders/noTBN.vert", "Shaders/color.frag"), // with no textures (so only color)
        MyShader("Shaders/noTBN.vert", "Shaders/sonar.frag") // sonar texture, single color (green)
    };

    /* Player contains its Model */
    Model playerModel = Model("3D/Odyssey/Odyssey.obj", glm::vec3(0.0f, 0.0f, 0.0f), 0.005f, glm::vec3(90.0f, 0.0f, 0.0f),
        modelShaders[0],
        MyTexture("3D/Odyssey/OdysseyHullTexture.png", false),
        MyTexture("3D/Odyssey/OdysseyHullEmissive.png", false),
        MyTexture("3D/Odyssey/OdysseyHullNormal.png", false)
    );

    /* Enemy Models */
    Model enemyModels[6] = {
        Model("3D/SubmarineV1.obj", glm::vec3(121.0f, -8.0f, 18.5f), 0.01f, glm::vec3(45.0f, 0.0f, 0.0f), modelShaders[3]),
        Model("3D/SubmarineV2/SubmarineV2.obj", glm::vec3(15.5f, -3.5f, 18.5f), 1.0f, glm::vec3(-30.0f, 0.0f, 0.0f), 
            modelShaders[2],
            MyTexture("3D/SubmarineV2/SubmarineV2Base.png", false)
        ),
        Model("3D/SubmarineV3/SubmarineV3.obj", glm::vec3(40.0f, -5.5f, -24.5f), 0.001f, glm::vec3(45.0f, 90.0f, 0.0f), modelShaders[0]),
        Model("3D/Dolphin/Dolphin.obj", glm::vec3(25.0f, -10.0f, -15.0f), 0.025f, glm::vec3(30.0f, 90.0f, 0.0f), 
            modelShaders[2],
            MyTexture("3D/Dolphin/Dolphin.jpg", false)
        ),
        Model("3D/SeaTurtle/SeaTurtle.obj", glm::vec3(150.0f, -4.0f, -9.5f), 0.025f, glm::vec3(45.0f, 45.0f, -120.0f),
            modelShaders[2],
            MyTexture("3D/SeaTurtle/SeaTurtle.jpg", false)),
        Model("3D/SharkV2/SharkV2.obj", glm::vec3(150.0f, -4.0f, 8.5f), 0.025f, glm::vec3(-45.0f, 90.0f, 30.0f), 
            modelShaders[2], 
            MyTexture("3D/SharkV2/SharkV2Texture.jpg", false)
        ),
    };

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    float horD = THIRD_POV_X + glm::cos(glm::radians(tp_camera.getPitch()));
    float verD = THIRD_POV_X + glm::sin(glm::radians(tp_camera.getPitch()));
    float newX = playerPos.x - horD * glm::sin(glm::radians(rotX));
    float newZ = playerPos.z - horD * glm::cos(glm::radians(rotX));

    tp_camera.yaw = 90 - rotX;
    tp_camera.setCameraPos(glm::vec3(newX, playerPos.y + THIRD_POV_Y, newZ));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (view_select == 0) {
            // third person
            tp_camera.recalculateFront();

            curr_view = tp_camera.getView();
            curr_projection = tp_camera.getProjection();
            curr_cameraPos = tp_camera.getCameraPos();
        } else if (view_select == 1) {
            // first person
            fp_camera.recalculateFront();

            curr_view = fp_camera.getView();
            curr_projection = fp_camera.getProjection();
            curr_cameraPos = fp_camera.getCameraPos();
        } else if (view_select == 2) {
            // ortho person
            curr_view = ortho_camera.getView();
            curr_projection = ortho_camera.getProjection();
            curr_cameraPos = ortho_camera.getCameraPos();

            cout << curr_cameraPos.x << ";" << curr_cameraPos.y << ";" << curr_cameraPos.z << endl;
        }

        playerModel.setPosition(playerPos);
        playerModel.setRotation(rotX, 0.0f, 0.0f);

        if (view_select != 1) skybox.drawSkybox(skyboxShader, curr_view, curr_projection, skybox_uwTexture.getTexture());

        // Load All Shaders
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
        }

        // Draw Player Model
        playerModel.draw();

        /* Loop each enemy model and render based on default position, scale, and rotation */
        for (Model enemyModel : enemyModels) {
            if(view_select != 1) enemyModel.draw();
            else enemyModel.draw(modelShaders[4]);
        }

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
    
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        pointLight.cycleIntensity();
    }

    // selection of view
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        if (view_select == 0) 
            view_select = 1;
        else
            view_select = 0;
    }
    else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        view_select = 2;

    if (view_select == 2) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            ortho_camera.cameraPos.z -= 0.3;
            ortho_camera.cameraFront.z -= 0.3;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            ortho_camera.cameraPos.z += 0.3;
            ortho_camera.cameraFront.z += 0.3;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            ortho_camera.cameraPos.x -= 0.3;
            ortho_camera.cameraFront.x -= 0.3;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            ortho_camera.cameraPos.x += 0.3;
            ortho_camera.cameraFront.x += 0.3;
        }
    } else {
        // based on movement of player update camera
        float dx = cameraSpeed * glm::sin(glm::radians(rotX));
        float dz = cameraSpeed * glm::cos(glm::radians(rotX));

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            playerPos += glm::vec3(dx, 0.0f, dz);

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            playerPos -= glm::vec3(dx, 0.0f, dz);

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            rotX += cameraSpeed * 10;

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            rotX -= cameraSpeed * 10;

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            if(playerPos.y < 0) playerPos.y += cameraSpeed;

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            playerPos.y -= cameraSpeed;

        if (view_select == 0) {
            float horD = THIRD_POV_X + glm::cos(glm::radians(tp_camera.getPitch()));
            float verD = THIRD_POV_X + glm::sin(glm::radians(tp_camera.getPitch()));
            float newX = playerPos.x - horD * glm::sin(glm::radians(rotX));
            float newZ = playerPos.z - horD * glm::cos(glm::radians(rotX));

            tp_camera.yaw = 90 - rotX;
            tp_camera.setCameraPos(glm::vec3(newX, playerPos.y + THIRD_POV_Y, newZ));
        } else if (view_select == 1) {
            float horD = FIRST_POV_X + glm::cos(glm::radians(fp_camera.getPitch()));
            float verD = FIRST_POV_X + glm::sin(glm::radians(fp_camera.getPitch()));
            float newX = playerPos.x - horD * glm::sin(glm::radians(rotX));
            float newZ = playerPos.z - horD * glm::cos(glm::radians(rotX));

            fp_camera.yaw = 90 - rotX;
            fp_camera.setCameraPos(glm::vec3(newX, playerPos.y, newZ));
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
        last_x = xpos;

        // takes into consideration the intended sensitivity
        float sensitivity = 0.1f;
        xoffset *= sensitivity;

        // modifies the yaw and pitch values based on the offset of mouse movement
        if (view_select == 0) {
            rotX -= xoffset;

            float horD = THIRD_POV_X + glm::cos(glm::radians(0.0f));
            float verD = THIRD_POV_X + glm::sin(glm::radians(0.0f));
            float newX = playerPos.x - horD * glm::sin(glm::radians(rotX));
            float newZ = playerPos.z - horD * glm::cos(glm::radians(rotX));

            tp_camera.yaw = 90 - rotX;
            tp_camera.setCameraPos(glm::vec3(newX, playerPos.y + THIRD_POV_Y, newZ));
        }
    }
}