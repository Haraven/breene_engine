#include <Windows.h>
#include "opengl_app.h"
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <glm\gtc\type_ptr.hpp>
#include "my_constants.h"

static const GLfloat FIELD_DEPTH = 20.0f;
static const GLfloat FIELD_WIDTH = 10.0f;
const glm::vec3 UP(0.0f, 1.0f, 0.0f);

void gl_app::OpenGLApplication::Init()
{
    //glm::vec3 pos(7.0f, 3.0f, 0.0f);
    //glm::vec3 look_at(0.0f, -0.2f, 1.0f);

    /*if (_shadow_fbo == nullptr)
        _shadow_fbo = new ShadowMapFBO();
    _shadow_fbo->Init(_wnd_width, _wnd_height);*/

    if (_geometry_buffer == nullptr)
        _geometry_buffer = new GeometryBuffer();
    _geometry_buffer->Init(_wnd_width, _wnd_height);

    if (_camera == nullptr)
        _camera = new Camera(_wnd_width, _wnd_height);

    if (_deferred_shading_program == nullptr)
        _deferred_shading_program = new DefShadingGeomProgram();
    _deferred_shading_program->Init().Use();
    _deferred_shading_program->SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);

    //if (_lighting_program == nullptr)
    //    _lighting_program = new LightingProgram();
    //_lighting_program->Init();
    //if (_text_renderer == nullptr)
    //    _text_renderer = new text_rendering::TextRenderer();
    //_text_renderer->Init(_wnd_width, _wnd_height);
    //    _text_renderer->SetTextureUnit(TEXT_TEXTURE_UNIT_INDEX);
    //_verdana = _text_renderer->GetAtlas("resources/fonts/verdana.ttf", 50);
    //_arial = _text_renderer->GetAtlas("resources/fonts/arial.ttf", 20);
    //if (_picking_fbo == nullptr)
    //    _picking_fbo = new PickingFBO();
    //_picking_fbo->Init(_wnd_width, _wnd_height);
    //if (_picking_program == nullptr)
    //    _picking_program = new PickingProgram();
    //_picking_program->Init();

    //if (_plain_program == nullptr)
    //    _plain_program = new PlainColorProgram();
    //_plain_program->Init();

    //glActiveTexture(DISPLACEMENT_TEXTURE_UNIT);
    //if (_displacement_map == nullptr)
    //    _displacement_map = new Texture2D("heightmap.jpg", GL_TEXTURE_2D);
    //_displacement_map->Load().Bind(DISPLACEMENT_TEXTURE_UNIT);
    //glActiveTexture(COLOR_TEXTURE_UNIT);
    /*if (_color_map == nullptr)
        _color_map = new Texture2D("white.png", GL_TEXTURE_2D);
    _color_map->Load();*/
    //glActiveTexture(NORMAL_TEXTURE_UNIT);
    //if (_normal_map == nullptr)
    //    _normal_map = new Texture2D("diffuse_normal.png", GL_TEXTURE_2D);
    //_normal_map->Load();
    //_lighting_program->Use();
    //_lighting_program//->AddSpotLight(_spot_light)
    //                 //.SetSpotLightsCount(1)
    //    ->SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX)
    //    //.SetSpecularIntensity(0.0f)
    //    //.SetSpecularPower(0.0f)
    //    .SetDirectionalLight(_dir_light)
    //    .SetColor(0, glm::vec4(1.0f, 0.5f, 0.5f, 0.0f))
    //    .SetColor(1, glm::vec4(0.5f, 1.0f, 1.0f, 0.0f))
    //    .SetColor(2, glm::vec4(1.0f, 0.5f, 1.0f, 0.0f))
    //    .SetColor(3, glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
        //.SetDisplacementMapTextureUnit(DISPLACEMENT_TEXTURE_UNIT_INDEX)
        //.SetNormalMapTextureUnit(NORMAL_TEXTURE_UNIT_INDEX)
        //.SetDisplacementFactor(_displacement_factor);
        //.SetShadowMapTextureUnit(SHADOW_TEXTURE_UNIT_INDEX)

    //if (_shadowmap_program == nullptr)
    //    _shadowmap_program = new ShadowProgram();
    //_shadowmap_program->Init();
    try
    {
        if (_mesh == nullptr)
            _mesh = new Mesh();
        _mesh->Load("resources/models/spider.obj");
        //if (_ground == nullptr)
        //    _ground = new Mesh();
        //_ground->Load("quad.obj");
        //if (_billboard == nullptr)
        //    _billboard = new Billboard();
        //_billboard->Init("excal.png");

        //if (_ground_tex == nullptr)
        //    _ground_tex = new Texture2D("bricks.jpg", GL_TEXTURE_2D);
        //_ground_tex->Load().Bind(COLOR_TEXTURE_UNIT);
        //if (_ground_tex_normal_map == nullptr)
        //    _ground_tex_normal_map = new Texture2D("normal_map.jpg", GL_TEXTURE_2D);
        //_ground_tex_normal_map->Load();
        /*if (_mesh_tex == nullptr)
            _mesh_tex = new Texture2D("bricks.jpg", GL_TEXTURE_2D);*/
        //_mesh_tex->Load().Bind(COLOR_TEXTURE_UNIT);

        //if (_skybox == nullptr)
        //    _skybox = new SkyBox(_camera, _perspective_info);
        //_skybox->Init
        //(
        //    "cube.obj",
        //    "sp3right.jpg",
        //    "sp3left.jpg",
        //    "sp3top.jpg",
        //    "sp3bot.jpg",
        //    "sp3front.jpg",
        //    "sp3back.jpg"
        //);

        //if (_particle_system == nullptr)
        //    _particle_system = new ParticleSystem();
        //_particle_system->Init(glm::vec3(0.0f, 0.0f, 1.0f));
    }
    catch (std::runtime_error& e)
    {
        DeallocateResources();
        std::cerr << "Error initializing meshes" << std::endl;
        throw e;
    }
    //CalcPositions();
}

