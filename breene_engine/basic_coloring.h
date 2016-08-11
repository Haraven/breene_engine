#pragma once

#include "shader_program.h"
#include <glm\glm.hpp>

namespace breene
{
    class PlainColorProgram : public ShaderProgram
    {
    public:
        PlainColorProgram();

        virtual breene::PlainColorProgram& Init() override;

        breene::PlainColorProgram& SetWVP(const glm::mat4& wvp);
    };
}
