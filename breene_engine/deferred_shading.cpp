#include "deferred_shading.h"
#include <glm\gtc\type_ptr.hpp>
#include <iostream>
#include <string>
#include "my_constants.h"

breene::GeometryBuffer::GeometryBuffer()
: FramebufferObject()
{
    memset(_textures, NULL, sizeof(GLuint) * GBUFFER_NUM_TEXTURES);
    _depth_tex = NULL;
	_final_tex = NULL;
}

breene::GeometryBuffer & breene::GeometryBuffer::Init(GLuint width, GLuint height)
{
	// Create the FBO
	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);

	GLuint tex_count = sizeof(_textures) / sizeof(GLuint);
	glGenTextures(tex_count, _textures);
	glGenTextures(1, &_depth_tex);
	glGenTextures(1, &_final_tex);

	for (GLuint i = 0; i < tex_count; ++i)
	{
		glBindTexture(GL_TEXTURE_2D, _textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _textures[i], 0);
	}

	glBindTexture(GL_TEXTURE_2D, _depth_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, _depth_tex, 0);

	glBindTexture(GL_TEXTURE_2D, _final_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + GBUFFER_NUM_TEXTURES, GL_TEXTURE_2D, _final_tex, 0);
	
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) throw std::runtime_error("Error initializing G buffer. Framebuffer status: " + std::to_string(status));

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    return *this;
}

breene::GeometryBuffer & breene::GeometryBuffer::StartFrame()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
	glDrawBuffer(GL_COLOR_ATTACHMENT0 + GBUFFER_NUM_TEXTURES);
	glClear(GL_COLOR_BUFFER_BIT);

	return *this;
}

breene::GeometryBuffer & breene::GeometryBuffer::BindGeomPass()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);

	GLenum draw_buffers[GBUFFER_NUM_TEXTURES];
	for (GLuint i = 0; i < GBUFFER_NUM_TEXTURES; ++i)
		draw_buffers[i] = GL_COLOR_ATTACHMENT0 + i;

	glDrawBuffers(GBUFFER_NUM_TEXTURES, draw_buffers);

	return *this;
}

breene::GeometryBuffer & breene::GeometryBuffer::BindStencilPass()
{
	glDrawBuffer(GL_NONE);

	return *this;
}

breene::GeometryBuffer & breene::GeometryBuffer::BindLightPass()
{
	glDrawBuffer(GL_COLOR_ATTACHMENT0 + GBUFFER_NUM_TEXTURES);

	GLuint tex_count = sizeof(_textures) / sizeof(GLuint);
	for (unsigned int i = 0; i < tex_count; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, _textures[GBUFFER_TEX_TYPE_POSITION + i]);
	}

	return *this;
}

breene::GeometryBuffer & breene::GeometryBuffer::BindFinalPass()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _fbo);
	glReadBuffer(GL_COLOR_ATTACHMENT0 + GBUFFER_NUM_TEXTURES);

	return *this;
}

//breene::GeometryBuffer & breene::GeometryBuffer::BindRead()
//{
//	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
//
//	for (GLuint i = 0; i < sizeof(_textures) / sizeof(GLuint); ++i)
//	{
//		glActiveTexture(GL_TEXTURE0 + i);
//		glBindTexture(GL_TEXTURE_2D, _textures[GBUFFER_TEX_TYPE_POSITION + i]);
//	}
//
//    return *this;
//}

breene::GeometryBuffer::~GeometryBuffer()
{
    FramebufferObject::~FramebufferObject();
    if (_textures[0] != NULL)
    {
        glDeleteTextures(GBUFFER_NUM_TEXTURES, _textures);
        memset(_textures, NULL, sizeof(GLuint) * GBUFFER_NUM_TEXTURES);
    }
    if (_depth_tex != NULL)
    {
        glDeleteTextures(1, &_depth_tex);
        _depth_tex = NULL;
    }
	if (_final_tex != NULL)
	{
		glDeleteTextures(1, &_final_tex);
		_final_tex = NULL;
	}
}

breene::DefShadingGeomProgram::DefShadingGeomProgram()
: ShaderProgram()
{}

breene::DefShadingGeomProgram & breene::DefShadingGeomProgram::Init()
{
    ShaderProgram::Init();

    AddShader(Shader(DEFSHADING_GEOM_PASS_VERTEX_SHADER, GL_VERTEX_SHADER));
	AddShader(Shader(DEFSHADING_GEOM_PASS_TESC_SHADER, GL_TESS_CONTROL_SHADER));
	AddShader(Shader(DEFSHADING_GEOM_PASS_TESE_SHADER, GL_TESS_EVALUATION_SHADER));
    AddShader(Shader(DEFSHADING_GEOM_PASS_FRAGMENT_SHADER, GL_FRAGMENT_SHADER));
    Finalize();

    return *this;
}

breene::DefShadingGeomProgram & breene::DefShadingGeomProgram::SetWVP(const glm::mat4 & wvp)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint wvp_loc = GetUniformLocation(WVP_UNIFORM);

    glUniformMatrix4fv(wvp_loc, 1, GL_TRUE, glm::value_ptr(wvp));

    return *this;
}

