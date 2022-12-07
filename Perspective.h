#pragma once
#include "Camera.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
class Perspective :
    public Camera
{
public:
    glm::mat4 projection;

    Perspective(float x, float y, float z, float screenHeight, float screenWidth, float degrees, float zNear, float zFar) :Camera(x, y, z) {
        projection = glm::perspective(
            glm::radians(degrees),
            screenHeight / screenWidth, //aspect ratio
            zNear,
            zFar
        );
    }

    /** Getter **/
    glm::mat4 getProjection() {
        return projection;
    }
};

