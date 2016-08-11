#pragma once

//#include "utils.h"
#include "shader.h"
#include <map>

namespace breene
{
    class ShaderProgram
    {
    protected:
        //virtual void AddHandlesToUniformHandler();
        GLuint GetUniformLocation(const GLchar* uniform);
        GLuint GetUniformLocation(const std::string& uniform);
    public:
        ShaderProgram();

        virtual breene::ShaderProgram& Init();
        breene::ShaderProgram& Use();
        breene::ShaderProgram& Disable();

        //typedef std::function<void(GLuint)> UniformHandlerFn;
        
        breene::ShaderProgram& AddShader(Shader& shader);
        breene::ShaderProgram& AddShaders(std::vector<Shader>& shaders);
        breene::ShaderProgram& Finalize();
        //gl_app::OpenGLShaderProgram& AddUniformHandler(const GLchar* uniform_name, const UniformHandlerFn& handler);
        //gl_app::OpenGLShaderProgram& HandleUniforms();

        ~ShaderProgram();
    protected:     
        GLuint _program;
    private:
        std::vector<Shader> _shaders;
        //UniformHandler _uniforms_handler;
        //std::map<const GLchar*, UniformHandlerFn> _handlers;
    };
}

