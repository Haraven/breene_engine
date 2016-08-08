#version 330

layout (location = 0) in vec3 position;

uniform mat4 g_world_view_perspective;

smooth out vec4 color;

void main()
{
    gl_Position = g_world_view_perspective * vec4(position, 1.0f);
    color = vec4(clamp(position, 0.0f, 1.0f), 1.0f);
}