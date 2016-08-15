#version 420 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coord;
layout (location = 2) in vec3 normal;

uniform mat4 g_wvp;
uniform mat4 g_world;

out vec2 tex_coord_frag;
out vec3 normal_frag;
out vec3 world_pos_frag;

void main()
{       
    gl_Position    = g_wvp * vec4(position, 1.0f);
    tex_coord_frag   = tex_coord;
    normal_frag      = (g_world * vec4(normal, 0.0f)).xyz;
    world_pos_frag   = (g_world * vec4(position, 1.0f)).xyz;
}