#pragma once

#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include "shader_program.h"
#include "fbo.h"

namespace breene
{
    class ShadowMapFBO : public FramebufferObject
    {
    public:
        ShadowMapFBO();

        breene::ShadowMapFBO& Init(GLuint wnd_width, GLuint wnd_height) override;

        breene::ShadowMapFBO& BindTextureRead(GLenum texture_unit) override;

        ~ShadowMapFBO();
    private:

        GLuint _fbo;
        GLuint _shadow_map_texture;
    };

    class ShadowProgram : public ShaderProgram
    {
    public:
        ShadowProgram() {}

        virtual breene::ShadowProgram& Init() override;

        breene::ShadowProgram& SetWVP(const glm::mat4& wvp);
        breene::ShadowProgram& SetTextureUnit(GLuint texture_unit);
    };
}