void gl_app::OpenGLApplication::ShadowMapPass()
{
    /*_shadow_fbo->BindWrite();

    glClear(GL_DEPTH_BUFFER_BIT);

    _shadowmap_program->Use();

    glm::vec3 up = _camera->GetUp();
    transform::Transformation trans;
    trans.Scaling(0.1f)
        .Rotation(0.0f, _scale, 0.0f)
        .Translation(0.0f, 1.0f, 3.0f)
        .PerspectiveProjection(_perspective_info)
        .Cam(_spot_light.GetPosition(), _spot_light.GetDirection(), UP);
    
    _shadowmap_program->SetWVP(trans.WVPTransform());
    _mesh->Render();

    glBindFramebuffer(GL_FRAMEBUFFER, NULL);*/
}

void gl_app::OpenGLApplication::PickingPass()
{
    //transform::Transformation trans;

    //trans.Scaling(0.1f)
    //    .Rotation(0.0f, 90.0f, 0.0f)
    //    .Cam(*_camera)
    //    .PerspectiveProjection(_perspective_info);
    //
    //_picking_fbo->BindWrite();
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //_picking_program->Use();
    //
    //size_t size = sizeof(_world_pos) / sizeof(_world_pos[0]);
    //for (int i = 0; i < size; ++i)
    //{
    //    trans.Translation(_world_pos[i]);
    //    _picking_program->SetObjIndex(i)
    //        .SetWVP(trans.WVPTransform());
    //    _mesh->Render(_picking_program);
    //}

    //_picking_fbo->DisableWrite();
}

