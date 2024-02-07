//#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

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

out vec3 FragPos;
out vec3 fragColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_emissive1;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirectionalLight directionalLight;

vec2 TexCoords;
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 currentTexture, vec3 specularTexture);

void main()
{
//vertex shader calcualtions
    TexCoords = aTexCoords;
    vec3 Normal = mat3(transpose(inverse(model))) * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));

//gourard shading
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0f);
    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    vec3 currentTexture = vec3(texture(texture_diffuse1, TexCoords));
    result += vec3(texture(texture_emissive1, TexCoords)) * vec3(currentTexture);

    result += CalculateDirectionalLight(directionalLight, norm, FragPos, viewDir, currentTexture, vec3(0.3f,0.3f,0.3f));

    fragColor = result;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {

    vec3 currentTexture = vec3(texture(texture_diffuse1, TexCoords));
    vec3 specularTexture = vec3(1.0f,1.0f,1.0f);

    vec3 ambient = currentTexture * light.ambient * 0.01f;
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

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 currentTexture, vec3 specularTexture)
{
    vec3 ambient = light.ambient * currentTexture;

    // diffuse
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * currentTexture;

    // specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 65.0f);
    vec3 specular = light.specular * spec * specularTexture;

    return ambient + diffuse + specular;
}

//#shader fragment
#version 330 core
out vec4 FragColor;

in vec3 fragColor;
in vec3 FragPos;

uniform vec3 viewPos;
uniform vec3 fogColor;

float getFogFactor(float d);

void main()
{
    float fogFactor = getFogFactor(distance(viewPos, FragPos));
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


