//#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
/**out vec3 Normal;*/
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;
    /**Normal = mat3(transpose(inverse(model))) * aNormal;*/
    FragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

//#shader fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
/**in vec3 Normal;*/
in vec3 FragPos;

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOffFar;
    float cutOffClose;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};


uniform vec3 viewPos;
uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D texture_normal;
uniform vec3 fogColor;
#define NR_POINT_LIGHTS 4
#define NR_SPOT_LIGHTS 2
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight  spotLights[NR_SPOT_LIGHTS];

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 currentTexture, vec3 specularTexture);
float getFogFactor(float d);
vec3 CalculateSpotLight(SpotLight light, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 currentTexture, vec3 specularTexture);

void main()
{
    vec3 Normal = vec3(texture(texture_normal, TexCoords));
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0f);

    vec3 currentTexture = vec3(texture(texture_diffuse, TexCoords));
    vec3 specularTexture = vec3(texture(texture_specular, TexCoords));
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, currentTexture, specularTexture);
    }

    for(int i=0; i<NR_SPOT_LIGHTS; i++)
    {
        result += CalculateSpotLight(spotLights[i], norm, FragPos, viewDir, currentTexture, specularTexture);
    }

    float fogFactor = getFogFactor(distance(viewPos, FragPos));

    FragColor = mix(vec4(clamp(result, vec3(0.0f) ,vec3(1.0f)), 1.0f), vec4(fogColor, 1.0f), fogFactor);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 currentTexture, vec3 specularTexture) {

    vec3 ambient = currentTexture * light.ambient;
    vec3 lightDir = normalize(light.position - fragPos);
    float distanceToLight = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distanceToLight + light.quadratic * distanceToLight * distanceToLight);

    // diffuse
    float diff = abs(dot(normal, lightDir));
    vec3 diffuse = light.diffuse * diff * currentTexture;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 65.0f);
    vec3 specular = light.specular * spec * specularTexture;

    return (ambient + diffuse + specular) * attenuation;
}

float getFogFactor(float d)
{
    const float FogMax = 50.0;
    const float FogMin = 10.0;

    if (d>=FogMax) return 1;
    if (d<=FogMin) return 0;

    return 1 - (FogMax - d) / (FogMax - FogMin);
}

vec3 CalculateSpotLight(SpotLight light, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 currentTexture, vec3 specularTexture)
{
    vec3 ambient = light.ambient * currentTexture;
    vec3 lightDir = normalize(light.position - fragPos);

    float distanceToLight = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distanceToLight + light.quadratic * distanceToLight * distanceToLight);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = (light.cutOffClose - light.cutOffFar);
    float intensity = clamp((theta - light.cutOffFar) / epsilon, 0.0, 1.0);

    //intensity = 1.0f;

    // diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * currentTexture;

    // specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 65.0f);
    vec3 specular = light.specular * spec * specularTexture;

    //return (ambient + diffuse + specular)* attenuation;
    return ((ambient + diffuse + specular) * intensity)* attenuation;
}
