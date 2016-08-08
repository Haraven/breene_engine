#version 330

layout (location = 0) in float type;
layout (location = 1) in vec3 position;
layout (location = 2) in vec3 velocity;
layout (location = 3) in float age;

out float type_0;
out vec3 pos_0;
out vec3 vel_0;
out float age_0;

void main()
{
    type_0 = type;
    pos_0 = position;
    vel_0 = velocity;
    age_0 = age;
}