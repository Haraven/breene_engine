#version 330

in vec2 tex_coord_0;
in vec3 normal_0;
in vec3 world_pos_0;

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

uniform DirectionalLight g_directional_light;
uniform sampler2D g_sampler;
uniform vec3 g_eye_world_pos;
uniform float g_specular_intensity;
uniform float g_specular_power;

void main()
{
    vec4 ambient_color   = vec4(g_directional_light.base.color, 1.0f) * g_directional_light.base.ambient_intensity;
    vec3 light_direction = -g_directional_light.direction;
    vec3 normal          = normalize(normal_0);

    float diffuse_factor = dot(normal, light_direction);
    
    vec4 diffuse_color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    vec4 specular_color = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    if (diffuse_factor > 0.0f)
    {
        diffuse_color = vec4(g_directional_light.base.color, 1.0f) * g_directional_light.base.diffuse_intensity * diffuse_factor;

        vec3 vertex_to_eye    = normalize(g_eye_world_pos - world_pos_0);
        vec3 light_reflection = normalize(reflect(g_directional_light.direction, normal));
        float specular_factor = dot(vertex_to_eye, light_reflection);

        if (specular_factor > 0.0f)
        {
            specular_factor = pow(specular_factor, g_specular_power);
            specular_color = vec4(g_directional_light.base.color * g_specular_intensity * specular_factor, 1.0f);
        }
    }

    frag_color = texture2D(g_sampler, tex_coord_0.xy) * (ambient_color + diffuse_color + specular_color);
}