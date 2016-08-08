#include "lighting.h"
#include <glm\gtc\type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "my_constants.h"

const GLfloat DEFAULT_CONSTANT_ATTENUATION = 1.0f;
const GLfloat DEFAULT_LINEAR_ATTENUATION   = 0.0f;
const GLfloat DEFAULT_EXP_ATTENUATION      = 0.0f;
const GLfloat DEFAULT_SPECULAR_POWER       = 32.0f;

gl_app::PointLightUniform gl_app::LightingProgram::GetNextUniform(const PointLight& light)
{
    std::string base_name = PTLIGHTS_UNIFORM;
    base_name += "[";
    base_name += std::to_string(_point_lights_count);
    base_name += "].";

    PointLightUniform unif;
    unif.color                   = GetUniformLocation(base_name + "base.color");
    unif.ambient_intensity       = GetUniformLocation(base_name + "base.ambient_intensity");
    unif.diffuse_intensity       = GetUniformLocation(base_name + "base.diffuse_intensity");
    unif.position                = GetUniformLocation(base_name + "position");
    unif.attenuation.constant    = GetUniformLocation(base_name + "attenuation.constant");
    unif.attenuation.linear      = GetUniformLocation(base_name + "attenuation.linear");
    unif.attenuation.exponential = GetUniformLocation(base_name + "attenuation.exponential");

    return unif;
}

gl_app::SpotLightUniform gl_app::LightingProgram::GetNextUniform(const SpotLight& light)
{
    std::string base_name = SPOTLIGHTS_UNIFORM;
    base_name += "[";
    base_name += std::to_string(_spot_lights_count);
    base_name += "].";

    SpotLightUniform unif;
    unif.color = GetUniformLocation(base_name + "base.base.color");
    unif.ambient_intensity = GetUniformLocation(base_name + "base.base.ambient_intensity");
    unif.diffuse_intensity = GetUniformLocation(base_name + "base.base.diffuse_intensity");
    unif.position = GetUniformLocation(base_name + "base.position");
    unif.attenuation.constant = GetUniformLocation(base_name + "base.attenuation.constant");
    unif.attenuation.linear = GetUniformLocation(base_name + "base.attenuation.linear");
    unif.attenuation.exponential = GetUniformLocation(base_name + "base.attenuation.exponential");
    unif.direction = GetUniformLocation(base_name + "direction");
    unif.cone_angle = GetUniformLocation(base_name + "cone_angle");

    return unif;
}

gl_app::LightingProgram::LightingProgram()
: ShaderProgram()
, _specular_intensity(LIGHT_INTENSITY_MAX)
, _specular_power(DEFAULT_SPECULAR_POWER)
, _point_lights_count(0)
, _spot_lights_count(0)
{}

gl_app::LightingProgram & gl_app::LightingProgram::Init()
{
    ShaderProgram::Init();

    AddShader(Shader(LIGHTING_VERTEX_SHADER, GL_VERTEX_SHADER));
    //AddShader(Shader(TCS_SHADER, GL_TESS_CONTROL_SHADER));
    //AddShader(Shader(TES_SHADER, GL_TESS_EVALUATION_SHADER));
    AddShader(Shader(LIGHTING_FRAGMENT_SHADER, GL_FRAGMENT_SHADER));
    Finalize();

    return *this;
}

//gl_app::LightingProgram& gl_app::LightingProgram::SetWVP(const glm::mat4& wvp)
//{
//    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");
//
//    GLuint wvp_uniform_loc = GetUniformLocation(WVP_UNIFORM);
//
//    glUniformMatrix4fv(wvp_uniform_loc, 1, GL_TRUE, glm::value_ptr(wvp));
//
//    return *this;
//}

gl_app::LightingProgram & gl_app::LightingProgram::SetVP(const glm::mat4 & vp)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint vp_uniform_loc = GetUniformLocation(VP_UNIFORM);

    glUniformMatrix4fv(vp_uniform_loc, 1, GL_TRUE, glm::value_ptr(vp));

    return *this;
}

