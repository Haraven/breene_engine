#version 330

in vec3 tex_coord_0;

out vec4 frag_color;

uniform samplerCube g_cubemap_tex;

void main()
{
    frag_color = texture(g_cubemap_tex, tex_coord_0);
}