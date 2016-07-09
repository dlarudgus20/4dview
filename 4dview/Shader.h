// Copyright (c) 2014, 임경현
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/**
 * @file Shader.h
 * @date 2015. 9. 18.
 * @author dlarudgus20
 * @copyright The BSD (2-Clause) License
 */

#ifndef SHADER_H_
#define SHADER_H_

class Shader final
{
public:
	static constexpr int POINTLIGHT_COUNT = 4;

	static Shader *getCurrentShader();

private:
	GLuint m_vertexShader = 0, m_fragmentShader = 0;
	GLuint m_shaderProgram = 0;

	std::string m_vsInfoString, m_fsInfoString;

public:
	Shader();
	~Shader();

	void compile(const char *vertex, const char *fragment);
	void use();

	const std::string &getVSInfoString() { return m_vsInfoString; }
	const std::string &getFSInfoString() { return m_fsInfoString; }

	void setUniform1f(const char *var, GLfloat f);
	void setUniform1f(const std::string &var, GLfloat f) { setUniform1f(var.c_str(), f); }

	void setUniform1fv(const char *var, GLsizei count, const GLfloat *value);

	void setUniform3f(const char *var, const glm::vec3 &vec3);
	void setUniform3f(const std::string &var, const glm::vec3 &vec3) { setUniform3f(var.c_str(), vec3); }

	void setUniform4f(const char *var, const glm::vec4 &vec4);
	void setUniform4f(const std::string &var, const glm::vec4 &vec4) { setUniform4f(var.c_str(), vec4); }

	void setUniform1i(const char *var, GLint i);
	void setUniform1i(const std::string &var, GLint i) { setUniform1i(var.c_str(), i); }

	void setUniformMatrix3f(const char *var, const glm::mat3 &mat);
	void setUniformMatrix3f(const std::string &var, const glm::mat3 &mat) { setUniformMatrix3f(var.c_str(), mat); }

	void setUniformMatrix4f(const char *var, const glm::mat4 &mat);
	void setUniformMatrix4f(const std::string &var, const glm::mat4 &mat) { setUniformMatrix4f(var.c_str(), mat); }

	class CompileError : public std::runtime_error
	{
	public:
		explicit CompileError(const std::string &msg)
			: std::runtime_error(msg) { }
	};

	class UniformError : public std::runtime_error
	{
	public:
		explicit UniformError(const std::string &msg)
			: std::runtime_error(msg) { }
	};

private:
	GLint findUniform(const char *var);

	static GLuint loadFile(const char *filename, GLuint kind, std::string &infoString);
};

#endif /* SHADER_H_ */
