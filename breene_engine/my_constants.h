#pragma once

#include <glm\glm.hpp>

extern const unsigned long DEFAULT_WND_WIDTH;
extern const unsigned long DEFAULT_WND_HEIGHT;

enum UnitIndex
{
    COLOR_TEXTURE_UNIT_INDEX = 0,
    SHADOW_TEXTURE_UNIT_INDEX,
    NORMAL_TEXTURE_UNIT_INDEX,
    RANDOM_TEXTURE_UNIT_INDEX,
    DISPLACEMENT_TEXTURE_UNIT_INDEX,
    TEXT_TEXTURE_UNIT_INDEX,
	SKYBOX_TEXTURE_UNIT_INDEX = 7
};
extern const unsigned int COLOR_TEXTURE_UNIT;
extern const unsigned int SHADOW_TEXTURE_UNIT;
extern const unsigned int NORMAL_TEXTURE_UNIT;
extern const unsigned int RANDOM_TEXTURE_UNIT;
extern const unsigned int DISPLACEMENT_TEXTURE_UNIT;
extern const unsigned int TEXT_TEXTURE_UNIT;
extern const unsigned int POSITION_TEXTURE_UNIT;
extern const unsigned int SKYBOX_TEXTURE_UNIT;

extern const float Z_NEAR;
extern const float Z_FAR;
extern const float DEFAULT_FOV;
extern const float DEFAULT_SCALE;

extern const char LIGHTING_VERTEX_SHADER[];
extern const char LIGHTING_FRAGMENT_SHADER[];
extern const char SHADOW_VERTEX_SHADER[];
extern const char SHADOW_FRAGMENT_SHADER[];
extern const char SKYBOX_VERTEX_SHADER[];
extern const char SKYBOX_FRAGMENT_SHADER[];
extern const char BILLBOARD_VERTEX_SHADER[];
extern const char BILLBOARD_GEOMETRY_SHADER[];
extern const char BILLBOARD_FRAGMENT_SHADER[];
extern const char PARTSYSUPDATE_VERTEX_SHADER[];
extern const char PARTSYSUPDATE_GEOMETRY_SHADER[];
extern const char PARTSYSUPDATE_FRAGMENT_SHADER[];
extern const char PICKING_VERTEX_SHADER[];
extern const char PICKING_FRAGMENT_SHADER[];
extern const char PLAIN_VERTEX_SHADER[];
extern const char PLAIN_FRAGMENT_SHADER[];
extern const char BLANK_VERTEX_SHADER[];
extern const char BLANK_FRAGMENT_SHADER[];
extern const char TES_SHADER[];
extern const char TCS_SHADER[];
extern const char FONT_VERT_SHADER[];
extern const char FONT_FRAG_SHADER[];
extern const char DEFSHADING_GEOM_PASS_VERTEX_SHADER[];
extern const char DEFSHADING_GEOM_PASS_FRAGMENT_SHADER[];
extern const char DEFSHADING_GEOM_PASS_TESE_SHADER[];
extern const char DEFSHADING_GEOM_PASS_TESC_SHADER[];
extern const char DEFSHADING_LIGHT_PASS_VERTEX_SHADER[];
extern const char DEFSHADING_DIRLIGHT_PASS_FRAGMENT_SHADER[];
extern const char DEFSHADING_PTLIGHT_PASS_FRAGMENT_SHADER[];
extern const char DEFSHADING_SPOTLIGHT_PASS_FRAGMENT_SHADER[];
extern const char FXAA_VERT_SHADER[];
extern const char FXAA_FRAG_SHADER[];

extern const int INVALID_UNIFORM_LOCATION;

extern const char LIGHT_WVP_UNIFORM[];
extern const char W_UNIFORM[];
extern const char VP_UNIFORM[];
extern const char WVP_UNIFORM[];
extern const char OP_UNIFORM[];

extern const char CAMPOS_UNIFORM[];
extern const char COLORMAP_UNIFORM[];
extern const char SHADOWMAP_UNIFORM[];
extern const char NORMALMAP_UNIFORM[];
extern const char TEXTMAP_UNIFORM[];
extern const char CUBEMAP_UNIFORM[];
extern const char RANDTEX_UNIFORM[];
extern const char DISPLACEMENTMAP_UNIFORM[];
extern const char TESSLEVEL_UNIFORM[];
extern const char TESSALPHA_UNIFORM[];
extern const char POSITION_UNIFORM[];

extern const float LIGHT_INTENSITY_MIN;
extern const float LIGHT_INTENSITY_MAX;
extern const float SPECULAR_POWER_MIN;
extern const float SPECULAR_POWER_MAX;

