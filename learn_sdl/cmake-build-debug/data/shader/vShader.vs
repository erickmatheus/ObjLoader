#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNor;

out vec3 normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 combined;

void main(){
    gl_Position = combined * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);

    fragPos = vec3(model * vec4(aPos, 1.0));

    normal = mat3(transpose(inverse(model))) * aNor;
}