void gl_app::OpenGLApplication::RenderPass()
{
    //long long crt_time = GetTickCount();
    //assert(crt_time >= _crt_time_ms);
    //GLuint delta_millis = static_cast<GLuint>(crt_time - _crt_time_ms);
    //_crt_time_ms = crt_time;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _lighting_program->Use();
    _lighting_program->SetEWP(_camera->GetEye())
        .SetDirectionalLight(_dir_light);

    transform::Transformation trans;
    trans.Scaling(0.005f)
        //.Translation(5.0f, -6.0f, 0.0f)
        .Rotation(0.0f, 90.0f, 0.0f)
        .Cam(*_camera)
        .PerspectiveProjection(_perspective_info);
    glm::mat4 wvp_matrices[INSTANCE_COUNT];
    glm::mat4 w_matrices[INSTANCE_COUNT];
    for (GLuint i = 0; i < INSTANCE_COUNT; ++i)
    {
        glm::vec3 pos = _positions[i];
        pos.y += glm::sin(_scale) * _velocities[i];
        trans.Translation(pos);
        wvp_matrices[i] = glm::transpose(trans.WVPTransform());
        w_matrices[i] = glm::transpose(trans.WorldTransform());
    }

    //_text_renderer->SetWVP(trans.WVPTransform());
    _mesh->Render(INSTANCE_COUNT, wvp_matrices, w_matrices);

    /*if (_left_mb.is_pressed)
    {
        PickingFBO::PixelInfo pixel = _picking_fbo->ReadPixel(_left_mb.x, _wnd_height - _left_mb.y - 1);
        if (pixel.primitive_id != 0)
        {
            _plain_program->Use();
            if (pixel.object_id >= (sizeof(_world_pos) / sizeof(_world_pos[0]))) throw std::runtime_error("Primitive ID was out of range");
            
            trans.Translation(_world_pos[static_cast<GLint>(pixel.object_id)]);
            _plain_program->SetWVP(trans.WVPTransform());
            _mesh->Render(static_cast<GLuint>(pixel.draw_id), static_cast<GLuint>(pixel.primitive_id - 1));
        }
    }*/
    //_normal_map->Bind(NORMAL_TEXTURE_UNIT);
        //.SetVP(trans.VPTransform())
        //.SetWVP(trans.WVPTransform())
        //.SetWM(trans.WorldTransform())
        //.SetDirectionalLight(_dir_light)
        //.SetTesselationLevel(_tess_level)
        //.SetTesselationAlpha(_tess_alpha);
        //.SetDisplacementFactor(_displacement_factor);
    //_mesh->Render(nullptr, true);
    //trans.Translation(10.0f, -6.0f, 0.0f)
        //.Rotation(-45.0f, -40.0f, -10.0f);
    //_lighting_program->SetWVP(trans.WVPTransform())
        //.SetWM(trans.WorldTransform())
        //.SetTesselationLevel(1.0f);
    //_mesh->Render(nullptr, true);
}

void gl_app::OpenGLApplication::DeferredShadingGeometryPass()
{
    _deferred_shading_program->Use();

    _geometry_buffer->BindWrite();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    transform::Transformation trans;
    trans.Scaling(0.1f, 0.1f, 0.1f);
    trans.Rotation(0.0f, _scale, 0.0f);
    trans.Translation(-0.8f, -1.0f, 12.0f);
    trans.Cam(*_camera);
    trans.PerspectiveProjection(_perspective_info);
    _deferred_shading_program->SetWVP(trans.WVPTransform())
        .SetWorldMatrix(trans.WorldTransform());
    _mesh->Render();
}

