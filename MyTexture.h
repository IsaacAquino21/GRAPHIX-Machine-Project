#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class MyTexture
{
public:
	GLuint texId; //address

	MyTexture(const char* texName, bool alpha) {
		//Assigns the index for the texture

		//load image using stbi
		int img_width, img_height, color_channels;

		stbi_set_flip_vertically_on_load(true);
		unsigned char* tex_bytes = stbi_load(
			texName,
			&img_width,
			&img_height,
			&color_channels,
			0);

		//generate texture id
		glGenTextures(1, &texId); //generate id
		glActiveTexture(GL_TEXTURE0); //index
		glBindTexture(GL_TEXTURE_2D, texId); //bind to address

		
		//if texture has an alpha channel
		if (alpha) {
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA, //gl_rgb - jpegs/ img w/o alpha, gl_rgba -  png/ img w/ alpha
				img_width,
				img_height,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				tex_bytes
			);
		}

		//if texture does not have an alpha channel
		else if (!alpha) {
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGB, //gl_rgb - jpegs/ img w/o alpha, gl_rgba -  png/ img w/ alpha
				img_width,
				img_height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				tex_bytes
			);
		}

		//generate minmap
		glGenerateMipmap(GL_TEXTURE_2D);

		//cleanup
		stbi_image_free(tex_bytes);
	}

	//method used to get texture id
	GLuint getTexId() {
		return texId;
	}
	
};

