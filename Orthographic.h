#pragma once
#include "Camera.h"
class Orthographic :
    public Camera
{
public:
    glm::mat4 projection;

    Orthographic(float x, float y, float z, float screenHeight, float screenWidth, float zNear, float zFar) :Camera(x, y, z) {
        projection = glm::ortho(
            -1.0f, //left
            screenWidth, //right
            -1.0f, //bottom
            screenHeight, //top
            zNear, //zNear
            zFar //zFar
        );
    }

    glm::mat4 getProjection() {
        return projection;
    }
};

