#version 330 core // version


/** Camera **/
uniform vec3 cameraPos;

/** Point Light Variables **/
//Light position and color of point light
uniform vec3 lightPos;
uniform vec3 lightColor;

//Ambient of point light
uniform float ambientStr;
uniform vec3 ambientColor;

//specular of point light
uniform float specStr;
uniform float specPhong;

//intensity of point light
uniform float intensity;

/** Directional Light Variables **/
//Light position and color of directional light
uniform vec3 lightPos2;
uniform vec3 lightColor2;

//Ambient of directional light
uniform float ambientStr2;
uniform vec3 ambientColor2;

//specular of directional light
uniform float specStr2;
uniform float specPhong2;

in vec2 texCoord;
in vec3 normCoord;
in vec3 fragPos;

out vec4 FragColor;

void main(){
	// Normalize the received normals
	vec3 normal = normalize(normCoord);
    
	// Get our view direction from the camera to the fragment
    vec3 viewDir = normalize(cameraPos - fragPos);
    
    //apply intensity to specular, diffuse and ambient light
    FragColor = vec4(0.0f, 0.7f, 0.0f, 0.0f);
}