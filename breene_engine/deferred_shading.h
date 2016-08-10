#pragma once

#include "fbo.h"
#include "shader_program.h"

namespace gl_app
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

        virtual gl_app::GeometryBuffer& Init(GLuint width, GLuint height) override;
        gl_app::GeometryBuffer& BindRead();

        gl_app::GeometryBuffer& SetReadBuffer(GBufferTexType type);

        ~GeometryBuffer();
    private:
        GLuint _textures[GBUFFER_NUM_TEXTURES];
        GLuint _depth_tex;
    };

    class DefShadingGeomProgram : public ShaderProgram 
    {
    public:
        DefShadingGeomProgram();

        virtual gl_app::DefShadingGeomProgram& Init() override;

        gl_app::DefShadingGeomProgram& SetWVP(const glm::mat4& wvp);
        gl_app::DefShadingGeomProgram& SetWorldMatrix(const glm::mat4& world);
        gl_app::DefShadingGeomProgram& SetColorTextureUnit(GLuint texture_unit);
    };
}
