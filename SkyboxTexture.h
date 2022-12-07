#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include "MyTexture.h"

class SkyboxTexture
{
public:
	GLuint texId;
	
	SkyboxTexture(std::string faceSkybox[6]) {
        glGenTextures(1, &texId);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texId);

        //PREVENTS PIXELATING
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        //PREVENTS TILING
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        for (unsigned i = 0; i < 6; i++) {
            int w, h, skyCChannel;
            stbi_set_flip_vertically_on_load(false);

            unsigned char* data = stbi_load(faceSkybox[i].c_str(),
                &w,
                &h,
                &skyCChannel,
                0);

            if (data) {
                glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0,
                    GL_RGB,
                    w,
                    h,
                    0,
                    GL_RGB,
                    GL_UNSIGNED_BYTE,
                    data
                );

                stbi_image_free(data);
            }
        }
        stbi_set_flip_vertically_on_load(true);
	}

    GLuint getTexture() {
        return texId;
    }
};

