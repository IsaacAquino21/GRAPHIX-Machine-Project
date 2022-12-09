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

/** Function prototypes **/
vec3 calcPointLight(vec3 lightPos, vec3 lightColor, float ambientStr, vec3 ambientColor, float specStr, float specPhong, vec3 normal, vec3 viewDir);
vec3 calcDirLight(vec3 lightPos, vec3 lightColor, float ambientStr, vec3 ambientColor, float specStr, float specPhong, vec3 normal, vec3 viewDir);

void main(){
	// Normalize the received normals
	vec3 normal = normalize(normCoord);
    
	// Get our view direction from the camera to the fragment
    vec3 viewDir = normalize(cameraPos - fragPos);

    //calculate point and directional light color contribution
    vec3 pointLightColor = calcPointLight(lightPos, lightColor, ambientStr, ambientColor, specStr, specPhong, normal, viewDir);
    vec3 dirLightColor = calcDirLight(lightPos2, lightColor2, ambientStr2, ambientColor2, specStr2, specPhong2, normal, viewDir);

    vec3 finalColor = pointLightColor + dirLightColor;
    
    //apply intensity to specular, diffuse and ambient light
    FragColor = vec4(finalColor, 1.0f) * vec4(0.3f, 0.3f, 0.3f, 0.0f);
}

/* This function calculates the light contribution of the Point light to the object */
vec3 calcPointLight(vec3 lightPos, vec3 lightColor, float ambientStr, vec3 ambientColor, float specStr, float specPhong, vec3 normal, vec3 viewDir) { 
    
    vec3 lightDir = normalize(lightPos - fragPos);

    float diff = max(
        dot(normal, lightDir) , 0.0f
    );

    vec3 diffuse = (diff) * lightColor;

    vec3 ambientCol = ambientStr * ambientColor;

    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(
        max(
            dot(reflectDir, viewDir), 0.1f    
        ),
    specPhong);

    vec3 specCol = spec * specStr * lightColor;

    //Getting the distance
    float distance = length(lightPos - fragPos);

    /* Formula for attentuation: 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance))
     * Used values are for light that can cover a distance of 100 units
     */
    float attentuation = 1 / (1.0f + 0.045f * distance + 0.0075f * (distance * distance));

    //return combined value with applied attentuation and intensity to specular, diffuse and ambient light
    return (diffuse + ambientCol + specCol) * attentuation * intensity;
}

/* This function calculates the light contribution of the Directional light to the object */
vec3 calcDirLight(vec3 lightPos, vec3 lightColor, float ambientStr, vec3 ambientColor, float specStr, float specPhong, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(lightPos - fragPos);

    float diff = max(
        dot(normal, lightDir) , 0.0f
    );

    vec3 diffuse = (diff) * lightColor;

    vec3 ambientCol = ambientStr * ambientColor;

    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(
        max(
            dot(reflectDir, viewDir), 0.1f    
        ),
    specPhong);

    vec3 specCol = spec * specStr * lightColor;

    //return combined value with applied intensity
    return (diffuse + ambientCol + specCol) * 1.0f;
}