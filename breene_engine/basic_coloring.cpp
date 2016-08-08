#include "basic_coloring.h"
#include <glm\gtc\type_ptr.hpp>
#include "my_constants.h"

gl_app::PlainColorProgram::PlainColorProgram()
: ShaderProgram()
{}

gl_app::PlainColorProgram & gl_app::PlainColorProgram::Init()
{
    ShaderProgram::Init();

    AddShader(Shader(PLAIN_VERTEX_SHADER, GL_VERTEX_SHADER));
    AddShader(Shader(PLAIN_FRAGMENT_SHADER, GL_FRAGMENT_SHADER));
    Finalize();

    return *this;
}

gl_app::PlainColorProgram & gl_app::PlainColorProgram::SetWVP(const glm::mat4 & wvp)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint wvp_uniform_loc = GetUniformLocation(WVP_UNIFORM);

    glUniformMatrix4fv(wvp_uniform_loc, 1, GL_TRUE, glm::value_ptr(wvp));

    return *this;
}
