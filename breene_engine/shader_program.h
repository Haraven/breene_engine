#pragma once

#include "shader.h"

namespace breene
{
    class ShaderProgram
    {
    protected:
        GLuint GetUniformLocation(const GLchar* uniform);
        GLuint GetUniformLocation(const std::string& uniform);
    public:
        ShaderProgram();

        virtual breene::ShaderProgram& Init();
        breene::ShaderProgram& Use();
        breene::ShaderProgram& Disable();

        breene::ShaderProgram& AddShader(Shader& shader);
        breene::ShaderProgram& AddShaders(std::vector<Shader>& shaders);
        breene::ShaderProgram& Finalize();

        ~ShaderProgram();
    protected:     
        GLuint _program;
    private:
        std::vector<Shader> _shaders;
    };
}

