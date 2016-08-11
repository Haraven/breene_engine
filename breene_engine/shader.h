#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <string>
#include <vector>

namespace breene
{
	// Basic shader class, that allows for compiling a shader & attaching it to a program
	class Shader
	{
	public:
		Shader() {}
		Shader(const std::string& file_path, GLenum shader_type);
	
        const std::string& GetShaderPath() const { return _shader_path; }
        const GLenum& GetShaderType() const { return _shader_type; }
        Shader& SetShaderPath(const std::string& new_path) { _shader_path = new_path; return *this; }
        Shader& SetShaderType(const GLenum new_type) { _shader_type = new_type; return *this; }

        ///<summary>
		/// Compiles the shader, using the shader path to locate the file, and the shader type to determine the type of shader.
		///</summary>
        ///<exception cref="std::runtime_error">
        /// Thrown when the shader cannot be compiled, either because of an internal error, or because the path or shader type were not valid.
        ///</exception>
		Shader& Compile();
        ///<summary>
        /// Attaches the compiled shader to a program.
        ///</summary>
        ///<exception cref="std::runtime_error">
        /// Thrown when the shader is not compiled beforehand.
        ///</exception>
		Shader& AttachTo(GLuint program);
        ///<summary>
        /// Compiles and attaches the shader to a program, using calls to Compile() and AttachTo(program).
        ///</summary>
        ///<remarks>
        /// Exceptions thrown by Compile() or AttachTo(program) will be propagated.
        ///</remarks>
		Shader& CompileAndAttachTo(GLuint program);
        Shader& DetachFrom(GLuint program);


        // creates and links a program from the given shaders
        friend GLuint InitializeProgram(std::vector<Shader> shaders);
    private:
        std::string _shader_path;
        GLenum _shader_type;
        GLuint _shader_obj;
        const static size_t log_size = 1024;
	};
}
