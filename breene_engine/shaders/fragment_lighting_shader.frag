#version 330

in vec2 tex_coord_0;

out vec4 frag_color;

struct DirectionalLight
{
    vec3 color;
    float ambient_intensity;
};

uniform DirectionalLight g_directional_light;
uniform sampler2D g_sampler;

void main()
{
    frag_color = texture2D(g_sampler, tex_coord_0.xy) * vec4(g_directional_light.color, 1.0f) * g_directional_light.ambient_intensity;
}