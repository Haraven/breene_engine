#pragma once

#include "fbo.h"
#include "shader_program.h"
#include <glm\glm.hpp>

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
            GBUFFER_TEX_TYPE_TEXCOORD,
            GBUFFER_NUM_TEXTURES
        };
        
        GeometryBuffer();

        virtual breene::GeometryBuffer& Init(GLuint width, GLuint height) override;
        breene::GeometryBuffer& BindRead();

        breene::GeometryBuffer& SetReadBuffer(GBufferTexType type);

        ~GeometryBuffer();
    private:
        GLuint _textures[GBUFFER_NUM_TEXTURES];
        GLuint _depth_tex;
    };

    class DefShadingGeomProgram : public ShaderProgram 
    {
    public:
        DefShadingGeomProgram();

        virtual breene::DefShadingGeomProgram& Init() override;

        breene::DefShadingGeomProgram& SetWVP(const glm::mat4& wvp);
        breene::DefShadingGeomProgram& SetWorldMatrix(const glm::mat4& world);
        breene::DefShadingGeomProgram& SetColorTextureUnit(GLuint texture_unit);
    };
}
