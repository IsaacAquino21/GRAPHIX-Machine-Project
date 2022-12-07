#version 330 core // version

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D norm_tex;

uniform vec3 lightPos;
uniform vec3 lightColor;

// Strength of the ambient light
uniform float ambientStr;

// Color of the ambient light
uniform vec3 ambientColor;

// Camera Position
uniform vec3 cameraPos;

// Specular Strength
uniform float specStr;

// Specular Phong
uniform float specPhong;

in vec2 texCoord;

// normal coords
in vec3 normCoord;

// world space
in vec3 fragPos;

// tangent, bitangent, normal
in mat3 TBN;

out vec4 FragColor;

void main(){
	// Normalize the received normals
	vec3 normal = normalize(normCoord);

	// Get the difference of the light to the fragment
	vec3 lightDir = normalize(lightPos - fragPos);

	// Apply the diffuse formula 
	float diff = max(dot(normal, lightDir), 0.0f);

	// Multiply it to the desired light color
	vec3 diffuse = diff * lightColor;

	// Get the ambient light
	vec3 ambientCol = ambientColor * ambientStr;

	// Get our view direction from the camera to the fragment
	vec3 viewDir = normalize(cameraPos - fragPos);

	// Get the reflection vector
	vec3 reflectDir = reflect(-lightDir, normal);

	// Get the specular light
	float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);

	vec3 specColor = spec * specStr * lightColor;

    //Getting the distance
    float distance = length(lightPos - fragPos);

    float attentuation = 1 / (1.0f + 0.045f * distance + 0.0075f * (distance * distance));

    //formula for intensity: 1/distance^2
    float intensity = 6.0f;

	vec4 texture0 = texture(tex0, texCoord);
	vec4 texture1 = texture(tex1, texCoord);
    
    //apply intensity to specular, diffuse and ambient light
    FragColor = vec4((specColor + diffuse + ambientCol) * attentuation * intensity, 1.0f) * mix(texture1, texture0, 0.6);
}