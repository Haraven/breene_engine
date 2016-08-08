#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coord;
layout (location = 2) in vec3 normal;

uniform mat4 g_world_view_perspective;
uniform mat4 g_world;

out vec2 tex_coord_0;
out vec3 normal_0;
out vec3 world_pos_0;

void main()
{
    gl_Position = g_world_view_perspective * vec4(position, 1.0f);
    tex_coord_0 = tex_coord;
    normal_0    = (g_world * vec4(normal, 0.0f)).xyz;
    world_pos_0 = (g_world * vec4(position, 1.0f)).xyz;
}