#include "fbo.h"
#include <stdexcept>
#include "my_constants.h"

breene::FramebufferObject::FramebufferObject()
: _fbo(INVALID_VAL)
{}


breene::FramebufferObject::~FramebufferObject()
{
    if (_fbo != INVALID_VAL)
    {
        glDeleteFramebuffers(1, &_fbo);
        _fbo = INVALID_VAL;
    }
}

breene::FramebufferObject& breene::FramebufferObject::BindWrite()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);

    return *this;
}

breene::FramebufferObject& breene::FramebufferObject::DisableWrite()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    return *this;
}

breene::FramebufferObject & breene::FramebufferObject::BindTextureRead(GLenum texture_unit)
{
    throw std::domain_error("Read binding is not available");
}
