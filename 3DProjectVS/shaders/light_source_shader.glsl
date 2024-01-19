//#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;

out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    FragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

//#shader fragment
#version 330 core

in vec3 FragPos;

out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 color;
uniform vec3 fogColor;

float getFogFactor(float d);

void main(){

    float d = distance(viewPos, FragPos);
    float fogFactor = getFogFactor(d);

    FragColor = mix(vec4(color, 1.0f), vec4(fogColor, 1.0f), fogFactor);
}

float getFogFactor(float d)
{
    const float FogMax = 50.0;
    const float FogMin = 10.0;

    if (d>=FogMax) return 1;
    if (d<=FogMin) return 0;

    return 1 - (FogMax - d) / (FogMax - FogMin);
}