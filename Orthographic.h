#pragma once
#include "Camera.h"
class Orthographic :
    public Camera
{
public:
    glm::mat4 projection;

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
    Orthographic(glm::vec3 position, glm::vec3 up, glm::vec3 front, float screenHeight, float screenWidth, float zNear, float zFar) :Camera(position, up, front) {
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

