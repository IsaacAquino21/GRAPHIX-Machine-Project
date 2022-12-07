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
	glm::vec3 worldUp;
	/* Center - where the camera is looking */
	glm::vec3 cameraCenter;

	/* View Matrix */
	glm::mat4 view;

	Camera(float x, float y, float z) {
		cameraPos = glm::vec3(x, y, z);
		worldUp = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
		cameraCenter = glm::vec3(0.0f, y, 0.0f);
		view = glm::lookAt(cameraPos, cameraCenter, worldUp);
	}

	/** Getters **/
	glm::vec3 getCameraPos() {
		return cameraPos;
	}

	glm::vec3 getWorldUp() {
		return worldUp;
	}

	glm::vec3 getCameraCenter() {
		return cameraCenter;
	}

	glm::mat4 getView() {
		return view;
	}

};
