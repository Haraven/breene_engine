#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coord;

uniform mat4 g_world_view_perspective;

out vec2 tex_coord_0;

void main()
{
    gl_Position = g_world_view_perspective * vec4(position, 1.0f);
    tex_coord_0 = tex_coord;
}