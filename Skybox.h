#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "MyShader.h"
class Skybox
{
public:
    GLuint skyboxVAO, skyboxVBO, skyboxEBO;
	Skybox() {
        /*
         7--------6
        /|       /|
        4--------5 |
        | |      | |
        | 3------|-2
        |/       |/
        0--------1
        */
        //Vertices for the cube
        float skyboxVertices[]{
            -1.f, -1.f, 1.f, //0
            1.f, -1.f, 1.f,  //1
            1.f, -1.f, -1.f, //2
            -1.f, -1.f, -1.f,//3
            -1.f, 1.f, 1.f,  //4
            1.f, 1.f, 1.f,   //5
            1.f, 1.f, -1.f,  //6
            -1.f, 1.f, -1.f  //7
        };

        //Skybox Indices
        unsigned int skyboxIndices[]{
            1,2,6,
            6,5,1,

            0,4,7,
            7,3,0,

            4,5,6,
            6,7,4,

            0,3,2,
            2,1,0,

            0,1,5,
            5,4,0,

            3,7,6,
            6,2,3
        };

        /* Create VAO, VBO, EBO*/
        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glGenBuffers(1, &skyboxEBO);

        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);

        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(skyboxVertices),
            skyboxVertices,
            GL_STATIC_DRAW);

        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            3 * sizeof(GL_FLOAT),
            (void*)0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            sizeof(GL_INT) * 36,
            &skyboxIndices,
            GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
	}

    void drawSkybox(MyShader skyboxShader, glm::mat4 view, glm::mat4 projection, GLuint skyboxTex) {
        /* Skybox */
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);

        skyboxShader.useProgram();
        glm::mat4 sky_view = glm::mat4(1.0f);
        sky_view = glm::mat4(glm::mat3(view));

        skyboxShader.setMat4("projection", projection);
        skyboxShader.setMat4("view", sky_view);

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
    }

    GLuint getVAO() {
        return skyboxVAO;
    }

    GLuint getVBO() {
        return skyboxVBO;
    }

    GLuint getEBO() {
        return skyboxEBO;
    }

    void cleanupSkybox() {
        glDeleteVertexArrays(1, &skyboxVAO);
        glDeleteBuffers(1, &skyboxVBO);
        glDeleteBuffers(1, &skyboxEBO);
    }
};

