#version 420 core

layout (vertices = 3) out;

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

uniform float g_tess_level;

in vec2 tex_coord_tesc[];
in vec3 normal_tesc[];
in vec3 world_pos_tesc[];

out vec3 normal_tese[3];
out vec2 tex_coord_tese[3];
out vec3 world_pos_tese[3];
out PNPatch patch_tese[3];

float wij(int i, int j)
{
    return dot(world_pos_tesc[j].xyz - world_pos_tesc[i].xyz, normal_tesc[i]);
}

float vij(int i, int j)
{
    vec3 pdiff = world_pos_tesc[j].xyz - world_pos_tesc[i].xyz;
    vec3 nsum = normal_tesc[i] + normal_tesc[j];

    return 2.0f * dot(pdiff, nsum) / dot(pdiff, pdiff);
}

void main()
{
    world_pos_tese[gl_InvocationID] = world_pos_tesc[gl_InvocationID];
    normal_tese[gl_InvocationID] = normal_tesc[gl_InvocationID];
    tex_coord_tese[gl_InvocationID] = tex_coord_tesc[gl_InvocationID];

    float p0 = world_pos_tesc[0][gl_InvocationID];
    float p1 = world_pos_tesc[1][gl_InvocationID];
    float p2 = world_pos_tesc[2][gl_InvocationID];

    float n0 = normal_tesc[0][gl_InvocationID];
    float n1 = normal_tesc[1][gl_InvocationID];
    float n2 = normal_tesc[2][gl_InvocationID];

    patch_tese[gl_InvocationID].b210 = (2.0f * p0 + p1 - wij(0, 1) * n0) / 3.0f;
    patch_tese[gl_InvocationID].b120 = (2.0f * p1 + p0 - wij(1, 0) * n1) / 3.0f;
    patch_tese[gl_InvocationID].b021 = (2.0f * p1 + p2 - wij(1, 2) * n1) / 3.0f;
    patch_tese[gl_InvocationID].b012 = (2.0f * p2 + p1 - wij(2, 1) * n2) / 3.0f;
    patch_tese[gl_InvocationID].b102 = (2.0f * p2 + p0 - wij(2, 0) * n2) / 3.0f;
    patch_tese[gl_InvocationID].b201 = (2.0f * p0 + p2 - wij(0, 2) * n0) / 3.0f;

    float e = (patch_tese[gl_InvocationID].b210
        + patch_tese[gl_InvocationID].b120
        + patch_tese[gl_InvocationID].b021
        + patch_tese[gl_InvocationID].b012
        + patch_tese[gl_InvocationID].b102
        + patch_tese[gl_InvocationID].b201) / 6.0f;

    float v = (p0 + p1 + p2) / 3.0f;

    patch_tese[gl_InvocationID].b111 = e + (e - v) * 0.5f;
    patch_tese[gl_InvocationID].n110 = n0 + n1 - vij(0, 1) * (p1 - p0);
    patch_tese[gl_InvocationID].n011 = n1 + n2 - vij(1, 2) * (p2 - p1);
    patch_tese[gl_InvocationID].n101 = n2 + n0 - vij(2, 0) * (p0 - p2);

    gl_TessLevelOuter[gl_InvocationID] = g_tess_level;
    gl_TessLevelInner[gl_InvocationID] = g_tess_level;
}