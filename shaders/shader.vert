#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 interp_normal;
out vec3 interp_pos;

uniform mat4 model;
uniform mat4 mvp;

void main ()
{
    interp_normal = mat3(transpose(inverse(model))) * normal;
    interp_pos = vec3(model * vec4(position, 1.0));
    gl_Position = mvp * vec4(position, 1.0);
}
