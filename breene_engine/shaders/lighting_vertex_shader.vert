#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coord;
layout (location = 2) in vec3 normal;

out vec2 tex_coord_0;
out vec3 normal_0;
out vec3 world_pos_0;

uniform mat4 g_wvp;
uniform mat4 g_world;

void main()
{
    gl_Position = g_wvp * vec4(position, 1.0f);
    tex_coord_0 = tex_coord;
    normal_0    = (g_world * vec4(normal, 0.0f)).xyz;
    world_pos_0  = (g_world * vec4(position, 1.0f)).xyz;
}