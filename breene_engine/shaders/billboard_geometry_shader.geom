#version 330

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

uniform mat4 g_vp;
uniform vec3 g_cam_pos;
uniform float g_billboard_size;

out vec2 tex_coord;

void main()
{
    vec3 pos = gl_in[0].gl_Position.xyz;
    vec3 to_cam = normalize(g_cam_pos - pos);
    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    vec3 right = cross(to_cam, up) * g_billboard_size; // reverse cross-product gives you left

    pos -= right;
    gl_Position = g_vp * vec4(pos, 1.0f);
    tex_coord = vec2(0.0f, 0.0f);
    EmitVertex();
    
    pos.y += g_billboard_size;
    gl_Position = g_vp * vec4(pos, 1.0f);
    tex_coord = vec2(0.0f, 1.0f);
    EmitVertex();
    
    pos.y -= g_billboard_size;
    pos += right;
    gl_Position = g_vp * vec4(pos, 1.0f);
    tex_coord = vec2(1.0f, 0.0f);
    EmitVertex();

    pos.y += g_billboard_size;
    gl_Position = g_vp * vec4(pos, 1.0f);
    tex_coord = vec2(1.0f, 1.0f);
    EmitVertex();

    EndPrimitive();
}