#pragma once
#include <cstdio>
using namespace std;
#include "Camera.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
class Perspective :
    public Camera
{
public:
    glm::mat4 projection;
    float yaw;
    float pitch;
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
    Perspective(glm::vec3 position, glm::vec3 up, glm::vec3 front, float screenHeight, float screenWidth, float degrees, float zNear, float zFar) :Camera(position, up, front) {
        projection = glm::perspective(
            glm::radians(degrees),
            screenHeight / screenWidth, //aspect ratio
            zNear,
            zFar
        );
        yaw =  0.0f;
        pitch = 0.0f;
    }

    void recalculateFront() {
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
    }

    void moveForward(float cameraSpeed) {
        cameraPos += cameraSpeed * cameraFront; // moves forward
    }

    void moveBackward(float cameraSpeed) {
      cameraPos -= cameraSpeed * cameraFront; // moves backward
    }

    void moveLeft(float cameraSpeed) {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; // moves to the left
    }

    void moveRight(float cameraSpeed) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; //move to the right
    }

    /** Getters **/
    glm::mat4 getProjection() {
        return projection;
    }

    float getPitch() {
        return pitch;
    }

    float getYaw() {
        return yaw;
    }

    /** Setters **/
    void addPitch(float value) {
        pitch += value;
    }

    void addYaw(float value) {
        yaw += value;
    }

    void subPitch(float value) {
        pitch -= value;
    }

    void subYaw(float value) {
        yaw -= value;
    }

    void addY(float value) {
        cameraPos.y += value;
    }

    void subY(float value) {
        cameraPos.y -= value;
    }
};

