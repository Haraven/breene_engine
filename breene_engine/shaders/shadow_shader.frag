#version 330                                                                        
                                                                                    
in vec2 tex_coord_out;        
                                                        
uniform sampler2D g_shadow_map;                                                       
                                                                                    
out vec4 frag_color;                                                                 
                                                                                    
void main()                                                                         
{                                                                                   
    float depth = texture(g_shadow_map, tex_coord_out).x;                               
    depth = 1.0f - (1.0f - depth) * 25.0f;                                             
    frag_color = vec4(depth);                                                        
}