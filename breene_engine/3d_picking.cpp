#include "3d_picking.h"
#include <glm\gtc\type_ptr.hpp>
#include <iostream>

gl_app::PickingProgram::PickingProgram()
: ShaderProgram()
{}

gl_app::PickingProgram & gl_app::PickingProgram::Init()
{
    ShaderProgram::Init();

    AddShader(Shader(PICKING_VERTEX_SHADER, GL_VERTEX_SHADER)); 
    AddShader(Shader(PICKING_FRAGMENT_SHADER, GL_FRAGMENT_SHADER));
    Finalize();

    return *this;
}

gl_app::PickingProgram & gl_app::PickingProgram::SetWVP(const glm::mat4 & wvp)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint wvp_uniform_loc = GetUniformLocation(WVP_UNIFORM);

    glUniformMatrix4fv(wvp_uniform_loc, 1, GL_TRUE, glm::value_ptr(wvp));

    return *this;
}

gl_app::PickingProgram & gl_app::PickingProgram::SetObjIndex(GLuint index)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint obj_index_uniform_loc = GetUniformLocation(PICKING_OBJECT_INDEX_UNIFORM);

    glUniform1ui(obj_index_uniform_loc, index);

    return *this;
}

gl_app::PickingProgram & gl_app::PickingProgram::DrawStart(GLuint draw_index)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint draw_index_uniform_loc = GetUniformLocation(PICKING_DRAW_INDEX_UNIFORM);

    glUniform1ui(draw_index_uniform_loc, draw_index);

    return *this;
}

gl_app::PickingFBO::PickingFBO()
: FramebufferObject()
{
    _picking_tex = INVALID_VAL;
    _depth_tex = INVALID_VAL;
}

gl_app::FramebufferObject & gl_app::PickingFBO::Init(GLuint wnd_width, GLuint wnd_height)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLint old_fbo, old_tex;
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &old_fbo);
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &old_tex);

    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glGenTextures(1, &_picking_tex);
    glBindTexture(GL_TEXTURE_2D, _picking_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, wnd_width, wnd_height, 0, GL_RGB, GL_FLOAT, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _picking_tex, 0);

    glGenTextures(1, &_depth_tex);
    glBindTexture(GL_TEXTURE_2D, _depth_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, wnd_width, wnd_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depth_tex, 0);

    glReadBuffer(GL_NONE);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    auto res = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (res != GL_FRAMEBUFFER_COMPLETE) throw std::runtime_error("Error initializing 3D picking framebuffer. Error code: " + std::to_string(res));

    glBindTexture(GL_TEXTURE_2D, old_tex);
    glBindFramebuffer(GL_FRAMEBUFFER, old_fbo);

    return *this;
}

gl_app::PickingFBO::PixelInfo gl_app::PickingFBO::ReadPixel(GLuint x, GLuint y)
{
    GLint old_fbo;
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &old_fbo);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _fbo);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    PixelInfo pixel;
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &pixel);
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, old_fbo);

    return pixel;
}

gl_app::PickingFBO::~PickingFBO()
{
    FramebufferObject::~FramebufferObject();
    if (_picking_tex != INVALID_VAL)
    {
        glDeleteTextures(1, &_picking_tex);
        _picking_tex = INVALID_VAL;
    }
    if (_depth_tex != INVALID_VAL)
    {
        glDeleteTextures(1, &_depth_tex);
        _depth_tex = INVALID_VAL;
    }
}
