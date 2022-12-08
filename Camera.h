#pragma once

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/string_cast.hpp>

class Camera
{
public:
	/* Position of camera in the world */
	glm::vec3 cameraPos;
	/* World up*/
	glm::vec3 cameraUp;
	/* Center - where the camera is looking */
	glm::vec3 cameraFront;

	Camera(glm::vec3 position, glm::vec3 up, glm::vec3 front) {
		cameraPos = position;
		cameraUp = up;
		cameraFront = front;
	}

	/** Getters **/
	glm::vec3 getCameraPos() {
		return cameraPos;
	}

	glm::vec3 getCameraUp() {
		return cameraUp;
	}

	glm::vec3 getCameraFront() {
		return cameraFront;
	}

	glm::mat4 getView() {
		return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}

	void setCameraPos(glm::vec3 pos) {
		cameraPos = pos;
	}

	void setCameraFront(glm::vec3 front) {
		cameraFront = front;
	}

	void setCameraUp(glm::vec3 up) {
		cameraUp = up;
	}
};
