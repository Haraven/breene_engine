#pragma once

#include <glm\glm.hpp>
#include "shader_program.h"
#include "fbo.h"
#include "lighting.h"

namespace breene
{
    class GeometryBuffer : public FramebufferObject
    {
    public:
        enum GBufferTexType
        {
            GBUFFER_TEX_TYPE_POSITION,
            GBUFFER_TEX_TYPE_DIFFUSE,
            GBUFFER_TEX_TYPE_NORMAL,
            GBUFFER_NUM_TEXTURES
        };
        
        GeometryBuffer();

        virtual breene::GeometryBuffer& Init(GLuint width, GLuint height) override;
        //breene::GeometryBuffer& BindRead();
		breene::GeometryBuffer& StartFrame();
		breene::GeometryBuffer& BindGeomPass();
		breene::GeometryBuffer& BindStencilPass();
		breene::GeometryBuffer& BindLightPass();
		breene::GeometryBuffer& BindFinalPass();

        ~GeometryBuffer();
    private:
        GLuint _textures[GBUFFER_NUM_TEXTURES];
        GLuint _depth_tex;
		GLuint _final_tex;
    };

    class DefShadingGeomProgram : public ShaderProgram 
    {
    public:
        DefShadingGeomProgram();

        virtual breene::DefShadingGeomProgram& Init() override;

        breene::DefShadingGeomProgram& SetWVP(const glm::mat4& wvp);
        breene::DefShadingGeomProgram& SetWorldMatrix(const glm::mat4& world);
        breene::DefShadingGeomProgram& SetColorTextureUnit(GLuint texture_unit);
		breene::DefShadingGeomProgram& SetTessLevel(GLfloat level);
		breene::DefShadingGeomProgram& SetTessAlpha(GLfloat alpha);
    };

    class DefShadingLight : public ShaderProgram
    {
    public:
        DefShadingLight();

        virtual breene::DefShadingLight& Init() override;

        breene::DefShadingLight& SetWVP(const glm::mat4& wvp);
        breene::DefShadingLight& SetPositionTextureUnit(GLuint texture_unit);
        breene::DefShadingLight& SetColorTextureUnit(GLuint texture_unit);
        breene::DefShadingLight& SetNormalTextureUnit(GLuint texture_unit);
        breene::DefShadingLight& SetEWP(const glm::vec3& ewp);
        breene::DefShadingLight& SetSpecularIntensity(GLfloat intensity);
        breene::DefShadingLight& SetSpecularPower(GLfloat power);
        breene::DefShadingLight& SetScreenSize(GLuint width, GLuint height);
    };

    class DefShadingDirLight : public DefShadingLight
    {
    public:
        DefShadingDirLight();

        virtual breene::DefShadingDirLight& Init() override;
        breene::DefShadingDirLight& SetDirectionalLight(const DirectionalLight& light);
    };

    class DefShadingPointLight : public DefShadingLight
    {
    public:
        DefShadingPointLight();

        virtual breene::DefShadingPointLight& Init() override;
        breene::DefShadingPointLight& SetPointLight(const PointLight& light);
    };

    class DefShadingSpotLight : public DefShadingLight
    {
    public:
        DefShadingSpotLight();

        virtual breene::DefShadingSpotLight& Init() override;
        breene::DefShadingSpotLight& SetSpotLight(const SpotLight& light);
    };
}
