#include "shader_program.h"
#include <iostream>

namespace gl_app
{
    class Light
    {
    public:
        Light();
        Light(const glm::vec3& color, GLfloat ambient_intensity);
        Light(const glm::vec3& color, GLfloat ambient_intensity, GLfloat diffuse_intensity);

        gl_app::Light& SetColor(const glm::vec3& color);
        gl_app::Light& SetAmbientIntensity(GLfloat ambient_intensity);
        gl_app::Light& SetDiffuseIntensity(GLfloat diffuse_intensity);
        const glm::vec3& GetColor() const { return _color; }
        GLfloat GetAmbientIntensity() const { return _ambient_intensity; }
        GLfloat GetDiffuseIntensity() const { return _diffuse_intensity; }
    private:
        glm::vec3 _color;
        GLfloat _ambient_intensity;
        GLfloat _diffuse_intensity;
    };

    class DirectionalLight : public Light
    {
    public:
        DirectionalLight();
        DirectionalLight(const glm::vec3& direction);
        DirectionalLight(const glm::vec3& color, GLfloat ambient_intensity, const glm::vec3& direction);
        DirectionalLight(const glm::vec3& color, GLfloat ambient_intensity, GLfloat diffuse_intensity, const glm::vec3& direction);

        gl_app::DirectionalLight& SetDirection(const glm::vec3&);
        const glm::vec3& GetDirection() const { return _direction; }
    private:
        glm::vec3 _direction;
    };

    struct PointLightUniform
    {
        GLuint color;
        GLuint ambient_intensity;
        GLuint diffuse_intensity;
        GLuint position;
        struct
        {
            GLuint constant;
            GLuint linear;
            GLuint exponential;
        } attenuation;

        PointLightUniform() {}

        PointLightUniform(GLuint color, GLuint ambient_intensity, GLuint diffuse_intensity, GLuint position, GLuint attenuation_const, GLuint attenuation_linear, GLuint attenuation_exp)
        : color(color)
        , ambient_intensity(ambient_intensity)
        , diffuse_intensity(diffuse_intensity)
        , position(position)
        {
            attenuation.constant    = attenuation_const;
            attenuation.linear      = attenuation_linear;
            attenuation.exponential = attenuation_exp;
        }
    };

    struct SpotLightUniform : public PointLightUniform
    {
        GLuint direction;
        GLuint cone_angle;

        SpotLightUniform() : PointLightUniform() {}

        SpotLightUniform(GLuint color, GLuint ambient_intensity, GLuint diffuse_intensity, GLuint position, GLuint attenuation_const, GLuint attenuation_linear, GLuint attenuation_exp)
        : PointLightUniform(color, ambient_intensity, diffuse_intensity, position, attenuation_const, attenuation_linear, attenuation_exp)
        {}

        SpotLightUniform(GLuint color, GLuint ambient_intensity, GLuint diffuse_intensity, GLuint position, GLuint attenuation_const, GLuint attenuation_linear, GLuint attenuation_exp, GLuint direction, GLuint cone_angle)
            : SpotLightUniform(color, ambient_intensity, diffuse_intensity, position, attenuation_const, attenuation_linear, attenuation_exp)
        {
            this->direction  = direction;
            this->cone_angle = cone_angle;
        }
    };

    struct LightAttenuation
    {
        GLfloat constant;
        GLfloat linear;
        GLfloat exponential;

        LightAttenuation()
        : constant(0.0f)
        , linear(0.0f)
        , exponential(0.0f)
        {}

        LightAttenuation(GLfloat constant, GLfloat linear, GLfloat exponential)
        : constant(constant)
        , linear(linear)
        , exponential(exponential)
        {}
    };

    class PointLight : public Light
    {
    public:
        PointLight();
        PointLight(const glm::vec3& color, GLfloat ambient_intensity);
        PointLight(const glm::vec3& color, GLfloat ambient_intensity, GLfloat diffuse_intensity);
        PointLight(const glm::vec3& color, GLfloat ambient_intensity, GLfloat diffuse_intensity, const glm::vec3& position, GLfloat constant_attenuation, GLfloat linear_attenuation, GLfloat exponential_attenuation);
        
