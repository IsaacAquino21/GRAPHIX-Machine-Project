#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
class Light
{
public:
	// Position of Light
	glm::vec3 lightPos;
	// Light color
	glm::vec3 lightColor;
    // Ambient Strength
    float ambientStr;

    // Ambient Color
    glm::vec3 ambientColor;

    // Specular Strength
    float specStr;

    // Specular Phong
    float specPhong;

    float intensity;

    /* Constructors */
    Light(glm::vec3 position, glm::vec3 color, float amb_str, glm::vec3 amb_clr, float spc_str, float spc_phn) {
        lightPos = position;
        lightColor = color;
        ambientStr = amb_str;
        ambientColor = amb_clr;
        specStr = spc_str;
        specPhong = spc_phn;
        intensity = 6.0f;
    }
    /* Getters */
    glm::vec3 getLightPos() {
        return lightPos;
    }

    glm::vec3 getLightColor() {
        return lightColor;
    }

    float getAmbientStr() {
        return ambientStr;
    }

    glm::vec3 getAmbientColor() {
        return ambientColor;
    }

    float getSpecularStrength() {
        return specStr;
    }

    float getSpecularPhong() {
        return specPhong;
    }

    void cycleIntensity() {
        if (intensity == 6.0f) {
            intensity = 8.0f;
            printf("Intensity set to: Medium\n");
        }
            
        else if (intensity == 8.0f) {
            intensity = 10.0f;
            printf("Intensity set to: High\n");
        }
            
        else if (intensity == 10.0f) {
            intensity = 6.0f;
            printf("Intensity set to: Low\n");
        }       
    }


};

