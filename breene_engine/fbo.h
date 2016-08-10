#pragma once

#include <GL\glew.h>

namespace gl_app
{

    class FramebufferObject
    {
    public:
        FramebufferObject();
        ~FramebufferObject();

        virtual gl_app::FramebufferObject& Init(GLuint wnd_width, GLuint wnd_height) = 0;
        virtual gl_app::FramebufferObject& BindWrite();
        virtual gl_app::FramebufferObject& DisableWrite();
        virtual gl_app::FramebufferObject& BindTextureRead(GLenum texture_unit);
    protected:
        GLuint _fbo;
    private:
        GLint _prev_fbo;
    };
}
