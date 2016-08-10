#include "deferred_shading.h"
#include <glm\gtc\type_ptr.hpp>
#include <stdexcept>
#include <string>
#include <iostream>
#include "my_constants.h"

gl_app::GeometryBuffer::GeometryBuffer()
: FramebufferObject()
{
    memset(_textures, NULL, sizeof(GLuint) * GBUFFER_NUM_TEXTURES);
    _depth_tex = NULL;
}

gl_app::GeometryBuffer & gl_app::GeometryBuffer::Init(GLuint width, GLuint height)
{
    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    glGenTextures(GBUFFER_NUM_TEXTURES, _textures);
    glGenTextures(1, &_depth_tex);

    GLenum draw_buffers[GBUFFER_NUM_TEXTURES];
    for (GLuint i = 0; i < GBUFFER_NUM_TEXTURES; ++i)
    {
        draw_buffers[i] = GL_COLOR_ATTACHMENT0 + i;
        glBindTexture(GL_TEXTURE_2D, _textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, draw_buffers[i], GL_TEXTURE_2D, _textures[i], 0);
    }

    glBindTexture(GL_TEXTURE_2D, _depth_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depth_tex, 0);

    glDrawBuffers(GBUFFER_NUM_TEXTURES, draw_buffers);

    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) throw std::runtime_error("Error initializing G buffer. Framebuffer status: " + std::to_string(status));
    
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    return *this;
}

gl_app::GeometryBuffer & gl_app::GeometryBuffer::BindRead()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _fbo);

    return *this;
}

gl_app::GeometryBuffer & gl_app::GeometryBuffer::SetReadBuffer(GBufferTexType type)
{
    if (type == GBUFFER_NUM_TEXTURES) throw std::runtime_error("Invalid buffer texture type");
    glReadBuffer(GL_COLOR_ATTACHMENT0 + type);

    return *this;
}

gl_app::GeometryBuffer::~GeometryBuffer()
{
    FramebufferObject::~FramebufferObject();
    if (_textures[0] != NULL)
    {
        glDeleteTextures(GBUFFER_NUM_TEXTURES, _textures);
        memset(_textures, NULL, sizeof(GLuint) * GBUFFER_NUM_TEXTURES);
    }
    if (_depth_tex != NULL)
    {
        glDeleteTextures(1, &_depth_tex);
        _depth_tex = NULL;
    }
}

gl_app::DefShadingGeomProgram::DefShadingGeomProgram()
: ShaderProgram()
{}

gl_app::DefShadingGeomProgram & gl_app::DefShadingGeomProgram::Init()
{
    ShaderProgram::Init();

    AddShader(Shader(DEFSHADING_GEOM_PASS_VERTEX_SHADER, GL_VERTEX_SHADER));
    AddShader(Shader(DEFSHADING_GEOM_PASS_FRAGMENT_SHADER, GL_FRAGMENT_SHADER));
    Finalize();

    return *this;
}

gl_app::DefShadingGeomProgram & gl_app::DefShadingGeomProgram::SetWVP(const glm::mat4 & wvp)
{
    GLuint wvp_loc = GetUniformLocation(WVP_UNIFORM);

    glUniformMatrix4fv(wvp_loc, 1, GL_TRUE, glm::value_ptr(wvp));

    return *this;
}

gl_app::DefShadingGeomProgram & gl_app::DefShadingGeomProgram::SetWorldMatrix(const glm::mat4 & world)
{
    GLuint world_loc = GetUniformLocation(W_UNIFORM);
    glUniformMatrix4fv(world_loc, 1, GL_TRUE, glm::value_ptr(world));

    return *this;
}

gl_app::DefShadingGeomProgram & gl_app::DefShadingGeomProgram::SetColorTextureUnit(GLuint texture_unit)
{
    GLuint color_sampler_loc = GetUniformLocation(COLORMAP_UNIFORM);

    glUniform1i(color_sampler_loc, texture_unit);

    return *this;
}
