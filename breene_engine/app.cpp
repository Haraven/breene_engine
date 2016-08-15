#include <Windows.h>
#include "app.h"
#include <iostream>

static const GLfloat FIELD_DEPTH = 20.0f;
static const GLfloat FIELD_WIDTH = 10.0f;
const glm::vec3 UP(0.0f, 1.0f, 0.0f);

void breene::BreeneApplication::Init()
{
	if (_blank_program == nullptr)
		_blank_program = new BlankProgram();
	_blank_program->Init();

	if (_geometry_buffer == nullptr)
		_geometry_buffer = new GeometryBuffer();
	_geometry_buffer->Init(_wnd_width, _wnd_height);
	
	glm::vec3 pos = ORIGIN + glm::vec3(0.0f, 1.5f, 3.0f);
	glm::vec3 look_at(0.09f, -0.45f, 0.88f);
	_camera = new Camera(_wnd_width, _wnd_height, look_at, pos, UP);

	if (_deferred_shading_geometry_program == nullptr)
		_deferred_shading_geometry_program = new DefShadingGeomProgram();
	_deferred_shading_geometry_program->Init().Use();
	_deferred_shading_geometry_program->SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
	//	.SetTessAlpha(1.0f)
	//	.SetTessLevel(1.0f);

	if (_dir_light_program == nullptr)
		_dir_light_program = new DefShadingDirLight();
	_dir_light_program->Init().Use();
	_dir_light_program->SetPositionTextureUnit(GeometryBuffer::GBUFFER_TEX_TYPE_POSITION)
		.SetColorTextureUnit(GeometryBuffer::GBUFFER_TEX_TYPE_DIFFUSE)
		.SetNormalTextureUnit(GeometryBuffer::GBUFFER_TEX_TYPE_NORMAL)
		//.SetSpecularIntensity(0.1f)
		//.SetSpecularPower(0.5f)
		.SetScreenSize(_wnd_width, _wnd_height);
	_dir_light_program->SetDirectionalLight(_dir_light)
		.SetWVP(glm::mat4(1.0f));

	if (_pt_light_program == nullptr)
		_pt_light_program = new DefShadingPointLight();
	_pt_light_program->Init().Use();
	_pt_light_program->SetPositionTextureUnit(GeometryBuffer::GBUFFER_TEX_TYPE_POSITION)
		.SetColorTextureUnit(GeometryBuffer::GBUFFER_TEX_TYPE_DIFFUSE)
		.SetNormalTextureUnit(GeometryBuffer::GBUFFER_TEX_TYPE_NORMAL)
		.SetScreenSize(_wnd_width, _wnd_height);

	if (_spot_light_program == nullptr)
		_spot_light_program = new DefShadingSpotLight();
	_spot_light_program->Init().Use();
	_spot_light_program->SetPositionTextureUnit(GeometryBuffer::GBUFFER_TEX_TYPE_POSITION)
		.SetColorTextureUnit(GeometryBuffer::GBUFFER_TEX_TYPE_DIFFUSE)
		.SetNormalTextureUnit(GeometryBuffer::GBUFFER_TEX_TYPE_NORMAL)
		.SetScreenSize(_wnd_width, _wnd_height);
	_spot_light_program->SetSpotLight(_spot_light);

    if (_text_renderer == nullptr)
        _text_renderer = new text_rendering::TextRenderer();
    _text_renderer->Init(_wnd_width, _wnd_height);
        _text_renderer->SetTextureUnit(TEXT_TEXTURE_UNIT_INDEX);
    _verdana = _text_renderer->GetAtlas("resources/fonts/verdana.ttf", 50);
    _arial = _text_renderer->GetAtlas("resources/fonts/arial.ttf", 20);
    
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

	//if (_lighting_program == nullptr)
	//	_lighting_program = new LightingProgram();
	//_lighting_program->Init().Use();
	//_lighting_program->SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX)
	//	//.AddPointLight(_point_lights[0])
	//	//.SetPointLightsCount(1)
	//	//.SetSpotLightsCount(1)
	//	.AddSpotLight(_spot_light)
	//	.SetSpotLightsCount(1)
	//	.SetDirectionalLight(_dir_light)
	//	.SetTesselationAlpha(1.0f)
	//	.SetTesselationLevel(1.0f)
	//	//.SetSpecularIntensity(0.5f)
	//	//.SetSpecularPower(0.6f);
	//	.SetColor(0, glm::vec4(1.0f, 0.5f, 0.5f, 0.0f))
	//	.SetColor(1, glm::vec4(0.5f, 1.0f, 1.0f, 0.0f))
	//	.SetColor(2, glm::vec4(1.0f, 0.5f, 1.0f, 0.0f))
	//	.SetColor(3, glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
        //.SetDisplacementMapTextureUnit(DISPLACEMENT_TEXTURE_UNIT_INDEX)
        //.SetDisplacementFactor(_displacement_factor);
        //.SetNormalMapTextureUnit(NORMAL_TEXTURE_UNIT_INDEX)
        //.SetShadowMapTextureUnit(SHADOW_TEXTURE_UNIT_INDEX)
    //if (_shadowmap_program == nullptr)
    //    _shadowmap_program = new ShadowProgram();
    //_shadowmap_program->Init();
    try
    {
        if (_box == nullptr)
            _box = new Mesh();
        _box->Load("resources/models/box.obj");
		if (_quad == nullptr)
			_quad = new Mesh();
		_quad->Load("resources/models/quad.obj");
		if (_sphere == nullptr)
			_sphere = new Mesh();
		_sphere->Load("resources/models/sphere.obj");
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

        if (_skybox == nullptr)
            _skybox = new SkyBox(_camera, _perspective_info);
        _skybox->Init
        (
            "resources/models/cube.obj",
            "resources/images/skybox2/right.png",
            "resources/images/skybox2/left.png",
            "resources/images/skybox2/up.png",
            "resources/images/skybox2/down.png",
            "resources/images/skybox2/front.png",
            "resources/images/skybox2/back.png"
        );

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
}

void breene::BreeneApplication::InitLights()
{
	_spot_light.SetAmbientIntensity(0.9f);
	_spot_light.SetDiffuseIntensity(0.9f);
	_spot_light.SetColor(COLOR_MAGENTA);
	_spot_light.SetAttenuation(0.0f, 0.01f, 0.0f);
	_spot_light.SetPosition(_positions[0] + glm::vec3(0.0f, 1.0f, 0.0f));
	_spot_light.SetDirection(glm::vec3(1.0f, -1.0f, 0.0f));
	_spot_light.SetConeAngle(80.0f);

	_dir_light.SetAmbientIntensity(0.05f);
	_dir_light.SetColor(glm::vec3(1.0f, 1.0f, 0.67f));
	_dir_light.SetDiffuseIntensity(0.9f);
	_dir_light.SetDirection(glm::vec3(-50.0f, -10.0f, 0.0f));

	_point_lights[0].SetDiffuseIntensity(0.2f);
	_point_lights[0].SetColor(COLOR_GREEN);
	_point_lights[0].SetPosition(glm::vec3(0.0f, 1.5f, 5.0f));
	_point_lights[0].SetAttenuation(0.0f, 0.0f, 0.3f);

	_point_lights[1].SetDiffuseIntensity(0.2f);
	_point_lights[1].SetColor(COLOR_RED);
	_point_lights[1].SetPosition(glm::vec3(2.0f, 0.0f, 5.0f));
	_point_lights[1].SetAttenuation(0.0f, 0.0f, 0.3f);

	_point_lights[2].SetDiffuseIntensity(0.2f);
	_point_lights[2].SetColor(COLOR_BLUE);
	_point_lights[2].SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
	_point_lights[2].SetAttenuation(0.0f, 0.0f, 0.3f);
}

void breene::BreeneApplication::InitPositions()
{
	_positions[0] = glm::vec3(0.0f, 0.0f, 5.0f);
	_positions[1] = glm::vec3(6.0f, 1.0f, 10.0f);
	_positions[2] = glm::vec3(-5.0f, -1.0f, 12.0f);
	_positions[3] = glm::vec3(4.0f, 4.0f, 15.0f);
	_positions[4] = glm::vec3(-4.0f, 2.0f, 20.0f);
}

void breene::BreeneApplication::ShadowMapPass()
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

void breene::BreeneApplication::PickingPass()
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

void breene::BreeneApplication::RenderPass()
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
    trans.Scaling(0.1f)
        //.Translation(5.0f, -6.0f, 0.0f)
        .Rotation(0.0f, 180.0f, 0.0f)
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

    _box->Render(INSTANCE_COUNT, wvp_matrices, w_matrices, nullptr, true);

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

GLfloat breene::BreeneApplication::CalcPointLightSphere(const PointLight & light)
{
	glm::vec3 color = light.GetColor();
	GLfloat max_channel = glm::max(glm::max(color.r, color.g), color.b);

	LightAttenuation atten = light.GetAttenuation();
	GLfloat ret = (-atten.linear + glm::sqrt(atten.linear * atten.linear - 4 * atten.exponential * (atten.exponential - 256 * max_channel * light.GetDiffuseIntensity())))
		/
		(2 * atten.exponential);
	
	return ret;
}

GLfloat breene::BreeneApplication::CalcSpotLightSphere(const SpotLight & light)
{
	glm::vec3 color = light.GetColor();
	GLfloat max_channel = glm::max(glm::max(color.r, color.g), color.b);

	LightAttenuation atten = light.GetAttenuation();
	GLfloat ret = (-atten.linear + glm::sqrt(atten.linear * atten.linear - 4 * atten.exponential * (atten.exponential - 256 * max_channel * light.GetDiffuseIntensity())))
		/
		(2 * atten.exponential);

	return ret;
}

void breene::BreeneApplication::DeferredShadingGeometryPass()
{
	_deferred_shading_geometry_program->Use();

	_geometry_buffer->BindGeomPass();

	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	transform::Transformation trans;
	trans.Cam(*_camera)
		.PerspectiveProjection(_perspective_info)
		//.Scaling(10.0f, 0.1f, 10.0f)
		.Rotation(0.0f, _scale, 0.0f);
	for (GLuint i = 0; i < sizeof(_positions) / sizeof(glm::vec3); ++i)
	{
		trans.Translation(_positions[i]);
		_deferred_shading_geometry_program->SetWVP(trans.WVPTransform())
			.SetWorldMatrix(trans.WorldTransform());
		_box->Render();
	}

	glDepthMask(GL_FALSE);
}

void breene::BreeneApplication::DefShadingStencilPass(GLuint index)
{
	_blank_program->Use();

	_geometry_buffer->BindStencilPass();
	
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glStencilFunc(GL_ALWAYS, 0, 0);
	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

	GLfloat box_Scale = CalcPointLightSphere(_point_lights[index]);
	transform::Transformation trans;
	trans.Translation(_point_lights[index].GetPosition())
		.Scaling(box_Scale)
		.Cam(*_camera)
		.PerspectiveProjection(_perspective_info);

	_blank_program->SetWVP(trans.WVPTransform());
	_sphere->Render();
}

void breene::BreeneApplication::DefShadingPointLightStencilPass()
{
	_blank_program->Use();

	_geometry_buffer->BindStencilPass();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glStencilFunc(GL_ALWAYS, 0, 0);
	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

	//GLfloat box_Scale = CalcPointLightSphere(_spot_light);
	transform::Transformation trans;
	trans.Translation(_spot_light.GetPosition())
		.Scaling(10.0f)
		.Cam(*_camera)
		.PerspectiveProjection(_perspective_info);

	_blank_program->SetWVP(trans.WVPTransform());
	_sphere->Render();
}

void breene::BreeneApplication::DefShadingPointLightPass(GLuint index)
{
	_geometry_buffer->BindLightPass();

	_pt_light_program->Use();
	_pt_light_program->SetEWP(_camera->GetEye());

	glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	GLfloat box_scale = CalcPointLightSphere(_point_lights[index]);
	transform::Transformation trans;
	trans.Translation(_point_lights[index].GetPosition())
		.Scaling(box_scale)
		.Cam(*_camera)
		.PerspectiveProjection(_perspective_info);
	_pt_light_program->SetWVP(trans.WVPTransform());
	_pt_light_program->SetPointLight(_point_lights[index]);
	_sphere->Render();

	glCullFace(GL_BACK);
	glDisable(GL_BLEND);
}

void breene::BreeneApplication::DefShadingSpotLightPass()
{
	_geometry_buffer->BindLightPass();

	_spot_light_program->Use();
	_spot_light_program->SetEWP(_camera->GetEye());

	glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	//GLfloat box_scale = CalcPointLightSphere(_spot_light);
	transform::Transformation trans;
	trans.Translation(_spot_light.GetPosition())
		.Scaling(10.0f)
		.Cam(*_camera)
		.PerspectiveProjection(_perspective_info);
	_spot_light_program->SetWVP(trans.WVPTransform());
	_sphere->Render();

	glCullFace(GL_BACK);
	glDisable(GL_BLEND);
}

void breene::BreeneApplication::DefShadingFinalPass()
{
	_geometry_buffer->BindFinalPass();
	glBlitFramebuffer(0, 0, _wnd_width, _wnd_height, 0, 0, _wnd_width, _wnd_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void breene::BreeneApplication::DeferredShadingDirLightPass()
{
	_geometry_buffer->BindLightPass();

	_dir_light_program->Use();
	_dir_light_program->SetEWP(_camera->GetEye());

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	_quad->Render();

	glDisable(GL_BLEND);
}

void breene::BreeneApplication::CalcPositions()
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

void breene::BreeneApplication::CalcFPS()
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

void breene::BreeneApplication::RenderFPS()
{
	if (_display_stats)
		if (_stats_to_display & DISPLAY_FPS)
		{
			CalcFPS();
			_text_renderer->Render("FPS: " + std::to_string(_fps), _arial, glm::vec2(10.0f, _wnd_height - 30), 1.0f, glm::vec4(COLOR_WHITE, 0.4f));
		}
		if (_stats_to_display & DISPLAY_UPTIME)
			_text_renderer->Render("Uptime: " + std::to_string(CalcUptime()) + " seconds", _arial, glm::vec2(10.0f, _wnd_height - 55), 1.0f, glm::vec4(COLOR_WHITE, 0.4f));
}

GLfloat breene::BreeneApplication::CalcUptime()
{
    GLfloat running_time = static_cast<GLfloat>(GetTickCount() - _start_time) / 1000.0f;

    return running_time;
}

void breene::BreeneApplication::DeallocateResources()
{
    if (_camera != nullptr)
    {
        delete _camera;
        _camera = nullptr;
    }
    if (_box != nullptr)
    {
        delete _box;
        _box = nullptr;
    }
    //if (_shadowmap_program != nullptr)
    //{
    //    delete _shadowmap_program;
    //    _shadowmap_program = nullptr;
    //}
	if (_blank_program != nullptr)
	{
		delete _blank_program;
		_blank_program = nullptr;
	}
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
    if (_deferred_shading_geometry_program != nullptr)
    {
        delete _deferred_shading_geometry_program;
        _deferred_shading_geometry_program = nullptr;
    }
	if (_dir_light_program != nullptr)
	{
		delete _dir_light_program;
		_dir_light_program = nullptr;
	}
	if (_pt_light_program != nullptr)
	{
		delete _pt_light_program;
		_pt_light_program = nullptr;
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
    if (_skybox != nullptr)
    {
        delete _skybox;
        _skybox = nullptr;
    }
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

breene::BreeneApplication::BreeneApplication()
: BreeneApplication(DEFAULT_WND_WIDTH, DEFAULT_WND_HEIGHT)
{}

breene::BreeneApplication::BreeneApplication(GLulong _wnd_width, GLulong _wnd_height)
: _wnd_width(_wnd_width)
, _wnd_height(_wnd_height)
, _wnd(nullptr)
, _camera(nullptr)
, _box(nullptr)
, _quad(nullptr)
, _sphere(nullptr)
//, _color_map(nullptr)
//, _displacement_map(nullptr)
//, _normal_map(nullptr)
//, _picking_program(nullptr)
, _blank_program(nullptr)
, _deferred_shading_geometry_program(nullptr)
, _lighting_program(nullptr)
, _dir_light_program(nullptr)
, _pt_light_program(nullptr)
, _spot_light_program(nullptr)
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
	
	InitPositions();
	//CalcPositions();
	InitLights();
    _frametime = _start_time = GetTickCount();
}

breene::BreeneApplication::BreeneApplication(GLulong _wnd_width, GLulong _wnd_height, Camera* camera)
: BreeneApplication(_wnd_width, _wnd_height)
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

RetCodes breene::BreeneApplication::MakeWindow(GLchar* title, GLenum is_fullscreen, GLint sampling, GLint openGL_version_major, GLint openGL_version_minor, GLint openGL_profile, GLenum capture_input, GLenum hide_cursor, GLenum depth_test)
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
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    Init();
    
    return SUCCESS;
}

breene::BreeneApplication& breene::BreeneApplication::SetWindowWidth(GLulong width)
{
    _wnd_width = width;

    return *this;
}

breene::BreeneApplication & breene::BreeneApplication::SetWindowHeight(GLulong height)
{
    _wnd_height = height;

    return *this;
}

//breene::BreeneApplication & breene::BreeneApplication::SetBackgroundColor(const glm::vec4& rgba)
//{
//    _clear_color = rgba;
//
//    return *this;
//}
//
//breene::BreeneApplication & breene::BreeneApplication::SetBackgroundColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
//{
//    SetBackgroundColor(glm::vec4(r, g, b, a));
//
//    return *this;
//}

breene::BreeneApplication & breene::BreeneApplication::Run()
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");
	do
    {
        _camera->OnRender();
        _scale += 0.05f;

		_geometry_buffer->StartFrame();
        DeferredShadingGeometryPass();
		
		glEnable(GL_STENCIL_TEST);
		GLuint ptlights_count = sizeof(_point_lights) / sizeof(PointLight);
		for (GLuint i = 0; i < ptlights_count; ++i)
		{
			DefShadingStencilPass(i);
			DefShadingPointLightPass(i);
		}
		//DefShadingPointLightStencilPass();
		//DefShadingSpotLightPass();
		glDisable(GL_STENCIL_TEST);

		DeferredShadingDirLightPass();

		_skybox->Render();
		RenderFPS();
		DefShadingFinalPass();

        glfwSwapBuffers(_wnd);
        glfwPollEvents();
    } while (!glfwWindowShouldClose(_wnd));
    return *this;
}

breene::ApplicationBuilder & breene::BreeneApplication::GetBuilder()
{
	static ApplicationBuilder builder = ApplicationBuilder();

	return builder;
}

breene::BreeneApplication & breene::BreeneApplication::SetCamera(Camera* camera)
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

breene::BreeneApplication & breene::BreeneApplication::SetPerspectiveZFAR(GLfloat z_far)
{
    _perspective_info.z_far = z_far;

    return *this;
}

breene::BreeneApplication & breene::BreeneApplication::SetPerspectiveFOV(GLfloat fov)
{
    _perspective_info.fov = fov;

    return *this;
}

breene::BreeneApplication& breene::BreeneApplication::SetErrorCallback(const ErrorCallbackFn& callback)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    glfwSetErrorCallback(callback);

    return *this;
}

breene::BreeneApplication& breene::BreeneApplication::SetKeyCallback(const KeyCallbackFn& callback)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    glfwSetKeyCallback(_wnd, callback);

    return *this;
}

