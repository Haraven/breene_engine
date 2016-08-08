#version 330

uniform sampler2D g_color_map;

in vec2 tex_coord;

out vec4 frag_color;

void main()
{
    frag_color = texture2D(g_color_map, tex_coord);

    if (frag_color.r >= 0.9f && frag_color.g >= 0.9f && frag_color.b >= 0.9f) 
        discard;       
    if (frag_color.a < 0.5f)
        discard;
}