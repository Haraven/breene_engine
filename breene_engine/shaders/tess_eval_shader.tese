#version 410 core

layout(triangles, equal_spacing, ccw) in;

//uniform float g_displacement_factor;
uniform mat4 g_vp;
//uniform sampler2D g_displacement_map;

struct PatchStruct
{
    vec3 w_pos_b030;
    vec3 w_pos_b021;
    vec3 w_pos_b012;
    vec3 w_pos_b003;
    vec3 w_pos_b102;
    vec3 w_pos_b201;
    vec3 w_pos_b300;
    vec3 w_pos_b210;
    vec3 w_pos_b120;
    vec3 w_pos_b111;
    vec3 normal[3];
    vec2 tex_coord[3];
};

in patch PatchStruct patch_tese;

out vec2 tex_coord_frag;
out vec3 w_pos_frag;
out vec3 normal_frag;

vec2 Interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 Interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

void main()
{
    tex_coord_frag = Interpolate2D(patch_tese.tex_coord[0], patch_tese.tex_coord[1], patch_tese.tex_coord[2]);
    normal_frag = Interpolate3D(patch_tese.normal[0], patch_tese.normal[1], patch_tese.normal[2]);

    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
    float w = gl_TessCoord.z;

    float u3 = pow(u, 3);
    float v3 = pow(v, 3);
    float w3 = pow(w, 3);
    float u2 = pow(u, 2);
    float v2 = pow(v, 2);
    float w2 = pow(w, 2);

    w_pos_frag = patch_tese.w_pos_b300 * w3 +
                    patch_tese.w_pos_b030 * u3 +
                    patch_tese.w_pos_b003 * v3 +
                    patch_tese.w_pos_b210 * 3.0f * w2 * u +
                    patch_tese.w_pos_b120 * 3.0f * w * u2 +
                    patch_tese.w_pos_b201 * 3.0f * w2 * v +
                    patch_tese.w_pos_b021 * 3.0f * u2 * v +
                    patch_tese.w_pos_b102 * 3.0f * w * v2 +
                    patch_tese.w_pos_b012 * 3.0f * u * v2 +
                    patch_tese.w_pos_b111 * 6.0f * w * u * v;

    gl_Position = g_vp * vec4(w_pos_frag, 1.0f);

//    tex_coord_frag = Interpolate2D(tex_coord_tese[0], tex_coord_tese[1], tex_coord_tese[2]);
//    w_pos_frag = Interpolate3D(w_pos_tese[0], w_pos_tese[1], w_pos_tese[2]);
//    normal_frag = normalize(Interpolate3D(normal_tese[0], normal_tese[1], normal_tese[2]));
//    tangent_frag = tangent_tese[0];
//    light_space_pos_frag = light_space_pos_tese[0];

//    float displacement = texture(g_displacement_map, tex_coord_frag.xy).x;
//    w_pos_frag += normal_frag * displacement * g_displacement_factor;
//    gl_Position = g_vp * vec4(w_pos_frag, 1.0f);
}