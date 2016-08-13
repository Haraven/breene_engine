#version 420 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coord;
layout (location = 2) in vec3 normal;

uniform mat4 g_wvp;
uniform mat4 g_world;

out vec2 tex_coord_tesc;
out vec3 normal_tesc;
out vec3 world_pos_tesc;

void main()
{       
    //gl_Position    = g_wvp * vec4(position, 1.0f);
    tex_coord_tesc   = tex_coord;
    normal_tesc      = (g_world * vec4(normal, 0.0f)).xyz;
    world_pos_tesc   = (g_world * vec4(position, 1.0f)).xyz;
}