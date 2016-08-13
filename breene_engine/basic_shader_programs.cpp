#include "basic_shader_programs.h"
#include <glm\gtc\type_ptr.hpp>
#include "my_constants.h"

breene::PlainColorProgram::PlainColorProgram()
: ShaderProgram()
{}

breene::PlainColorProgram & breene::PlainColorProgram::Init()
{
    ShaderProgram::Init();

    AddShader(Shader(PLAIN_VERTEX_SHADER, GL_VERTEX_SHADER));
    AddShader(Shader(PLAIN_FRAGMENT_SHADER, GL_FRAGMENT_SHADER));
    Finalize();

    return *this;
}

breene::PlainColorProgram & breene::PlainColorProgram::SetWVP(const glm::mat4 & wvp)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint wvp_uniform_loc = GetUniformLocation(WVP_UNIFORM);

    glUniformMatrix4fv(wvp_uniform_loc, 1, GL_TRUE, glm::value_ptr(wvp));

    return *this;
}

breene::BlankProgram::BlankProgram()
: ShaderProgram()
{}

breene::BlankProgram & breene::BlankProgram::Init()
{
	ShaderProgram::Init();

	AddShader(Shader(BLANK_VERTEX_SHADER, GL_VERTEX_SHADER));
	AddShader(Shader(BLANK_FRAGMENT_SHADER, GL_FRAGMENT_SHADER));
	Finalize();

	return *this;
}

breene::BlankProgram & breene::BlankProgram::SetWVP(const glm::mat4 & wvp)
{
	if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

	GLuint wvp_loc = GetUniformLocation(WVP_UNIFORM);

	glUniformMatrix4fv(wvp_loc, 1, GL_TRUE, glm::value_ptr(wvp));

	return *this;
}
