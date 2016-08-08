#version 330

in vec2 tex_coord_0;

out vec4 frag_color;

uniform sampler2D g_sampler;

void main()
{
    frag_color = texture2D(g_sampler, tex_coord_0.xy);
}