#version 420 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coord;
layout (location = 2) in vec3 normal;

out vec2 tex_coord_tesc;
out vec3 normal_tesc;
out vec3 world_pos_tesc;

void main()
{       
    tex_coord_tesc = tex_coord;
    normal_tesc    = normal;
    world_pos_tesc = position;
}