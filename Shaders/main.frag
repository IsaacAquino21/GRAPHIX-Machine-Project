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

/** Function Prototype**/
vec3 calcDirLight();

void main(){
	
    vec3 dirLightContribution = calcDirLight();

	vec4 texture0 = texture(tex0, texCoord);
	vec4 texture1 = texture(tex1, texCoord);
    
    //apply intensity to specular, diffuse and ambient light
    FragColor = vec4((dirLightContribution, 1.0f) * mix(texture1, texture0, 0.6));
}

/* This function calculates the light contribution of the Directional light to the object */
vec3 calcDirLight(){
    // Use normal map
	vec3 normal = texture(norm_tex, texCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(TBN * normal);

	// Get the difference of the light to the fragment
	vec3 lightDir = normalize(lightPos - fragPos);

    // Get our view direction from the camera to the fragment
	vec3 viewDir = normalize(cameraPos - fragPos);
    
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

    //return combined value
    return (diffuse + ambientCol + specCol);
}