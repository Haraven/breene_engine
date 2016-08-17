#include "fxaa.h"
#include <string>
#include <glm\gtc\type_ptr.hpp>
#include "my_constants.h"

breene::FXAABuffer::FXAABuffer()
: FramebufferObject()
{
	_txo_in = NULL;
	_txo_out = NULL;
}

breene::FXAABuffer & breene::FXAABuffer::Init(GLuint width, GLuint height)
{
	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);

	glGenTextures(1, &_txo_in);
	glBindTexture(GL_TEXTURE_2D, _txo_in);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _txo_in, 0);

	glGenTextures(1, &_txo_out);
	glBindTexture(GL_TEXTURE_2D, _txo_out);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _txo_out, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) throw std::runtime_error("Error initializing G buffer. Framebuffer status: " + std::to_string(status));
	
	_width = width;
	_height = height;

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return *this;
}

breene::FXAABuffer & breene::FXAABuffer::CopyFBO(const FramebufferObject & fbo, GLenum attachment_to_read)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo.GetHandle());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
	glReadBuffer(attachment_to_read);

	glBlitFramebuffer(0, 0, _width, _height, 0, 0, _width, _height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	return *this;
}

breene::FXAABuffer & breene::FXAABuffer::CopyFBO()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
	glReadBuffer(GL_COLOR_ATTACHMENT0);

	glBlitFramebuffer(0, 0, _width, _height, 0, 0, _width, _height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	return *this;
}

breene::FXAABuffer & breene::FXAABuffer::StartPostProcess()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
	glDrawBuffer(GL_COLOR_ATTACHMENT1);
	glClear(GL_COLOR_BUFFER_BIT);

	glActiveTexture(COLOR_TEXTURE_UNIT);
	glBindTexture(GL_TEXTURE_2D, _txo_in);

	return *this;
}

breene::FXAABuffer & breene::FXAABuffer::BindFinalPass()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _fbo);
	glReadBuffer(GL_COLOR_ATTACHMENT1);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return *this;
}

breene::FXAABuffer::~FXAABuffer()
{
	FramebufferObject::~FramebufferObject();
	if (_txo_in != NULL)
	{
		glDeleteTextures(1, &_txo_in);
		_txo_in = NULL;
	}
	if (_txo_out != NULL)
	{
		glDeleteTextures(1, &_txo_out);
		_txo_out = NULL;
	}
}

breene::FXAAProgram & breene::FXAAProgram::Init()
{
	ShaderProgram::Init();

	AddShader(Shader(FXAA_VERT_SHADER, GL_VERTEX_SHADER));
	AddShader(Shader(FXAA_FRAG_SHADER, GL_FRAGMENT_SHADER));
	Finalize();

	return *this;
}

breene::FXAAProgram & breene::FXAAProgram::SetWVP(const glm::mat4 & wvp)
{
	if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");
	
	GLuint wvp_uniform_loc = GetUniformLocation(WVP_UNIFORM);
	
	glUniformMatrix4fv(wvp_uniform_loc, 1, GL_TRUE, glm::value_ptr(wvp));
	

	return *this;
}

breene::FXAAProgram & breene::FXAAProgram::SetColorTextureUnit(GLuint texture_unit)
{
	if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

	GLuint texture_unit_loc = GetUniformLocation(COLORMAP_UNIFORM);

	glUniform1i(texture_unit_loc, texture_unit);

	return *this;
}

breene::FXAAProgram & breene::FXAAProgram::SetInverseTextureSize(const glm::vec2 & tex_inverse)
{
	if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

	GLuint tex_inverse_loc = GetUniformLocation(FXAA_TEX_INVERSE_SIZE_UNIFORM);

	glUniform2f(tex_inverse_loc, tex_inverse.x, tex_inverse.y);

	return *this;
}

breene::FXAAProgram & breene::FXAAProgram::SetSpanMax(GLfloat span)
{
	if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

	GLuint span_loc = GetUniformLocation(FXAA_SPAN_MAX_UNIFORM);

	glUniform1f(span_loc, span);

	return *this;
}

breene::FXAAProgram & breene::FXAAProgram::SetReductionMin(GLfloat reduction)
{
	if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

	GLuint reduction_min_loc = GetUniformLocation(FXAA_REDUCTION_MIN_UNIFORM);

	glUniform1f(reduction_min_loc, reduction);

	return *this;
}

breene::FXAAProgram & breene::FXAAProgram::SetReductionMultiple(GLfloat multiple)
{
	if (glfwGetCurrentContext() == nullptr) throw std::runtime_error("OpenGL context has not been initialized");

	GLuint reduction_mul_loc = GetUniformLocation(FXAA_REDUCTION_MUL_UNIFORM);

	glUniform1f(reduction_mul_loc, multiple);

	return *this;
}