gl_app::LightingProgram & gl_app::LightingProgram::SetLightWVP(const glm::mat4 & wvp)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint wvp_uniform_loc = GetUniformLocation(LIGHT_WVP_UNIFORM);

    glUniformMatrix4fv(wvp_uniform_loc, 1, GL_TRUE, glm::value_ptr(wvp));

    return *this;
}

gl_app::LightingProgram & gl_app::LightingProgram::SetEWP(const glm::vec3 & eye)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint eye_loc = GetUniformLocation(DIRLIGHT_EYE_WORLD_POS_UNIFORM);

    glUniform3f(eye_loc, eye.x, eye.y, eye.z);

    return *this;
}

//gl_app::LightingProgram& gl_app::LightingProgram::SetWM(const glm::mat4 & matrix)
//{
//    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");
//
//    GLuint world_uniform_loc = GetUniformLocation(W_UNIFORM);
//    glUniformMatrix4fv(world_uniform_loc, 1, GL_TRUE, glm::value_ptr(matrix));
//
//    return *this;
//}

gl_app::LightingProgram& gl_app::LightingProgram::SetColorTextureUnit(GLuint texture_unit)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint texture_unit_loc = GetUniformLocation(COLORMAP_UNIFORM);

    glUniform1i(texture_unit_loc, texture_unit);

    return *this;
}

gl_app::LightingProgram & gl_app::LightingProgram::SetShadowMapTextureUnit(GLuint texture_unit)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint shadow_map_loc = GetUniformLocation(SHADOWMAP_UNIFORM);

    glUniform1i(shadow_map_loc, texture_unit);

    return *this;
}

gl_app::LightingProgram & gl_app::LightingProgram::SetNormalMapTextureUnit(GLuint texture_unit)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint normal_map_loc = GetUniformLocation(NORMALMAP_UNIFORM);

    glUniform1i(normal_map_loc, texture_unit);

    return *this;
}

gl_app::LightingProgram & gl_app::LightingProgram::SetDisplacementMapTextureUnit(GLuint texture_unit)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint displacement_map_loc = GetUniformLocation(DISPLACEMENTMAP_UNIFORM);
    glUniform1i(displacement_map_loc, texture_unit);

    return *this;
}

gl_app::LightingProgram& gl_app::LightingProgram::SetDirectionalLight(const DirectionalLight& light)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    const glm::vec3& color = light.GetColor();
    GLuint light_color_loc = GetUniformLocation(DIRLIGHT_COLOR_UNIFORM);
    glUniform3f(light_color_loc, color.r, color.g, color.b);
    
    GLuint light_ambient_intensity_loc = GetUniformLocation(DIRLIGHT_AMBIENT_INTENSITY_UNIFORM);
    glUniform1f(light_ambient_intensity_loc, light.GetAmbientIntensity());

    GLuint light_diffuse_intensity_loc = GetUniformLocation(DIRLIGHT_DIFFUSE_INTENSITY_UNIFORM);
    glUniform1f(light_diffuse_intensity_loc, light.GetDiffuseIntensity());
 
    glm::vec3 direction = glm::normalize(light.GetDirection());
    GLuint light_direction_loc = GetUniformLocation(DIRLIGHT_DIRECTION_UNIFORM);
    glUniform3f(light_direction_loc, direction.x, direction.y, direction.z);

    return *this;
}

gl_app::LightingProgram & gl_app::LightingProgram::SetPointLights(const std::vector<PointLight>& lights)
{
    _point_lights_count = 0;
    AddPointLights(lights);

    return *this;
}

gl_app::LightingProgram & gl_app::LightingProgram::ResetPointLights()
{
    _point_lights_count = 0;
    SetPointLightsCount(0);

    return *this;
}

gl_app::LightingProgram & gl_app::LightingProgram::AddPointLights(const std::vector<PointLight>& lights)
{
    std::for_each(lights.begin(), lights.end(), [this](const PointLight& light)
    {
        AddPointLight(light);
    });

    return *this;
}