breene::DefShadingGeomProgram & breene::DefShadingGeomProgram::SetWorldMatrix(const glm::mat4 & world)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint world_loc = GetUniformLocation(W_UNIFORM);
    glUniformMatrix4fv(world_loc, 1, GL_TRUE, glm::value_ptr(world));

    return *this;
}

breene::DefShadingGeomProgram & breene::DefShadingGeomProgram::SetColorTextureUnit(GLuint texture_unit)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint color_sampler_loc = GetUniformLocation(COLORMAP_UNIFORM);

    glUniform1i(color_sampler_loc, texture_unit);

    return *this;
}

breene::DefShadingGeomProgram & breene::DefShadingGeomProgram::SetTessLevel(GLfloat level)
{
	if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

	GLuint level_loc = GetUniformLocation(TESSLEVEL_UNIFORM);

	glUniform1f(level_loc, level);

	return *this;
}

breene::DefShadingGeomProgram & breene::DefShadingGeomProgram::SetTessAlpha(GLfloat alpha)
{
	if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

	GLuint alpha_loc = GetUniformLocation(TESSALPHA_UNIFORM);

	glUniform1f(alpha_loc, alpha);

	return *this;
}

breene::DefShadingLight::DefShadingLight()
: ShaderProgram()
{}

breene::DefShadingLight & breene::DefShadingLight::Init()
{
    ShaderProgram::Init();

    return *this;
}

breene::DefShadingLight & breene::DefShadingLight::SetWVP(const glm::mat4 & wvp)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint wvp_loc = GetUniformLocation(WVP_UNIFORM);
    glUniformMatrix4fv(wvp_loc, 1, GL_TRUE, glm::value_ptr(wvp));

    return *this;
}

breene::DefShadingLight & breene::DefShadingLight::SetPositionTextureUnit(GLuint texture_unit)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint pos_loc = GetUniformLocation(POSITION_UNIFORM);

    glUniform1i(pos_loc, texture_unit);

    return *this;
}

breene::DefShadingLight & breene::DefShadingLight::SetColorTextureUnit(GLuint texture_unit)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint color_map_loc = GetUniformLocation(COLORMAP_UNIFORM);

    glUniform1i(color_map_loc, texture_unit);

    return *this;
}

breene::DefShadingLight & breene::DefShadingLight::SetNormalTextureUnit(GLuint texture_unit)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint color_map_loc = GetUniformLocation(NORMALMAP_UNIFORM);

    glUniform1i(color_map_loc, texture_unit);

    return *this;
}

breene::DefShadingLight & breene::DefShadingLight::SetEWP(const glm::vec3 & ewp)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint ewp_loc = GetUniformLocation(DIRLIGHT_EYE_WORLD_POS_UNIFORM);

    glUniform3f(ewp_loc, ewp.x, ewp.y, ewp.z);

    return *this;
}

breene::DefShadingLight & breene::DefShadingLight::SetSpecularIntensity(GLfloat intensity)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint intensity_loc = GetUniformLocation(DIRLIGHT_SPECULAR_INTENSITY_UNIFORM);

    glUniform1f(intensity_loc, intensity);

    return *this;
}

breene::DefShadingLight & breene::DefShadingLight::SetSpecularPower(GLfloat power)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint power_loc = GetUniformLocation(DIRLIGHT_SPECULAR_POWER_UNIFORM);

    glUniform1f(power_loc, power);

    return *this;
}

breene::DefShadingLight & breene::DefShadingLight::SetScreenSize(GLuint width, GLuint height)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    GLuint screensize_loc = GetUniformLocation(SCREENSIZE_UNIFORM);

    glUniform2f(screensize_loc, static_cast<GLfloat>(width), static_cast<GLfloat>(height));

	return *this;
}

breene::DefShadingDirLight::DefShadingDirLight()
: DefShadingLight()
{}

breene::DefShadingDirLight & breene::DefShadingDirLight::Init()
{
    DefShadingLight::Init();

    AddShader(Shader(DEFSHADING_LIGHT_PASS_VERTEX_SHADER, GL_VERTEX_SHADER));
    AddShader(Shader(DEFSHADING_DIRLIGHT_PASS_FRAGMENT_SHADER, GL_FRAGMENT_SHADER));
    Finalize();

    return *this;
}

breene::DefShadingDirLight & breene::DefShadingDirLight::SetDirectionalLight(const DirectionalLight & light)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    const glm::vec3& color = light.GetColor();
    GLuint light_color_loc = GetUniformLocation(DIRLIGHT_COLOR_UNIFORM);
    glUniform3f(light_color_loc, color.r, color.g, color.b);

    GLuint light_ambient_intensity_loc = GetUniformLocation(DIRLIGHT_AMBIENT_INTENSITY_UNIFORM);
    glUniform1f(light_ambient_intensity_loc, light.GetAmbientIntensity());

    GLuint light_diffuse_intensity_loc = GetUniformLocation(DIRLIGHT_DIFFUSE_INTENSITY_UNIFORM);
    glUniform1f(light_diffuse_intensity_loc, light.GetDiffuseIntensity());

    glm::vec3 direction = glm::normalize(light.GetDirection());
    GLuint light_direction_loc = GetUniformLocation(DIRLIGHT_DIRECTION_UNIFORM);
    glUniform3f(light_direction_loc, direction.x, direction.y, direction.z);

    return *this;
}

