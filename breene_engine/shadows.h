#pragma once

#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include "shader_program.h"
#include "fbo.h"

namespace gl_app
{
    class ShadowMapFBO : public FramebufferObject
    {
    public:
        ShadowMapFBO();

        gl_app::ShadowMapFBO& Init(GLuint wnd_width, GLuint wnd_height) override;

        gl_app::ShadowMapFBO& BindTextureRead(GLenum texture_unit) override;

        ~ShadowMapFBO();
    private:

        GLuint _fbo;
        GLuint _shadow_map_texture;
    };

    class ShadowProgram : public ShaderProgram
    {
    public:
        ShadowProgram() {}

        virtual gl_app::ShadowProgram& Init() override;

        gl_app::ShadowProgram& SetWVP(const glm::mat4& wvp);
        gl_app::ShadowProgram& SetTextureUnit(GLuint texture_unit);
    };
}

