#include "billboard.h"

#include <GLFW\glfw3.h>
#include <glm\gtc\type_ptr.hpp>
#include "my_constants.h"

const GLuint ROWCOUNT = 10, COLCOUNT = 10;

breene::Billboard & breene::Billboard::CreatePosBuffer()
{
    glm::vec3 positions[ROWCOUNT * COLCOUNT];

    for (unsigned int j = 0; j < ROWCOUNT; ++j)
        for (unsigned int i = 0; i < COLCOUNT; ++i)
            positions[j * COLCOUNT + i] = glm::vec3(static_cast<GLfloat>(i), 0.0f, static_cast<GLfloat>(j));

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    return *this;
}

breene::Billboard::Billboard()
: _tex(nullptr)
, _program(nullptr)
, _vbo(INVALID_VAL)
{}

breene::Billboard& breene::Billboard::Init(const std::string & tex_path)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    if (_program != nullptr)
        delete _program;
    _program = new BillboardProgram();

    if (_tex != nullptr)
        delete _tex;
    _tex = new Texture2D(tex_path, GL_TEXTURE_2D);
    _tex->Load();
    CreatePosBuffer();
    _program->Init();

    return *this;
}

breene::Billboard & breene::Billboard::Render(const glm::mat4 & vp, const glm::vec3& cam_pos)
{

    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    _program->Use();
    _program->SetVP(vp)
        .SetCamPos(cam_pos);

    _tex->Bind(COLOR_TEXTURE_UNIT);

    glEnableVertexAttribArray(COLOR_TEXTURE_UNIT_INDEX);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glDrawArrays(GL_POINTS, 0, ROWCOUNT * COLCOUNT);

    glDisableVertexAttribArray(COLOR_TEXTURE_UNIT_INDEX);

    return *this;
}


breene::Billboard::~Billboard()
{
    if (glfwGetCurrentContext() != nullptr &&_vbo != INVALID_VAL)
    {
        glDeleteBuffers(1, &_vbo);
        _vbo = INVALID_VAL;
    }
    if (_tex != nullptr)
    {
        delete _tex;
        _tex = nullptr;
    }
    if (_program != nullptr)
    {
        delete _program;
        _program = nullptr;
    }
}

breene::BillboardProgram::BillboardProgram()
{}

breene::BillboardProgram & breene::BillboardProgram::Init()
{
    ShaderProgram::Init();
    AddShader(Shader(BILLBOARD_VERTEX_SHADER, GL_VERTEX_SHADER));
    AddShader(Shader(BILLBOARD_FRAGMENT_SHADER, GL_FRAGMENT_SHADER));
    AddShader(Shader(BILLBOARD_GEOMETRY_SHADER, GL_GEOMETRY_SHADER));
    Finalize();

    return *this;
}

breene::BillboardProgram & breene::BillboardProgram::SetVP(const glm::mat4 & vp)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint vp_uniform_loc = GetUniformLocation(VP_UNIFORM);
    glUniformMatrix4fv(vp_uniform_loc, 1, GL_TRUE, glm::value_ptr(vp));

    return *this;
}

breene::BillboardProgram & breene::BillboardProgram::SetCamPos(const glm::vec3 & cam_pos)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint cam_pos_uniform_loc = GetUniformLocation(CAMPOS_UNIFORM);

    glUniform3f(cam_pos_uniform_loc, cam_pos.x, cam_pos.y, cam_pos.z);

    return *this;
}

breene::BillboardProgram & breene::BillboardProgram::SetColorTextureUnit(GLuint texture_unit)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint color_map_uniform_loc = GetUniformLocation(COLORMAP_UNIFORM);

    glUniform1i(color_map_uniform_loc, texture_unit);

    return *this;
}

breene::BillboardProgram & breene::BillboardProgram::SetSize(GLfloat billboard_size)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint billboard_size_uniform_loc = GetUniformLocation(BILLBOARD_SIZE_UNIFORM);

    glUniform1f(billboard_size_uniform_loc, billboard_size);

    return *this;
}
