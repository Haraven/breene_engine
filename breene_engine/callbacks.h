#pragma once

#include <GL\glew.h>

namespace gl_app
{
    class RenderCallback
    {
    public:
        virtual RenderCallback& DrawStart(GLuint draw_index) = 0;
    };
}
