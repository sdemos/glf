#version 330 core

layout (location = 0) in vec3 vposition;
layout (location = 1) in vec3 vnormal;
layout (location = 2) in vec2 vtex;

out vec3 pos;
out vec3 normal;
out vec2 tex;

uniform mat4 model;
uniform mat4 mvp;

void main ()
{
    normal = mat3(transpose(inverse(model))) * vnormal;

    pos = vec3(model * vec4(vposition, 1.0));

    tex = vtex;

    gl_Position = mvp * vec4(vposition, 1.0);
}