void gl_app::OpenGLApplication::DeferredShadingLightPass()
{
    _geometry_buffer->DisableWrite();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _geometry_buffer->BindRead();

    GLint half_width = (GLint)(_wnd_width / 2.0f);
    GLint half_height = (GLint)(_wnd_height / 2.0f);

    _geometry_buffer->SetReadBuffer(GeometryBuffer::GBUFFER_TEX_TYPE_POSITION);
    glBlitFramebuffer(0, 0, _wnd_width, _wnd_height, 0, 0, half_width, half_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    _geometry_buffer->SetReadBuffer(GeometryBuffer::GBUFFER_TEX_TYPE_DIFFUSE);
    glBlitFramebuffer(0, 0, _wnd_width, _wnd_height, 0, half_height, half_width, _wnd_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    _geometry_buffer->SetReadBuffer(GeometryBuffer::GBUFFER_TEX_TYPE_NORMAL);
    glBlitFramebuffer(0, 0, _wnd_width, _wnd_height, half_width, half_height, _wnd_width, _wnd_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    _geometry_buffer->SetReadBuffer(GeometryBuffer::GBUFFER_TEX_TYPE_TEXCOORD);
    glBlitFramebuffer(0, 0, _wnd_width, _wnd_height, half_width, 0, _wnd_width, half_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void gl_app::OpenGLApplication::CalcPositions()
{
    for (GLuint i = 0; i < ROWCOUNT; ++i)
        for (GLuint j = 0; j < COLCOUNT; ++j)
        {
            GLuint index = i * COLCOUNT + j;
            _positions[index].x = static_cast<GLfloat>(i);
            _positions[index].y = GenRandFloat();
            _positions[index].z = static_cast<GLfloat>(j);
            _velocities[index] = GenRandFloat();
            if (i & 0x1)
            {
                _velocities[index] *= -1.0f;
            }
        }
}

void gl_app::OpenGLApplication::CalcFPS()
{
    static GLuint framecount = 0;
    ++framecount;

    long long crt_time = GetTickCount();
    if (crt_time - _frametime >= 1000)
    {
        _frametime = crt_time;
        _fps = static_cast<GLfloat>(framecount);
        framecount = 0;
    }
}

GLfloat gl_app::OpenGLApplication::CalcUptime()
{
    GLfloat running_time = static_cast<GLfloat>(GetTickCount() - _start_time) / 1000.0f;

    return running_time;
}

void gl_app::OpenGLApplication::DeallocateResources()
{
    if (_camera != nullptr)
    {
        delete _camera;
        _camera = nullptr;
    }
    if (_mesh != nullptr)
    {
        delete _mesh;
        _mesh = nullptr;
    }
    //if (_shadowmap_program != nullptr)
    //{
    //    delete _shadowmap_program;
    //    _shadowmap_program = nullptr;
    //}
    if (_lighting_program != nullptr)
    {
        delete _lighting_program;
        _lighting_program = nullptr;
    }
    if (_text_renderer != nullptr)
    {
        delete _text_renderer;
        _text_renderer = nullptr;
    }
    if (_geometry_buffer != nullptr)
    {
        delete _geometry_buffer;
        _geometry_buffer = nullptr;
    }
    if (_deferred_shading_program != nullptr)
    {
        delete _deferred_shading_program;
        _deferred_shading_program = nullptr;
    }
    //if (_normal_map != nullptr)
    //{
    //    delete _normal_map;
    //    _normal_map = nullptr;
    //}
    //if (_picking_program != nullptr)
    //{
    //    delete _picking_program;
    //    _picking_program = nullptr;
    //}
    //if (_plain_program != nullptr)
    //{
    //    delete _plain_program;
    //    _plain_program = nullptr;
    //}
    //if (_picking_fbo != nullptr)
    //{
    //    delete _picking_fbo;
    //    _picking_fbo = nullptr;
    //}
    //if (_skybox != nullptr)
    //{
    //    delete _skybox;
    //    _skybox = nullptr;
    //}
    //if (_displacement_map != nullptr)
    //{
    //    delete _displacement_map;
    //    _displacement_map = nullptr;
    //}
    //if (_color_map != nullptr)
    //{
    //    delete _color_map;
    //    _color_map = nullptr;
    //}
    //if (_mesh_tex != nullptr)
    //{
    //    delete _mesh_tex;
    //    _mesh_tex = nullptr;
    //}
    //if (_ground_tex_normal_map != nullptr)
    //{
    //    delete _ground_tex_normal_map;
    //    _ground_tex_normal_map = nullptr;
    //}
    //if (_ground_tex != nullptr)
    //{
    //    delete _ground_tex;
    //    _ground_tex = nullptr;
    //}
    //if (_billboard != nullptr)
    //{
    //    delete _billboard;
    //    _billboard = nullptr;
    //}
    //if (_particle_system != nullptr)
    //{
    //    delete _particle_system;
    //    _particle_system = nullptr;
    //}

    glfwTerminate();
}

gl_app::OpenGLApplication::OpenGLApplication()
: OpenGLApplication(DEFAULT_WND_WIDTH, DEFAULT_WND_HEIGHT)
{}

gl_app::OpenGLApplication::OpenGLApplication(GLulong _wnd_width, GLulong _wnd_height)
: _wnd_width(_wnd_width)
, _wnd_height(_wnd_height)
, _wnd(nullptr)
, _camera(nullptr)
, _mesh(nullptr)
//, _color_map(nullptr)
//, _displacement_map(nullptr)
//, _normal_map(nullptr)
//, _picking_program(nullptr)
, _deferred_shading_program(nullptr)
, _lighting_program(nullptr)
, _geometry_buffer(nullptr)
//, _billboard(nullptr)
//, _particle_system(nullptr)
//, _skybox(nullptr)
//, _shadowmap_program(nullptr)
//, _ground(nullptr)
//, _ground_tex(nullptr)
//, _ground_tex_normal_map(nullptr)
//, _mesh_tex(nullptr)
, _scale(DEFAULT_SCALE)
, _display_stats(false)
, _fps(0.0f)
, _frametime(0LL)
, _start_time(0LL)
//, _displacement_factor(0.25f)
//, _tess_level(5.0f)
//, _tess_alpha(1.0f)
//, _is_wireframe(false)
{
    _perspective_info.fov    = DEFAULT_FOV;
    _perspective_info.width  = _wnd_width;
    _perspective_info.height = _wnd_height;
    _perspective_info.z_far  = Z_FAR;
    _perspective_info.z_near = Z_NEAR;

    //_spot_light.SetColor(COLOR_WHITE)
    //    .SetAmbientIntensity(LIGHT_INTENSITY_MAX / 100.0f)
    //    .SetDiffuseIntensity(LIGHT_INTENSITY_MAX - 0.7f);
    //_spot_light.SetDirection(glm::vec3(1.0f, -1.0f, 0.0f))
    //    .SetDirection(glm::vec3(1.0f, -1.0f, 0.0f))
    //    .SetConeAngle(20.0f)
    //    .SetPosition(glm::vec3(-20.0, 20.0, 1.0f))
    //    .SetAttenuation(0.0f, 0.01f, 0.0f);

    //_dir_light.SetColor(COLOR_WHITE)
    //    .SetAmbientIntensity(0.55f)
    //    .SetDiffuseIntensity(0.9f);
    //_dir_light.SetDirection(glm::vec3(1.0f, 0.0f, 0.0f));
    //_left_mb.is_pressed = false;
    //_world_pos[0] = glm::vec3(-10.0f, 0.0f, 5.0f);
    //_world_pos[1] = glm::vec3(10.0f, 0.0f, 5.0f);
    //_crt_time_ms = GetTickCount();

    _frametime = _start_time = GetTickCount();
}

gl_app::OpenGLApplication::OpenGLApplication(GLulong _wnd_width, GLulong _wnd_height, Camera* camera)
: OpenGLApplication(_wnd_width, _wnd_height)
{
    _camera = camera;
}

GLFWwindow* SetupMainWindow(GLchar* title, GLenum is_fullscreen, GLint sample_count, GLint openGL_version_major, GLint openGL_version_minor, GLint is_forward_compatible, GLint openGL_profile, GLulong width, GLulong height)
{
    glfwWindowHint(GLFW_SAMPLES, sample_count);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openGL_version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openGL_version_minor);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, is_forward_compatible);
    glfwWindowHint(GLFW_OPENGL_PROFILE, openGL_profile);

    return glfwCreateWindow(width, height, title, is_fullscreen == GL_TRUE ? glfwGetPrimaryMonitor() : nullptr, nullptr);
}

RetCodes gl_app::OpenGLApplication::MakeWindow(GLchar* title, GLenum is_fullscreen, GLint sampling, GLint openGL_version_major, GLint openGL_version_minor, GLint openGL_profile, GLenum capture_input, GLenum hide_cursor, GLenum depth_test)
{
    GLint result = glfwInit();
    if (result != GLFW_TRUE)
    {
        std::cerr << "An error occurred while initializing GLFW. Error code: " << result << ". The application will now close." << std::endl;
        return ERR_GLFW_INIT;
    }
    _wnd = SetupMainWindow(title, is_fullscreen, sampling, openGL_version_major, openGL_version_minor, GL_TRUE, openGL_profile, _wnd_width, _wnd_height);
    if (_wnd == nullptr)
        return ERR_WND_CREATE;

    // set newly created window as current context, and add an event handler for the keypresses
    glfwMakeContextCurrent(_wnd);
    if (capture_input == GL_TRUE)
        glfwSetInputMode(_wnd, GLFW_STICKY_KEYS, GL_TRUE); // make keyboard input be captured by window
    if (hide_cursor == GL_TRUE)
        glfwSetInputMode(_wnd, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); // hide cursor

    // setup GLEW
    glewExperimental = true;
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cerr << "Error " << err << " initializing GLEW: " << glewGetErrorString(err) << std::endl;
        return ERR_GLEW_INIT;
    }

    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    if (depth_test == GL_TRUE)
        glEnable(GL_DEPTH_TEST);

    glfwSetCursorPos(_wnd, _wnd_width / 2, _wnd_height / 2);

    Init();
    
    return SUCCESS;
}

gl_app::OpenGLApplication& gl_app::OpenGLApplication::SetWindowWidth(GLulong width)
{
    _wnd_width = width;

    return *this;
}

gl_app::OpenGLApplication & gl_app::OpenGLApplication::SetWindowHeight(GLulong height)
{
    _wnd_height = height;

    return *this;
}

gl_app::OpenGLApplication & gl_app::OpenGLApplication::SetBackgroundColor(const glm::vec4& rgba)
{
    _clear_color = rgba;

    return *this;
}

gl_app::OpenGLApplication & gl_app::OpenGLApplication::SetBackgroundColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    SetBackgroundColor(glm::vec4(r, g, b, a));

    return *this;
}

gl_app::OpenGLApplication & gl_app::OpenGLApplication::Run()
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");
    do
    {
        glClearColor(_clear_color.r, _clear_color.g, _clear_color.b, _clear_color.a);

        _camera->OnRender();
        _scale += 0.05f;
        
        //ShadowMapPass();
        //PickingPass();
        //RenderPass();
        
        DeferredShadingGeometryPass();
        DeferredShadingLightPass();

        //if (_display_stats)
        //{
        //    CalcFPS();
        //    _text_renderer->Render("FPS: " + std::to_string(_fps), _arial, glm::vec2(10.0f, _wnd_height - 30), 1.0f, glm::vec4(COLOR_WHITE, 0.2f));
        //    _text_renderer->Render("Uptime: " + std::to_string(CalcUptime()) + " seconds", _arial, glm::vec2(10.0f, _wnd_height - 55), 1.0f, glm::vec4(COLOR_WHITE, 0.2f));
        //}

        glfwSwapBuffers(_wnd);
        glfwPollEvents();
    } while (!glfwWindowShouldClose(_wnd));
    return *this;
}

