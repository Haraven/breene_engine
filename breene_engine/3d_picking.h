#pragma once

#include "shader_program.h"
#include "callbacks.h"
#include "fbo.h"
#include "my_constants.h"

namespace gl_app
{
    class PickingProgram : public ShaderProgram, public RenderCallback
    {
    public:
        PickingProgram();

        virtual gl_app::PickingProgram& Init() override;
        gl_app::PickingProgram& SetWVP(const glm::mat4& wvp);
        gl_app::PickingProgram& SetObjIndex(GLuint index);
        virtual gl_app::PickingProgram& DrawStart(GLuint draw_index) override;
    };

    class PickingFBO : public FramebufferObject
    {
    public:
        struct PixelInfo
        {
            PixelInfo()
            : object_id(0)
            , draw_id(0)
            , primitive_id(0)
            {}

            PixelInfo(GLfloat object_id, GLfloat draw_id, GLfloat primitive_id)
            : object_id(object_id)
            , draw_id(draw_id)
            , primitive_id(primitive_id)
            {}

            GLfloat object_id;
            GLfloat draw_id;
            GLfloat primitive_id;
        };

        PickingFBO();

        gl_app::FramebufferObject& Init(GLuint wnd_width, GLuint wnd_height) override;

        PixelInfo ReadPixel(GLuint x, GLuint y);

        ~PickingFBO();
    private:
        GLuint _picking_tex;
        GLuint _depth_tex;
    };
}
