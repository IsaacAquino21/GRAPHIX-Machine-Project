//Implementing Point Light
#version 330 core

//Texture to be received
uniform sampler2D tex0;
uniform sampler2D tex1;

uniform sampler2D norm_tex;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform float ambientStr;
uniform vec3 ambientColor;

uniform vec3 cameraPos;
uniform float specStr;
uniform float specPhong;


in vec2 texCoord;
in vec3 normCoord;
in vec3 fragPos;

in mat3 TBN;

out vec4 FragColor;

void main(){
    //mix textures
    vec4 pixelColor = mix(texture(tex0, texCoord),texture(tex1, texCoord), 0.6);

    //discard pixels if alpha is < 0.1
    if(pixelColor.a < 0.1){
        discard;
    }

    vec3 normal = texture(norm_tex, texCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(TBN * normal);

    vec3 lightDir = normalize(lightPos - fragPos);

    float diff = max(
        dot(normal, lightDir) , 0.0f
    );

    vec3 diffuse = (diff) * lightColor;

    vec3 ambientCol = ambientStr * ambientColor;

    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(
        max(
            dot(reflectDir, viewDir), 0.1f    
        ),
    specPhong);

    vec3 specCol = spec * specStr * lightColor;

    //Getting the distance
    float distance = length(lightPos - fragPos);

    float attentuation = 1 / (1.0f + 0.045f * distance + 0.0075f * (distance * distance));

    //formula for intensity: 1/distance^2
    float intensity = 2.0f;
    
    //apply intensity to specular, diffuse and ambient light
    diffuse = diffuse * attentuation * intensity;
    specCol = specCol * attentuation * intensity;
    ambientCol = ambientCol * attentuation * intensity;

    FragColor = vec4(specCol + diffuse + ambientCol, 1.0f) * pixelColor;
}