gl_app::OpenGLApplication & gl_app::OpenGLApplication::SetCamera(Camera* camera)
{
    _camera = camera;

    return *this;
}

//gl_app::OpenGLApplication & gl_app::OpenGLApplication::SetDisplacementFactor(GLfloat displacement_factor)
//{
//    _displacement_factor = glm::clamp(displacement_factor, 0.01f, 1000.0f);
//
//    return *this;
//}

gl_app::OpenGLApplication & gl_app::OpenGLApplication::SetPerspectiveZNear(GLfloat z_near)
{
    _perspective_info.z_near = z_near;

    return *this;
}

gl_app::OpenGLApplication & gl_app::OpenGLApplication::SetPerspectiveZFAR(GLfloat z_far)
{
    _perspective_info.z_far = z_far;

    return *this;
}

gl_app::OpenGLApplication & gl_app::OpenGLApplication::SetPerspectiveFOV(GLfloat fov)
{
    _perspective_info.fov = fov;

    return *this;
}

gl_app::OpenGLApplication& gl_app::OpenGLApplication::SetErrorCallback(const ErrorCallbackFn& callback)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    glfwSetErrorCallback(callback);

    return *this;
}

gl_app::OpenGLApplication& gl_app::OpenGLApplication::SetKeyCallback(const KeyCallbackFn& callback)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    glfwSetKeyCallback(_wnd, callback);

    return *this;
}

