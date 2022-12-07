#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>


class MyShader
{
public:
	GLuint shaderProgram;

	/* Constructor of the MyShader Class
	 * Parameters:
	 * const char* v - Code from our vertex shader file
	 * const char* f - Code from our fragment shader file
	 */
	MyShader(const char* vName, const char* fName) {
		/* Loading of vertex and fragment shaders from file */
		std::fstream vertSrc(vName);
		std::stringstream vertBuff;
		vertBuff << vertSrc.rdbuf();
		std::string vertS = vertBuff.str();
		const char* v = vertS.c_str();

		std::fstream fragSrc(fName);
		std::stringstream fragBuff;
		fragBuff << fragSrc.rdbuf();
		std::string fragS = fragBuff.str();
		const char* f = fragS.c_str();

		/* Creating vertex and fragment shaders */
		GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertShader, 1, &v, NULL);
		glCompileShader(vertShader);

		GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragShader, 1, &f, NULL);
		glCompileShader(fragShader);

		/* Create Shader Program*/
		shaderProgram = glCreateProgram();

		/* Attach shaders we created to program */
		glAttachShader(shaderProgram, vertShader);
		glAttachShader(shaderProgram, fragShader);
		glLinkProgram(shaderProgram);
	}

	/* This method allows us to use the shader program by calling glUseProgram() */
	void useProgram() {
		glUseProgram(shaderProgram);
	}

	/* This method returns the id of the shader*/
	GLuint getProgram() {
		return shaderProgram;
	}

	/* This method is used to set a vec3 variable in the shader program */
	void setVec3(const GLchar* varName, glm::vec3 vec3) {
		unsigned int uniformLoc = glGetUniformLocation(shaderProgram, varName);

		glUniform3fv(uniformLoc, 1, glm::value_ptr(vec3));
	}

	/* This method is used to set a float variable in the shader program */
	void setFloat(const GLchar* varName, float floatVal) {
		unsigned int uniformLoc = glGetUniformLocation(shaderProgram, varName);

		
		glUniform1f(uniformLoc, floatVal);
	}

	/* This method is used to set a mat4 variable in the shader program */
	void setMat4(const GLchar* varName, glm::mat4 mat4) {
		unsigned int uniformLoc = glGetUniformLocation(shaderProgram, varName);


		glUniformMatrix4fv(uniformLoc, 1, GL_FALSE,
			glm::value_ptr(mat4));
	}

	/* This method is used to set the Tex0 variable in the shader program */
	void setTexture(const GLchar* varName, GLuint texture, GLint texIndex) {
		/* Get tex0 variable in the shader */ 
		glActiveTexture(GL_TEXTURE0 + texIndex);
		GLuint texAddress = glGetUniformLocation(shaderProgram, varName);
		glBindTexture(GL_TEXTURE_2D, texture);

		/* Change the value of texture */
		glUniform1i(texAddress, texIndex);
	}

};