        const glm::vec3& GetPosition() const { return _position; }
        const LightAttenuation& GetAttenuation() const { return _attenuation; }
        gl_app::PointLight& SetPosition(const glm::vec3& position);
        gl_app::PointLight& SetAttenuation(GLfloat constant, GLfloat linear, GLfloat exponential);
    private:
        glm::vec3 _position;
        LightAttenuation _attenuation;
    };

    class SpotLight : public PointLight
    {
    public:
        SpotLight();
        SpotLight(const glm::vec3& color, GLfloat ambient_intensity);
        SpotLight(const glm::vec3& color, GLfloat ambient_intensity, GLfloat diffuse_intensity);
        SpotLight(const glm::vec3& color, GLfloat ambient_intensity, GLfloat diffuse_intensity, const glm::vec3& position, GLfloat constant_attenuation, GLfloat linear_attenuation, GLfloat exponential_attenuation);
        SpotLight(const glm::vec3& color, GLfloat ambient_intensity, GLfloat diffuse_intensity, const glm::vec3& position, GLfloat constant_attenuation, GLfloat linear_attenuation, GLfloat exponential_attenuation, const glm::vec3& direction, GLfloat cone_angle);

        const glm::vec3& GetDirection() const { return _direction; }
        GLfloat GetConeAngle() const { return _cone_angle; }
        gl_app::SpotLight& SetDirection(const glm::vec3& direction);
        gl_app::SpotLight& SetConeAngle(GLfloat cone_angle);
    private:
        glm::vec3 _direction;
        GLfloat _cone_angle;
    };

    class LightingProgram : public ShaderProgram
    {
    private:
        PointLightUniform GetNextUniform(const PointLight& light);
        SpotLightUniform GetNextUniform(const SpotLight& light);
    public:
        static const GLuint MAX_POINT_LIGHTS = 3;
        static const GLuint MAX_SPOT_LIGHTS  = 3;
        static const GLuint MAX_LIGHTS       = 8;

        LightingProgram();

        virtual gl_app::LightingProgram& Init() override;

        GLfloat GetSpecularIntensity() const { return _specular_intensity; }
        GLfloat GetSpecularPower() const { return _specular_power; }
        //gl_app::LightingProgram& SetWVP(const glm::mat4& wvp);
        gl_app::LightingProgram& SetVP(const glm::mat4& vp);
        gl_app::LightingProgram& SetLightWVP(const glm::mat4& wvp);
        gl_app::LightingProgram& SetEWP(const glm::vec3& eye);
        //gl_app::LightingProgram& SetWM(const glm::mat4& matrix);
        gl_app::LightingProgram& SetColorTextureUnit(GLuint texture_unit);
        gl_app::LightingProgram& SetShadowMapTextureUnit(GLuint texture_unit);
        gl_app::LightingProgram& SetNormalMapTextureUnit(GLuint texture_unit);
        gl_app::LightingProgram& SetDisplacementMapTextureUnit(GLuint texture_unit);
        gl_app::LightingProgram& SetDirectionalLight(const DirectionalLight& light);
        gl_app::LightingProgram& SetPointLights(const std::vector<PointLight>& lights);
        gl_app::LightingProgram& ResetPointLights();
        gl_app::LightingProgram& SetPointLightsCount(GLuint count);
        gl_app::LightingProgram& AddPointLights(const std::vector<PointLight>& lights);
        gl_app::LightingProgram& AddPointLight(const PointLight& light);
        gl_app::LightingProgram& SetSpotLights(const std::vector<SpotLight>& lights);
        gl_app::LightingProgram& ResetSpotLights();
        gl_app::LightingProgram& SetSpotLightsCount(GLuint count);
        gl_app::LightingProgram& AddSpotLights(const std::vector<SpotLight>& lights);
        gl_app::LightingProgram& AddSpotLight(const SpotLight& light);
        gl_app::LightingProgram& SetSpecularIntensity(GLfloat specular_intensity);
        gl_app::LightingProgram& SetSpecularPower(GLfloat specular_power);
        gl_app::LightingProgram& SetDisplacementFactor(GLfloat disp_factor);
        gl_app::LightingProgram& SetTesselationLevel(GLfloat level);
        gl_app::LightingProgram& SetTesselationAlpha(GLfloat alpha);
        gl_app::LightingProgram& SetColor(GLuint index, const glm::vec4& color);
    private:
        GLfloat _specular_intensity;
        GLfloat _specular_power;
        GLuint _point_lights_count;
        GLuint _spot_lights_count;
    };
}