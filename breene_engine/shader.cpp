#include "shader.h"
#include "utils.h"

breene::Shader::Shader(const std::string& file_path, GLenum shader_type)
: _shader_path(file_path)
, _shader_type(shader_type)
{}

breene::Shader& breene::Shader::Compile()
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized.");
	if (_shader_path.empty() || _shader_type == NULL) throw std::runtime_error("Shader data is invalid");

	_shader_obj = glCreateShader(_shader_type);
	if (_shader_obj == NULL) throw std::runtime_error("Error creating shader of type: " + std::to_string(_shader_type));
	
	std::string shader_data = GetFileAsString(_shader_path);
	const GLchar* sz_data = shader_data.c_str();
	GLint data_size = static_cast<GLint>(shader_data.size());
	glShaderSource(_shader_obj, 1, &sz_data, &data_size);
	glCompileShader(_shader_obj);

	GLint res;
	glGetShaderiv(_shader_obj, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE)
	{
		GLchar info_log[log_size];
		glGetShaderInfoLog(_shader_obj, log_size, NULL, info_log);
		throw std::runtime_error("Error compiling shader \"" + _shader_path + "\", of type: " + std::to_string(_shader_type) + ". Info log:\n" + info_log);
	}

	return *this;
}

breene::Shader& breene::Shader::AttachTo(GLuint program)
{
	GLint res;
	glGetShaderiv(_shader_obj, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE)
	{
		GLchar info_log[log_size];
		glGetShaderInfoLog(_shader_obj, log_size, NULL, info_log);
		throw std::runtime_error("Shader type: " + std::to_string(_shader_type) + " was not successfully compiled. Cannot attach to program " + std::to_string(program));
	}

	glAttachShader(program, _shader_obj);

	return *this;
}

breene::Shader& breene::Shader::CompileAndAttachTo(GLuint program)
{
	return Compile().AttachTo(program);
}

breene::Shader & breene::Shader::DetachFrom(GLuint program)
{
    glDetachShader(program, _shader_obj);
    glDeleteShader(_shader_obj);

    return *this;
}

GLuint breene::InitializeProgram(std::vector<Shader> shaders)
{
    const size_t log_size = 1024;

    GLuint program = glCreateProgram();

    if (program == GL_FALSE) throw std::runtime_error("Error creating shader program");

    std::for_each(shaders.begin(), shaders.end(), [&program](Shader shader)
    {
        shader.CompileAndAttachTo(program);
    });

    glLinkProgram(program);

    GLint res;
    GLchar error_log[log_size] = { NULL };
    glGetProgramiv(program, GL_LINK_STATUS, &res);
    if (res == GL_FALSE)
    {
        glGetProgramInfoLog(program, sizeof(error_log), NULL, error_log);
        throw std::runtime_error("Error creating program. Error log: " + std::string(error_log));
    }

    glValidateProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &res);
    if (res == GL_FALSE)
    {
        glGetProgramInfoLog(program, sizeof(error_log), NULL, error_log);
        throw std::runtime_error("Error creating program. Error log: " + std::string(error_log));
    }

    std::for_each(shaders.begin(), shaders.end(), [&program](Shader shader)
    {
        glDetachShader(program, shader._shader_obj);
        glDeleteShader(shader._shader_obj);
    });

    return program;
}
