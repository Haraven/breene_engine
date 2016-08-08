#version 420 core

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS  = 3;
const int SHADOWMAP_SAMPLES = 16;
const float SHADOWMAP_BIAS = 0.00001f;
const vec2 POISSON_DISK[16] = vec2[]
(
   vec2(-0.9328896, -0.03145855), // left check offset
   vec2(0.8162807, -0.05964844),  // right check offset
   vec2(-0.184551, 0.9722522),    // top check offset
   vec2(0.04031969, -0.8589798),  // bottom check offset
   vec2(-0.91588581, 0.45771432),
   vec2(-0.81544232, -0.87912464),
   vec2(-0.38277543, 0.27676845),
   vec2(0.97484398, 0.75648379),
   vec2(0.44323325, -0.97511554),
   vec2(0.53742981, -0.47373420),
   vec2(-0.26496911, -0.41893023),
   vec2(0.79197514, 0.19090188),
   vec2(-0.24188840, 0.99706507),
   vec2(-0.81409955, 0.91437590),
   vec2(0.19984126, 0.78641367),
   vec2(0.14383161, -0.14100790)
);
vec3 light_dir = vec3(0.0f);

in vec2 tex_coord_frag;
in vec3 w_pos_frag;
in vec3 normal_frag;

out vec4 frag_color;

struct BaseLight
{
    vec3 color;
    float ambient_intensity;
    float diffuse_intensity;
};

struct DirectionalLight
{
    BaseLight base;
    vec3 direction;
};

struct LightAttenuation
{
    float constant;
    float linear;
    float exponential;
};

struct PointLight
{
    BaseLight base;
    vec3 position;
    LightAttenuation attenuation;
};

struct SpotLight
{
    PointLight base;
    vec3 direction;
    float cone_angle;
};

uniform DirectionalLight g_directional_light;

uniform int g_point_lights_count;
uniform PointLight g_point_lights[MAX_POINT_LIGHTS];

uniform int g_spot_lights_count;
uniform SpotLight g_spot_lights[MAX_SPOT_LIGHTS];

uniform float g_specular_intensity;
uniform float g_specular_power;

uniform sampler2D g_color_map;
uniform sampler2D g_shadow_map;
uniform sampler2D g_normal_map;

uniform vec3 g_eye_world_pos;

float Random(vec3 seed, int freq)
{
    vec4 tmp = vec4(seed, freq);
    float dot_prod = dot(tmp, vec4(12.9898f, 78.233f, 45.164f, 94.673f));
    return fract(sin(dot_prod) * 43758.5453f);
}

//vec3 CalcBumpedNormal()
//{
//    vec3 normal = normalize(normal_frag);
//    vec3 normalized_tangent = normalize(tangent_frag);
//    vec3 tangent = normalize(normalized_tangent - dot(normalized_tangent, normal) * normal);
//    vec3 bitangent = cross(tangent, normal);
    
//    vec3 bump_map_normal = 2.0f * texture(g_normal_map, tex_coord_frag).xyz - vec3(1.0f);
//    mat3 tbn = mat3(tangent, bitangent, normal);
//    vec3 res = normalize(tbn * bump_map_normal);

//    return res;
//}

float CalcShadowFactor(vec4 light_pos, vec3 normal)
{
    float bias = 0.005f;
    if (light_dir.x != 0.0f || light_dir.y != 0.0f || light_dir.z != 0.0f)
    {
        vec3 normalized_dir = normalize(light_dir);
        light_dir = vec3(0.0f);
        if (dot(normal, normalized_dir) == 0.0f)
            return 0.0f;
    
        float cos_theta = clamp(dot(normal, light_dir), 0.0f, 1.0f);
        bias = 0.005f * tan(acos(cos_theta));
        bias = clamp(bias, 0.0f, 0.00001f);
        light_dir = vec3(0.0f);
    }

    vec3 projection_coord = light_pos.xyz / light_pos.w;
    vec3 uv_coord;
    uv_coord.x = 0.5f * projection_coord.x + 0.5f;
    uv_coord.y = 0.5f * projection_coord.y + 0.5f;
    uv_coord.z = 0.5f * projection_coord.z + 0.5f;
    
    float visibility = 1.0f;

    for (int i = 0; i < SHADOWMAP_SAMPLES; ++i)
    {
        int index = int(16.0f * Random(floor(w_pos_frag * 1000.0f), i)) % 16;

        float depth = texture(g_shadow_map, POISSON_DISK[index] / 700.0f + uv_coord.xy).x;
    
        if (depth < uv_coord.z - bias)
            visibility -= 1.0f / SHADOWMAP_SAMPLES;
    }

    return visibility;
}

vec4 CalcLightInternal(BaseLight light, vec3 light_direction, vec3 normal, float shadow_factor)
{
    vec4 ambient_color = vec4(light.color * light.ambient_intensity, 1.0f);
    float diffuse_factor = dot(normal, -light_direction);

    vec4 diffuse_color  = vec4(0, 0, 0, 0);
    vec4 specular_color = vec4(0, 0, 0, 0);

    if (diffuse_factor > 0)
    {
        diffuse_color = vec4(light.color * light.diffuse_intensity * diffuse_factor, 1.0f);

        vec3 vertex_to_eye    = normalize(g_eye_world_pos - w_pos_frag);
        vec3 light_reflection = normalize(reflect(light_direction, normal));
        float specular_factor = dot(vertex_to_eye, light_reflection);
        if (specular_factor > 0.0f)
        {
            specular_factor = pow(specular_factor, g_specular_power);
            specular_color  = vec4(light.color * g_specular_intensity * specular_factor, 1.0f);
        }
    }

    return (ambient_color + shadow_factor * (diffuse_color + specular_color));
}

vec4 CalcDirectionalLight(vec3 normal)
{
    return CalcLightInternal(g_directional_light.base, g_directional_light.direction, normal, 1.0f);
}

vec4 CalcPointLight(PointLight light, vec3 normal)//, vec4 light_pos)
{
    vec3 light_direction = w_pos_frag - light.position;
    float distance = length(light_direction);
    light_direction = normalize(light_direction);
    //float shadow_factor = CalcShadowFactor(light_pos, normal);
    float shadow_factor = 1.0f;

    vec4 color = CalcLightInternal(light.base, light_direction, normal, shadow_factor);
    float attenuation =  light.attenuation.constant +
                         light.attenuation.linear * distance +
                         light.attenuation.exponential * distance * distance;

    return color / attenuation;
}

vec4 CalcSpotLight(SpotLight light, vec3 normal)//, vec4 light_pos)
{
    vec3 light_to_pixel = normalize(w_pos_frag - light.base.position);
    float spotlight_factor = dot(light_to_pixel, light.direction);

    vec4 spotlight_color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    if (spotlight_factor > light.cone_angle)
    {
        light_dir = light.direction;
        vec4 color = CalcPointLight(light.base, normal);//, light_pos);
        spotlight_color = color * (1.0f - (1.0f - spotlight_factor) / (1.0f - light.cone_angle));
    }

    return spotlight_color;
}

void main()
{
    vec3 normal = normalize(normal_frag); //CalcBumpedNormal();
    vec4 total_light = CalcDirectionalLight(normal);

    for (int i = 0 ; i < g_point_lights_count; ++i)
        total_light += CalcPointLight(g_point_lights[i], normal);//, light_space_pos_frag);

    for (int i = 0; i < g_spot_lights_count; ++i)
        total_light += CalcSpotLight(g_spot_lights[i], normal);//, light_space_pos_frag);

    vec4 sampled_color = texture(g_color_map, tex_coord_frag);
    frag_color = sampled_color * total_light;
}