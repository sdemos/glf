#version 330 core

in vec3 pos;
in vec3 normal;
in vec2 tex;

uniform vec3 camera_pos;

struct Material {
    sampler2D d;  // diffuse map
    sampler2D s;  // specular highlight properties
    float     se; // specular exponent
};
uniform Material m; // material properties

struct DirLight {
    vec3 direction; // directional light direction

    vec3 a; // directional light ambient component
    vec3 d; // directional light diffuse component
    vec3 s; // directional light specular component
};
uniform DirLight dir_light;

struct PointLight {
    vec3 p; // point light position

    vec3 a; // point light ambient component
    vec3 d; // point light diffuse component
    vec3 s; // point light specular component

    float c; // attenuation constant factor
    float l; // attenuation linear factor
    float q; // attenuation quadratic factor
};
#define NR_POINT_LIGHTS 4
uniform PointLight point_lights[NR_POINT_LIGHTS];

struct SpotLight {
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
uniform SpotLight spotlight;

out vec4 color;

vec3 CalcDirLight (DirLight l, vec3 normal, vec3 view_dir)
{
    vec3 light_dir = normalize(-l.direction);
    vec3 reflect_dir = reflect(-light_dir, normal);

    float diff = max(dot(normal, light_dir), 0.0);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), m.se);

    vec3 ambient  = l.a * vec3(texture(m.d, tex));
    vec3 diffuse  = l.d * diff * vec3(texture(m.d, tex));
    vec3 specular = l.s * spec * vec3(texture(m.s, tex));

    return ambient + diffuse + specular;
}

vec3 CalcPointLight (PointLight l, vec3 normal, vec3 pos, vec3 view_dir)
{
    vec3 light_dir = normalize(l.p - pos);
    vec3 reflect_dir = reflect(-light_dir, normal);

    float diff = max(dot(normal, light_dir), 0.0);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), m.se);

    float dist = length(l.p - pos);
    float attenuation = 1.0 / (l.c + l.l * dist + l.q * (dist * dist));

    vec3 ambient  = attenuation * l.a * vec3(texture(m.d, tex));
    vec3 diffuse  = attenuation * l.d * diff * vec3(texture(m.d, tex));
    vec3 specular = attenuation * l.s * spec * vec3(texture(m.s, tex));

    return ambient + diffuse + specular;
}

vec3 CalcSpotLight (SpotLight l, vec3 normal, vec3 pos, vec3 view_dir)
{
    vec3 light_dir = normalize(l.p - pos);
    vec3 reflect_dir = reflect(-light_dir, normal);

    float theta = dot(light_dir, normalize(-l.direction));
    float epsilon = l.cutoff - l.outercutoff;
    float intensity = clamp((theta - l.outercutoff) / epsilon, 0.0, 1.0);

    float diff = max(dot(normal, light_dir), 0.0);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), m.se);

    float dist = length(l.p - pos);
    float attenuation = 1.0 / (l.c + l.l * dist + l.q * (dist * dist));

    vec3 ambient  = attenuation * l.a * vec3(texture(m.d, tex));
    vec3 diffuse  = intensity * attenuation * l.d * diff * vec3(texture(m.d, tex));
    vec3 specular = intensity * attenuation * l.s * spec * vec3(texture(m.s, tex));

    return ambient + diffuse + specular;
}

void main ()
{
    vec3 normal = normalize(normal);
    vec3 view_dir = normalize(camera_pos - pos);

    vec3 total_light = CalcDirLight(dir_light, normal, view_dir);

    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        total_light += CalcPointLight(point_lights[i], normal, pos, view_dir);
    }

    total_light += CalcSpotLight(spotlight, normal, pos, view_dir);

    color = vec4(total_light, 1.0);
}
