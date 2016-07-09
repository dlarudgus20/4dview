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
 * @file HyperCube.cpp
 * @date 2016. 7. 3.
 * @author dlarudgus20
 * @copyright The BSD (2-Clause) License
 */

#include "pch.h"
#include "HyperCube.h"
#include "Shader.h"

namespace
{
	const GLsizei strides = 4 * sizeof(GLfloat);
/*
	static GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f,

		-0.5f, 0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f,

		0.5f, 0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f,

		-0.5f, 0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f,
	};
	void InitalizeVertices() { }
//*/
///*
	GLfloat vertices[8 * 6 * 2 * 3 * 4];
	void InitalizeVertices()
	{
		static bool inited = false;
		if (inited)
			return;
		inited = true;

		static GLfloat cube3_vertices[][3] = {
			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, 0.5f, -0.5f,
			0.5f, 0.5f, -0.5f,
			-0.5f, 0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f, -0.5f, 0.5f,
			0.5f, -0.5f, 0.5f,
			0.5f, 0.5f, 0.5f,
			0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f, 0.5f,
			-0.5f, -0.5f, 0.5f,

			-0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, 0.5f,
			-0.5f, 0.5f, 0.5f,

			0.5f, 0.5f, 0.5f,
			0.5f, 0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, 0.5f,
			0.5f, 0.5f, 0.5f,

			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, 0.5f,
			0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f, 0.5f, -0.5f,
			0.5f, 0.5f, -0.5f,
			0.5f, 0.5f, 0.5f,
			0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f, -0.5f,
		};

		int idx = 0;

		for (float x = -0.5f; x != 0.5f; x = 0.5f)
		{
			for (const auto &vt : cube3_vertices)
			{
				vertices[idx++] = x;
				vertices[idx++] = vt[0];
				vertices[idx++] = vt[1];
				vertices[idx++] = vt[2];
			}
		}
		for (float y = -0.5f; y != 0.5f; y = 0.5f)
		{
			for (const auto &vt : cube3_vertices)
			{
				vertices[idx++] = vt[0];
				vertices[idx++] = y;
				vertices[idx++] = vt[1];
				vertices[idx++] = vt[2];
			}
		}
		for (float z = -0.5f; z != 0.5f; z = 0.5f)
		{
			for (const auto &vt : cube3_vertices)
			{
				vertices[idx++] = vt[0];
				vertices[idx++] = vt[1];
				vertices[idx++] = z;
				vertices[idx++] = vt[2];
			}
		}
		for (float w = -0.5f; w != 0.5f; w = 0.5f)
		{
			for (const auto &vt : cube3_vertices)
			{
				vertices[idx++] = vt[0];
				vertices[idx++] = vt[1];
				vertices[idx++] = vt[2];
				vertices[idx++] = w;
			}
		}
	}
//*/
}

HyperCube::HyperCube()
	: m_color(1.0f, 1.0f, 1.0f, 1.0f)
{
	InitalizeVertices();

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, strides, (GLvoid *)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	glBindVertexArray(0);
}

HyperCube::~HyperCube()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
}

void HyperCube::draw(bool bUseMaterial /* = true */) const
{
	if (bUseMaterial)
	{
		Shader *pShader = Shader::getCurrentShader();
		pShader->setUniform4f("material.color", m_color);
	}

	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / strides);
	glBindVertexArray(0);
}