breene::BreeneApplication& breene::BreeneApplication::SetCursorCallback(const MouseCallbackFn& callback)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    glfwSetCursorPosCallback(_wnd, callback);

    return *this;
}

breene::BreeneApplication & breene::BreeneApplication::SetMouseButtonCallback(const GLFWmousebuttonfun & callback)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    glfwSetMouseButtonCallback(_wnd, callback);

    return *this;
}

breene::BreeneApplication& breene::BreeneApplication::SetMouseScrollCallback(const MouseCallbackFn& callback)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    glfwSetScrollCallback(_wnd, callback);

    return *this;
}

breene::BreeneApplication & breene::BreeneApplication::ToggleStatsDisplay()
{
    _display_stats = !_display_stats;

    return *this;
}

breene::BreeneApplication & breene::BreeneApplication::SetStatsToDisplay(uint8_t stats)
{
	_stats_to_display = stats;

	return *this;
}

breene::BreeneApplication & breene::BreeneApplication::SetVsync(GLenum on_off)
{
	if (_wnd == nullptr) throw std::runtime_error("Application has not yet been started");

	if (on_off == GL_TRUE)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);

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

std::pair<GLint, GLint> breene::BreeneApplication::GetMousePos()
{
    GLdouble x, y;
    glfwGetCursorPos(_wnd, &x, &y);

    return std::pair<GLint, GLint>(static_cast<GLint>(x), static_cast<GLint>(y));
}

