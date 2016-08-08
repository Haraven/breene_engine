#version 330

layout (location = 0) in vec3 position;

uniform mat4 g_world;

void main()
{
    gl_Position = g_world * vec4(position, 1.0);
}