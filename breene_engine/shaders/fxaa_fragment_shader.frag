#version 330

in vec2 tex_coord_frag;

out vec4 frag_color;

uniform sampler2D g_color_map;
uniform vec2 g_tex_inverse_size;
uniform float g_span_max;
uniform float g_reduction_min;
uniform float g_reduction_mul;

void main()
{
	vec3 luma = vec3(0.299f, 0.587f, 0.114f);
	float luma_top_left = dot(luma, texture2D(g_color_map, tex_coord_frag.xy + (vec2(-1.0f, -1.0f) * g_tex_inverse_size)).xyz);
	float luma_top_right = dot(luma, texture2D(g_color_map, tex_coord_frag.xy + (vec2(1.0f, -1.0f) * g_tex_inverse_size)).xyz);
	float luma_bottom_left = dot(luma, texture2D(g_color_map, tex_coord_frag.xy + (vec2(-1.0f, 1.0f) * g_tex_inverse_size)).xyz);
	float luma_bottom_right = dot(luma, texture2D(g_color_map, tex_coord_frag.xy + (vec2(1.0f, 1.0f) * g_tex_inverse_size)).xyz);
	float luma_middle = dot(luma, texture2D(g_color_map, tex_coord_frag.xy).xyz);

	vec2 direction;
	direction.x = -((luma_top_left + luma_top_right) - (luma_bottom_left + luma_bottom_right));
	direction.y = ((luma_top_left + luma_bottom_left) - (luma_top_right + luma_bottom_right));
	
	float dirReduce = max((luma_top_left + luma_top_right + luma_bottom_left + luma_bottom_right) * (g_reduction_mul * 0.25), g_reduction_min);
	float inverseDirAdjustment = 1.0/(min(abs(direction.x), abs(direction.y)) + dirReduce);
	
	direction = min(vec2(g_span_max, g_span_max), 
		max(vec2(-g_span_max, -g_span_max), direction * inverseDirAdjustment)) * g_tex_inverse_size;

	vec3 filter_1 = (1.0/2.0) * (
		texture2D(g_color_map, tex_coord_frag.xy + (direction * vec2(1.0f / 3.0f - 0.5f))).xyz +
		texture2D(g_color_map, tex_coord_frag.xy + (direction * vec2(2.0f / 3.0f - 0.5f))).xyz);

	vec3 filter_2 = filter_1 * (1.0f / 2.0f) + (1.0f/ 4.0f) * (
		texture2D(g_color_map, tex_coord_frag.xy + (direction * vec2(-0.5f))).xyz +
		texture2D(g_color_map, tex_coord_frag.xy + (direction * vec2(-0.5f))).xyz);

	float luma_min = min(luma_middle, min(min(luma_top_left, luma_top_right), min(luma_bottom_left, luma_bottom_right)));
	float luma_max = max(luma_middle, max(max(luma_top_left, luma_top_right), max(luma_bottom_left, luma_bottom_right)));
	float res = dot(luma, filter_2);
	
	if (res < luma_min || res > luma_max)
		frag_color = vec4(filter_1, 1.0f);
	else
		frag_color = vec4(filter_2, 1.0f);
}