#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coord;
layout (location = 2) in vec3 normal;
layout (location = 4) in mat4 wvp;
layout (location = 8) in mat4 world;

out vec2 tex_coord_tesc;
out vec3 normal_tesc;
out vec3 world_pos_tesc;
out mat4 wvp_tesc;
flat out int instance_id_tesc;

void main()
{
    //gl_Position    = vec4(position, 1.0f);
	wvp_tesc         = wvp;
    tex_coord_tesc   = tex_coord;
    normal_tesc      = normal;
    world_pos_tesc   = position;
    instance_id_tesc = gl_InstanceID;
}