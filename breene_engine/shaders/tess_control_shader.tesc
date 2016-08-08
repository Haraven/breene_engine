#version 410 core

layout (vertices = 1) out;

uniform float g_tess_level;

in vec2 tex_coord_tesc[];
in vec3 w_pos_tesc[];
in vec3 normal_tesc[];

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

out patch PatchStruct patch_tese;

float TesselationLevel(float dist0, float dist1)
{
    float average = (dist0 + dist1) / 2.0f;

    if (average <= 2.0f)
        return 10.0f;
    else if (average <= 5.0f)
        return 7.0f;
    return 3.0f;
}

vec3 ProjectToPlane(vec3 point, vec3 plane_point, vec3 plane_normal)
{
    vec3 v = point - plane_point;
    float len = dot(v, plane_normal);
    vec3 d = len * plane_normal;
    return (point - d);
}

void CalcPositions()
{
    // The original vertices stay the same
    patch_tese.w_pos_b030 = w_pos_tesc[0];
    patch_tese.w_pos_b003 = w_pos_tesc[1];
    patch_tese.w_pos_b300 = w_pos_tesc[2];

    // Edges are names according to the opposing vertex
    vec3 edge_b300 = patch_tese.w_pos_b003 - patch_tese.w_pos_b030;
    vec3 edge_b030 = patch_tese.w_pos_b300 - patch_tese.w_pos_b003;
    vec3 edge_b003 = patch_tese.w_pos_b030 - patch_tese.w_pos_b300;

    // Generate two midpoints on each edge
    patch_tese.w_pos_b021 = patch_tese.w_pos_b030 + edge_b300 / 3.0f;
    patch_tese.w_pos_b012 = patch_tese.w_pos_b030 + edge_b300 * 2.0f / 3.0f;
    patch_tese.w_pos_b102 = patch_tese.w_pos_b003 + edge_b030 / 3.0f;
    patch_tese.w_pos_b201 = patch_tese.w_pos_b003 + edge_b030 * 2.0f / 3.0f;
    patch_tese.w_pos_b210 = patch_tese.w_pos_b300 + edge_b003 / 3.0f;
    patch_tese.w_pos_b120 = patch_tese.w_pos_b300 + edge_b003 * 2.0f / 3.0f;

    // Project each midpoint on the plane defined by the nearest vertex and its normal
    patch_tese.w_pos_b021 = ProjectToPlane(patch_tese.w_pos_b021, patch_tese.w_pos_b030, patch_tese.normal[0]);
    patch_tese.w_pos_b012 = ProjectToPlane(patch_tese.w_pos_b012, patch_tese.w_pos_b003, patch_tese.normal[1]);
    patch_tese.w_pos_b102 = ProjectToPlane(patch_tese.w_pos_b102, patch_tese.w_pos_b003, patch_tese.normal[1]);
    patch_tese.w_pos_b201 = ProjectToPlane(patch_tese.w_pos_b201, patch_tese.w_pos_b300, patch_tese.normal[2]);
    patch_tese.w_pos_b210 = ProjectToPlane(patch_tese.w_pos_b210, patch_tese.w_pos_b300, patch_tese.normal[2]);
    patch_tese.w_pos_b120 = ProjectToPlane(patch_tese.w_pos_b120, patch_tese.w_pos_b030, patch_tese.normal[0]);

    // Handle the center
    vec3 center = (patch_tese.w_pos_b003 + patch_tese.w_pos_b030 + patch_tese.w_pos_b300) / 3.0f;
    patch_tese.w_pos_b111 = (patch_tese.w_pos_b021 + patch_tese.w_pos_b012 + patch_tese.w_pos_b102 +
                          patch_tese.w_pos_b201 + patch_tese.w_pos_b210 + patch_tese.w_pos_b120) / 6.0f;
    patch_tese.w_pos_b111 += (patch_tese.w_pos_b111 - center) / 2.0f;
}

void main()
{
    for (int i = 0; i < 3; ++i)
    {
        patch_tese.normal[i] = normal_tesc[i];
        patch_tese.tex_coord[i] = tex_coord_tesc[i];
    }

    CalcPositions();

    gl_TessLevelOuter[0] = g_tess_level;
    gl_TessLevelOuter[1] = g_tess_level;
    gl_TessLevelOuter[2] = g_tess_level;
    gl_TessLevelInner[0] = g_tess_level;

//    tex_coord_tese[gl_InvocationID] = tex_coord_tesc[gl_InvocationID];
//    w_pos_tese[gl_InvocationID] = w_pos_tesc[gl_InvocationID];
//    normal_tese[gl_InvocationID] = normal_tesc[gl_InvocationID];
//    tangent_tese[gl_InvocationID] = tangent_tesc[gl_InvocationID];
//    light_space_pos_tese[gl_InvocationID] = light_space_pos_tesc[gl_InvocationID];

//    float eye_to_vertex_dist0 = distance(g_eye_world_pos, w_pos_tese[0]);
//    float eye_to_vertex_dist1 = distance(g_eye_world_pos, w_pos_tese[1]);
//    float eye_to_vertex_dist2 = distance(g_eye_world_pos, w_pos_tese[2]);

//    gl_TessLevelOuter[0] = TesselationLevel(eye_to_vertex_dist1, eye_to_vertex_dist2);
//    gl_TessLevelOuter[1] = TesselationLevel(eye_to_vertex_dist2, eye_to_vertex_dist0);
//    gl_TessLevelOuter[2] = TesselationLevel(eye_to_vertex_dist0, eye_to_vertex_dist1);
//    gl_TessLevelInner[0] = gl_TessLevelOuter[2];
}