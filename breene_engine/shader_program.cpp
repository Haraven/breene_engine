#include "shader_program.h"
#include <algorithm>
#include <iostream>
#include "my_constants.h"

GLuint gl_app::ShaderProgram::GetUniformLocation(const GLchar* uniform)
{
    if (_program == NULL) throw std::runtime_error("Shader program has not been initialized");
    GLuint uniform_loc = glGetUniformLocation(_program, uniform);
    if (uniform_loc == INVALID_UNIFORM_LOCATION) throw std::runtime_error("Uniform \"" + std::string(uniform) + "\" could not be located");

    return uniform_loc;
}

GLuint gl_app::ShaderProgram::GetUniformLocation(const std::string & uniform)
{
    return GetUniformLocation(uniform.c_str());
}

gl_app::ShaderProgram::ShaderProgram()
: _program(NULL)
{}

gl_app::ShaderProgram & gl_app::ShaderProgram::Init()
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    _program = glCreateProgram();

    if (_program == NULL)
        throw std::runtime_error("Error creating program");

    return *this;
}

gl_app::ShaderProgram & gl_app::ShaderProgram::Use()
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    glUseProgram(_program);

    return *this;
}

gl_app::ShaderProgram & gl_app::ShaderProgram::Disable()
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    glUseProgram(NULL);

    return *this;
}


gl_app::ShaderProgram::~ShaderProgram()
{
    if (_program != NULL)
    {
        glDeleteProgram(_program);
        _program = NULL;
    }
}

gl_app::ShaderProgram & gl_app::ShaderProgram::AddShader(Shader & shader)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    shader.CompileAndAttachTo(_program);
    _shaders.push_back(shader);

    return *this;
}

gl_app::ShaderProgram& gl_app::ShaderProgram::AddShaders(std::vector<Shader>& shaders)
{
    std::for_each(shaders.begin(), shaders.end(), [this](Shader& shader)
    {
        AddShader(shader);
    });

    return *this;
}

gl_app::ShaderProgram & gl_app::ShaderProgram::Finalize()
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    const size_t log_size = 1024;

    if (_program == NULL) throw std::runtime_error("Shader program has not been initialized");

    glLinkProgram(_program);

    GLint res;
    GLchar error_log[log_size] = { NULL };
    glGetProgramiv(_program, GL_LINK_STATUS, &res);
    if (res == GL_FALSE)
    {
        glGetProgramInfoLog(_program, sizeof(error_log), NULL, error_log);
        throw std::runtime_error("Error finalizing program. Error log: " + std::string(error_log));
    }

    glValidateProgram(_program);
    glGetProgramiv(_program, GL_LINK_STATUS, &res);
    if (res == GL_FALSE)
    {
        glGetProgramInfoLog(_program, sizeof(error_log), NULL, error_log);
        throw std::runtime_error("Error validating program. Error log: " + std::string(error_log));
    }

    std::for_each(_shaders.begin(), _shaders.end(), [this](Shader shader)
    {
        shader.DetachFrom(_program);
    });
    
    _shaders.clear();

    return *this;
}