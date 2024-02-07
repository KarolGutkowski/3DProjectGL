//#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirectionalLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 fogColor;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirectionalLight directionalLight;

out vec3 fragPos;
flat out vec3 fragColor;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 norm, vec3 fragPos, vec3 viewDir);

void main(){
    //normal vertex shader:
    vec3 actual_pos = vec3(aPos.x, aPos.z, aPos.y); // z and y are flipped

    gl_Position = projection * view * model * vec4(actual_pos, 1.0);
    vec3 normal = mat3(transpose(inverse(model)))*aNormal;
    fragPos = vec3(model * vec4(actual_pos, 1.0));
    fragColor = aColor;

    //gourard shading part:
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 result = vec3(0.0f);

    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += CalcPointLight(pointLights[i], norm, fragPos, viewDir);
    }

    result += CalculateDirectionalLight(directionalLight, norm, fragPos, viewDir);

    fragColor = result;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {

    vec3 currentTexture = vec3(fragColor);
    vec3 specularTexture = vec3(1.0f,1.0f,1.0f);

    vec3 ambient = currentTexture * light.ambient;
    vec3 lightDir = normalize(light.position - fragPos);
    float distanceToLight = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distanceToLight + light.quadratic * distanceToLight * distanceToLight);

    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * currentTexture;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 80.0f);
    vec3 specular = light.specular * spec * specularTexture;

    return (ambient + diffuse + specular) * attenuation;
}


vec3 CalculateDirectionalLight(DirectionalLight light, vec3 norm, vec3 fragPos, vec3 viewDir)
{
    vec3 ambient = light.ambient * fragColor;

    // diffuse
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * fragColor;

    // specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 65.0f);
    vec3 specular = light.specular * spec * vec3(1.0f);

    return ambient + diffuse + specular;
}


//#shader fragment
#version 330 core
out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 fogColor;
in vec3 fragPos;
flat in vec3 fragColor;

float getFogFactor(float d);

void main(){
    float fogFactor = getFogFactor(distance(viewPos, fragPos));

    FragColor = mix(vec4(fragColor, 1.0f), vec4(fogColor, 1.0f), fogFactor);
}


float getFogFactor(float d)
{
    const float FogMax = 50.0;
    const float FogMin = 20.0;

    if (d>=FogMax) return 1;
    if (d<=FogMin) return 0;

    return 1 - (FogMax - d) / (FogMax - FogMin);
}