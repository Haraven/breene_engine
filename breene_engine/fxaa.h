#pragma once

#include <glm\glm.hpp>
#include "fbo.h"
#include "shader_program.h"

namespace breene
{
	class FXAABuffer : public FramebufferObject
	{
	public:
		FXAABuffer();

		virtual breene::FXAABuffer& Init(GLuint width, GLuint height) override;
		breene::FXAABuffer& CopyFBO(const FramebufferObject& fbo, GLenum attachment_to_read);
		// will copy the default fbo from color attachment 0
		breene::FXAABuffer& CopyFBO(); 

		breene::FXAABuffer& StartPostProcess();
		breene::FXAABuffer& BindFinalPass();

		~FXAABuffer();
	private:
		GLuint _txo_in;
		GLuint _txo_out;
		GLulong _width;
		GLulong _height;
	};

	class FXAAProgram : public ShaderProgram
	{
	public:
		virtual breene::FXAAProgram& Init();

		breene::FXAAProgram& SetWVP(const glm::mat4& wvp);
		breene::FXAAProgram& SetColorTextureUnit(GLuint texture_unit);
		breene::FXAAProgram& SetInverseTextureSize(const glm::vec2& tex_inverse);
		breene::FXAAProgram& SetSpanMax(GLfloat span);
		breene::FXAAProgram& SetReductionMin(GLfloat reduction);
		breene::FXAAProgram& SetReductionMultiple(GLfloat multiple);
	private:
	};
}