breene::BreeneApplication::~BreeneApplication()
{
    DeallocateResources();
}

breene::ApplicationBuilder::ApplicationBuilder()
: _width(DEFAULT_WND_WIDTH)
, _height(DEFAULT_WND_HEIGHT)
, _camera(nullptr)
, _draw_dist(Z_FAR)
, _fov(DEFAULT_FOV)
, _samples(0)
, _gl_ver_major(3)
, _gl_ver_minor(3)
, _title(nullptr)
, _display_stats(DISPLAY_NONE)
, _startup_params(VSYNC | FULLSCREEN)
//, _bg_color(glm::vec4(COLOR_BLACK, 1.0f))
{}

breene::ApplicationBuilder & breene::ApplicationBuilder::Width(GLulong width)
{
	_width = glm::clamp(width, 0UL, ULONG_MAX);

	return *this;
}

breene::ApplicationBuilder & breene::ApplicationBuilder::Height(GLulong height)
{
	_height = glm::clamp(height, 0UL, ULONG_MAX);

	return *this;
}

breene::ApplicationBuilder & breene::ApplicationBuilder::Cam(Camera * camera)
{
	_camera = camera;

	return *this;
}

breene::ApplicationBuilder & breene::ApplicationBuilder::DrawDistance(GLfloat units)
{
	_draw_dist = glm::clamp(units, 0.0f, FLT_MAX);

	return *this;
}