breene::DefShadingPointLight::DefShadingPointLight()
: DefShadingLight()
{}

breene::DefShadingPointLight & breene::DefShadingPointLight::Init()
{
    DefShadingLight::Init();

    AddShader(Shader(DEFSHADING_LIGHT_PASS_VERTEX_SHADER, GL_VERTEX_SHADER));
    AddShader(Shader(DEFSHADING_PTLIGHT_PASS_FRAGMENT_SHADER, GL_FRAGMENT_SHADER));
    Finalize();

    return *this;
}

breene::DefShadingPointLight & breene::DefShadingPointLight::SetPointLight(const PointLight & light)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    const glm::vec3& color = light.GetColor();
    GLuint light_color_loc = GetUniformLocation(PTLIGHT_COLOR_UNIFORM);
    glUniform3f(light_color_loc, color.r, color.g, color.b);

    GLuint light_ambient_intensity_loc = GetUniformLocation(PTLIGHT_AMBIENT_INTENSITY_UNIFORM);
    glUniform1f(light_ambient_intensity_loc, light.GetAmbientIntensity());

    GLuint light_diffuse_intensity_loc = GetUniformLocation(PTLIGHT_DIFFUSE_INTENSITY_UNIFORM);
    glUniform1f(light_diffuse_intensity_loc, light.GetDiffuseIntensity());

    glm::vec3 position = light.GetPosition();
    GLuint light_direction_loc = GetUniformLocation(PTLIGHT_POSITION_UNIFORM);
    glUniform3f(light_direction_loc, position.x, position.y, position.z);

    GLuint atten_const_loc = GetUniformLocation(PTLIGHT_ATTEN_CONST_UNIFORM);
    glUniform1f(atten_const_loc, light.GetAttenuation().constant);

    GLuint atten_linear_loc = GetUniformLocation(PTLIGHT_ATTEN_LINEAR_UNIFORM);
    glUniform1f(atten_linear_loc, light.GetAttenuation().linear);

    GLuint atten_exp_loc = GetUniformLocation(PTLIGHT_ATTEN_EXP_UNIFORM);
    glUniform1f(atten_exp_loc, light.GetAttenuation().exponential);

    return *this;
}

breene::DefShadingSpotLight::DefShadingSpotLight()
: DefShadingLight()
{}

breene::DefShadingSpotLight & breene::DefShadingSpotLight::Init()
{
    DefShadingLight::Init();

    AddShader(Shader(DEFSHADING_LIGHT_PASS_VERTEX_SHADER, GL_VERTEX_SHADER));
    AddShader(Shader(DEFSHADING_SPOTLIGHT_PASS_FRAGMENT_SHADER, GL_FRAGMENT_SHADER));
    Finalize();

    return *this;
}

breene::DefShadingSpotLight & breene::DefShadingSpotLight::SetSpotLight(const SpotLight & light)
{
    if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

    const glm::vec3& color = light.GetColor();
    GLuint light_color_loc = GetUniformLocation(SPOTLIGHT_COLOR_UNIFORM);
    glUniform3f(light_color_loc, color.r, color.g, color.b);

    GLuint light_ambient_intensity_loc = GetUniformLocation(SPOTLIGHT_AMBIENT_INTENSITY_UNIFORM);
    glUniform1f(light_ambient_intensity_loc, light.GetAmbientIntensity());

    GLuint light_diffuse_intensity_loc = GetUniformLocation(SPOTLIGHT_DIFFUSE_INTENSITY_UNIFORM);
    glUniform1f(light_diffuse_intensity_loc, light.GetDiffuseIntensity());

    glm::vec3 position = light.GetPosition();
    GLuint light_direction_loc = GetUniformLocation(SPOTLIGHT_POSITION_UNIFORM);
    glUniform3f(light_direction_loc, position.x, position.y, position.z);

    GLuint atten_const_loc = GetUniformLocation(SPOTLIGHT_ATTEN_CONST_UNIFORM);
    glUniform1f(atten_const_loc, light.GetAttenuation().constant);

    GLuint atten_linear_loc = GetUniformLocation(SPOTLIGHT_ATTEN_LINEAR_UNIFORM);
    glUniform1f(atten_linear_loc, light.GetAttenuation().linear);

    GLuint atten_exp_loc = GetUniformLocation(SPOTLIGHT_ATTEN_EXP_UNIFORM);
    glUniform1f(atten_exp_loc, light.GetAttenuation().exponential);

    glm::vec3 direction = light.GetDirection();
    GLuint direction_loc = GetUniformLocation(SPOTLIGHT_DIRECTION_UNIFORM);
    glUniform3f(direction_loc, direction.x, direction.y, direction.z);

    GLuint cone_loc = GetUniformLocation(SPOTLIGHT_CONE_UNIFORM);
    glUniform1f(cone_loc, light.GetConeAngle());

    return *this;
}


