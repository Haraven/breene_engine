#pragma once

#include "shader_program.h"

namespace gl_app
{
    class PlainColorProgram : public ShaderProgram
    {
    public:
        PlainColorProgram();

        virtual gl_app::PlainColorProgram& Init() override;

        gl_app::PlainColorProgram& SetWVP(const glm::mat4& wvp);
    };
}
