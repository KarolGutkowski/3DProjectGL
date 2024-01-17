//#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 color;

void main(){
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    color = aPos;
}

//#shader fragment
#version 330 core

in vec3 color;
out vec4 FragColor;

void main(){
    FragColor = vec4(1.0f);
}