gl_app::LightingProgram & gl_app::LightingProgram::SetPointLightsCount(GLuint count)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint point_lights_count_loc = GetUniformLocation(PTLIGHTS_COUNT_UNIFORM);

    glUniform1i(point_lights_count_loc, count);

    return *this;
}

gl_app::LightingProgram & gl_app::LightingProgram::AddPointLight(const PointLight& light)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    if (_point_lights_count < MAX_POINT_LIGHTS)
    {
        const glm::vec3& color = light.GetColor();
        PointLightUniform uniform = GetNextUniform(light);
        glUniform3f(uniform.color, color.r, color.g, color.b);
        glUniform1f(uniform.ambient_intensity, light.GetAmbientIntensity());
        glUniform1f(uniform.diffuse_intensity, light.GetDiffuseIntensity());
        const glm::vec3& pos = light.GetPosition();
        glUniform3f(uniform.position, pos.x, pos.y, pos.z);
        LightAttenuation attenuation = light.GetAttenuation();
        glUniform1f(uniform.attenuation.constant, attenuation.constant);
        glUniform1f(uniform.attenuation.linear, attenuation.linear);
        glUniform1f(uniform.attenuation.exponential, attenuation.exponential);

        ++_point_lights_count;
    }

    return *this;
}

gl_app::LightingProgram & gl_app::LightingProgram::SetSpotLights(const std::vector<SpotLight>& lights)
{
    _spot_lights_count = 0;
    AddSpotLights(lights);

    return *this;
}

gl_app::LightingProgram & gl_app::LightingProgram::ResetSpotLights()
{
    _spot_lights_count = 0;
    SetSpotLightsCount(0);

    return *this;
}

gl_app::LightingProgram & gl_app::LightingProgram::SetSpotLightsCount(GLuint count)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint spot_lights_count_loc = GetUniformLocation(SPOTLIGHTS_COUNT_UNIFORM);

    glUniform1i(spot_lights_count_loc, count);

    return *this;
}

gl_app::LightingProgram& gl_app::LightingProgram::AddSpotLights(const std::vector<SpotLight>& lights)
{
    std::for_each(lights.begin(), lights.end(), [this](SpotLight light)
    {
        AddSpotLight(light);
    });

    return *this;
}

gl_app::LightingProgram & gl_app::LightingProgram::AddSpotLight(const SpotLight & light)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    if (_spot_lights_count < MAX_SPOT_LIGHTS)
    {
        const glm::vec3& color = light.GetColor();
        SpotLightUniform uniform = GetNextUniform(light);
        glUniform3f(uniform.color, color.r, color.g, color.b);
        glUniform1f(uniform.ambient_intensity, light.GetAmbientIntensity());
        glUniform1f(uniform.diffuse_intensity, light.GetDiffuseIntensity());
        const glm::vec3& pos = light.GetPosition();
        glUniform3f(uniform.position, pos.x, pos.y, pos.z);
        LightAttenuation attenuation = light.GetAttenuation();
        glUniform1f(uniform.attenuation.constant, attenuation.constant);
        glUniform1f(uniform.attenuation.linear, attenuation.linear);
        glUniform1f(uniform.attenuation.exponential, attenuation.exponential);
        glm::vec3 direction = glm::normalize(light.GetDirection());
        glUniform3f(uniform.direction, direction.x, direction.y, direction.z);
        glUniform1f(uniform.cone_angle, glm::cos(glm::radians(light.GetConeAngle())));

        ++_spot_lights_count;
    }

    return *this;
}

gl_app::Light::Light()
: _color(COLOR_WHITE)
, _ambient_intensity(LIGHT_INTENSITY_MIN)
, _diffuse_intensity(LIGHT_INTENSITY_MIN)
{}

gl_app::Light::Light(const glm::vec3 & color, GLfloat ambient_intensity)
: Light(color, ambient_intensity, LIGHT_INTENSITY_MIN)
{}

