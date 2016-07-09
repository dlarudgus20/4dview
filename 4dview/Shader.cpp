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
 * @file Shader.cpp
 * @date 2015. 9. 18.
 * @author dlarudgus20
 * @copyright The BSD (2-Clause) License
 */

#include "pch.h"
#include "Shader.h"

namespace
{
	Shader *pCurrentShader = nullptr;
}

Shader *Shader::getCurrentShader()
{
	return pCurrentShader;
}

Shader::Shader()
{
}

Shader::~Shader()
{
	glDeleteShader(m_vertexShader);
	glDeleteShader(m_fragmentShader);
	glDeleteProgram(m_shaderProgram);

	if (pCurrentShader == this)
		pCurrentShader = nullptr;
}

void Shader::compile(const char *vertex, const char *fragment)
{
	m_vertexShader = loadFile(vertex, GL_VERTEX_SHADER, m_vsInfoString);
	m_fragmentShader = loadFile(fragment, GL_FRAGMENT_SHADER, m_fsInfoString);

	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, m_vertexShader);
	glAttachShader(m_shaderProgram, m_fragmentShader);
	glLinkProgram(m_shaderProgram);

	GLint success;
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLint size;
		glGetProgramiv(m_shaderProgram, GL_INFO_LOG_LENGTH, &size);
		std::string info(size, '\0');
		glGetProgramInfoLog(m_shaderProgram, size, nullptr, &info[0]);

		// trim
		info.erase(
			std::find_if(info.rbegin(), info.rend(),
				[](char c) { return !(isspace(c) || iscntrl(c)); }).base(),
			info.end());

		using namespace std::literals::string_literals;
		throw CompileError("["s + vertex + " & " + "] link err : " + info);
	}
}

void Shader::use()
{
	glUseProgram(m_shaderProgram);
	pCurrentShader = this;
}

void Shader::setUniform1f(const char *var, GLfloat f)
{
	glUniform1f(findUniform(var), f);
}

void Shader::setUniform1fv(const char *var, GLsizei count, const GLfloat *value)
{
	glUniform1fv(findUniform(var), count, value);
}

void Shader::setUniform3f(const char *var, const glm::vec3 &vec3)
{
	glUniform3fv(findUniform(var), 1, glm::value_ptr(vec3));
}

void Shader::setUniform4f(const char *var, const glm::vec4 &vec4)
{
	glUniform4fv(findUniform(var), 1, glm::value_ptr(vec4));
}

void Shader::setUniform1i(const char *var, GLint i)
{
	glUniform1i(findUniform(var), i);
}

void Shader::setUniformMatrix3f(const char *var, const glm::mat3 &mat)
{
	glUniformMatrix3fv(findUniform(var), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setUniformMatrix4f(const char *var, const glm::mat4 &mat)
{
	glUniformMatrix4fv(findUniform(var), 1, GL_FALSE, glm::value_ptr(mat));
}

GLint Shader::findUniform(const char *var)
{
	GLint loc = glGetUniformLocation(m_shaderProgram, var);
	if (loc == -1)
		throw UniformError("cannot find '" + std::string(var) + "' uniform variable.");
	return loc;
}

GLuint Shader::loadFile(const char *filename, GLuint shaderType, std::string &infoString)
{
	try
	{
		std::ifstream file;
		file.exceptions(std::ios::badbit);
		file.open(filename);

		std::string strSource;
		std::copy(std::istreambuf_iterator<char> { file }, std::istreambuf_iterator<char> { },
			std::back_inserter(strSource));

		const GLchar *src = strSource.c_str();
		GLuint shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);

		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		GLint size;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &size);
		std::string info(size, '\0');
		glGetShaderInfoLog(shader, size, nullptr, &info[0]);

		// trim
		info.erase(
			std::find_if(info.rbegin(), info.rend(),
				[](char c) { return !(isspace(c) || iscntrl(c)); }).base(),
			info.end());

		infoString = info;

		if (success)
		{
			return shader;
		}
		else
		{
			throw CompileError("[" + std::string(filename) + "] : " + info);
		}
	}
	catch (std::ios::failure &e)
	{
		throw CompileError("[" + std::string(filename) + "] : " + e.what());
	}
}
