#version 330

layout (location = 0) in vec3 position;

uniform float g_scale;

void main()
{
    gl_Position = vec4(g_scale * position.x, g_scale * position.y, position.z, 1.0f);
}