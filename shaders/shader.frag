#version 330 core

in vec3 interpColor;
in vec2 interpTexCoord;

out vec4 color;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main ()
{
    //color = vec4(interpColor, 1.0);
    //color = texture(ourTexture, interpTexCoord) * vec4(interpColor, 1.0);
    color = mix(texture(ourTexture1, interpTexCoord), texture(ourTexture2, interpTexCoord), 0.2);
}
