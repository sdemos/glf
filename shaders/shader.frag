#version 330 core

in vec3 interp_normal;
in vec3 interp_pos;

out vec4 color;

uniform vec3 object_color;
uniform vec3 light_color;

uniform vec3 camera_pos;
uniform vec3 light_pos;

void main ()
{
    float ambi_strength = 0.1;
    float spec_strength = 0.5;

    vec3 normal = normalize(interp_normal);
    vec3 light_dir = normalize(light_pos - interp_pos);
    vec3 view_dir = normalize(camera_pos - interp_pos);
    vec3 reflect_dir = reflect(-light_dir, normal);

    vec3 ambient = ambi_strength * light_color;
    vec3 diffuse = max(dot(normal, light_dir), 0.0) * light_color;
    vec3 specular = pow(max(dot(view_dir, reflect_dir), 0.0), 32) * spec_strength * light_color;

    vec3 light = ambient + diffuse + specular;
    color = vec4(light * object_color, 1.0);
}