gl_app::Light::Light(const glm::vec3 & color, GLfloat ambient_intensity, GLfloat diffuse_intensity)
: _color(color)
, _ambient_intensity(glm::clamp(ambient_intensity, LIGHT_INTENSITY_MIN, LIGHT_INTENSITY_MAX))
, _diffuse_intensity(glm::clamp(ambient_intensity, LIGHT_INTENSITY_MIN, LIGHT_INTENSITY_MAX))
{}

gl_app::Light & gl_app::Light::SetColor(const glm::vec3 & color)
{
    _color = color;

    return *this;
}

gl_app::Light & gl_app::Light::SetAmbientIntensity(GLfloat ambient_intensity)
{
    _ambient_intensity = glm::clamp(ambient_intensity, LIGHT_INTENSITY_MIN, LIGHT_INTENSITY_MAX);

    return *this;
}

gl_app::Light & gl_app::Light::SetDiffuseIntensity(GLfloat diffuse_intensity)
{
    _diffuse_intensity = glm::clamp(diffuse_intensity, LIGHT_INTENSITY_MIN, LIGHT_INTENSITY_MAX);

    return *this;
}

gl_app::DirectionalLight::DirectionalLight()
: Light()
, _direction(0.0f)
{}

gl_app::DirectionalLight::DirectionalLight(const glm::vec3 & direction)
: Light()
, _direction(direction)
{}

gl_app::DirectionalLight::DirectionalLight(const glm::vec3 & color, GLfloat ambient_intensity, const glm::vec3 & direction)
: Light(color, ambient_intensity, LIGHT_INTENSITY_MIN)
, _direction(direction)
{}

gl_app::DirectionalLight::DirectionalLight(const glm::vec3 & color, GLfloat ambient_intensity, GLfloat diffuse_intensity, const glm::vec3 & direction)
: Light(color, ambient_intensity, diffuse_intensity)
, _direction(direction)
{}

gl_app::DirectionalLight & gl_app::DirectionalLight::SetDirection(const glm::vec3 & direction)
{
    _direction = direction;

    return *this;
}

gl_app::LightingProgram & gl_app::LightingProgram::SetSpecularIntensity(GLfloat specular_intensity)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    _specular_intensity = glm::clamp(specular_intensity, LIGHT_INTENSITY_MIN, LIGHT_INTENSITY_MAX);

    GLuint light_specular_intensity_loc = GetUniformLocation(DIRLIGHT_SPECULAR_INTENSITY_UNIFORM);
    glUniform1f(light_specular_intensity_loc, _specular_intensity);

    return *this;
}

gl_app::LightingProgram & gl_app::LightingProgram::SetSpecularPower(GLfloat specular_power)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    _specular_power = specular_power;

    GLuint specular_power_loc = GetUniformLocation(DIRLIGHT_SPECULAR_POWER_UNIFORM);
    glUniform1f(specular_power_loc, _specular_power);

    return *this;
}

gl_app::LightingProgram & gl_app::LightingProgram::SetDisplacementFactor(GLfloat disp_factor)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint disp_factor_loc = GetUniformLocation(DISPLACEMENT_FACTOR_UNIFORM);
    glUniform1f(disp_factor_loc, disp_factor);

    return *this;
}

gl_app::LightingProgram & gl_app::LightingProgram::SetTesselationLevel(GLfloat level)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint tess_level_loc = GetUniformLocation(TESSLEVEL_UNIFORM);
    glUniform1f(tess_level_loc, level);

    return *this;
}

gl_app::LightingProgram & gl_app::LightingProgram::SetTesselationAlpha(GLfloat alpha)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint tess_alpha_loc = GetUniformLocation(TESSALPHA_UNIFORM);
    glUniform1f(tess_alpha_loc, alpha);

    return *this;
}

gl_app::LightingProgram & gl_app::LightingProgram::SetColor(GLuint index, const glm::vec4 & color)
{
    GLuint color_loc = GetUniformLocation(std::string(COLOR_UNIFORM) + "[" + std::to_string(index) + "]");

    glUniform4f(color_loc, color.r, color.g, color.b, color.a);

    return *this;
}

