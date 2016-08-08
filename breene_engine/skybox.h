#pragma once

#include "shader_program.h"
#include "texture.h"
#include "mesh.h"
#include "app_setup.h"
#include "my_constants.h"

namespace gl_app
{

    class SkyBoxProgram : public ShaderProgram
    {
    public:
        SkyBoxProgram();

        virtual gl_app::SkyBoxProgram& Init();

        gl_app::SkyBoxProgram& SetWVP(const glm::mat4 wvp);
        gl_app::SkyBoxProgram& SetTextureUnit(GLuint texture_unit);
    };

    class SkyBox
    {
    public:
        SkyBox(const Camera* cam, const transform::PerspectiveProjectionInfo& info);

        gl_app::SkyBox& Init(const GLchar* mesh_file, const std::string & posx_file, const std::string & negx_file, const std::string & posy_file, const std::string & negy_file, const std::string & posz_file, const std::string & negz_file);

        gl_app::SkyBox& Render();

        ~SkyBox();
    private:
        SkyBoxProgram* _program;
        TextureCubeMap* _tex;
        Mesh* _mesh;
        const Camera* _cam;
        transform::PerspectiveProjectionInfo _perspective_info;
    };
}
