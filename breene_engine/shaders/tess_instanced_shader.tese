#version 420 core

struct PNPatch
{
    float b210;
    float b120;
    float b021;
    float b012;
    float b102;
    float b201;
    float b111;
    float n110;
    float n011;
    float n101;
};

uniform float g_tess_alpha;

layout (triangles, fractional_odd_spacing, ccw) in;

in vec2 tex_coord_tese[];
in vec3 normal_tese[];
in vec3 world_pos_tese[];
in mat4 wvp_tese[];
in PNPatch patch_tese[];
flat in int instance_id_tese[];

out vec2 tex_coord_frag;
out vec3 world_pos_frag;
out vec3 normal_frag;
flat out int instance_id_frag;

#define b300 world_pos_tese[0]
#define b030 world_pos_tese[1]
#define b003 world_pos_tese[2]
#define n200 normal_tese[0]
#define n020 normal_tese[1]
#define n002 normal_tese[2]
#define uvw  gl_TessCoord

void main()
{
	instance_id_frag = instance_id_tese[0];

    vec3 uvw2 = uvw * uvw;
    vec3 uvw3 = uvw2 * uvw;

    vec3 b210 = vec3(patch_tese[0].b210, patch_tese[1].b210, patch_tese[2].b210);
    vec3 b120 = vec3(patch_tese[0].b120, patch_tese[1].b120, patch_tese[2].b120);
    vec3 b021 = vec3(patch_tese[0].b021, patch_tese[1].b021, patch_tese[2].b021);
    vec3 b012 = vec3(patch_tese[0].b012, patch_tese[1].b012, patch_tese[2].b012);
    vec3 b102 = vec3(patch_tese[0].b102, patch_tese[1].b102, patch_tese[2].b102);
    vec3 b201 = vec3(patch_tese[0].b201, patch_tese[1].b201, patch_tese[2].b201);
    vec3 b111 = vec3(patch_tese[0].b111, patch_tese[1].b111, patch_tese[2].b111);

    vec3 n110 = normalize(vec3(patch_tese[0].n110, patch_tese[1].n110, patch_tese[2].n110));
    vec3 n011 = normalize(vec3(patch_tese[0].n011, patch_tese[1].n011, patch_tese[2].n011));
    vec3 n101 = normalize(vec3(patch_tese[0].n101, patch_tese[1].n101, patch_tese[2].n101));

    // compute texcoords
    tex_coord_frag = gl_TessCoord[2] * tex_coord_tese[0]
        + gl_TessCoord[0] * tex_coord_tese[1]
        + gl_TessCoord[1] * tex_coord_tese[2];

    // normal
    vec3 normal = gl_TessCoord[2] * normal_tese[0]
        + gl_TessCoord[0] * normal_tese[1]
        + gl_TessCoord[1] * normal_tese[2];

    vec3 pn_normal = n200 * uvw2[2]
        + n020 * uvw2[0]
        + n002 * uvw2[1]
        + n110 * uvw[2] * uvw[0]
        + n011 * uvw[0] * uvw[1]
        + n101 * uvw[2] * uvw[1];
    normal_frag = g_tess_alpha * pn_normal + (1.0f - g_tess_alpha) * normal;

    // compute interpolated pos
    vec3 position = gl_TessCoord[2] * b300
        + gl_TessCoord[0] * b030
        + gl_TessCoord[1] * b003;

    // save some computations
    uvw2 *= 3.0f;

    // compute PN position
    vec3 pn_position = b300 * uvw3[2]
    + b030 * uvw3[0]
    + b003 * uvw3[1]
    + b210 * uvw2[2] * uvw[0]
    + b120 * uvw2[0] * uvw[2]
    + b201 * uvw2[2] * uvw[1]
    + b021 * uvw2[0] * uvw[1]
    + b102 * uvw2[1] * uvw[2]
    + b012 * uvw2[1] * uvw[0]
    + b111 * 6.0f * uvw[0] * uvw[1] * uvw[2];

    // final position and normal
    vec3 final_pos = (1.0f - g_tess_alpha) * position + g_tess_alpha * pn_position;
    gl_Position = wvp_tese[0] * vec4(final_pos, 1.0f);
}
