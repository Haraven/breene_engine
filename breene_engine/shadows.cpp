#include "shadows.h"
#include <glm\gtc\type_ptr.hpp>
#include <string>
#include "my_constants.h"

breene::ShadowMapFBO::ShadowMapFBO()
: FramebufferObject()
{
    _shadow_map_texture = NULL;
}

breene::ShadowMapFBO& breene::ShadowMapFBO::Init(GLuint wnd_width, GLuint wnd_height)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL Context has not been initialized");

    // Create the FBO
    glGenFramebuffers(1, &_fbo);

    // Create the depth buffer
    glGenTextures(1, &_shadow_map_texture);
    glBindTexture(GL_TEXTURE_2D, _shadow_map_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, wnd_width, wnd_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _shadow_map_texture, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    GLenum res = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (res != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("Error initializing framebuffer object. Error code: " + std::to_string(res));

    return *this;
}

breene::ShadowMapFBO & breene::ShadowMapFBO::BindTextureRead(GLenum texture_unit)
{
    glActiveTexture(texture_unit);
    glBindTexture(GL_TEXTURE_2D, _shadow_map_texture);

    return *this;
}


breene::ShadowMapFBO::~ShadowMapFBO()
{
    FramebufferObject::~FramebufferObject();

    if (_shadow_map_texture != INVALID_VAL)
    {
        glDeleteTextures(1, &_shadow_map_texture);
        _shadow_map_texture = INVALID_VAL;
    }
}

breene::ShadowProgram & breene::ShadowProgram::Init()
{
    ShaderProgram::Init();

    AddShader(Shader(SHADOW_VERTEX_SHADER, GL_VERTEX_SHADER));
    AddShader(Shader(SHADOW_FRAGMENT_SHADER, GL_FRAGMENT_SHADER));
    Finalize();

    return *this;
}

breene::ShadowProgram & breene::ShadowProgram::SetWVP(const glm::mat4 & wvp)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint wvp_uniform_loc = GetUniformLocation(WVP_UNIFORM);

    glUniformMatrix4fv(wvp_uniform_loc, 1, GL_TRUE, glm::value_ptr(wvp));

    return *this;
}

breene::ShadowProgram & breene::ShadowProgram::SetTextureUnit(GLuint texture_unit)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint texture_unit_loc = GetUniformLocation(SHADOWMAP_UNIFORM);

    glUniform1i(texture_unit_loc, texture_unit);

    return *this;
}