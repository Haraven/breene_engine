#include "skybox.h"
#include <glm\gtc\type_ptr.hpp>
#include "my_constants.h"

breene::SkyBoxProgram::SkyBoxProgram()
: ShaderProgram()
{}

breene::SkyBoxProgram & breene::SkyBoxProgram::Init()
{
    ShaderProgram::Init();

    AddShader(Shader(SKYBOX_VERTEX_SHADER, GL_VERTEX_SHADER));
    AddShader(Shader(SKYBOX_FRAGMENT_SHADER, GL_FRAGMENT_SHADER));
    Finalize();

    return *this;
}

breene::SkyBoxProgram & breene::SkyBoxProgram::SetWVP(const glm::mat4 wvp)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint wvp_uniform_loc = GetUniformLocation(WVP_UNIFORM);

    glUniformMatrix4fv(wvp_uniform_loc, 1, GL_TRUE, glm::value_ptr(wvp));

    return *this;
}

breene::SkyBoxProgram& breene::SkyBoxProgram::SetTextureUnit(GLuint texture_unit)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint texture_unit_loc = GetUniformLocation(CUBEMAP_UNIFORM);

    glUniform1i(texture_unit_loc, texture_unit);

    return *this;
}

breene::SkyBox::SkyBox(const Camera * cam, const transform::PerspectiveProjectionInfo & info)
: _program(nullptr)
, _tex(nullptr)
, _mesh(nullptr)
, _cam(cam)
, _perspective_info(info)
{}

breene::SkyBox & breene::SkyBox::Init(const GLchar* mesh_file, const std::string & posx_file, const std::string & negx_file, const std::string & posy_file, const std::string & negy_file, const std::string & posz_file, const std::string & negz_file)
{
	if (_program == nullptr)
		_program = new SkyBoxProgram();
    _program->Init().Use();
    _program->SetTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
    
	if (_tex == nullptr)
		_tex = new TextureCubeMap(posx_file, negx_file, posy_file, negy_file, posz_file, negz_file);
    _tex->Load();

	if (_mesh == nullptr)
		_mesh = new Mesh();
    _mesh->Load(mesh_file);

    return *this;
}

breene::SkyBox & breene::SkyBox::Render()
{
    _program->Use();
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

    GLint old_cull_face, old_depth_fn;
    glGetIntegerv(GL_CULL_FACE_MODE, &old_cull_face);
    glGetIntegerv(GL_DEPTH_FUNC, &old_depth_fn);

    glCullFace(GL_FRONT);
    glDepthFunc(GL_LEQUAL);

    transform::Transformation trans;
    glm::vec3 eye = _cam->GetEye();
    trans.Scaling(20.0f)
        .Rotation(0.0f, 0.0f, 0.0f)
        .Translation(eye.x, eye.y, eye.z)
        .Cam(*_cam)
        .PerspectiveProjection(_perspective_info);

    _program->SetWVP(trans.WVPTransform());
    _tex->Bind(COLOR_TEXTURE_UNIT);
    _mesh->Render();

    glCullFace(old_cull_face);
    glDepthFunc(old_depth_fn);
	_program->Disable();
    return *this;
}

breene::SkyBox::~SkyBox()
{
    if (_program != nullptr)
    {
        delete _program;
        _program = nullptr;
    }
    if (_tex != nullptr)
    {
        delete _tex;
        _tex = nullptr;
    }
    if (_mesh != nullptr)
    {
        delete _mesh;
        _mesh = nullptr;
    }
}
