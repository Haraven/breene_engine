#version 330

in vec2 tex_coord_frag;
in vec3 normal_frag;
in vec3 world_pos_frag;

layout (location = 0) out vec3 world_pos_out;
layout (location = 1) out vec3 diffuse_out;
layout (location = 2) out vec3 normal_out;
layout (location = 3) out vec3 tex_coord_out;

uniform sampler2D g_color_map;

void main()
{
	world_pos_out = world_pos_frag;
	diffuse_out   = texture(g_color_map, tex_coord_frag).xyz;
	normal_out    = normalize(normal_frag);
	tex_coord_out = vec3(tex_coord_frag, 0.0f);
}
