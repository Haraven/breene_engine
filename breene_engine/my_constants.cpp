#include "my_constants.h"
#include <GL\glew.h>

const unsigned long DEFAULT_WND_WIDTH = 1920L;
const unsigned long DEFAULT_WND_HEIGHT = 1080L;

const float Z_NEAR = 1.0f;
const float Z_FAR = 1080.0f;
const float DEFAULT_FOV = 60.0f;
const float DEFAULT_SCALE = 0.0f;

const unsigned int COLOR_TEXTURE_UNIT = GL_TEXTURE0;
const unsigned int SHADOW_TEXTURE_UNIT = GL_TEXTURE1;
const unsigned int NORMAL_TEXTURE_UNIT = GL_TEXTURE2;
const unsigned int RANDOM_TEXTURE_UNIT = GL_TEXTURE3;
const unsigned int DISPLACEMENT_TEXTURE_UNIT = GL_TEXTURE4;
const unsigned int TEXT_TEXTURE_UNIT = GL_TEXTURE5;


const char LIGHTING_VERTEX_SHADER[] = "shaders/instanced_rendering_shader.vert";
const char LIGHTING_FRAGMENT_SHADER[] = "shaders/instanced_rendering_shader.frag";
const char SHADOW_VERTEX_SHADER[] = "shaders/shadow_shader.vert";
const char SHADOW_FRAGMENT_SHADER[] = "shaders/shadow_shader.frag";
const char SKYBOX_VERTEX_SHADER[] = "shaders/skybox_shader.vert";
const char SKYBOX_FRAGMENT_SHADER[] = "shaders/skybox_shader.frag";
const char BILLBOARD_VERTEX_SHADER[] = "shaders/billboard_vertex_shader.vert";
const char BILLBOARD_GEOMETRY_SHADER[] = "shaders/billboard_geometry_shader.geom";
const char BILLBOARD_FRAGMENT_SHADER[] = "shaders/billboard_fragment_shader.frag";
const char PARTSYSUPDATE_VERTEX_SHADER[] = "shaders/partsysupdate_vertex_shader.vert";
const char PARTSYSUPDATE_GEOMETRY_SHADER[] = "shaders/partsysupdate_geometry_shader.geom";
const char PARTSYSUPDATE_FRAGMENT_SHADER[] = "shaders/partsysupdate_fragment_shader.frag";
const char PICKING_VERTEX_SHADER[] = "shaders/picking_vertex_shader.vert";
const char PICKING_FRAGMENT_SHADER[] = "shaders/picking_fragment_shader.frag";
const char PLAIN_VERTEX_SHADER[] = "shaders/plain_vertex_shader.vert";
const char PLAIN_FRAGMENT_SHADER[] = "shaders/plain_fragment_shader.frag";
const char TES_SHADER[] = "shaders/tess_alt_shader.tese";
const char TCS_SHADER[] = "shaders/tess_alt_shader.tesc";
const char FONT_VERT_SHADER[] = "shaders/font_vertex_shader.vert";
const char FONT_FRAG_SHADER[] = "shaders/font_fragment_shader.frag";
const char DEFSHADING_GEOM_PASS_VERTEX_SHADER[] = "shaders/geom_pass_vertex_shader.vert";
const char DEFSHADING_GEOM_PASS_FRAGMENT_SHADER[] = "shaders/geom_pass_fragment_shader.frag";

const int INVALID_UNIFORM_LOCATION = 0xFFFFFFFF;

const char LIGHT_WVP_UNIFORM[] = "g_light_wvp";
const char W_UNIFORM[] = "g_world";
const char VP_UNIFORM[] = "g_vp";
const char WVP_UNIFORM[] = "g_wvp";
const char OP_UNIFORM[] = "g_ortho_proj";

const char CAMPOS_UNIFORM[] = "g_cam_pos";
const char COLORMAP_UNIFORM[] = "g_color_map";
const char SHADOWMAP_UNIFORM[] = "g_shadow_map";
const char NORMALMAP_UNIFORM[] = "g_normal_map";
const char TEXTMAP_UNIFORM[] = "g_text_map";
const char CUBEMAP_UNIFORM[] = "g_cubemap_tex";
const char RANDTEX_UNIFORM[] = "g_rand_tex";
const char DISPLACEMENTMAP_UNIFORM[] = "g_displacement_map";
const char TESSLEVEL_UNIFORM[] = "g_tess_level";
const char TESSALPHA_UNIFORM[] = "g_tess_alpha";

const float LIGHT_INTENSITY_MIN = 0.0f;
const float LIGHT_INTENSITY_MAX = 1.0f;
const float SPECULAR_POWER_MIN  = 0.0f;
const float SPECULAR_POWER_MAX  = 128.0f;
const char DIRLIGHT_EYE_WORLD_POS_UNIFORM[] = "g_eye_world_pos";
const char DIRLIGHT_COLOR_UNIFORM[] = "g_directional_light.base.color";
const char DIRLIGHT_AMBIENT_INTENSITY_UNIFORM[] = "g_directional_light.base.ambient_intensity";
const char DIRLIGHT_DIRECTION_UNIFORM[] = "g_directional_light.direction";
const char DIRLIGHT_DIFFUSE_INTENSITY_UNIFORM[] = "g_directional_light.base.diffuse_intensity";
const char DIRLIGHT_SPECULAR_INTENSITY_UNIFORM[] = "g_specular_intensity";
const char DIRLIGHT_SPECULAR_POWER_UNIFORM[] = "g_specular_power";
const char PTLIGHTS_UNIFORM[] = "g_point_lights";
const char PTLIGHTS_COUNT_UNIFORM[] = "g_point_lights_count";
const char SPOTLIGHTS_UNIFORM[] = "g_spot_lights";
const char SPOTLIGHTS_COUNT_UNIFORM[] = "g_spot_lights_count";
const char BILLBOARD_SIZE_UNIFORM[] = "g_billboard_size";
const char PARTSYSUPDATE_DELTA_TIME_UNIFORM[] = "g_delta_time";
const char PARTSYSUPDATE_TIME_UNIFORM[] = "g_time";
const char PARTSYSUPDATE_LAUNCHER_LIFESPAN_UNIFORM[] = "g_launcher_lifespan";
const char PARTSYSUPDATE_PARTICLEI_LIFESPAN_UNIFORM[] = "g_primary_particle_lifespan";
const char PARTSYSUPDATE_PARTICLEII_LIFESPAN_UNIFORM[] = "g_secondary_particle_lifespan";
const char PICKING_DRAW_INDEX_UNIFORM[] = "g_draw_index";
const char PICKING_OBJECT_INDEX_UNIFORM[] = "g_object_index";
const char DISPLACEMENT_FACTOR_UNIFORM[] = "g_displacement_factor";
const char COLOR_UNIFORM[] = "g_color";
const char DISCARD_COLOR_UNIFORM[] = "g_discard";

const int INVALID_VAL = 0xFFFFFFFF;

const glm::vec3 ORIGIN(0.0f);

const glm::vec3 COLOR_WHITE(1.0f);
const glm::vec3 COLOR_BLACK(0.0f);
const glm::vec3 COLOR_RED(1.0f, 0.0f, 0.0f);
const glm::vec3 COLOR_GREEN(0.0f, 1.0f, 0.0f);
const glm::vec3 COLOR_BLUE(0.0f, 0.0f, 1.0f);
const glm::vec3 COLOR_CYAN(0.0f, 1.0f, 1.0f);
const glm::vec3 COLOR_MAGENTA(1.0f, 0.0f, 1.0f);