breene::ApplicationBuilder & breene::ApplicationBuilder::MSAA(GLuint samples)
{
	_samples = samples;

	return *this;
}

breene::ApplicationBuilder & breene::ApplicationBuilder::FOV(GLfloat fov)
{
	_fov = glm::clamp(fov, 0.0f, FLT_MAX);

	return *this;
}

breene::ApplicationBuilder & breene::ApplicationBuilder::GLVersion(GLint major, GLint minor)
{
	_gl_ver_major = glm::clamp(major, 0, INT_MAX);
	_gl_ver_minor = glm::clamp(minor, 0, INT_MAX);

	return *this;
}

breene::ApplicationBuilder & breene::ApplicationBuilder::Title(GLchar* title)
{
	_title = title;

	return *this;
}

breene::ApplicationBuilder & breene::ApplicationBuilder::StatsToDisplay(uint8_t stats)
{
	_display_stats = stats;

	return *this;
}

breene::ApplicationBuilder & breene::ApplicationBuilder::Vsync(bool on_off)
{
	if (on_off)
		_startup_params |= VSYNC;
	else
		_startup_params &= ~VSYNC;

	return *this;
}

breene::ApplicationBuilder & breene::ApplicationBuilder::Fullscreen(bool on_off)
{
	if (on_off)
		_startup_params |= FULLSCREEN;
	else
		_startup_params &= ~FULLSCREEN;

	return *this;
}

