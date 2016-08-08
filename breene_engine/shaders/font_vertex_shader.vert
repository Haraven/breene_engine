#version 330

layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>

out vec2 tex_coord;

uniform mat4 g_ortho_proj;

void main()
{
    gl_Position = g_ortho_proj * vec4(vertex.xy, 0.0f, 1.0f);
    tex_coord = vertex.zw;
}  