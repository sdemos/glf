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

in vec3 interp_normal;
in vec3 interp_pos;

out vec4 color;

uniform vec3 camera_pos;

uniform Light l; // light properties
uniform Material m; // material properties

void main ()
{
    vec3 normal = normalize(interp_normal);
    vec3 light_dir = normalize(l.p - interp_pos);
    vec3 view_dir = normalize(camera_pos - interp_pos);
    vec3 reflect_dir = reflect(-light_dir, normal);

    float diff = max(dot(normal, light_dir), 0.0);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), m.se);

    vec3 ambient  = l.a * m.a;
    vec3 diffuse  = l.d * (diff * m.d);
    vec3 specular = l.s * (spec * m.s);

    color = vec4(ambient + diffuse + specular, 1.0);
}
