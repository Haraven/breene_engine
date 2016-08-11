#pragma once

#include <GL\glew.h>

namespace breene
{

    class FramebufferObject
    {
    public:
        FramebufferObject();
        ~FramebufferObject();

        virtual breene::FramebufferObject& Init(GLuint wnd_width, GLuint wnd_height) = 0;
        virtual breene::FramebufferObject& BindWrite();
        virtual breene::FramebufferObject& DisableWrite();
        virtual breene::FramebufferObject& BindTextureRead(GLenum texture_unit);
    protected:
        GLuint _fbo;
    };
}
