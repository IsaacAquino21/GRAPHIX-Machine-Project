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

/* Screen Resolution */
float screenWidth = 600.0f;
float screenHeight = 600.0f;

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
    stbi_set_flip_vertically_on_load(true);

    MyShader skyboxShader = MyShader("Shaders/skybox.vert", "Shaders/skybox.frag");

    /* Create Skybox */
    Skybox skybox = Skybox();

    /* Create texture for skybox */
    //underwater textures
    std::string faceSkybox[]{
        "Skybox/uw_rt.jpg", //right
        "Skybox/uw_lf.jpg", //left
        "Skybox/uw_up.jpg", //up
        "Skybox/uw_dn.jpg", //down
        "Skybox/uw_ft.jpg", //front
        "Skybox/uw_bk.jpg" //back
    };

    SkyboxTexture skybox_uwTexture = SkyboxTexture(faceSkybox);

    
    //Temp - remove later
    //projection
    
    glm::mat4 projection = glm::perspective(
        glm::radians(60.0f),
        screenHeight / screenWidth, //aspect ratio
        0.1f, //0 < zNear < zFar
        1000.0f
    );

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Position of camera in the world */
        glm::vec3 cameraPos = glm::vec3(0, 0, 10.0f);

        /* World up*/
        glm::vec3 WorldUp = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));

        /* Center- where the camera is looking */
        glm::vec3 cameraCenter = glm::vec3(0, 0, 0);

        /* View matrix*/
        glm::mat4 view = glm::lookAt(cameraPos, cameraCenter, WorldUp);

        skybox.drawSkybox(skyboxShader, view, projection, skybox_uwTexture.getTexture());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}