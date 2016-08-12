#pragma once

#include <glm\glm.hpp>
#include "shader_program.h"
#include "callbacks.h"
#include "fbo.h"

namespace breene
{
    class PickingProgram : public ShaderProgram, public RenderCallback
    {
    public:
        PickingProgram();

        virtual breene::PickingProgram& Init() override;
        breene::PickingProgram& SetWVP(const glm::mat4& wvp);
        breene::PickingProgram& SetObjIndex(GLuint index);
        virtual breene::PickingProgram& DrawStart(GLuint draw_index) override;
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

        breene::FramebufferObject& Init(GLuint wnd_width, GLuint wnd_height) override;

        PixelInfo ReadPixel(GLuint x, GLuint y);

        ~PickingFBO();
    private:
        GLuint _picking_tex;
        GLuint _depth_tex;
    };
}
