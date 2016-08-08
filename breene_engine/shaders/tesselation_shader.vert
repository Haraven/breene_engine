#version 420 core

layout (location = 1) in vec2 tex_coord_vert;
layout (location = 0) in vec3 position_vert;
layout (location = 2) in vec3 normal_vert;
layout (location = 3) in vec3 tangent_vert;

out vec2 tex_coord_tesc;
out vec3 normal_tesc;
out vec3 w_pos_tesc;

void main()
{
    tex_coord_tesc = tex_coord_vert;
    normal_tesc = normal_vert;
    w_pos_tesc = position_vert;
}