breene::ApplicationBuilder & breene::ApplicationBuilder::CaptureInput(bool on_off)
{
	if (on_off)
		_startup_params |= CAPTURE_INPUT;
	else
		_startup_params &= ~CAPTURE_INPUT;

	return *this;
}

breene::ApplicationBuilder & breene::ApplicationBuilder::HideCursor(bool on_off)
{
	if (on_off)
		_startup_params |= HIDE_CURSOR;
	else
		_startup_params &= ~HIDE_CURSOR;

	return *this;
}

breene::ApplicationBuilder & breene::ApplicationBuilder::DepthTest(bool on_off)
{
	if (on_off)
		_startup_params |= DEPTH_TEST;
	else
		_startup_params &= ~DEPTH_TEST;

	return *this;
}

breene::BreeneApplication * breene::ApplicationBuilder::Build()
{
	static BreeneApplication* app = new BreeneApplication();

	app->SetWindowWidth(_width)
		.SetWindowHeight(_height)
		.SetCamera(_camera)
		.SetPerspectiveZFAR(_fov)
		.SetPerspectiveFOV(_fov)
		.SetStatsToDisplay(_display_stats);

	auto res = app->MakeWindow
	(
		_title,
		_startup_params & FULLSCREEN ? GL_TRUE : GL_FALSE,
		_samples,
		_gl_ver_major,
		_gl_ver_minor,
		GLFW_OPENGL_CORE_PROFILE,
		_startup_params & CAPTURE_INPUT ? GL_TRUE : GL_FALSE,
		_startup_params & HIDE_CURSOR ? GL_TRUE : GL_FALSE,
		_startup_params & DEPTH_TEST ? GL_TRUE : GL_FALSE
	);

	if (res != SUCCESS)
	{
		delete app;
		app = nullptr;
		throw std::runtime_error("Error initializing Breene application window. Error code: " + std::to_string(res));
	}

	if (_display_stats != DISPLAY_NONE)
		app->ToggleStatsDisplay()
		.SetStatsToDisplay(_display_stats);
	//app->SetBackgroundColor(_bg_color);
	app->SetVsync(_startup_params & VSYNC ? GL_TRUE : GL_FALSE);

	return app;
}


