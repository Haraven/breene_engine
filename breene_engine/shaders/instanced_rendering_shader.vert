#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coord;
layout (location = 2) in vec3 normal;
layout (location = 4) in mat4 wvp;
layout (location = 8) in mat4 world;

out vec2 tex_coord_0;
out vec3 normal_0;
out vec3 worldPos_0;
flat out int instance_id;

void main()
{
    gl_Position = wvp * vec4(position, 1.0f);
    tex_coord_0 = tex_coord;
    normal_0    = (world * vec4(normal, 0.0f)).xyz;
    worldPos_0  = (world * vec4(position, 1.0f)).xyz;
    instance_id = gl_InstanceID;
}