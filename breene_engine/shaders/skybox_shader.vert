#version 330

layout (location = 0) in vec3 position;

uniform mat4 g_wvp;

out vec3 tex_coord_0;

void main()
{
    vec4 wvp_pos = g_wvp * vec4(position, 1.0f);
    gl_Position = wvp_pos.xyww;
    tex_coord_0 = position;
}