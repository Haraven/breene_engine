#pragma once

#include <GL\glew.h>

namespace breene
{
    class RenderCallback
    {
    public:
        virtual RenderCallback& DrawStart(GLuint draw_index) = 0;
    };
}
