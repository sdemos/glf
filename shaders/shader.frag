#version 330 core

struct Material {
    vec3 a;   // ambient material color
    vec3 d;   // diffuse material color
    vec3 s;   // specular highlight properties
    float se; // specular exponent
};

struct Light {
    vec3 p; // light position
    vec3 a; // light ambient component
    vec3 d; // light diffuse component
    vec3 s; // light specular component
};

in vec3 normal;
in vec3 view;

out vec4 color;

uniform Light l; // light properties
uniform Material m; // material properties

void main ()
{
    vec3 n = normalize(normal);
    vec3 v = normalize(view);
    vec3 light = normalize(normalize(l.p) - v);
    vec3 reflection = normalize(reflect(-light, n));

    float diff =     max(dot( n, light),      0.0);
    float spec = pow(max(dot(-v, reflection), 0.0), m.se);

    vec3 ambient  = l.a * m.a;
    vec3 diffuse  = l.d * (diff * m.d);
    vec3 specular = l.s * (spec * m.s);

    //color = vec4(ambient + diffuse + specular, 1.0);
    color = vec4(n, 1.0);
}
