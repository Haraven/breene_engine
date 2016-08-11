#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include "texture.h"
#include "shader_program.h"

namespace breene
{
    class BillboardProgram : public ShaderProgram
    {
    public:
        BillboardProgram();

        virtual breene::BillboardProgram& Init() override;

        breene::BillboardProgram& SetVP(const glm::mat4& vp);
        breene::BillboardProgram& SetCamPos(const glm::vec3& cam_pos);
        breene::BillboardProgram& SetColorTextureUnit(GLuint texture_unit);
        breene::BillboardProgram& SetSize(GLfloat billboard_size);
    };

    class Billboard
    {
    private:
        breene::Billboard& CreatePosBuffer();
    public:
        Billboard();

        breene::Billboard& Init(const std::string& tex_path);
        breene::Billboard& Render(const glm::mat4& vp, const glm::vec3& cam_pos);

        ~Billboard();
    private:
        GLuint _vbo;
        Texture2D* _tex;
        BillboardProgram* _program;
    };
}
