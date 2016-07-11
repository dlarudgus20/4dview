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
	const GLsizei strides = 8 * sizeof(GLfloat);

	GLfloat vertices[] = {
		// position, color
		-0.5f, -0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f, 0.5f,
		-0.5f, -0.5f, -0.5f, +0.5f,    0.4f, 1.0f, 0.0f, 0.5f,
		-0.5f, -0.5f, +0.5f, -0.5f,    0.0f, 1.0f, 0.4f, 0.5f,
		-0.5f, -0.5f, +0.5f, +0.5f,    0.8f, 1.0f, 0.0f, 0.5f,
		-0.5f, +0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.8f, 0.5f,
		-0.5f, +0.5f, -0.5f, +0.5f,    0.4f, 1.0f, 0.7f, 0.5f,
		-0.5f, +0.5f, +0.5f, -0.5f,    0.7f, 1.0f, 0.4f, 0.5f,
		-0.5f, +0.5f, +0.5f, +0.5f,    0.8f, 1.0f, 0.8f, 0.5f,
		+0.5f, -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 1.0f, 0.5f,
		+0.5f, -0.5f, -0.5f, +0.5f,    0.4f, 0.0f, 1.0f, 0.5f,
		+0.5f, -0.5f, +0.5f, -0.5f,    0.0f, 0.4f, 1.0f, 0.5f,
		+0.5f, -0.5f, +0.5f, +0.5f,    0.8f, 0.0f, 1.0f, 0.5f,
		+0.5f, +0.5f, -0.5f, -0.5f,    0.0f, 0.8f, 1.0f, 0.5f,
		+0.5f, +0.5f, -0.5f, +0.5f,    0.4f, 0.7f, 1.0f, 0.5f,
		+0.5f, +0.5f, +0.5f, -0.5f,    0.7f, 0.4f, 1.0f, 0.5f,
		+0.5f, +0.5f, +0.5f, +0.5f,    0.8f, 0.8f, 1.0f, 0.5f,
	};
	std::array<GLint, 8 * 6 * 6> indices;

	void InitializeVertices()
	{
		static bool inited = false;
		if (inited)
			return;
		inited = true;

		int idx = 0;

		const int X = 1;
		const int Y = 2;
		const int Z = 4;
		const int W = 8;

		auto getidx = [](int xyzw) {
			return (xyzw & 1) * 8
				+ ((xyzw & 2) >> 1) * 4
				+ ((xyzw & 4) >> 2) * 2
				+ ((xyzw & 8) >> 3);
		};

		// (+): 013 320
		// (-): 023 310
		// 023 310
		// 457 764
		// 046 620
		// 137 751
		// 015 540
		// 267 732

		auto put_cube3_indices = [&idx, getidx](int o, int i, int j, int k) {
			std::array<GLint, 8> v = {
				o, o | i, o | j, o | i | j,
				o | k, o | k | i, o | k | j, o | k | i | j
			};
			std::array<GLint, 36> a = {
				v[0], v[2], v[3], v[3], v[1], v[0],
				v[4], v[5], v[7], v[7], v[6], v[4],
				v[0], v[4], v[6], v[6], v[2], v[0],
				v[1], v[3], v[7], v[7], v[5], v[1],
				v[0], v[1], v[5], v[5], v[4], v[0],
				v[2], v[6], v[7], v[7], v[3], v[2]
			};
			std::transform(a.cbegin(), a.cend(), indices.begin() + idx, getidx);
			idx += (int)a.size();
		};

		put_cube3_indices(0, Z, Y, W); // x = 0, i j k = z y w
		put_cube3_indices(X, Y, Z, W); // x = 1, i j k = y z w
		put_cube3_indices(0, W, Z, X); // y = 0, i j k = w z x
		put_cube3_indices(Y, Z, W, X); // y = 1, i j k = z w x
		put_cube3_indices(0, X, W, Y); // z = 0, i j k = x w y
		put_cube3_indices(Z, W, X, Y); // z = 1, i j k = w x y
		put_cube3_indices(0, Y, X, Z); // w = 0, i j k = y x z
		put_cube3_indices(W, X, Y, Z); // w = 1, i j k = x y z
	}
}

HyperCube::HyperCube()
	: m_color(1.0f, 1.0f, 1.0f, 1.0f)
{
	InitializeVertices();

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	glBindVertexArray(m_vao);
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, strides, (const GLvoid *)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, strides, (const GLvoid *)(4 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
	//glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / strides);
	glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, (const GLvoid *)0);
	glBindVertexArray(0);
}
