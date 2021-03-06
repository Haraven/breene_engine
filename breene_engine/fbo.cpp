#include "fbo.h"
#include <stdexcept>
#include "my_constants.h"

gl_app::FramebufferObject::FramebufferObject()
: _fbo(INVALID_VAL)
, _prev_fbo(NULL)
{}


gl_app::FramebufferObject::~FramebufferObject()
{
    if (_fbo != INVALID_VAL)
    {
        glDeleteFramebuffers(1, &_fbo);
        _fbo = INVALID_VAL;
    }
}

gl_app::FramebufferObject& gl_app::FramebufferObject::BindWrite()
{
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &_prev_fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);

    return *this;
}

gl_app::FramebufferObject& gl_app::FramebufferObject::DisableWrite()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _prev_fbo);

    return *this;
}

gl_app::FramebufferObject & gl_app::FramebufferObject::BindRead(GLenum texture_unit)
{
    throw std::domain_error("Read binding is not available");
}
