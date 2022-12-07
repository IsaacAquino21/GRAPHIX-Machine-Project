#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 aTex;
layout(location = 3) in vec3 m_tan;
layout(location = 4) in vec3 m_btan;

out vec2 texCoord;
out vec3 normCoord;
out vec3 fragPos;

// convert tangent space to object space
out mat3 TBN;

uniform mat4 transform;
uniform mat4 projection;
uniform	mat4 view;

void main(){
	// this denote the final position of the vertex/point
	gl_Position = projection * view * transform * vec4(aPos, 1.0);

	texCoord = aTex;
	
	mat3 modelMat = mat3(transpose(inverse(transform)));

	// Get the normal matrix and convert it into 3x3 matrix
	normCoord = modelMat * vertexNormal;

	// tangent
	vec3 T = normalize(modelMat * m_tan);

	// bitangent
	vec3 B = normalize(modelMat * m_btan);

	// normal
	vec3 N = normalize(normCoord);

	TBN = mat3(T, B, N);
	
	// Get position based on the transform matrix
	fragPos = vec3(transform * vec4(aPos, 1.0));
}