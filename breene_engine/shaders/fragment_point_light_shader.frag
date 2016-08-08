#version 330
 
const int MAX_LIGHTS = 8;                                                     
 
in vec2 tex_coord_0;
in vec3 normal_0;
in vec3 world_pos_0;

out vec4 frag_color;                                                             
 
struct BaseLight                                                                    
{
    vec3 color;                                                                     
    float ambient_intensity;                                                         
    float diffuse_intensity;                                                         
};       
 
struct DirectionalLight                                                             
{
    BaseLight base;                                                                 
    vec3 direction;                                                                 
};       
 
struct LightAttenuation                                                                  
{                        
    float constant;                                                                 
    float linear;                                                                   
    float exponential;
};
 
struct PointLight
{
    BaseLight base; 
    vec3 position;  
    LightAttenuation attenuation;
};       
 
uniform int g_point_lights_count;                                                                
uniform DirectionalLight g_directional_light;                                                 
uniform PointLight g_point_lights[MAX_LIGHTS];
uniform sampler2D g_sampler;
uniform vec3 g_eye_world_pos;
uniform float g_specular_intensity;
uniform float g_specular_power;                                                           
 
vec4 CalcLightInternal(BaseLight light, vec3 light_direction, vec3 normal)                   
{
    vec4 ambient_color = vec4(light.color * light.ambient_intensity, 1.0f);
    float diffuse_factor = dot(normal, -light_direction);                                     
 
    vec4 diffuse_color  = vec4(0, 0, 0, 0);                                                  
    vec4 specular_color = vec4(0, 0, 0, 0);                                                  
 
    if (diffuse_factor > 0) 
    {                                                                
        diffuse_color = vec4(light.color * light.diffuse_intensity * diffuse_factor, 1.0f);    
 
        vec3 vertex_to_eye    = normalize(g_eye_world_pos - world_pos_0);                             
        vec3 light_reflection = normalize(reflect(light_direction, normal));                     
        float specular_factor = dot(vertex_to_eye, light_reflection);                                      
        if (specular_factor > 0.0f) 
        {                                                           
            specular_factor = pow(specular_factor, g_specular_power);
            specular_color  = vec4(light.color * g_specular_intensity * specular_factor, 1.0f);
        }
    }    
 
    return (ambient_color + diffuse_color + specular_color);                                   
}
 
vec4 CalcDirectionalLight(vec3 normal)                                                      
{
    return CalcLightInternal(g_directional_light.base, g_directional_light.direction, normal); 
}
 
vec4 CalcPointLight(int index, vec3 normal)                                                 
{
    vec3 light_direction = world_pos_0 - g_point_lights[index].position;
    float distance = length(light_direction);
    light_direction = normalize(light_direction);

    vec4 color = CalcLightInternal(g_point_lights[index].base, light_direction, normal);
    float attenuation =  g_point_lights[index].attenuation.constant +            
                         g_point_lights[index].attenuation.linear * distance +
                         g_point_lights[index].attenuation.exponential * distance * distance;

    return color / attenuation;
}
 
void main()      
{
    vec3 normal = normalize(normal_0);                                                       
    vec4 total_light = CalcDirectionalLight(normal);                                         
 
    for (int i = 0 ; i < g_point_lights_count ; ++i) 
        total_light += CalcPointLight(i, normal);
 
    frag_color = texture2D(g_sampler, tex_coord_0.xy) * total_light;                             
}
