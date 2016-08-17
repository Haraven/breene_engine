#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coord;

uniform mat4 g_wvp;

out vec2 tex_coord_frag;

void main()
{
	gl_Position = g_wvp * vec4(position, 1.0f);
    tex_coord_frag = vec2(tex_coord.x, 1.0f - tex_coord.y); 
}