#version 330

layout (location = 0) in vec3 position;

void main()
{
    gl_Position = vec4(0.5f * position.x, 0.5f * position.y, position.z, 1.0f);
}