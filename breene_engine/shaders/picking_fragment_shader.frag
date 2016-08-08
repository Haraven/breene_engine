#version 330

uniform uint g_draw_index;
uniform uint g_object_index;

out vec3 frag_color;

void main()
{
   frag_color = vec3(float(g_object_index), float(g_draw_index), float(gl_PrimitiveID + 1));
}