gl_app::OpenGLApplication& gl_app::OpenGLApplication::SetCursorCallback(const MouseCallbackFn& callback)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    glfwSetCursorPosCallback(_wnd, callback);

    return *this;
}

gl_app::OpenGLApplication & gl_app::OpenGLApplication::SetMouseButtonCallback(const GLFWmousebuttonfun & callback)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    glfwSetMouseButtonCallback(_wnd, callback);

    return *this;
}

gl_app::OpenGLApplication& gl_app::OpenGLApplication::SetMouseScrollCallback(const MouseCallbackFn& callback)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    glfwSetScrollCallback(_wnd, callback);

    return *this;
}

gl_app::OpenGLApplication & gl_app::OpenGLApplication::ToggleStatsDisplay()
{
    _display_stats = !_display_stats;

    return *this;
}

//gl_app::OpenGLApplication & gl_app::OpenGLApplication::SetTesselationLevel(const GLfloat level)
//{
//    _tess_level = glm::clamp(level, 0.0f, 1000.0f);
//
//    return *this;
//}
//
//gl_app::OpenGLApplication & gl_app::OpenGLApplication::SetTesselationAlpha(const GLfloat alpha)
//{
//    _tess_alpha = glm::clamp(alpha, 0.0f, 1.0f);
//    
//    return *this;
//}

std::pair<GLint, GLint> gl_app::OpenGLApplication::GetMousePos()
{
    GLdouble x, y;
    glfwGetCursorPos(_wnd, &x, &y);

    return std::pair<GLint, GLint>(static_cast<GLint>(x), static_cast<GLint>(y));
}

gl_app::OpenGLApplication::~OpenGLApplication()
{
    DeallocateResources();
}
