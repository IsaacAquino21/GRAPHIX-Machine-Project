#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 aTex;

out vec2 texCoord;
out vec3 normCoord;
out vec3 fragPos;

uniform mat4 transform;
uniform mat4 projection;
uniform	mat4 view;

void main(){
	// this denote the final position of the vertex/point
	gl_Position = projection * view * transform * vec4(aPos, 1.0);

	texCoord = aTex;
	
	// Get the normal matrix and convert it into 3x3 matrix
	normCoord = mat3(transpose(inverse(transform))) * vertexNormal;
	
	// Get position based on the transform matrix
	fragPos = vec3(transform * vec4(aPos, 1.0));
}