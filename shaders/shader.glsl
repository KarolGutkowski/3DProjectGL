//#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 color;
out vec3 normal;
out vec3 fragPos;

void main(){
        gl_Position = projection * view * model * vec4(aPos, 1.0);
        normal = mat3(transpose(inverse(model)))*aNormal;
        fragPos = vec3(model * vec4(aPos, 1.0));
        color = aPos;
}

//#shader fragment
#version 330 core

in vec3 color;
in vec3 normal;
in vec3 fragPos;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main(){

        vec3 color = vec3(1.0f, 0.5f, 0.31f);
        vec3 ambient = color*0.2f;
        vec3 norm = normalize(normal);

        vec3 lightDir = normalize(lightPos - fragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = color * diff * 0.5;


        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 40.0f);
        vec3 specular = spec * color;

        vec3 result = ambient + diffuse + specular;

        FragColor = vec4(result, 0.2f);
}