extern const char DIRLIGHT_COLOR_UNIFORM[];
extern const char DIRLIGHT_AMBIENT_INTENSITY_UNIFORM[];
extern const char DIRLIGHT_DIRECTION_UNIFORM[];
extern const char DIRLIGHT_DIFFUSE_INTENSITY_UNIFORM[];
extern const char DIRLIGHT_SPECULAR_INTENSITY_UNIFORM[];
extern const char DIRLIGHT_SPECULAR_POWER_UNIFORM[];

extern const char PTLIGHT_COLOR_UNIFORM[];
extern const char PTLIGHT_POSITION_UNIFORM[];
extern const char PTLIGHT_AMBIENT_INTENSITY_UNIFORM[];
extern const char PTLIGHT_DIFFUSE_INTENSITY_UNIFORM[];
extern const char PTLIGHT_ATTEN_CONST_UNIFORM[];
extern const char PTLIGHT_ATTEN_LINEAR_UNIFORM[];
extern const char PTLIGHT_ATTEN_EXP_UNIFORM[];

extern const char SPOTLIGHT_COLOR_UNIFORM[];
extern const char SPOTLIGHT_AMBIENT_INTENSITY_UNIFORM[];
extern const char SPOTLIGHT_POSITION_UNIFORM[];
extern const char SPOTLIGHT_DIFFUSE_INTENSITY_UNIFORM[];
extern const char SPOTLIGHT_ATTEN_CONST_UNIFORM[];
extern const char SPOTLIGHT_ATTEN_LINEAR_UNIFORM[];
extern const char SPOTLIGHT_ATTEN_EXP_UNIFORM[];
extern const char SPOTLIGHT_DIRECTION_UNIFORM[];
extern const char SPOTLIGHT_CONE_UNIFORM[];

extern const char DIRLIGHT_EYE_WORLD_POS_UNIFORM[];
extern const char PTLIGHTS_UNIFORM[];
extern const char PTLIGHTS_COUNT_UNIFORM[];
extern const char SPOTLIGHTS_UNIFORM[];
extern const char SPOTLIGHTS_COUNT_UNIFORM[];
extern const char BILLBOARD_SIZE_UNIFORM[];
extern const char PARTSYSUPDATE_DELTA_TIME_UNIFORM[];
extern const char PARTSYSUPDATE_TIME_UNIFORM[];
extern const char PARTSYSUPDATE_LAUNCHER_LIFESPAN_UNIFORM[];
extern const char PARTSYSUPDATE_PARTICLEI_LIFESPAN_UNIFORM[];
extern const char PARTSYSUPDATE_PARTICLEII_LIFESPAN_UNIFORM[];
extern const char PICKING_DRAW_INDEX_UNIFORM[];
extern const char PICKING_OBJECT_INDEX_UNIFORM[];
extern const char DISPLACEMENT_FACTOR_UNIFORM[];
extern const char COLOR_UNIFORM[];
extern const char DISCARD_COLOR_UNIFORM[];
extern const char SCREENSIZE_UNIFORM[];
extern const char SKYBOX_BOOLEAN_UNIFORM[];
extern const char FXAA_TEX_INVERSE_SIZE_UNIFORM[];
extern const char FXAA_SPAN_MAX_UNIFORM[];
extern const char FXAA_REDUCTION_MIN_UNIFORM[];
extern const char FXAA_REDUCTION_MUL_UNIFORM[];

extern const int INVALID_VAL;

extern const glm::vec3 ORIGIN;

extern const glm::vec3 COLOR_WHITE;
extern const glm::vec3 COLOR_BLACK;
extern const glm::vec3 COLOR_RED;
extern const glm::vec3 COLOR_GREEN;
extern const glm::vec3 COLOR_BLUE;
extern const glm::vec3 COLOR_CYAN;
extern const glm::vec3 COLOR_MAGENTA;

extern const glm::vec3 EYE_ORIGIN;
extern const glm::vec3 LOOK_AT_ORIGIN;
extern const glm::vec3 UP_ORIGIN;

extern const float YAW;
extern const float PITCH;
extern const float ROLL;
extern const float MOVEMENT_SPEED_INCR;
extern const float MOVEMENT_SPEED_MAX;
extern const float MOUSE_SENSITIVTY;
extern const float PITCH_MIN;
extern const float PITCH_MAX;

enum RetCodes
{
    FAILURE = -1,
    SUCCESS,
    ERR_GLFW_INIT,
    ERR_GLEW_INIT,
    ERR_WND_CREATE,
    ERR_UNKNOWN
};