gl_app::PointLight::PointLight()
: Light()
, _position(ORIGIN)
, _attenuation(DEFAULT_CONSTANT_ATTENUATION, DEFAULT_LINEAR_ATTENUATION, DEFAULT_EXP_ATTENUATION)
{}

gl_app::PointLight::PointLight(const glm::vec3 & color, GLfloat ambient_intensity)
: Light(color, ambient_intensity)
, _position(ORIGIN)
, _attenuation(DEFAULT_CONSTANT_ATTENUATION, DEFAULT_LINEAR_ATTENUATION, DEFAULT_EXP_ATTENUATION)
{}

gl_app::PointLight::PointLight(const glm::vec3 & color, GLfloat ambient_intensity, GLfloat diffuse_intensity)
: Light(color, ambient_intensity, diffuse_intensity)
, _position(ORIGIN)
, _attenuation(DEFAULT_CONSTANT_ATTENUATION, DEFAULT_LINEAR_ATTENUATION, DEFAULT_EXP_ATTENUATION)
{}

gl_app::PointLight::PointLight(const glm::vec3 & color, GLfloat ambient_intensity, GLfloat diffuse_intensity, const glm::vec3 & position, GLfloat constant_attenuation, GLfloat linear_attenuation, GLfloat exponential_attenuation)
: Light(color, ambient_intensity, diffuse_intensity)
, _position(position)
, _attenuation(constant_attenuation, linear_attenuation, exponential_attenuation)
{}

gl_app::PointLight & gl_app::PointLight::SetPosition(const glm::vec3 & position)
{
    _position = position;

    return *this;
}

gl_app::PointLight & gl_app::PointLight::SetAttenuation(GLfloat constant, GLfloat linear, GLfloat exponential)
{
    _attenuation.constant    = constant;
    _attenuation.linear      = linear;
    _attenuation.exponential = exponential;

    return *this;
}

gl_app::SpotLight::SpotLight()
: PointLight()
, _direction(ORIGIN)
, _cone_angle(0.0f)
{}

gl_app::SpotLight::SpotLight(const glm::vec3 & color, GLfloat ambient_intensity)
: PointLight(color, ambient_intensity)
, _direction(ORIGIN)
, _cone_angle(0.0f)
{}

gl_app::SpotLight::SpotLight(const glm::vec3 & color, GLfloat ambient_intensity, GLfloat diffuse_intensity)
: PointLight(color, ambient_intensity, diffuse_intensity)
, _direction(ORIGIN)
, _cone_angle(0.0f)
{}

gl_app::SpotLight::SpotLight(const glm::vec3 & color, GLfloat ambient_intensity, GLfloat diffuse_intensity, const glm::vec3 & position, GLfloat constant_attenuation, GLfloat linear_attenuation, GLfloat exponential_attenuation)
: PointLight(color, ambient_intensity, diffuse_intensity, position, constant_attenuation, linear_attenuation, exponential_attenuation)
, _direction(ORIGIN)
, _cone_angle(0.0f)
{}

gl_app::SpotLight::SpotLight(const glm::vec3 & color, GLfloat ambient_intensity, GLfloat diffuse_intensity, const glm::vec3 & position, GLfloat constant_attenuation, GLfloat linear_attenuation, GLfloat exponential_attenuation, const glm::vec3& direction, GLfloat cone_angle)
: SpotLight(color, ambient_intensity, diffuse_intensity, position, constant_attenuation, linear_attenuation, exponential_attenuation)
{
    _direction  = direction;
    _cone_angle = cone_angle;
}

gl_app::SpotLight & gl_app::SpotLight::SetDirection(const glm::vec3 & direction)
{
    _direction = direction;

    return *this;
}

gl_app::SpotLight & gl_app::SpotLight::SetConeAngle(GLfloat cone_angle)
{
    _cone_angle = cone_angle;

    return *this;
}
