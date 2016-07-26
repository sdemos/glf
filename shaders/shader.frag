#version 330 core

struct Material {
    sampler2D d;  // diffuse map
    sampler2D s;  // specular highlight properties
    float     se; // specular exponent
};

struct Light {
    vec3 p; // light position
    vec3 direction; // spotlight direction
    float cutoff; // spotlight cutoff
    float outercutoff; // spotlight cutoff

    vec3 a; // light ambient component
    vec3 d; // light diffuse component
    vec3 s; // light specular component

    float c; // attenuation constant factor
    float l; // attenuation linear factor
    float q; // attenuation quadratic factor
};

in vec3 pos;
in vec3 normal;
in vec2 tex;

out vec4 color;

uniform vec3 camera_pos;

uniform Light l; // light properties
uniform Material m; // material properties

void main ()
{
    vec3 light_dir = normalize(l.p - pos);
    float theta = dot(light_dir, normalize(-l.direction));
    float epsilon = l.cutoff - l.outercutoff;
    float intensity = clamp((theta - l.outercutoff) / epsilon, 0.0, 1.0);

    vec3 normal = normalize(normal);
    //vec3 light_dir = normalize(l.p - pos);
    //vec3 light_dir = normalize(-l.p);
    vec3 view_dir = normalize(camera_pos - pos);
    vec3 reflect_dir = reflect(-light_dir, normal);

    float diff = max(dot(normal, light_dir), 0.0);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), m.se);

    float dist = length(l.p - pos);
    float attenuation = 1.0 / (l.c + l.l * dist + l.q * (dist * dist));

    vec3 ambient  = attenuation * l.a * vec3(texture(m.d, tex));
    vec3 diffuse  = intensity * attenuation * l.d * diff * vec3(texture(m.d, tex));
    vec3 specular = intensity * attenuation * l.s * spec * vec3(texture(m.s, tex));

    color = vec4(ambient + diffuse + specular, 1.0);
}
