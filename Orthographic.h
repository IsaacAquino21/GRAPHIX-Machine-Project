#pragma once
#include "Camera.h"
#include <cstdio>

class Orthographic :
    public Camera
{
public:
    /* Parameters:
     * Position: location of camera
     * up: World up for camera
     * center: where the camera is looking at
     * screenHeight: height of app (for projection)
     * screenWidth: width of app (for projection)
     * degrees: FoV (for projection)
     * zNear: near distance (for projection)
     * zFar: Far distance (for projection)
     */
    Orthographic(glm::vec3 position, glm::vec3 up, glm::vec3 front, float left, float right, float bottom, float top, float zNear, float zFar) :Camera(position, up, front, glm::ortho(
        left, //left
        right, //right
        bottom, //bottom
        top, //top
        zNear, //zNear
        zFar //zFar
    )) {
    }

    glm::mat4 getProjection() {
        return projection;
    }

    glm::mat4 getView() {
        return glm::lookAt(getCameraPos(), getCameraFront(), getCameraUp());
    }
};
