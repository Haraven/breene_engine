#version 330
in vec2 tex_coord;

out vec4 frag_color;

uniform sampler2D g_text_map;
uniform vec4 g_color;

void main()
{    
    vec4 sampled_color = vec4(1.0f, 1.0f, 1.0f, texture(g_text_map, tex_coord).r);
    frag_color = g_color * sampled_color;
}  