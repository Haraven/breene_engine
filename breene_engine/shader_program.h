#pragma once

#include "app_setup.h"
#include "shader.h"
#include <map>

namespace gl_app
{
    class ShaderProgram
    {
    protected:
        //virtual void AddHandlesToUniformHandler();
        GLuint GetUniformLocation(const GLchar* uniform);
        GLuint GetUniformLocation(const std::string& uniform);
    public:
        ShaderProgram();

        virtual gl_app::ShaderProgram& Init();
        gl_app::ShaderProgram& Use();
        gl_app::ShaderProgram& Disable();

        //typedef std::function<void(GLuint)> UniformHandlerFn;
        
        gl_app::ShaderProgram& AddShader(Shader& shader);
        gl_app::ShaderProgram& AddShaders(std::vector<Shader>& shaders);
        gl_app::ShaderProgram& Finalize();
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

