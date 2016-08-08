#version 330

in vec2 tex_coord_0;
in vec3 normal_0;

out vec4 frag_color;

struct DirectionalLight
{
    vec3 color;
    float ambient_intensity;
    float diffuse_intensity;
    vec3 direction;
};

uniform DirectionalLight g_directional_light;
uniform sampler2D g_sampler;

void main()
{
    vec4 ambient_color   = vec4(g_directional_light.color, 1.0f) * g_directional_light.ambient_intensity; 
    float diffuse_factor = dot(normalize(normal_0), -g_directional_light.direction);

    vec4 diffuse_color;

    if (diffuse_factor > 0.0f)
    {
        diffuse_color = vec4(g_directional_light.color, 1.0f) * g_directional_light.diffuse_intensity * diffuse_factor;
    }
    else
    {
        diffuse_color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }

    frag_color = texture2D(g_sampler, tex_coord_0.xy) * (ambient_color + diffuse_color);
}