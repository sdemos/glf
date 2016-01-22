#version 330 core

layout (location = 0) in vec3 vposition;
layout (location = 1) in vec3 vnormal;

out vec3 normal;
out vec3 view;

uniform mat4 mv;
uniform mat4 mvp;

void main ()
{
    normal = mat3(transpose(inverse(mv))) * vnormal;

    vec4 v4 = mv * vec4(vposition, 1.0);
    view = normalize(v4.xyz / v4.w);

    gl_Position = mvp * vec4(vposition, 1.0);
}
