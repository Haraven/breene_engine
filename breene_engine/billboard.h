#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include "texture.h"
#include "shader_program.h"

namespace gl_app
{
    class BillboardProgram : public ShaderProgram
    {
    public:
        BillboardProgram();

        virtual gl_app::BillboardProgram& Init() override;

        gl_app::BillboardProgram& SetVP(const glm::mat4& vp);
        gl_app::BillboardProgram& SetCamPos(const glm::vec3& cam_pos);
        gl_app::BillboardProgram& SetColorTextureUnit(GLuint texture_unit);
        gl_app::BillboardProgram& SetSize(GLfloat billboard_size);
    };

    class Billboard
    {
    private:
        gl_app::Billboard& CreatePosBuffer();
    public:
        Billboard();

        gl_app::Billboard& Init(const std::string& tex_path);
        gl_app::Billboard& Render(const glm::mat4& vp, const glm::vec3& cam_pos);

        ~Billboard();
    private:
        GLuint _vbo;
        Texture2D* _tex;
        BillboardProgram* _program;
    };
}
