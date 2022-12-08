#pragma once

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

class Camera
{
public:
	/* Position of camera in the world */
	glm::vec3 cameraPos;
	/* World up*/
	glm::vec3 cameraUp;
	/* Center - where the camera is looking */
	glm::vec3 cameraFront;

	/* View Matrix */
	glm::mat4 view;

	Camera(glm::vec3 position, glm::vec3 up, glm::vec3 front) {
		cameraPos = position;
		cameraUp = up;
		cameraFront = front;
		view = glm::lookAt(cameraPos, cameraFront, cameraUp);
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
		return view;
	}

};
